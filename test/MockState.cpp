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

const String MockState::toString() const
{
    std::cout << "MockState::toString" << std::endl;
    return Name;
}

String MockState::pop()
{
    return Fifo.Pop();    
}
