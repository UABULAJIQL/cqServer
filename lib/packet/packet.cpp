#include "packet.h"
#include <cstring>
#include <iostream>

Packet::Packet(int id, SOCKET socket, unsigned int size) : Buffer(size) {}
char *Packet::GetBuffer() const { return _pBuffer; }

void Packet::ChangeEndInedx(int size) { _endIndex += size; }

int Packet::GetMessgeId() const { return _msgId; }

void Packet::SetMessgeId(int msgId) { _msgId = msgId; }

Packet::~Packet() { Buffer::Dispose(); }
SOCKET Packet::GetSocket() const
{
    return _socket;
}
