#include "FSM.h"
#include <stdlib.h>
#include <stdio.h>

enum DoorEvent
{
    UNLOCK_DOOR,
    LOCK_DOOR,
    OPEN_DOOR,
    CLOSE_DOOR
};

const char *stateIdToName[] = {
    "DOOR_LOCKED_AND_CLOSED",      //
    "DOOR_UNLOCKEDAND_AND_CLOSED", //
    "DOOR_UNLOCKED_AND_OPEND",     //
    "DOOR_LOCKED_AND_OPEN"         //
};

enum DoorState
{
    DOOR_LOCKED_AND_CLOSED,
    DOOR_UNLOCKEDAND_AND_CLOSED,
    DOOR_UNLOCKED_AND_OPEND,
    DOOR_LOCKED_AND_OPEN
};

FSM fsm;
int age = 12;

void door_gets_opened(void) {
    printf("door_gets_opened\n");
}

bool test_age() {
    return age >= 18;
}

int main(int argc, char const *argv[])
{
    fsm
    .addState((StateId)(DOOR_LOCKED_AND_CLOSED), NULL, NULL, NULL)
        ->addTransition((EventId)(UNLOCK_DOOR), NULL, NULL, (StateId)(DOOR_UNLOCKEDAND_AND_CLOSED))
        ->end()
        ->addState((StateId)(DOOR_UNLOCKEDAND_AND_CLOSED), NULL, NULL, NULL)
        ->addTransition((EventId)(LOCK_DOOR), NULL, NULL, (StateId)(DOOR_LOCKED_AND_CLOSED))
        ->addTransition((EventId)(OPEN_DOOR), &door_gets_opened, &test_age, (StateId)(DOOR_UNLOCKED_AND_OPEND))

        ->end()
        ->addState((StateId)(DOOR_UNLOCKED_AND_OPEND), NULL, NULL, NULL)
        ->addTransition((EventId)(LOCK_DOOR), NULL, NULL, (StateId)(DOOR_LOCKED_AND_OPEN))
        ->addTransition((EventId)(CLOSE_DOOR), NULL, NULL, (StateId)(DOOR_UNLOCKEDAND_AND_CLOSED))

        ->end()
        ->addState((StateId)(DOOR_LOCKED_AND_OPEN), NULL, NULL, NULL)
        ->addTransition((EventId)(UNLOCK_DOOR), NULL, NULL, (StateId)(DOOR_UNLOCKED_AND_OPEND));

    fsm.setState((StateId)DOOR_LOCKED_AND_CLOSED);

    char inputBuffer[20];

    while (true)
    {

        printf("CurrentState: %s\n", stateIdToName[(int) (fsm.getState())]);
        printf("Age: %d\n\n", age);

        printf("> ");
        if (fgets(inputBuffer, 20, stdin) == 0)
        {
            printf("ERROR WHILE READING INPUT\n");
            return -1;
        }
        inputBuffer[strlen(inputBuffer) - 1] = '\0';

        if (strcmp(inputBuffer, "EXIT") == 0)
        {
            printf("Exit..\n");
            return 0;
        }
        else if (strcmp(inputBuffer, "LOCK") == 0)
        {
            printf("Try LOCK\n");
            fsm.dispatch(LOCK_DOOR);
        }
        else if (strcmp(inputBuffer, "UNLOCK") == 0)
        {
            printf("Try UNLOCK\n");
            fsm.dispatch(UNLOCK_DOOR);
        }
        else if (strcmp(inputBuffer, "OPEN") == 0)
        {
            printf("Try OPEN\n");
            fsm.dispatch(OPEN_DOOR);
        }
        else if (strcmp(inputBuffer, "CLOSE") == 0)
        {
            printf("Try CLOSE\n");
            fsm.dispatch(CLOSE_DOOR);
        }        
        else if (strcmp(inputBuffer, "MINOR") == 0)
        {
            printf("SET MINOR\n");
            age = 12;
        }
        else if (strcmp(inputBuffer, "ADULT") == 0)
        {
            printf("Set ADULT\n");
            age = 18;
        }
    }

    return 0;
}
