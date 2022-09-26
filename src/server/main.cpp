#include <iostream>

#include "server.h"
#include <signal.h>

bool isRun = true;
void stop(int signo){isRun = false;}

int main() {
    signal(SIGINT, stop);
    Server server;
    if(!server.Listen(4399, "")){
        std::cout << "服务器启动失败" << std::endl;
        return 1;
    }else{
        std::cout << "服务器启动成功" << std::endl;
    }

    while(isRun){
        server.Update();
        server.DataHandler();
    }
    server.Dispose();
    std::cout << "服务器正常退出" << std::endl;

    return 0;
}
