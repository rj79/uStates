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
    uint8_t FromState;
    void* UserData;
public: 
    MockState(IStateHandler* states, String name, StringFifo& fifo);
    void stateEnter(uint8_t fromState, void* userData);
    void stateLoop();
    void stateExit();

    String pop();
    uint8_t getFromState() const;
    void* getUserData() const;
    bool hasEvent() const;
};

#endif