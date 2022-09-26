#ifndef _CIRCULARQUEUEBUFFER_H
#define _CIRCULARQUEUEBUFFER_H

#include "buffer.h"

//顺序结构循环队列
//小测试ok
class CircularQueueBuffer : public Buffer {

    protected:
        //添加数据后是true 获取数据后是false
        //为了判断缓冲区是空还是满
        bool _addData{false};

    public:
        explicit CircularQueueBuffer(unsigned int size);

        //返回还可用的空间长度
        unsigned int AvailableLength() const override;

        //数据入队
        bool AddData(const char *data, unsigned int size) override;
        //获取数据并且出队数据
        bool GetData(char *data, unsigned int size) override;

        //扩容
        bool ExpansionBuffer(unsigned int size = INCREMENTAL_SIZE) override;
};

#endif
