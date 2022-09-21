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

    int result;
    result = bind(_socket, (struct sockaddr *)&si, sizeof(si));
    if (result == -1)
        perror("错误 bind\n");

    //设置监听
    result = listen(_socket, 99);
    if (result == -1)
        perror("错误 listne\n");

    //设置端口复用解除文件描述符的阻塞
    SetSocketOpt(_socket);

    return false;
}

bool NetworkListen::CreateConnectObj(SOCKET socket) {
    ConnectObj *cobj = new ConnectObj(this, _socket);
    // _connects.emplace(std::make_pair(socket, cobj));
    _connects.emplace(socket, cobj);

    //这个返回值再说----------
    return false;
}
