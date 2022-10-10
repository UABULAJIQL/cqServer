#ifndef _SOCKETOBJ_H
#define _SOCKETOBJ_H

class ISocketObject {
  public:
    virtual ~ISocketObject() = default;
    virtual int GetSocket() = 0;
};

#endif
