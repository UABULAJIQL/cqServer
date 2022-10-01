#ifndef _TESTTHREADOBJ_H
#define _TESTTHREADOBJ_H

#include "thread/threadObject.h"

class TestThreadObj : public ThreadObject {
    public:
        bool Init() override;
        void RegisterMsgFunction() override;
        bool Update() override;

        void Dispose() override;

    private:
        //消息处理函数
        void HandleMsg(Packet *pPacket);
};

#endif
