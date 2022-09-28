#include "robot.h"
bool Robot::Init() {
    if (!EpollClient::Init())
        return false;

    this->Connect("127.0.0.1", 2233);
    return true;
}
void Robot::RegisterMsgFunction() { EpollClient::RegisterMsgFunction(); }

bool Robot::Update() { return EpollClient::Update(); }
