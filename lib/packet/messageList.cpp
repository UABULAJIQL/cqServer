#include "messageList.h"
#include "packet.h"
#include <iostream>
void MessageList::RegisterFunction(int msgId, HandleFunction function) {
    std::lock_guard<std::mutex> guard(_msgMutex);
    //注册添加序列号 和绑定指定的处理函数
    _callbackHandle[msgId] = function;
}

bool MessageList::IsFollowMsgId(int msgId) {
    std::lock_guard<std::mutex> guard(_msgMutex);
    //判断该消息集合实例管不管这个消息序列号
    return _callbackHandle.find(msgId) != _callbackHandle.end();
}

void MessageList::ProcessPacket() {
    std::list<Packet *> tmpList;

    {
        std::lock_guard<std::mutex> guard(_msgMutex);
        std::copy(_msgList.begin(), _msgList.end(),
                std::back_inserter(tmpList));
        _msgList.clear();
    }

    //便利 执行相应的对应的处理函数
    for (auto &packet : tmpList) {
        const auto handleIter = _callbackHandle.find(packet->GetMessgeId());
        if (handleIter == _callbackHandle.end()) {
            std::cout << "packet is not hander. msg id;"
                << packet->GetMessgeId() << std::endl;
        } else {
            handleIter->second(packet);
        }
    }

    tmpList.clear();
}

//向待处理packet集合中添加packet
void MessageList::AddPacket(Packet *pPacket) {
    std::lock_guard<std::mutex> guard(_msgMutex);
    _msgList.push_back(pPacket);
}