#ifndef _SERVER_H
#define _SERVER_H

#include "network/epoll/epollServer.h"

class Server : public EpollServer {
    public:
        bool DataHandler();

    protected:
        void HandlerOne(ConnectObj *pConnectObj);
        int Accept() override;

    protected:
        int _acceptCount{0};
        int _sendMsgCount{0};
        int _recvMsgCount{0};

        bool _isShow{true};
};

#endif
