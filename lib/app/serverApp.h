//这个类是各个服务类的基类
#ifndef _SERVERAPP_H
#define _SERVERAPP_H
#include "tools/common.h"
#include "tools/disposable.h"

#include <string>

class ThreadMgr;

//公共需要的功能可以单独写一个模板函数
template <class APPClass> inline int MainTemplate() {

    APPClass *pApp = new APPClass();
    pApp->InitApp();
    pApp->StartAllThread();
    pApp->Run();
    pApp->Dispose();
    delete pApp;
    return 0;
}

class ServerApp : public IDisposable {
    public:
        ServerApp(APP_TYPE appType);
        ~ServerApp();

        virtual void InitApp() = 0;
        void Dispose() override;

        void StartAllThread() const;

        //启动
        void Run() const;

        //更新当前帧的时间 保存到全局单例类中
        void UpdateTime() const;

        bool AddListenerToThread(std::string ip, int port) const;

    protected:
        ThreadMgr *_pThreadMgr;
        APP_TYPE _appType;
};

#endif
