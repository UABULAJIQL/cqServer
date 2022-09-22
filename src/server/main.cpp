#include "network/epoll/epollServer.h"
#include <iostream>


int main() {
    EpollServer e;

    e.Listen(4399, "");

    while (1)
        if (!e.Update())
            break;

    // EpollServer中epoll树释放
    e.Dispose();
    return 0;
}
