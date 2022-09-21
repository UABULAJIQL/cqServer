#include "packet.h"
#include <cstring>
#include <iostream>

Packet::Packet(unsigned int size) : Buffer(size) {}
Packet::Packet() : Buffer(PACKET_DEFAULT_SIZE) {}

char *Packet::GetBuffer() const { return _buffer; }

void Packet::ChangeEndInedx(int size) { _endIndex += size; }

int Packet::GetMessgeId() const { return _msgId; }

void Packet::SetMessgeId(int msgId) { _msgId = msgId; }
