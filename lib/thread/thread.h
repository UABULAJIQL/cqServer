#ifndef _THREAD_H
#define _THREAD_H

#include "disposable.h"

#include <list>
#include <thread>
#include <mutex>

class ThreadObject;

//线程类 每个线程实例都管理一组 线程包裹类
class Thread : IDisposable {

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

    protected:
        bool _isRun{false};
        std::thread _thread;

        //这个是公共变量所以需要锁
        std::list<ThreadObject *> _objlist;

        std::list<ThreadObject*> _tempObjList;
        std::mutex _mutex;
};

#endif
