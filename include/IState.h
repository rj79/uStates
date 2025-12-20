#ifndef ISTATE_H
#define ISTATE_H

#include <Arduino.h>

class IState
{
public:
    virtual ~IState() { /* Empty */ };
    
    virtual void stateEnter(uint8_t fromState, void* userData) = 0;
    virtual void stateLoop() = 0;
    virtual void stateExit() = 0;
};

#endif
