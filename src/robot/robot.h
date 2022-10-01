#ifndef _ROBOT_H
#define _ROBOT_H
#include "network/epoll/epollClient.h"

class Packet;

class Robot : public EpollClient {
    private:
        //临时标记为了只发送一条数据
        bool _isSendMsg{false};

    public:
        bool Init() override;
        void RegisterMsgFunction() override;
        bool Update() override;
};

#endif
