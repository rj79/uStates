#include "StateHandler.h"

const uint8_t INVALID_STATE = 255;

#ifdef SERIAL_DEBUG
#define SERIAL_PRINTLN(x) Serial.println(x)
#else
#define SERIAL_PRINTLN(x) ;
#endif

StateHandler::StateHandler() : RequestedState(nullptr),
                               State(nullptr),
                               PreLoopHook(nullptr),
                               PostLoopHook(nullptr)
{
    for (int i = 0; i < MAX_STATES; ++i) {
        States[i].Id = INVALID_STATE;
        States[i].State = nullptr;
    }
}

bool StateHandler::requestState(uint8_t state_id)
{
    if (state_id == INVALID_STATE) {
        Serial.println("Error: Invalid state id requested.");
        return false;
    }
    for (int i = 0; i < MAX_STATES; ++i) {
        if (States[i].Id == state_id) {
            RequestedState = States[i].State;
            return true;
        }
    }
    return false;
}

IState* StateHandler::addState(uint8_t state_id, IState *state, String name)
{
    if (state_id == INVALID_STATE) {
        SERIAL_PRINTLN("Error: Invalid state id requested.");
        return nullptr;
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

    if (RequestedState != State) {
        if (State != nullptr) {
            SERIAL_PRINTLN((String("Exiting") + State->toString()).c_str());
            State->stateExit();
        }
        State = RequestedState;
        if (State != nullptr) {
            SERIAL_PRINTLN((String("Entering") + State->toString()).c_str());
            State->stateEnter();
        }
    }

    State->stateLoop();

    if (PostLoopHook != nullptr) {
        PostLoopHook();
    }
}
