#include "StateHandler.h"

const uint8_t INVALID_STATE_ID = 255;

#ifdef SERIAL_DEBUG

void serial_println(const char* fmt, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    int rc = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    Serial.println(buffer);
}

#define SERIAL_PRINTLN(...) { serial_println(__VA_ARGS__); }
#else
#define SERIAL_PRINTLN(...) ;
#endif

StateHandler::StateHandler() : State(nullptr),
                               StateId(INVALID_STATE_ID),
                               RequestedStateId(INVALID_STATE_ID),                                                              
                               StateIndex(0),
                               PreLoopHook(nullptr),
                               PostLoopHook(nullptr)
{
    for (int i = 0; i < MAX_STATES; ++i) {
        States[i].Id = INVALID_STATE_ID;
        States[i].State = nullptr;
    }
}

bool StateHandler::requestState(uint8_t state_id)
{
    if (state_id == INVALID_STATE_ID) {
        SERIAL_PRINTLN("Error: Invalid state id %d requested.", state_id);
        return false;
    }
    for (int i = 0; i < MAX_STATES; ++i) {
        if (States[i].Id == state_id) {
            RequestedStateId = state_id;
            StateIndex = i;
            return true;
        }
    }
    return false;
}

IState* StateHandler::addState(uint8_t state_id, IState *state, String name)
{
    if (state_id == INVALID_STATE_ID) {
        SERIAL_PRINTLN("Error: Invalid state id %d requested.", state_id);
        return nullptr;
    }
    
    for (int i = 0; i < MAX_STATES; ++i) {
        if (States[i].Id == state_id) {
            SERIAL_PRINTLN("Error: id %d already added", state_id);
            return nullptr;
        }
    }
    
    for (int i = 0; i < MAX_STATES; ++i) {
        if (States[i].State == nullptr) {
            States[i].Id = state_id;
            States[i].State = state;
            States[i].Name = name;
            return state;
        }
    }
    return nullptr;
}

uint8_t StateHandler::getStateId() const
{
    return StateId;
}

void StateHandler::setPreLoopHook(Callback callback)
{
    PreLoopHook = callback;
}

void StateHandler::setPostLoopHook(Callback callback)
{
    PostLoopHook = callback;
}

void StateHandler::loop()
{
    if (RequestedStateId == INVALID_STATE_ID) {
        SERIAL_PRINTLN("Error: No state requested.");
        return;
    }

    if (PreLoopHook != nullptr) {
        PreLoopHook();
    }

    if (RequestedStateId != StateId) {
        if (State != nullptr) {
            SERIAL_PRINTLN("Exiting state \"%s\"", State->toString().c_str());
            State->stateExit();
        }
        State = States[StateIndex].State;
        if (State != nullptr) {
            SERIAL_PRINTLN("Entering state \"%s\"", State->toString().c_str());
            StateId = RequestedStateId;
            State->stateEnter();
        }
    }

    if (State != nullptr) {
        State->stateLoop();
    }

    if (PostLoopHook != nullptr) {
        PostLoopHook();
    }
}

