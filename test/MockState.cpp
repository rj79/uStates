#include "MockState.h"

MockState::MockState(IStateHandler* stateHandler, String name, StringFifo& fifo) :
    State(stateHandler),
    Name(name),
    Fifo(fifo)
{
    // Empty
}

void MockState::stateEnter()
{
    Fifo.Push(Name + "::stateEnter");
}

void MockState::stateLoop()
{
    Fifo.Push(Name + "::stateLoop");    
}

void MockState::stateExit()
{
    Fifo.Push(Name + "::stateExit");    
}

String MockState::pop()
{
    return Fifo.Pop();    
}

bool MockState::hasEvent() const
{
    return !Fifo.IsEmpty();
}