#include "State.h"

State::State(IStateHandler* states) :
    States(states)
{
    // Empty
}

void State::stateEnter()
{
    // Override in subclass
}

void State::stateLoop()
{
    // Override in subclass
}

void State::stateExit()
{
    // Override in subclass
}

const String State::toString() const
{
    // Override in subclass
    return "";
}