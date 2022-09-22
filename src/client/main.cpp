#include <iostream>
#include "network/epoll/epollClient.h"
#include "packet/packet.h"

using namespace std;

int main(int argc, char *argv[]) {
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

    return 0;
}
