
#ifndef _COMMON_H
#define _COMMON_H
//类型
enum APP_TYPE {
    APP_Listen = 0,

    APP_DB_MGR = 1,
    APP_GAME_MGR = 1 << 1,
    APP_SPACE_MGR = 1 << 2,

    APP_LOGIN = 1 << 3,
    APP_GAME = 1 << 4,
    APP_SPACE = 1 << 5,
    APP_ROBOT = 1 << 6,

    APP_APPMGR = APP_GAME_MGR | APP_SPACE_MGR,

    APP_ALL = APP_DB_MGR | APP_GAME_MGR | APP_SPACE_MGR | APP_LOGIN | APP_GAME | APP_SPACE,

};

#endif
