//线程管理类
#include "threadMgr.h"
#include "thread.h"
#include "threadObject.h"
#include "network/networkListen.h"
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
    std::lock_guard<std::mutex> guard(_locator_lock);
    auto pThread = new Thread();
    _threads.emplace(pThread->GetSN(), pThread);
    // _threads.insert(std::make_pair(pThread->GetSN(), pThread));
}

bool ThreadMgr::AddObjToThread(ThreadObject *obj) {
    std::lock_guard<std::mutex> guard(_locator_lock);

    auto iter = _threads.begin();
    if (_lastThreadSn > 0) {
        iter = _threads.find(_lastThreadSn);
    }

    //要么没找到 要么一开始就是空的
    if (iter == _threads.end()) {
        std::cout << "AddObjToThread iter为空" << std::endl;
        return false;
    }

    do {
        ++iter;
        if (iter == _threads.end())
            iter = _threads.begin();

    } while (!iter->second->IsRun());

    auto pThread = iter->second;
    pThread->AddThreadObj(obj);
    _lastThreadSn = iter->first;
    // _lastThreadSn = pThread->GetSN();
    return true;
}
void ThreadMgr::Dispose() {

    ThreadObjectList::Dispose();

    auto iter = _threads.begin();
    while (iter != _threads.end()) {
        Thread *pThread = iter->second;
        pThread->Dispose();
        delete pThread;
        ++iter;
    }
}


void ThreadMgr::DispatchPacket(Packet *pPacket) {
    //主线程
    AddPacket(pPacket);

    std::lock_guard<std::mutex> guard(_locator_lock);
    //子线程
    for(auto & thread : _threads){
        thread.second->AddPacket(pPacket);
    }


}

void ThreadMgr::SendPacket(Packet *pPacket) {
    NetworkListen *pLocator =
        static_cast<NetworkListen *>(GetNetwork(APP_Listen));
    pLocator->SendPacket(pPacket);
}

void ThreadMgr::AddNetworkToThread(APP_TYPE appType, Network *pNetwork) {
    if (!AddObjToThread(pNetwork))
        return;

    std::lock_guard<std::mutex> guard(_locator_lock);
    _networkLocator[appType] = pNetwork;

}


Network *ThreadMgr::GetNetwork(APP_TYPE appType) {
    std::lock_guard<std::mutex> guard(_locator_lock);

    auto iter = _networkLocator.find(appType);
    if (iter == _networkLocator.end())
        return nullptr;

    return iter->second;
}
