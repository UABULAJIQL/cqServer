#ifndef _BUFFER_H
#define _BUFFER_H

#include "tools/disposable.h"

//默认_begin都是从0开始的顺序表
//小测试ok

//最大容量10kb
const unsigned int MAX_SIZE = 1024 * 10;
//默认扩容增量
const unsigned int INCREMENTAL_SIZE = 8;

//传输的数据长度类型
using TotalSizeType = unsigned short;

class Buffer : public IDisposable {
    protected:
        //缓冲区
        char *_pBuffer{nullptr};
        //缓冲区总大小
        unsigned int _bufferSize{0};
        //数据起始位置
        unsigned int _beginIndex{0};
        //数据结尾位置
        unsigned int _endIndex{0};

    public:
        //暂时移动到这边 方便测试数据
        // char *_buffer{nullptr};
        Buffer(unsigned int size);
        unsigned int GetBufferSize() const;
        unsigned int GetBeginIndex() const;
        unsigned int GetEndIndex() const;

        //还可用空间的长度 从0开始的顺序表
        virtual unsigned int AvailableLength() const;
        //不可用/已用 的空间长度
        unsigned int UnavailableLength() const;

        //判空
        bool IsEmpty() const;
        //判满
        bool IsFull() const;

        //扩容
        virtual bool ExpansionBuffer(unsigned int size = INCREMENTAL_SIZE);

        //添加数据
        virtual bool AddData(const char *data, unsigned int size);
        //获取数据
        virtual bool GetData(char *data, unsigned int size);


        //清空数据
        void Clear();

        void Dispose() override;
        //默认析构会把指针变量释放了 所以要使用这个类的时候只能创建实例
        virtual ~Buffer() = default;

    protected:
    //内存拷贝
    bool Memcpy(char *destination, unsigned int originIndex, unsigned int size);

};

#endif
