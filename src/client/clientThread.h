#ifndef _CLIENTTHREAD_H
#define _CLIENTTHREAD_H
#include "thread/thread.h"
class Client;
class ClientThread : public Thread {

    public:
        explicit ClientThread(int msgCount);
        void Update() override;
        void Dispose() override;

    private:
        Client *_pClient{nullptr};
        //消息数量
        int _msgCount{0};
};

#endif
