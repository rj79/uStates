#include "StateHandler.h"

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

StateHandler::StateHandler() : CurrentState(nullptr),
                               RequestedState(nullptr),
                               LastState(nullptr),
                               UserData(nullptr),
                               PreLoopHook(nullptr),
                               PostLoopHook(nullptr)
{
    for (int i = 0; i < MAX_STATES; ++i) {
        States[i].Id = UNDEFINED_STATE;
        States[i].State = nullptr;
    }
}

bool StateHandler::requestState(uint8_t state_id, void* userData)
{
    if (state_id == UNDEFINED_STATE) {
        SERIAL_PRINTLN("Error: Invalid state id %d requested.", state_id);
        return false;
    }
    for (int i = 0; i < MAX_STATES; ++i) {
        if (States[i].Id == state_id) {
            RequestedState = &States[i];
            UserData = userData;
            return true;
        }
    }
    return false;
}

bool StateHandler::requestLastState()
{
    if (LastState) {
        return requestState(LastState->Id, nullptr);
    }
    return false;
}

IState* StateHandler::addState(uint8_t state_id, IState *state, String name)
{
    if (state == nullptr) {
        SERIAL_PRINTLN("Error: State can't be nullptr.");
        return nullptr;
    }

    if (state_id == UNDEFINED_STATE) {
        SERIAL_PRINTLN("Error: Invalid state id %d.", state_id);
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
    if (CurrentState) {
        return CurrentState->Id;
    }
    return UNDEFINED_STATE;
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
    if (RequestedState == nullptr) {
        SERIAL_PRINTLN("Error: No state requested.");
        return;
    }

    if (PreLoopHook != nullptr) {
        PreLoopHook();
    }
 
    if (RequestedState != CurrentState) {
        if (CurrentState != nullptr) {
            SERIAL_PRINTLN("Exiting state \"%s\"", CurrentState->Name.c_str());
            CurrentState->State->stateExit();
        }
        LastState = CurrentState;
        CurrentState = RequestedState;
        if (CurrentState != nullptr) {
            SERIAL_PRINTLN("Entering state \"%s\"", CurrentState->Name.c_str());
            uint8_t lastStateId = LastState == nullptr ? UNDEFINED_STATE : LastState->Id;
            CurrentState->State->stateEnter(lastStateId, UserData);
            UserData = nullptr;
        }
    }

    if (CurrentState != nullptr) {
        CurrentState->State->stateLoop();
    }

    if (PostLoopHook != nullptr) {
        PostLoopHook();
    }
}
