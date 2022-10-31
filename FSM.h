#include <stdlib.h>
#include <map>
#include <string.h>


typedef int EventId;
typedef int StateId;
typedef void (*callback)(void);
typedef bool (*guard)(void);

class Transition {
    friend class FSM;

    private:
    EventId eventId;
    callback fnOnTransition;
    guard fnGuard;
    StateId nextState;
    
    public:
    Transition(EventId eventId,
    callback fnOnTransition,
    guard fnGuard,
    StateId nextState);
    
};

class FSM;

class State {
    friend class FSM;

    private:
    StateId stateId;
    FSM* parent;
    callback fnEnterState;
    callback fnExitState;
    callback fnStayInState;

    std::map<EventId, Transition*> transitions { };
    
    public:
    State(FSM* parent, StateId stateId,
    callback fnEnterState,
    callback fnExitState,
    callback fnStayInState);
    
    State* addTransition(EventId eventId,
    callback fnOnTransition,
    guard fnGuard,
    StateId nextState);
    
    FSM* end();
};

class FSM {

    private:
    std::map<StateId, State*> states;
    StateId currentStateId;

    public:
    State* addState(StateId stateId,
    callback fnEnterState,
    callback fnExitState,
    callback fnStayInState);

    void setState(StateId initialState);
    StateId getState(void);

    //zustand wurde gewechselt
    bool dispatch(EventId e);

};