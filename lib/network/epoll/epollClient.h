#ifndef _EPOLLCLIENT_H
#define _EPOLLCLIENT_H

#include "network/connectObj.h"
#include "network/networkConnector.h"
#include <sys/epoll.h>

class EpollClient : public NetworkConnector {
    private:
        struct epoll_event _setEvent;

        struct epoll_event _getEvent;

        int _epfd;

    public:
        EpollClient();

        bool Update() override;

        bool Init() override;
        void RegisterMsgFunction() override;

};

#endif
