//线程管理类
#include "threadMgr.h"
#include "thread.h"
#include "threadObject.h"
#include <iostream>

ThreadMgr::ThreadMgr() {}

void ThreadMgr::StartAllThread() {
    for (auto bIter = _threads.begin(), eIter = _threads.end(); bIter != eIter;
            ++bIter)
        bIter->second->Start();
}

bool ThreadMgr::IsGameLoop() {

    //只要有一个线程还在运行那么就说明还没结束
    for (auto bIter = _threads.begin(), eIter = _threads.end(); bIter != eIter;
            ++bIter)
        if (bIter->second->IsRun())
            return true;

    return false;
}

void ThreadMgr::NewThread() {
    std::lock_guard<std::mutex> guard(_mutex);
    auto pThread = new Thread();
    _threads.emplace(pThread->GetSN(), pThread);
    // _threads.insert(std::make_pair(pThread->GetSN(), pThread));
}

void ThreadMgr::AddObjToThread(ThreadObject *obj) {
    std::lock_guard<std::mutex> guard(_mutex);
    if (!obj->Init()) {
        std::cout << "AddObjToThread 失败 obj没有初始化" << std::endl;
        return;
    }

    auto iter = _threads.begin();
    if (_lastThreadSn > 0) {
        iter = _threads.find(_lastThreadSn);
    }

    //要么没找到 要么一开始就是空的
    if (iter == _threads.end()) {
        std::cout << "AddObjToThread iter为空" << std::endl;
        return;
    }

    do {
        ++iter;
        if (iter == _threads.end())
            iter = _threads.begin();

    } while (!iter->second->IsRun());

    auto pThread = iter->second;
    pThread->AddThreadObj(obj);
    _lastThreadSn = iter->first;
    // _lastThreadSn = pThread.GetSN();
}
void ThreadMgr::Dispose() {

    auto iter = _threads.begin();
    while (iter != _threads.end()) {
        Thread *pThread = iter->second;
        pThread->Dispose();
        delete pThread;
        ++iter;
    }
}

void ThreadMgr::AddPacket(Packet *pPacket) {
    std::lock_guard<std::mutex> guard(_mutex);
    // std::cout << "线程管理层" << std::endl;
    for(auto & thread : _threads){
        thread.second->AddPacket(pPacket);
    }


}
