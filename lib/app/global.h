// 全局类
// 记录了时间 生成序列号
#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "tools/singleton.h"
#include "tools/time.h"

#include <mutex>

class Global : public Singleton<Global> {
  public:
    // 生成序列号
    uint64_t GenerateSN();

    int YearDay;
    timeutil::Time TimeTick;

  private:
    std::mutex _mtx;
    unsigned int _snTicket{1};
    unsigned int _serverId{0};
};

#endif
