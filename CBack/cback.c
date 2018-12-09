#include "cback.h"
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

static char* StackTop;
char* StackBottom;

//#define StackSize (StackBottom - StackTop + 1)

typedef struct Notification
{
    void* Base;
    size_t Size;
    struct Notification* Next;
} Notification;

typedef struct State
{
    struct State* Previous;
    size_t LastChoice;
    jmp_buf Environment;
    char* StackTop;
    char* StackBottom;
    size_t StackSize;          // stack size  
    void* Stack;               // saved stack
} State;

static State Head;
static State* TopState = &Head;
static State* Previous;
static size_t LastChoice;
void (*Fiasco)();

static void PushState()
{
    char Dummy;

    StackTop = &Dummy;
    Previous = TopState;
    TopState = (State *)malloc(sizeof(State));
    if (!TopState) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    TopState->Previous = Previous;
    /* initialized to be one */;
    TopState->LastChoice = 1; 
    TopState->StackBottom = StackBottom;
    TopState->StackTop = StackTop;
    TopState->StackSize = TopState->StackBottom - TopState->StackTop;
    /*
     * Store a copy of C's runtime stack in TopState.
     */ 
    assert(TopState->StackSize > 0);
    TopState->Stack = malloc(TopState->StackSize);
    if (!TopState->Stack) {
        perror("malloc");
        exit(-1);
    }
    memcpy(TopState->Stack, StackTop, TopState->StackSize);
}

static void PopState()
{
    Previous = TopState->Previous;
    free(TopState->Stack);
    free(TopState);
    TopState = Previous;
}

size_t Choice(size_t N)
{
    /* we got no choice, therefore make a exit call within Backtrack() */
    //if (N <= 0)
    //    Backtrack();

    /* we got one choice, therefore return immediately */
    if (N == 1) 
        return 1;

    PushState();
    setjmp(TopState->Environment);
    LastChoice = TopState->LastChoice;
    if (LastChoice == (size_t)N)
        PopState();
    return LastChoice;
}

void Backtrack()
{
    char Dummy;

    /* when TopState is NULL(all Choice() finished), Backtrack() returns */
    if (TopState == &Head) {
        if (Fiasco) 
            Fiasco();
        return; /* just return or exit */
    }

    /* make memory space for restored stack */
    if (TopState->StackTop < &Dummy)
        Backtrack();
    //LastChoice = ++TopState->LastChoice;
    TopState->LastChoice += 1;
    StackBottom = TopState->StackBottom;
    StackTop = TopState->StackTop;
    /*  Restore C's runtime stack from TopState. */
    memcpy(StackTop, TopState->Stack, TopState->StackSize);
    /*
     * longjmp() restores the state saved by the most recent
     * invocation of setjmp with the corresponding env argument.
     */
    longjmp(TopState->Environment, 1);
}

void ClearChoices(void)
{
    while (TopState != &Head)
        PopState();
    LastChoice = 0;
}
