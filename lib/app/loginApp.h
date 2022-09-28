#ifndef _LOGINAPP_H
#define _LOGINAPP_H

#include "serverApp.h"

class LoginApp : public ServerApp {
    public:
        explicit LoginApp();
        void InitApp() override;
};

#endif
