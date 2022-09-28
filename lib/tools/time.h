//这个头文件不知道怎么添加头文件不重复的宏语句
#include <stdint.h>

namespace timeutil {

    using Time = uint64_t;
    // typedef uint64_t Time;

    inline Time AddSeconds(Time timeValue, int second) {
        return timeValue + second * 1000;
    }

    inline Time AddMilliseconds(Time timeValue, int milliseconds) {
        return timeValue + milliseconds;
    }


} // namespace timeutil
