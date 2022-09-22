#include "networkListen.h"
#include "network/connectObj.h"

bool NetworkListen::Listen(int prot, std::string ip = "") {
    //绑定ip和端口
    struct sockaddr_in si;
    memset(&si, 0, sizeof(sockaddr_in));

    si.sin_family = AF_INET;
    if (ip == "") {
        si.sin_addr.s_addr = htonl(INADDR_ANY);
        std::cout << "系统分配的IP" << std::endl;
    } else {
        //返回值---
        inet_pton(AF_INET, ip.c_str(), &si.sin_addr.s_addr);
    }

    si.sin_port = htons(prot);

    int rs = bind(_socket, (struct sockaddr *)&si, sizeof(si));
    if (rs == -1)
        perror("错误 bind\n");

    //设置监听
    rs = listen(_socket, 99);
    if (rs == -1)
        perror("错误 listne\n");

    //设置端口复用解除文件描述符的阻塞
    SetSocketOpt(_socket);

    return true;
}

bool NetworkListen::CreateConnectObj(SOCKET socket) {
    ConnectObj *cobj = new ConnectObj(this, socket);
    // _connects.emplace(std::make_pair(socket, cobj));
    _connects.emplace(socket, cobj);

    //非阻塞设置+++++++++++++++++++++忘记了
    SetSocketOpt(socket);

    //这个返回值再说----------
    return false;
}

void NetworkListen::Dispose() {
    Network::Dispose();
    for (auto bIter = _connects.begin(), eIter = _connects.end();
            bIter != eIter; ++bIter) {

        bIter->second->Dispose();

        if (bIter->second != nullptr) {
            delete bIter->second;
            bIter->second = nullptr;
        }
    }
    _connects.clear();
}
