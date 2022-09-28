#ifndef _ROBOT_H
#define _ROBOT_H
#include "network/epoll/epollClient.h"

class Robot : public EpollClient {
    public:
        bool Init() override;
        void RegisterMsgFunction() override;
        bool Update() override;
};

#endif
