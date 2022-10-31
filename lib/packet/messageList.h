// 要实现消息队列机制 所以需要一个存储消息的类
#ifndef _MESSAGELIST_H
#define _MESSAGELIST_H

#include "tools/disposable.h"

#include <algorithm>
#include <functional>
#include <list>
#include <map>
#include <mutex>

#include "packet/packet.h"

class Packet;

// 消息信息基类
class MessageCallBackFunctionInfo {
  public:
    virtual ~MessageCallBackFunctionInfo() = default;
    // 判断该协议号是不是自己需要关心的 是的话可以调用类似AddPacke将Packet缓存
    virtual bool IsFollowMsgId(Packet *packet) = 0;
    // 便利消息集合 执行相应的处理函数
    virtual void ProcessPacket() = 0;
    void AddPacket(Packet *pPacket);

  protected:
    std::mutex _msgMutex;
    // std::list<Packet *> _msgList;
    std::list<Packet *> _msgList;
};

// 无差别消息接收
class MessageCallBackFunction : public MessageCallBackFunctionInfo {
  public:
    using HandleFunction = std::function<void(Packet *)>;

    void RegisterFunction(int msgId, HandleFunction function);
    bool IsFollowMsgId(Packet *packet) override;
    void ProcessPacket() override;

    std::map<int, HandleFunction> &GetCallBackHandler() {
        return _callbackHandle;
    }

  protected:
    std::map<int, HandleFunction> _callbackHandle;
};

// 有条件的接收
template <class T>
class MessageCallBackFunctionFilterObj : public MessageCallBackFunction {
  public:
    using HandleFunctionWithObj = std::function<void(T *, Packet *)>;
    using SOCKET = int;

    using HandleGetObject = std::function<T *(SOCKET)>;

    void RegisterFunctionWithObj(int msgId, HandleFunctionWithObj function);
    bool IsFollowMsgId(Packet *packet) override;
    void ProcessPacket() override;

    // 返回值类型为T*参数为socket函数指针类型 条件赛选的条件函数
    // 为nullptr则过滤掉
    HandleGetObject GetPacketObject{nullptr};

    // 将无差别接收协议的map集合数据拷贝过来
    void CopyFrom(MessageCallBackFunction *pInfo);

  private:
    std::map<int, HandleFunctionWithObj> _callbackHandleWithObj;
};

// using HandleFunction = std::function<void(Packet *)>;
template <class T>
void MessageCallBackFunctionFilterObj<T>::RegisterFunctionWithObj(
    const int msgId, HandleFunctionWithObj function) {
    _callbackHandleWithObj[msgId] = function;
}

template <class T>
bool MessageCallBackFunctionFilterObj<T>::IsFollowMsgId(Packet *packet) {
    if (MessageCallBackFunction::IsFollowMsgId(packet))
        return true;

    if (_callbackHandleWithObj.find(packet->GetMessgeId()) !=
        _callbackHandleWithObj.end()) {
        if (GetPacketObject != nullptr) {
            T *pObj = GetPacketObject(packet->GetSocket());
            if (pObj != nullptr)
                return true;
        }
    }

    return false;
}

template <class T> void MessageCallBackFunctionFilterObj<T>::ProcessPacket() {
    std::list<Packet *> tmpList;
    _msgMutex.lock();
    std::copy(_msgList.begin(), _msgList.end(), std::back_inserter(tmpList));
    _msgList.clear();
    _msgMutex.unlock();

    for (auto packet : tmpList) {
        const auto handleIter = _callbackHandle.find(packet->GetMessgeId());
        if (handleIter != _callbackHandle.end()) {
            handleIter->second(packet);
            continue;
        }

        auto iter = _callbackHandleWithObj.find(packet->GetMessgeId());
        if (iter != _callbackHandleWithObj.end()) {
            if (GetPacketObject != nullptr) {
                T *pObj = GetPacketObject(packet->GetSocket());
                if (pObj != nullptr) {
                    iter->second(pObj, packet);
                }
            }
        }
    }

    tmpList.clear();
}

// 将无差别接收协议的map集合数据拷贝过来
template <class T>
void MessageCallBackFunctionFilterObj<T>::CopyFrom(
    MessageCallBackFunction *pInfo) {

    // 获取map集合
    auto copyFromData = pInfo->GetCallBackHandler();
    // 数据转移
    std::transform(copyFromData.begin(), copyFromData.end(),
                   std::back_inserter(_callbackHandle),
                   [](const std::pair<int, HandleFunction> &p) {
                       // 原封不动的返回
                       return p;
                   });
}

class MessageList : public IDisposable {
  public:
    void Dispose() override;
    void AttachCallBackHandler(MessageCallBackFunctionInfo *pCallback);
    // 判断该协议号是不是自己需要关心的 是的话可以调用类似AddPacke将Packet缓存
    bool IsFollowMsgId(Packet *packet) const;
    // 便利消息集合 执行相应的处理函数
    void ProcessPacket() const;
    // 添加Packet
    void AddPacket(Packet *pPacket) const;

    static void DispatchPacket(Packet *pPacket);
    static void SendPacket(Packet *pPacket);

  protected:
    MessageCallBackFunctionInfo *_pCallBackFuns{nullptr};
};

#endif
