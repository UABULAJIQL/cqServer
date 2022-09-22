#include "epollClient.h"

EpollClient::EpollClient() {
    _epfd = epoll_create(1);

    _setEvent.events = EPOLLIN | EPOLLRDHUP;

    epoll_ctl(_epfd, EPOLL_CTL_ADD, _socket, &_setEvent);
}

bool EpollClient::Update() {

    //重连 目前这里就一次-----------
    if (_socket == -1) {
        if (!Connect(_ip, _port))
            return false; //重试一次就直接退出
    }

    //判断发送缓冲区中如果有东西 添加发送事件
    if (_connectObj->HasSendData()) {

        _setEvent.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP;

        epoll_ctl(_epfd, EPOLL_CTL_MOD, _socket, &_setEvent);
    }

    //设置50毫秒的阻塞
    int nready = epoll_wait(_epfd, &_getEvent, sizeof(_getEvent), 50);

    if (nready > 0) {

        do {
            //读端关闭 错误 读写都关闭
            if (_getEvent.events & EPOLLRDHUP || _getEvent.events & EPOLLERR ||
                    _getEvent.events & EPOLLHUP) {

                std::cout << "断开连接" << std::endl;

                //释放cobj
                _connectObj->Dispose();
                if (_connectObj != nullptr) {
                    delete _connectObj;
                    _connectObj = nullptr;
                }

                //下树
                epoll_ctl(_epfd, EPOLL_CTL_DEL, _socket, nullptr);

                // return false;
                break;
            }

            //接收事件
            if (_getEvent.events & EPOLLIN) {
                if (!_connectObj->Recv()) {
                    //释放cobj
                    _connectObj->Dispose();
                    if (_connectObj != nullptr) {
                        delete _connectObj;
                        _connectObj = nullptr;
                    }

                    //下树
                    epoll_ctl(_epfd, EPOLL_CTL_DEL, _socket, nullptr);
                }
                break;
            }

            //发送事件
            if (_getEvent.events & EPOLLOUT) {
                //发送
                _connectObj->Send();

                _setEvent.events = EPOLLIN | EPOLLRDHUP;

                epoll_ctl(_epfd, EPOLL_CTL_MOD, _socket, &_setEvent);

                break;
            }

        } while (false);

        if (IsConnected())
            return true;

        /* //还是重新尝试连接？
           if (_getEvent.events & EPOLLIN || _getEvent.events & EPOLLOUT) {
        // TryCreateConnectObj();
        } */
    }

    return false;
}
