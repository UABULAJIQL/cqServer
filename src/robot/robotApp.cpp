#include "robotApp.h"
#include "robot.h"
#include "thread/threadMgr.h"

RobotApp::RobotApp() : ServerApp(APP_TYPE::APP_ROBOT) {}
void RobotApp::InitApp() {
    for (int i = 0; i < 10; ++i) {

        _pThreadMgr->AddObjToThread(new Robot());
    }
}
