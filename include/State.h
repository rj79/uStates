#ifndef STATE_H
#define STATE_H

#include "IState.h"
#include "IStateHandler.h"

class State: public IState
{
protected:
    IStateHandler* States;

public:
    State(IStateHandler* states);
    void stateEnter();
    void stateLoop();
    void stateExit();
    const String toString() const;
};

#endif
