#include "MockState.h"

MockState::MockState(IStateHandler* stateHandler, String name, StringFifo& fifo) :
    State(stateHandler),
    Name(name),
    Fifo(fifo),
    FromState(0),
    UserData(nullptr)
{
    // Empty
}

void MockState::stateEnter(uint8_t fromState, void* userData)
{
    Fifo.Push(Name + "::stateEnter");
    FromState = fromState;
    UserData = userData;
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

bool MockState::hasEvent() const
{
    return !Fifo.IsEmpty();
}

uint8_t MockState::getFromState() const
{
    return FromState;
}

void* MockState::getUserData() const
{
    return UserData;
}