#include "connectObj.h"
#include "buffer/networkBuffer.h"
#include "network.h"
#include "thread/threadMgr.h"

ConnectObj::ConnectObj(Network *network, int socket)
    : _socket(socket), _network(network) {
    _recvBuf = new RecvNetworkBuffer(this);
    _sendBuf = new SendNetworkBuffer(this);
}

int ConnectObj::GetSocket() const { return _socket; }

// 这里真的是操蛋 要判断是否满足一个packet
bool ConnectObj::HasRecvData() const {

    unsigned short totalSizeTypeSize = sizeof(TotalSizeType);

    if (_recvBuf->UnavailableLength() <= totalSizeTypeSize)
        return false;

    unsigned int rightSize = 0;
    if (_recvBuf->GetEndIndex() < _recvBuf->GetBeginIndex())
        rightSize = _recvBuf->GetBufferSize() - _recvBuf->GetBeginIndex();
    else if (_recvBuf->GetEndIndex() > _recvBuf->GetBeginIndex())
        rightSize = _recvBuf->GetEndIndex() - _recvBuf->GetBeginIndex();

    // 数据包总长度
    unsigned short totalSize = 0;
    char *beginBuffer = _recvBuf->GetBuffer() + _recvBuf->GetBeginIndex();

    if (rightSize >= totalSizeTypeSize) {
        ::memcpy(&totalSize, beginBuffer, totalSizeTypeSize);
    } else {
        ::memcpy(&totalSize, beginBuffer, rightSize);
        ::memcpy(&totalSize + rightSize, _recvBuf->GetBuffer(),
                 totalSizeTypeSize - rightSize);
    }

    if (_recvBuf->UnavailableLength() >= totalSize) {
        // std::cout << "够" << totalSize << "啊！！！！" << std::endl;
        return true;
    }

    return false;
}

bool ConnectObj::HasSendData() const {

    if (_sendBuf->UnavailableLength() > 0)
        return true;

    return false;
}

bool ConnectObj::Recv() const {
    // std::cout << "来消息了" << std::endl;

    char *dataTemp = nullptr;
    unsigned int len = 0;
    int size = 0;

    bool rs = false;

    while (true) {

        // 在这个地方扩容
        if (_recvBuf->AvailableLength() < sizeof(TotalSizeType)) {
            if (!_recvBuf->ExpansionBuffer()) {
                std::cout << "接收扩容失败" << std::endl;
            } else {
                std::cout << "接收扩容成功" << std::endl;
            }
        }

        len = _recvBuf->GetBuffer(dataTemp);
        size = ::recv(_socket, dataTemp, len, 0);

        // std::cout << "剩余空间大小" << len << std::endl;

        if (size > 0) {

            _recvBuf->ChangeEndIndex(size);

        } else if (size == 0) {
            // 断开连接了
            break;
        } else {
            // 中断错误 || 发送缓冲区满了 ||
            // 下次或许可以成功一般出现在非阻塞的操作
            if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
                // std::cout << "重新" << std::endl;
                rs = true;
                break;
            }

            std::cout << "有错误 错误码" << errno << std::endl;
            break;
        }
    }

    if (rs) {
        while (HasRecvData()) {
            const auto pPacket = _recvBuf->GetPacket();
            if (pPacket == nullptr)
                break;
            // std::cout << "广播消息" << std::endl;
            ThreadMgr::GetInstance()->AddPacket(pPacket);
        }
    }

    return rs;
}

// int totalSize = 0;
// int msgNum = 0;

bool ConnectObj::Send() const {
    char *dataTemp = nullptr;
    int len;
    int size;
    bool rs = false;
    while (true) {
        len = _sendBuf->GetBuffer(dataTemp);
        if (len == 0) {

            // std::cout << "长度为0" << std::endl;
            rs = true;
            break;
        }

        size = ::send(_socket, dataTemp, len, 0);

        if (size > 0) {
            // totalSize += size;
            // ++msgNum;
            // std::cout << "发送了" << totalSize << "长度的消息 这是第" <<
            // msgNum
            //     << "条消息" << std::endl;
            _sendBuf->ChangeBeginIndex(size);
            // 下帧送达
            if (size < len) {
                std::cout << "下帧送达" << std::endl;

                rs = true;
                break;
            }
        }

        if (size == -1) {
            std::cout << "needSendSize:" << len << " error:" << errno
                      << std::endl;
            break;
        }
    }

    return rs;
}

bool ConnectObj::AddPacket(Packet *packet) {
    return _sendBuf->AddPacket(packet);
}
Packet *ConnectObj::GetPacket() { return _recvBuf->GetPacket(); }

// RecvNetworkBuffer *ConnectObj::GetRecvNetworkBuffer() const { return
// _recvBuf; } SendNetworkBuffer *ConnectObj::GetSendNetworkBuffer() const {
// return _sendBuf; }

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

    // 这个可不是你可以释放的
    _network = nullptr;
}
void ConnectObj::Close() { _isClose = true; }
bool ConnectObj::IsClose() const { return _isClose; }
