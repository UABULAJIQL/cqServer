#include "epoll/epollClient.h"
#include "epoll/epollServer.h"
#include "network/packet.h"
#include <iostream>

void EpollServerTest() {
    EpollServer e;

    e.Listen(4663, "");

    while (1)
        if (!e.Update())
            break;

    // EpollServer中epoll树释放
    e.Dispose();
}

void EpollClientTest() {

    EpollClient e;
    e.Connect("127.0.0.1", 4663);

    if (e.IsConnected())
        std::cout << "连接成功" << std::endl;
    else
        std::cout << "连接失败" << std::endl;

    Packet p(10);
    p.AddData("hello", sizeof("hello"));

    while (1) {
        e.AddPacket(&p);

        if (!e.Update())
            break;
        sleep(1);
    }

    e.Dispose();
}

int main() {
    std::cout << "启动" << std::endl;

    EpollClientTest();

    std::cout << "结束" << std::endl;

    return 0;
}
