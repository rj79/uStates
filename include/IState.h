#ifndef ISTATE_H
#define ISTATE_H

#include <Arduino.h>

class IState
{
public:
    virtual ~IState() { /* Empty */ };
    
    virtual void stateEnter() = 0;
    virtual void stateLoop() = 0;
    virtual void stateExit() = 0;

    virtual const String toString() const = 0;
};

#endif
