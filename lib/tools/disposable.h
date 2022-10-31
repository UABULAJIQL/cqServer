/*
   清理资源的基类
   退出的时候清理资源

*/
#ifndef _DISPOSABLE_H
#define _DISPOSABLE_H

class IDisposable {
  public:
    virtual ~IDisposable() = default;
    // 清理资源函数
    virtual void Dispose() = 0;
};

#endif
