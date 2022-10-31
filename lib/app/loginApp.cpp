#include "loginApp.h"
#include "thread/threadMgr.h"

#include "account.h"
LoginApp::LoginApp() : ServerApp(APP_TYPE::APP_LOGIN) {}
void LoginApp::InitApp() {
    // 添加了一个监听包裹实例到线程中
    AddListenerToThread("", 2233);

    // 将Account包裹实例添加到线程中
    Account *account = new Account();
    _pThreadMgr->AddObjToThread(account);
}
