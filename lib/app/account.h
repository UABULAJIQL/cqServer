#ifndef _ACCOUNT_H
#define _ACCOUNT_H

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

  private:
    void HandleAccountCheck(Packet *pPacket);
    void HandleAccountCheckToHttpRs(Packet *pPacket);
};

#endif
