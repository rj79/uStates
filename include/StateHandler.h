#ifndef STATE_HANDLER_H
#define STATE_HANDLER_H

#include "IState.h"
#include "IStateHandler.h"
#include <Arduino.h>

#ifndef MAX_STATES
#define MAX_STATES 16
#endif

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

    bool requestState(uint8_t state_id);

    IState* addState(uint8_t state_id, IState* state, String name="");
    
    void setPreLoopHook(Callback);
    void setPostLoopHook(Callback);
    void loop();

private:
    IState* RequestedState;
    IState* State;
    StateEntry States[MAX_STATES];
    Callback PreLoopHook;
    Callback PostLoopHook;
};

#endif
