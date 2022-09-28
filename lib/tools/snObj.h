#ifndef _SNOBJ_H
#define _SNOBJ_H

#include "app/global.h"

class SnObject {
    public:
        virtual ~SnObject() {}

        SnObject() { _sn = Global::GetInstance()->GenerateSN(); }

        SnObject(uint64_t sn) { _sn = sn; }

        uint64_t GetSN() const { return _sn; }

    protected:
        uint64_t _sn;
};

#endif
