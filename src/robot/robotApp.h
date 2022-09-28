#ifndef _ROBOTAPP_H
#define _ROBOTAPP_H
#include "app/serverApp.h"

class RobotApp : public ServerApp {
    public:
        RobotApp();
        void InitApp() override;

};

#endif
