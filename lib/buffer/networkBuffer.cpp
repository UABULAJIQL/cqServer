#include "networkBuffer.h"
#include "packet/packet.h"
#include <cstring>
#include <iostream>

//-----Recv----
RecvNetworkBuffer::RecvNetworkBuffer(unsigned int size)
    : CircularQueueBuffer(size) {}
RecvNetworkBuffer::RecvNetworkBuffer()
    : CircularQueueBuffer(RECVBUF_DEFAULT_SIZE) {}

    //获取一个packet
    Packet *RecvNetworkBuffer::GetPacket() {
        // std::cout << "已用长度:" << this->UnavailableLength() << std::endl;
        //获取消息长度
        TotalSizeType totalSize = 0;
        this->GetData((char *)&totalSize, sizeof(TotalSizeType));

        //获取包头
        PacketHead pHead;
        this->GetData((char *)&pHead, sizeof(PacketHead));

        //防止意外多开一个大小
        Packet *packet = new Packet(totalSize + 1);
        packet->SetMessgeId(pHead._msgId);
        this->GetData(packet->GetBuffer(),
                totalSize - sizeof(TotalSizeType) - sizeof(PacketHead));

        //这里原来好像错了
        packet->ChangeEndInedx(totalSize - sizeof(TotalSizeType) -
                sizeof(PacketHead));

        // std::cout << "已用长度:" << this->UnavailableLength() << std::endl;
        return packet;
    }

unsigned int RecvNetworkBuffer::GetBuffer(char *&buf) const {

    buf = _buffer + _endIndex;

    if (_beginIndex > _endIndex)
        return _beginIndex - _endIndex;
    else if (_beginIndex < _endIndex)
        return _bufferSize - _endIndex;
    else {

        if (_addData)
            return 0;
        else
            return _bufferSize - _endIndex;
    }
}

void RecvNetworkBuffer::ChangeEndIndex(int size) {
    _endIndex += size;
    if (_endIndex == _bufferSize)
        _endIndex = 0;

    _addData = true; //添加了数据
}

//------Send--------
SendNetworkBuffer::SendNetworkBuffer(unsigned int size)
    : CircularQueueBuffer(size) {}

SendNetworkBuffer::SendNetworkBuffer()
    : CircularQueueBuffer(SENDBUF_DEFAULT_SIZE) {}

    //将packet添加到 发送缓冲区中 结构（长度 包头 包体)
    bool SendNetworkBuffer::AddPacket(Packet *packet) {

        //总的数据长度
        TotalSizeType totalSize = sizeof(TotalSizeType) + sizeof(PacketHead) +
            packet->UnavailableLength();

        //----------------------------------
        //将长度添加进去
        this->AddData((char *)&totalSize, sizeof(TotalSizeType));

        //包头
        PacketHead pHead;
        pHead._msgId = packet->GetMessgeId();

        //----------------------------------

        //添加包头
        this->AddData((char *)&pHead, sizeof(pHead));

        //添加内容
        this->AddData(packet->GetBuffer(), packet->UnavailableLength());

        // packet->ChangeEndInedx(-packet->UnavailableLength());

        return true;
    }

unsigned int SendNetworkBuffer::GetBuffer(char *&buf) const {
    buf = _buffer + _beginIndex;

    if (_beginIndex < _endIndex)
        return _endIndex - _beginIndex;
    else if (_beginIndex > _endIndex)
        return _bufferSize - _beginIndex;
    else {
        if (_addData) {
            // std::cout << "满" << std::endl;
            return _bufferSize - _beginIndex;
        } else {
            // std::cout << "空" << std::endl;
            return 0;
        }
    }
}

void SendNetworkBuffer::ChangeBeginIndex(int size) {
    //可以在这里清零
    _beginIndex += size;
    if (_beginIndex == _bufferSize)
        _beginIndex = 0;

    //由于是移除消息所以 添加消息的标记要变成false
    _addData = false;
}
