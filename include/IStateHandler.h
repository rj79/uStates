#ifndef ISTATE_HANDLER_H
#define ISTATE_HANDLER_H

#include <stdint.h>

class IStateHandler
{
public:
    virtual ~IStateHandler() { /* Empty */ };
    virtual bool requestState(uint8_t state_id) = 0;
};

#endif
