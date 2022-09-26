#include "circularQueueBuffer.h"
#include <cstring>
#include <iostream>
CircularQueueBuffer::CircularQueueBuffer(unsigned int size) : Buffer(size) {}

unsigned int CircularQueueBuffer::AvailableLength() const {

    if (_beginIndex < _endIndex)
        return _beginIndex + (_bufferSize - _endIndex);
    else if (_beginIndex > _endIndex)
        return _beginIndex - _endIndex;

    //如果是添加数据后那么是满可用空间就为0 反之则反之
    //满了
    if (_addData)
        return 0;
    else //空的
        return _bufferSize;
}

bool CircularQueueBuffer::ExpansionBuffer(unsigned int size) {

    if (_bufferSize >= MAX_SIZE) {
        std::cout << "超出最大大小 扩容失败" << std::endl;
        return false;
    }

    //开辟新buffer空间
    char *newBuffer = new char[_bufferSize + size];
    //清空垃圾数据
    ::memset(newBuffer, 0, _bufferSize + size);

    //起始下标在尾部下标左边
    if (_beginIndex < _endIndex) {
        // std::cout << "起始下标在尾部下标左边" << std::endl;
        unsigned int dataLength = _endIndex - _beginIndex + 1;

        ::memcpy(newBuffer, _buffer + _beginIndex, dataLength);

        //更新尾部下标
        _endIndex = dataLength - 1;

        //起始下标在尾部下标右边
    } else if (_beginIndex > _endIndex) {
        // std::cout << "起始下标在尾部下标右边" << std::endl;
        unsigned int rightDataLength = _bufferSize - _beginIndex;
        unsigned int leftDataLenght = _endIndex + 1;

        ::memcpy(newBuffer, _buffer + _endIndex, rightDataLength);
        ::memcpy(newBuffer + rightDataLength, _buffer, leftDataLenght);

        //更新尾部下标
        _endIndex = rightDataLength + leftDataLenght - 1;
    }

    //释放原来缓冲区
    delete[] _buffer;

    //更新缓冲区
    _buffer = newBuffer;
    //更新起始下标
    _beginIndex = 0;
    //更新最大长度
    _bufferSize += size;

    return true;
}

bool CircularQueueBuffer::AddData(const char *data, unsigned int size) {

    bool whetherToExpand = false;
    //如果剩余空间不够需要扩容
    while (true) {
        //空间足够
        if (AvailableLength() >= size) {
            //不用扩容----

            break;
        } else {
            
            std::cout << "需要扩容 扩容前容量大小:" << this->_bufferSize << std::endl;
            //需要扩容---- 为了安全加一
            //扩容失败也要退出
            if (!ExpansionBuffer(size - AvailableLength() + 1)) {
                return false;
            }
            //扩容成功----
            std::cout << "扩容成功 容量大小" << this->_bufferSize << std::endl;

            //有扩容
            whetherToExpand = true;
        }
    }

    //没有扩容 并且_endIndex右边的空间大小不够的时候
    if (!whetherToExpand && (_bufferSize - _endIndex) < size) {

        unsigned int rightSpaceLength = _bufferSize - _endIndex;
        //先将一部分数据存储到右边可用空间
        ::memcpy(_buffer + _endIndex, data, rightSpaceLength);

        ::memcpy(_buffer, data + rightSpaceLength, size - rightSpaceLength);
        _endIndex = size - rightSpaceLength;

    } else { //直接塞到_endIndex后面

        ::memcpy(_buffer + _endIndex, data, size);
        _endIndex += size;
    }

    //右边刚好放满数据 那没尾部下标需要重置为0
    if (_endIndex == _bufferSize) {
        _endIndex = 0;
    }

    //添加数据后
    _addData = true;

    return true;
}

bool CircularQueueBuffer::GetData(char *data, unsigned int size) {

    if (data == nullptr){
        // std::cout << "要插入的数据地址为空" << std::endl;
        return false;
    }

    //数据长度没有这么长
    if (UnavailableLength() < size){
        // std::cout << "剩余的数据没有那么长" << std::endl;

        return false;
    }

    unsigned int rightDataLength = _bufferSize - _beginIndex;
    if (rightDataLength < size) {

        Memcpy(data, _beginIndex, rightDataLength);

        Memcpy(data + rightDataLength, 0, size - rightDataLength);

        _beginIndex = size - rightDataLength;
    } else {

        Memcpy(data, _beginIndex, size);

        _beginIndex += size;
    }

    //起始下标越界需要重新归零
    if (_beginIndex == _bufferSize) {
        _beginIndex = 0;
    }

    _addData = false;

    return true;
}

