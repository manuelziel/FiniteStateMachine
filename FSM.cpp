#include "FSM.h"

Transition::Transition(EventId eventId,
                       callback fnOnTransition,
                       guard fnGuard,
                       StateId nextState)
{
    this->eventId = eventId;
    this->fnOnTransition = fnOnTransition;
    this->fnGuard = fnGuard;
    this->nextState = nextState;
}

State::State(FSM *parent, StateId stateId,
             callback fnEnterState,
             callback fnExitState,
             callback fnStayInState)
{
    this->parent = parent;
    this->stateId = stateId;
    this->fnEnterState = fnEnterState;
    this->fnExitState = fnExitState;
    this->fnStayInState = fnStayInState;
}

State *State::addTransition(EventId eventId,
                            callback fnOnTransition,
                            guard fnGuard,
                            StateId nextState)
{

    this->transitions[eventId] = new Transition(eventId, fnOnTransition, fnGuard, nextState);

    return this;
}

FSM *State::end()
{
    return this->parent;
}

State *FSM::addState(StateId stateId,
                     callback fnEnterState,
                     callback fnExitState,
                     callback fnStayInState)
{
    this->states[stateId] = new State(this, stateId, fnEnterState, fnExitState, fnStayInState);

    return this->states[stateId];
}

void FSM::setState(StateId initialState)
{
    this->currentStateId = initialState;
}

bool FSM::dispatch(EventId e)
{
    State *currentState = this->states[this->currentStateId];

    Transition *currentTransition = currentState->transitions[e];
    
    if (currentTransition == NULL)
    {
        if ( currentState->fnStayInState)
        currentState->fnStayInState();
        return false;
    }

    if (currentTransition->fnGuard)
    {
        if (!currentTransition->fnGuard())
        {
             if ( currentState->fnStayInState)
            currentState->fnStayInState();
            return false;
        }
    }

    if (currentState->fnExitState)
        currentState->fnExitState();

    if (currentTransition->fnOnTransition)
        currentTransition->fnOnTransition();

    this->setState(currentTransition->nextState);

    currentState = this->states[this->currentStateId];

    if (currentState->fnEnterState)
        currentState->fnEnterState();

    return true;
}

StateId FSM::getState(void) {
    return this->currentStateId;
}