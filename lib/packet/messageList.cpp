#include "messageList.h"
#include "thread/threadMgr.h"
#include <iostream>

// 向待处理packet集合中添加packet
void MessageCallBackFunctionInfo::AddPacket(Packet *pPacket) {
    // std::cout << "消息包添加到集合" << std::endl;
    std::lock_guard<std::mutex> guard(_msgMutex);
    _msgList.push_back(pPacket);
}

void MessageCallBackFunction::RegisterFunction(int msgId,
                                               HandleFunction function) {
    std::lock_guard<std::mutex> guard(_msgMutex);
    // 注册添加序列号 和绑定指定的处理函数
    _callbackHandle[msgId] = function;
}

bool MessageCallBackFunction::IsFollowMsgId(Packet *packet) {
    std::lock_guard<std::mutex> guard(_msgMutex);
    // 判断该消息集合实例管不管这个消息序列号
    return _callbackHandle.find(packet->GetMessgeId()) != _callbackHandle.end();
}

void MessageCallBackFunction::ProcessPacket() {
    std::list<Packet *> tmpList;

    {
        std::lock_guard<std::mutex> guard(_msgMutex);
        std::copy(_msgList.begin(), _msgList.end(),
                  std::back_inserter(tmpList));
        // 清空
        _msgList.clear();
    }

    // 便利 执行相应的对应的处理函数
    for (auto &pPacket : tmpList) {
        const auto handleIter = _callbackHandle.find(pPacket->GetMessgeId());
        if (handleIter == _callbackHandle.end()) {
            std::cout << "packet is not hander. msg id;"
                      << pPacket->GetMessgeId() << std::endl;
        } else {
            // 调用处理函数
            // 将packet分发之后怎么释放packet的内存？如何判断改接收到的都已经处理了，好像接收的都是同一个消息的地址
            handleIter->second(pPacket);
        }
    }

    tmpList.clear();
}

void MessageList::Dispose() {
    if (_pCallBackFuns != nullptr) {
        delete _pCallBackFuns;
        _pCallBackFuns = nullptr;
    }
}

void MessageList::AttachCallBackHandler(
    MessageCallBackFunctionInfo *pCallback) {
    _pCallBackFuns = pCallback;
}

bool MessageList::IsFollowMsgId(Packet *packet) const {
    if (_pCallBackFuns == nullptr)
        return false;

    return _pCallBackFuns->IsFollowMsgId(packet);
}

void MessageList::ProcessPacket() const {
    if (_pCallBackFuns == nullptr)
        return;

    _pCallBackFuns->ProcessPacket();
}

void MessageList::AddPacket(Packet *pPacket) const {
    if (_pCallBackFuns == nullptr) {
        std::cout << "add packet failed. _pCallBackFuns == nullptr";
        return;
    }
    _pCallBackFuns->AddPacket(pPacket);
}

void MessageList::DispatchPacket(Packet *pPacket) {
    ThreadMgr::GetInstance()->DispatchPacket(pPacket);
}

void MessageList::SendPacket(Packet *pPacket) {
    ThreadMgr::GetInstance()->SendPacket(pPacket);
}
