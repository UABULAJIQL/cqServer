#include "loginApp.h"
#include "testThreadObj.h"
#include "thread/threadMgr.h"
LoginApp::LoginApp() : ServerApp(APP_TYPE::APP_LOGIN) {}
void LoginApp::InitApp() {
    //添加了一个监听包裹类
    AddListenerToThread("", 2233);
    _pThreadMgr->AddObjToThread(new TestThreadObj());
}
