// 这是一个包裹类

#ifndef _THREADOBJECT_H
#define _THREADOBJECT_H
#include "tools/disposable.h"

class ThreadObject : public IDisposable {
    public:
        // 初始函数
        virtual bool Init() = 0;
        // 注册想要的协议协议
        virtual void RegisterMsgFunction() = 0;
        // 帧函数 更新数据
        virtual bool Update() = 0;

        virtual ~ThreadObject() = default;

        bool IsActive() const;

    protected:
        bool _active{true};
};

#endif
