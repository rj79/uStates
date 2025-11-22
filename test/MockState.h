#ifndef MOCKSTATE_H
#define MOCKSTATE_H

#include "State.h"
#include "StringFifo.h"

class MockState: public State
{
private:
    IStateHandler* StateHandler;
    String Name;
    StringFifo& Fifo;
public: 
    MockState(IStateHandler* states, String name, StringFifo& fifo);
    void stateEnter();
    void stateLoop();
    void stateExit();
    const String toString() const;

    String pop();
    bool hasEvent() const;
};

#endif