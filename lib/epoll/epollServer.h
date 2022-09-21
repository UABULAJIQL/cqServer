#ifndef _EPOLLSERVER_H
#define _EPOLLSERVER_H
#include "network/networkListen.h"
#include <sys/epoll.h>

const size_t EVENTS = 1024;

class EpollServer : public NetworkListen {
    private:
        struct epoll_event _events[EVENTS];
        int _epfd;
        bool _acceptOn = {false};

    private:
        void InitEpoll() {
            //初始设置最大可以同时接收的通知数
            _epfd = epoll_create(EVENTS);

            //先将监听文件描述符上树
            //输入 输出 读端关闭
            AddEventEpoll(_epfd, _socket, EPOLLIN | EPOLLOUT | EPOLLRDHUP);
        }

        void AddEventEpoll(int epfd, int fd, int events) {

            struct epoll_event ev;
            ev.events = events;
            ev.data.ptr = nullptr;
            ev.data.fd = fd;

            epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
        }

        void DeleteEventEpoll(int epfd, int fd) {
            epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
        }

        void ModifyEventEpoll(int epfd, int fd, int events) {

            struct epoll_event ev;
            ev.events = events;
            ev.data.ptr = nullptr;
            ev.data.fd = fd;

            epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
        }

    public:
        EpollServer() { InitEpoll(); }

        bool Update() override;

    protected:
        int Accept() override;
};
#endif
