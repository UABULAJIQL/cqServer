#include "networkConnector.h"
#include "buffer/networkBuffer.h"
#include "connectObj.h"
#include "packet/packet.h"

bool NetworkConnector::Connect(std::string ip, int port) {

    _ip = ip;
    _port = port;

    if (_port == 0 || _ip == "")
        return false;

    if (_socket == -1)
        return false;

    sockaddr_in addr;
    memset(&addr, 0, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    ::inet_pton(AF_INET, _ip.c_str(), &addr.sin_addr.s_addr);

    int rs = ::connect(_socket, (struct sockaddr *)&addr, sizeof(sockaddr_in));
    if (rs == 0) {
        _connectObj = new ConnectObj(this, _socket);
    }

    // 设置非阻塞 已经被这个问题搞了两次了 服了
    SetSocketOpt(_socket);

    return true;
}

bool NetworkConnector::IsConnected() const {
    if (_connectObj != nullptr)
        return true;
    return false;
}

ConnectObj *NetworkConnector::GetConnectObj() const { return _connectObj; }

bool NetworkConnector::AddPacket(Packet *p) {
    return _connectObj->AddPacket(p);
}

Packet *NetworkConnector::GetPacket() { return _connectObj->GetPacket(); }

void NetworkConnector::Dispose() {
    Network::Dispose();

    _connectObj->Dispose();

    if (_connectObj != nullptr) {
        delete _connectObj;
        _connectObj = nullptr;
    }
}

void NetworkConnector::HandleDisconnect(Packet *pPacket) {
    std::cout << "dis connect failed. socket not find. socket:"
              << pPacket->GetSocket() << std::endl;
    return;

    _connectObj->Close();
}
