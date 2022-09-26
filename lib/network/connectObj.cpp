#include "connectObj.h"
#include "buffer/networkBuffer.h"
#include "network.h"

ConnectObj::ConnectObj(Network *network, int socket)
    : _socket(socket), _network(network) {
        _recvBuf = new RecvNetworkBuffer();
        _sendBuf = new SendNetworkBuffer();
    }

int ConnectObj::GetSocket() const { return _socket; }

bool ConnectObj::HasRecvData() const {

    if (_recvBuf->UnavailableLength() > 0)
        return true;

    return false;
}

bool ConnectObj::HasSendData() const {

    if (_sendBuf->UnavailableLength() > 0)
        return true;

    return false;
}

bool ConnectObj::Recv() const {

    char *dataTemp = nullptr;
    unsigned int len = 0;
    int size = 0;

    while (true) {
        len = _recvBuf->GetBuffer(dataTemp);
        // std::cout << "是不是这里阻塞了" << std::endl;
        size = ::recv(_socket, dataTemp, len, 0);
        // std::cout << "果然是" << std::endl;

        // std::cout << "剩余空间大小" << len << std::endl;

        if (size > 0) {

            _recvBuf->ChangeEndIndex(size);

        } else if (size == 0) {
            //断开连接了
            return false;
        } else {
            //中断错误 || 发送缓冲区满了 ||
            //下次或许可以成功一般出现在非阻塞的操作
            if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
                // std::cout << "重新" << std::endl;
                return true;
            }

            std::cout << "有错误 错误码" << errno << std::endl;
            return false;
        }
    }
}

bool ConnectObj::Send() const {
    char *dataTemp = nullptr;
    int len;
    int size;
    while (true) {
        len = _sendBuf->GetBuffer(dataTemp);
        if (len == 0) {

            // std::cout << "长度为0" << std::endl;
            return true;
        }

        size = ::send(_socket, dataTemp, len, 0);

        if (size > 0) {
            // std::cout << "发送了消息" << std::endl;
            _sendBuf->ChangeBeginIndex(size);
            //下帧送达
            if (size < len) {
                std::cout << "下帧送达" << std::endl;

                return true;
            }
        }

        if (size == -1) {
            std::cout << "needSendSize:" << len << " error:" << errno
                << std::endl;
            return false;
        }
    }
}

RecvNetworkBuffer *ConnectObj::GetRecvNetworkBuffer() const { return _recvBuf; }
SendNetworkBuffer *ConnectObj::GetSendNetworkBuffer() const { return _sendBuf; }

void ConnectObj::Dispose() {
    _recvBuf->Dispose();
    _sendBuf->Dispose();

    if (_recvBuf != nullptr) {
        delete _recvBuf;
        _recvBuf = nullptr;
    }
    if (_sendBuf != nullptr) {
        delete _sendBuf;
        _sendBuf = nullptr;
    }

    //这个可不是你可以释放的
    _network = nullptr;
}
