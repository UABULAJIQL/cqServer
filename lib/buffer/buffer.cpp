#include "buffer.h"
#include <cstring>
#include <iostream>

Buffer::Buffer(unsigned int size) {
    _pBuffer = new char[size];
    ::memset(_pBuffer, 0, size);
    _bufferSize = size;
}

unsigned int Buffer::UnavailableLength() const {
    return _bufferSize - AvailableLength();
}

unsigned int Buffer::GetBufferSize() const { return _bufferSize; }

unsigned int Buffer::GetBeginIndex() const { return _beginIndex; }

unsigned int Buffer::GetEndIndex() const { return _endIndex; }

unsigned int Buffer::AvailableLength() const { return _bufferSize - _endIndex; }

bool Buffer::ExpansionBuffer(unsigned int size) {

    if (_bufferSize >= MAX_SIZE) {
        std::cout << "超出最大大小 扩容失败" << std::endl;
        return false;
    }

    char *newBuffer = new char[_bufferSize + size];
    ::memset(newBuffer, 0, (_bufferSize + size));

    //数据不为空才移动数据
    if (!IsEmpty()) {
        ::memcpy(newBuffer, _pBuffer, UnavailableLength());
    }
    delete[] _pBuffer;
    _pBuffer = newBuffer;
    _bufferSize += size;

    return true;
}

bool Buffer::IsEmpty() const {

    if (UnavailableLength() == 0) {
        return true;
    }

    return false;
}

bool Buffer::IsFull() const {
    if (AvailableLength() == 0)
        return true;
    return false;
}

bool Buffer::AddData(const char *data, unsigned int size) {

    //如果剩余空间不够需要扩容
    while (1) {
        if (AvailableLength() >= size) {
            break;
        } else {
            //需要扩容

            //扩容失败 false
            if (!ExpansionBuffer(size - AvailableLength() + 1))
                return false;
        }
    }

    //如果数据为空 从起始下标开始添加数据
    if (IsEmpty()) {
        ::memcpy(_pBuffer, data, size);
        _endIndex = size;
    } else { //数据不为空 从尾部下标开始添加数据
        ::memcpy(_pBuffer + _endIndex, data, size);
        _endIndex += size;
    }

    return true;
}

bool Buffer::GetData(char *data, unsigned int size) {
    if (data == nullptr)
        return false;

    if (_endIndex < size)
        return false;

    Memcpy(data, 0, size);

    return true;
}

bool Buffer::Memcpy(char *destination, unsigned int originIndex,
        unsigned int size) {
    if (destination == nullptr)
        return false;

    ::memcpy(destination, _pBuffer + originIndex, size);
    //强迫症还是清空一下数据
    ::memset(_pBuffer + originIndex, 0, size);
    return true;
}

void Buffer::Clear() {
    _beginIndex = 0;
    _endIndex = 0;
    ::memset(_pBuffer, 0, _bufferSize);
}

void Buffer::Dispose() {
    if (_pBuffer != nullptr) {
        delete[] _pBuffer;
        _pBuffer = nullptr;
    }
}
