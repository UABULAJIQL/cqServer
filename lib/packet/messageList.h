//要实现消息队列机制 所以需要一个存储消息的类
#ifndef _MESSAGELIST_H
#define _MESSAGELIST_H

#include <functional>
#include <list>
#include <map>
#include <mutex>

class Packet;

using HandleFunction = std::function<void(Packet *)>;

class MessageList {
    public:
        //初始化的时候要调用该函数 注册序列号 并且指定处理函数
        void RegisterFunction(int msgId, HandleFunction function);
        //判断该协议号是不是自己需要关心的 是的话可以调用类似AddPacke将Packet缓存
        bool IsFollowMsgId(int msgId);
        //便利消息集合 执行相应的处理函数
        void ProcessPacket();
        //添加Packet
        void AddPacket(Packet *pPacket);


        static void DispatchPacket(Packet* pPacket);
        static void SendPacket(Packet* pPacket);

    protected:
        std::mutex _msgMutex;
        //待处理的Packet
        std::list<Packet *> _msgList;
        //对应的序列号会有对应的处理函数
        std::map<int, HandleFunction> _callbackHandle;
};

#endif
