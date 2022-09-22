#ifndef _THREAD_H
#define _THREAD_H

#include "disposable.h"
#include <thread>

class Thread : IDisposable {

    public:
        //启动
        virtual bool Start();
        //暂停
        void Stop();
        //帧
        virtual void Update() = 0;

        bool IsRund() const;

        void Dispose() override;

    protected:
        bool _isRun{false};
        std::thread _thread;
};

#endif
