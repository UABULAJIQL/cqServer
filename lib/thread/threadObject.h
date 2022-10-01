// 这是一个包裹类

#ifndef _THREADOBJECT_H
#define _THREADOBJECT_H
#include "tools/disposable.h"
#include "packet/messageList.h"

//每个包裹类都要有一个消息处理基类
//每个Actor模型...
class ThreadObject : public IDisposable, public MessageList {
    public:
        // 初始函数
        virtual bool Init() = 0;
        // 注册想要的协议协议
        virtual void RegisterMsgFunction() = 0;
        // 帧函数 更新数据
        virtual bool Update() = 0;

        // virtual ~ThreadObject() = default;

        bool IsActive() const;

    protected:
        bool _active{true};
};

#endif
