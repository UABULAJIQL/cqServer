#ifndef _THREADMGR_H
#define _THREADMGR_H

#include "thread/thread.h"
#include "tools/common.h"
#include "tools/singleton.h"

#include <map>
#include <mutex>

class ThreadObject;
class Thread;
class Packet;
class Network;

// 单例类
class ThreadMgr : public Singleton<ThreadMgr>, public ThreadObjectList {
  private:
    // 最后一个线程的序列号 主要是为了吧包裹实例 尽可能的平均添加到线程中
    uint64_t _lastThreadSn{0};

    std::mutex _locator_lock;
    //<序列号, Thread*>
    std::map<uint64_t, Thread *> _threads;

    // 协议与网络实例
    std::mutex _locatorMutex;
    std::map<APP_TYPE, Network *> _networkLocator;

  public:
    ThreadMgr();
    // 启动所有管理的线程
    void StartAllThread();
    // 是否还有线程在运行
    bool IsGameLoop();
    // 添加一个线程
    void NewThread();
    // 添加一个包裹类
    bool AddObjToThread(ThreadObject *obj);
    // 添加一个网络类到线程
    void AddNetworkToThread(APP_TYPE appType, Network *pNetwork);

    // Packet分发
    void DispatchPacket(Packet *pPacket);

    // 向注册了APP_Listen协议的networkListen发送packet
    void SendPacket(Packet *pPacket);

    void Dispose() override;

  private:
    Network *GetNetwork(APP_TYPE appType);
};

#endif
