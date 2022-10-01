#include "thread.h"
#include "packet/packet.h"
#include "threadObject.h"

#include <iostream>

Thread::Thread() {
    //默认启动
    this->_isRun = true;
    _tempObjList.clear();
}

bool Thread::Start() {
    _isRun = true;
    _thread = std::thread([this]() {
            while (_isRun)
            Update();
            });

    return true;
}

void Thread::Stop() {
    //结束线程
    if (_isRun) {
        _isRun = false;
        if (_thread.joinable())
            _thread.join();
    }
}

bool Thread::IsRun() const { return _isRun; }

void Thread::Update() {
    //先将所有的包裹类都取出 放入到临时集合中
    _mutex.lock();
    std::copy(_objlist.begin(), _objlist.end(),
            std::back_inserter(_tempObjList));
    _mutex.unlock();

    //便利执行包裹类中的 帧函数Update
    for (ThreadObject *pThreadObj : _tempObjList) {
        pThreadObj->Update();
        pThreadObj->ProcessPacket();

        //如果不活跃了 移除
        if (!pThreadObj->IsActive()) {
            _mutex.lock();
            _objlist.remove(pThreadObj);
            _mutex.unlock();

            pThreadObj->Dispose();
            delete pThreadObj;
        }
    }

    _tempObjList.clear();

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void Thread::AddThreadObj(ThreadObject *threadObj) {
    threadObj->RegisterMsgFunction();

    std::lock_guard<std::mutex> guard(_mutex);
    _objlist.push_back(threadObj);
}

void Thread::AddPacket(Packet *pPacket) {
    // std::cout << "线程层" << std::endl;
    std::lock_guard<std::mutex> guard(_mutex);
    for (auto &obj : _objlist) {
        if (obj->IsFollowMsgId(pPacket->GetMessgeId())){

            obj->AddPacket(pPacket);
        }
    }
}

void Thread::Dispose() {

    std::list<ThreadObject *>::iterator iter = _objlist.begin();
    while (iter != _objlist.end()) {
        (*iter)->Dispose();
        delete (*iter);
        iter = _objlist.erase(iter);
    }

    _objlist.clear();
}
