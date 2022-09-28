#include "serverApp.h"
#include "global.h"
#include "network/epoll/epollServer.h"
#include "thread/threadMgr.h"

#include <thread>
ServerApp::ServerApp(APP_TYPE appType) {
    _appType = appType;

    //初始化各个单例类
    Global::Instance();
    _pThreadMgr = ThreadMgr::Instance();

    UpdateTime();
    //先启动三个线程
    for (int i = 0; i < 3; ++i) {
        _pThreadMgr->NewThread();
    }
}

ServerApp::~ServerApp() {
    //单例类实例销毁
    _pThreadMgr->DestroyInstance();
}

void ServerApp::Dispose() { _pThreadMgr->Dispose(); }

void ServerApp::StartAllThread() const { _pThreadMgr->StartAllThread(); }

void ServerApp::Run() const {
    bool isRun = true;
    while (isRun) {
        UpdateTime();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        isRun = _pThreadMgr->IsGameLoop();
    }
}

void ServerApp::UpdateTime() const {

    //获取当前时间 这是一个time_point<>类型
    auto timeValue = std::chrono::time_point_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now());

    //当前时间毫秒
    Global::GetInstance()->TimeTick = timeValue.time_since_epoch().count();

    //转换成time_tl类型
    auto tt = std::chrono::system_clock::to_time_t(timeValue);

    struct tm *ptm = localtime(&tt);

    //这里只记录了一个日
    Global::GetInstance()->YearDay = ptm->tm_yday;
}

bool ServerApp::AddListenerToThread(std::string ip, int port) const {
    EpollServer *pEpollServer = new EpollServer();
    if (!pEpollServer->Listen(port, ip)) {
        pEpollServer->Dispose();
        delete pEpollServer;
        return false;
    }
    _pThreadMgr->AddObjToThread(pEpollServer);
    return true;
}
