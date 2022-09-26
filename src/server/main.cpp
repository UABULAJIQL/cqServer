// #include "network/epoll/epollServer.h"
#include <iostream>

#include "server.h"
#include <signal.h>

bool isRun = true;
void stop(int signo){isRun = false;}

int main() {
    /* EpollServer e;

    e.Listen(4399, "");

    while (1)
        if (!e.Update())
            break;

    // EpollServer中epoll树释放
    e.Dispose(); */
    signal(SIGINT, stop);
    Server server;
    if(!server.Listen(4399, "")){
        std::cout << "服务器启动失败" << std::endl;
        return 1;
    }

    while(isRun){
        server.Update();
        server.DataHandler();
    }
    server.Dispose();
    std::cout << "服务器正常退出" << std::endl;

    return 0;
}
