#ifndef _THREAD_H
#define _THREAD_H

#include "tools/disposable.h"
#include "tools/snObj.h"

#include <list>
#include <mutex>
#include <thread>

class ThreadObject;
class Packet;

//线程类 每个线程实例都管理一组 线程包裹类
class Thread : public IDisposable, public SnObject {

    public:
        Thread();
        void AddThreadObj(ThreadObject *threadObj);

        //启动
        bool Start();
        //暂停
        void Stop();
        //帧
        void Update();

        bool IsRun() const;

        void Dispose() override;

        void AddPacket(Packet *pPacket);

    protected:
        bool _isRun{false};
        std::thread _thread;

        //这个是公共变量所以需要锁
        std::list<ThreadObject *> _objlist;

        std::list<ThreadObject *> _tempObjList;
        std::mutex _mutex;
};

#endif
