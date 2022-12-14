#include "epollServer.h"
#include "network/connectObj.h"

#include "buffer/networkBuffer.h" //暂时
#include "packet/packet.h"        //暂时

EpollServer::EpollServer() { InitEpoll(); }

bool EpollServer::Update() {

    _acceptOn = false;

    for (auto bIter = _connects.begin(), eIter = _connects.end();
         bIter != eIter; ++bIter) {

        // 如果有待发送的数据那么添加OUT事件
        if (bIter->second->HasSendData()) {
            // std::cout << "有发送的数据啊" << std::endl;
            ModifyEventEpoll(_epfd, bIter->first,
                             EPOLLIN | EPOLLOUT | EPOLLRDHUP);
        }
    }

    // 设置阻塞50毫秒
    int nready = epoll_wait(_epfd, _events, EVENTS, 100);

    if (nready < 0) {
        if (errno == EINTR || errno == ECONNABORTED) {
            perror("信号中断 或者客户端情况TCP重置\n");
            // 下一帧可以继续
            return true;
        }

        std::cout << "错误代码 " << errno << std::endl;
        // 可以退出了
        return false;
    }

    // if (nready <= 0)
    //     return false;//这可不能退出

    for (int i = 0; i < nready; ++i) {
        // 监听文件描述符
        if (_events[i].data.fd == _socket) {
            _acceptOn = true;
            std::cout << "来连接了!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            continue;
        }

        auto connectsOne = _connects.find(_events[i].data.fd);
        if (connectsOne == _connects.end()) {
            continue;
        }

        // 读端关闭 错误 读写都关闭
        if (_events[i].events & EPOLLRDHUP || _events[i].events & EPOLLERR ||
            _events[i].events & EPOLLHUP) {

            std::cout << "断开连接" << std::endl;

            // 释放cobj
            connectsOne->second->Dispose();
            delete connectsOne->second;
            connectsOne->second = nullptr;

            // 移除map集合
            _connects.erase(connectsOne);
            // 下树
            DeleteEventEpoll(_epfd, connectsOne->first);

            continue;
        }

        // 接收事件
        if (_events[i].events & EPOLLIN) {
            // std::cout << "来数据了" << std::endl;

            // 如果接收失败
            if (!connectsOne->second->Recv()) {
                std::cout << "已经断开连接了" << std::endl;

                // 释放cobj
                connectsOne->second->Dispose();
                delete connectsOne->second;
                connectsOne->second = nullptr;

                // 移除map集合
                _connects.erase(connectsOne);
                // 下树
                DeleteEventEpoll(_epfd, connectsOne->first);
            }

            continue;
        }

        // 发送事件
        if (_events[i].events & EPOLLOUT) {

            // std::cout << "服务端发送消息" << std::endl;
            // 发送
            connectsOne->second->Send();

            // 防止频繁的检测可发送事件所以吧OUt去掉了
            ModifyEventEpoll(_epfd, connectsOne->first, EPOLLIN | EPOLLRDHUP);

            continue;
        }
    }

    // 先处理其他 再处理连接请求
    if (_acceptOn)
        Accept();

    return true;
}

int EpollServer::Accept() {

    int acceptNum = 0;
    int tempfd;

    while (true) {
        // 记得解除监听文件描述符的阻塞
        tempfd = accept(_socket, NULL, NULL);
        if (tempfd == -1) {
            break;
        }
        // std::cout << "accept socket:" << tempfd << std::endl;
        // 上树操作
        AddEventEpoll(_epfd, tempfd, EPOLLIN | EPOLLRDHUP);
        // 添加创建cobj
        CreateConnectObj(tempfd);

        ++acceptNum;
    }

    return acceptNum;
}

void EpollServer::InitEpoll() {
    // 初始设置最大可以同时接收的通知数
    _epfd = epoll_create(EVENTS);

    // 先将监听文件描述符上树
    // 输入 输出 读端关闭
    AddEventEpoll(_epfd, _socket, EPOLLIN | EPOLLOUT | EPOLLRDHUP);
}

void EpollServer::AddEventEpoll(int epfd, int fd, int events) {

    struct epoll_event ev;
    ev.events = events;
    ev.data.ptr = nullptr;
    ev.data.fd = fd;

    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}
void EpollServer::DeleteEventEpoll(int epfd, int fd) {
    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
}
void EpollServer::ModifyEventEpoll(int epfd, int fd, int events) {

    struct epoll_event ev;
    ev.events = events;
    ev.data.ptr = nullptr;
    ev.data.fd = fd;

    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
}

bool EpollServer::Init() { return true; }
