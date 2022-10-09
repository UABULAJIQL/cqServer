#ifndef _THREAD_H
#define _THREAD_H

#include "tools/disposable.h"
#include "tools/snObj.h"

#include <list>
#include <mutex>
#include <thread>

class ThreadObject;
class Packet;

// 存储包裹类的类 从原来的线程类中分离出来
class ThreadObjectList : public IDisposable {

  protected:
    // 这个是公共变量所以需要锁
    std::list<ThreadObject *> _objlist;

    std::mutex _mutex;

  public:
    // 帧
    void Update();
    // 添加包裹类
    void AddThreadObj(ThreadObject *threadObj);
    // 添加packet
    void AddPacket(Packet *pPacket);

    void Dispose() override;
};

// 线程类 每个线程实例都管理一组 线程包裹类
class Thread : public ThreadObjectList, public SnObject {

  public:
    Thread();

    // 启动
    bool Start();
    // 暂停
    void Stop();

    bool IsRun() const;

  protected:
    bool _isRun{false};
    std::thread _thread;
};

#endif
