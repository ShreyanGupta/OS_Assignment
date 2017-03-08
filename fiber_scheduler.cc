#include "labs/fiber_scheduler.h"

//
// stackptrs:      Type: addr_t[stackptrs_size].  array of stack pointers (generalizing: main_stack and f_stack)
// stackptrs_size: number of elements in 'stacks'.
//
// arrays:      Type: uint8_t [arrays_size]. array of memory region for stacks (generalizing: f_array)
// arrays_size: size of 'arrays'. equal to stackptrs_size*STACK_SIZE.
//
// Tip: divide arrays into stackptrs_size parts.
// Tip: you may implement a circular buffer using arrays inside shellstate_t
//      if you choose linked lists, create linked linked using arrays in
//      shellstate_t. (use array indexes as next pointers)
// Note: malloc/new is not available at this point.
//
void shell_step_fiber_scheduler(shellstate_t& shellstate, addr_t stackptrs[], size_t stackptrs_size, addr_t arrays, size_t arrays_size)
{

    //insert your code here
    // f_stack = stackptr[curr]
    fiberstate &f = shellstate.fs;
    if (f.total_fiber == 0) return;
    bool found = false;
    while (!found)
    {
    	found = (f.running[f.curr_fiber%6] || f.started[f.curr_fiber%6]);
    	f.curr_fiber += 1;
    }
    f.curr_fiber = (f.curr_fiber-1)%6;
    // current decided! now ->
    int i = f.curr_fiber;
    f.f_stack = stackptrs[i];

    f.started[i] = false;
    f.running[i] = true;
    hoh_debug("f " << i << " started is true!");
    stack_init5(f.f_stack, arrays, arrays_size, (i<3)? &fiber_fibbo : &fiber_facto, 
        &f.main_stack, &f.f_stack, &f.x[i], &f.running[i], &f.answer[i]);
}
