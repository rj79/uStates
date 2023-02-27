#include <Arduino.h>

#include "IStateHandler.h"
#include "State.h"
#include "StateHandler.h"

const uint8_t STATE_BLINK = 1;
const uint8_t STATE_ON = 2;

class BlinkState: public State
{
private:
    long EnterTime;

public:
    BlinkState(IStateHandler* states) :
        State(states)
    {
        // Empty
    }

    void stateEnter()
    {
        EnterTime = millis();
    }

    void stateLoop()
    {
        if (millis() - EnterTime > 5000) {
            States->requestState(STATE_BLINK);
        }
    };

};

class OnState: public State
{ 
private:
    long EnterTime;

public:
    OnState(IStateHandler* states) :
        State(states) 
    {
        // Empty
    }

    void stateEnter() {
        EnterTime = millis();
    }

    void stateLoop() {
        if (millis() - EnterTime > 5000) {
            States->requestState(STATE_BLINK);
        }
    };
};

StateHandler States;
BlinkState StateBlink(&States);
OnState StateOn(&States);
long LastSwitch;

void setup()
{
    States.addState(STATE_BLINK, &StateBlink);
    States.addState(STATE_ON, &StateOn);
    LastSwitch = millis();
    States.requestState(STATE_BLINK);
}

void loop()
{
    States.loop();
}