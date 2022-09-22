#include "networkConnector.h"
#include "connectObj.h"
#include "buffer/networkBuffer.h"

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

    return true;
}

bool NetworkConnector::IsConnected() const {
    if (_connectObj != nullptr)
        return true;
    return false;
}

ConnectObj *NetworkConnector::GetConnectObj() const { return _connectObj; }

bool NetworkConnector::AddPacket(Packet *p) {
    return _connectObj->GetSendNetworkBuffer()->AddPacket(p);
}

Packet *NetworkConnector::GetPacket() {
    return _connectObj->GetRecvNetworkBuffer()->GetPacket();
}

void NetworkConnector::Dispose(){
    Network::Dispose();

    _connectObj->Dispose();

    if(_connectObj != nullptr){
        delete _connectObj;
        _connectObj = nullptr;
    }

}

