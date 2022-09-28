#ifndef _THREADMGR_H
#define _THREADMGR_H

#include "tools/sigleton.h"
#include <map>
#include <mutex>

class ThreadObject;
class Thread;

//单例类
class ThreadMgr : public Sigleton<ThreadMgr> {
    private:
        //最后一个线程的序列号 主要是为了吧包裹实例 尽可能的平均添加到线程中
        uint64_t _lastThreadSn{0};

        std::mutex _mutex;
        //<序列号, Thread*>
        std::map<uint64_t, Thread *> _threads;

    public:
        ThreadMgr();
        //启动所有管理的线程
        void StartAllThread();
        //所有线程是否还在运行中
        bool IsGameLoop();
        //添加一个线程
        void NewThread();
        //添加一个包裹类
        void AddObjToThread(ThreadObject *obj);

        void Dispose();
};

#endif
