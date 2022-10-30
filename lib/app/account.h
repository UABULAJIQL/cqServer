#ifndef _ACCOUNT_H
#define _ACCOUNT_H

//login特有的类

#include "entity/playerMgr.h"
#include "thread/threadObject.h"

class Packet;

class Account : public ThreadObject {
  private:
    PlayerMgr _playerMgr;

  public:
    bool Init() override;
    void RegisterMsgFunction() override;
    bool Update() override;

    void Dispose() override;

  private:
    void HandleAccountCheck(Packet *pPacket);
    void HandleAccountCheckToHttpRs(Packet *pPacket);
};

#endif
