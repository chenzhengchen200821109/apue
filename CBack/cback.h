#ifndef __CBACK__H__
#define __CBACK__H__ 

#include <stddef.h>

#define Backtracking(S) { char Dummy; StackBottom = &Dummy; S; }
//#define StackSize (StackTop - StackBottom + 1)
/**
 * Choice(N): N is a positive integer denoting the number
 * of alternatives, returns successive integer. Choice first
 * returns the value 1, and the program continues. The value 2
 * to N are returned by Choice through subsequent calls of Backtrack().
 */
// size_t Choice(size_t N);
size_t Choice(size_t N);

 /**
  * Backtrack(): causes the program to backtrack, that is to say
  * return to the most recent call of Choice, which has not yet
  * returns all its values.
  * All automatic variables will be re-established. The remaining
  * variables, such as static variables, are not touched. However
  * the user specified static variables can be re-established as an
  * extension of CBack facilities.
  */
 void Backtrack();

 /**
  * Clear all pending Choice-calls.
  */
 //void ClearChoices();

#endif
