#ifndef _PACKET_H
#define _PACKET_H
#include "buffer/buffer.h"

//这个东西非常重要 还不太理解到时候一点要好好研究一下 不搞一下接收的数据会出乱
#pragma pack(push)
#pragma pack(4)

struct PacketHead {
    unsigned short _msgId{0};
};

#pragma pack(pop)

const unsigned int PACKET_DEFAULT_SIZE = 64;

class Packet : public Buffer {
    private:
        //消息id
        int _msgId{0};

    public:
        explicit Packet(unsigned int size);
        Packet();

        //获取缓冲区地址
        char *GetBuffer() const;
        //改变尾部索引的值
        void ChangeEndInedx(int size);
        //获取消息ID
        int GetMessgeId() const;
        void SetMessgeId(int msgId);
        //Packet一般不会去调用Dispose吧所以还是使用析构来释放
        ~Packet();
};

#endif
