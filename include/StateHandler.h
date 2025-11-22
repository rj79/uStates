#ifndef STATE_HANDLER_H
#define STATE_HANDLER_H

#include "IState.h"
#include "IStateHandler.h"
#include <Arduino.h>

#ifndef MAX_STATES
#define MAX_STATES 16
#endif

const uint8_t UNDEFINED_STATE = 255;

typedef void (*Callback)(void);

struct StateEntry
{
    uint8_t Id;
    IState* State;
    String Name;
};

class StateHandler : public IStateHandler
{
public:
    StateHandler();

    bool requestState(uint8_t state_id) override;
    bool requestLastState() override;

    IState* addState(uint8_t state_id, IState* state, String name="");
    uint8_t getStateId() const;
    void setPreLoopHook(Callback);
    void setPostLoopHook(Callback);
    void loop();    

private:
    IState* State;
    uint8_t StateId;
    uint8_t RequestedStateId;
    uint8_t LastStateId;
    uint8_t StateIndex;    
    StateEntry States[MAX_STATES];
    Callback PreLoopHook;
    Callback PostLoopHook;
};

#endif
