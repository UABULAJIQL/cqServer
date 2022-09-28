#ifndef _EPOLLSERVER_H
#define _EPOLLSERVER_H
#include "network/networkListen.h"
#include <sys/epoll.h>

const size_t EVENTS = 1024;

class EpollServer : public NetworkListen {
    private:
        struct epoll_event _events[EVENTS];
        int _epfd{-1};
        bool _acceptOn = {false};

    private:
        void InitEpoll();
        void AddEventEpoll(int epfd, int fd, int events);
        void DeleteEventEpoll(int epfd, int fd);
        void ModifyEventEpoll(int epfd, int fd, int events);

    public:
        EpollServer();

        bool Init() override;
        void RegisterMsgFunction() override;


        bool Update() override;

    protected:
        int Accept() override;
};
#endif
