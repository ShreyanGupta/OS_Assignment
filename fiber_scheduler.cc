#include "labs/fiber_scheduler.h"

void fiber_facto(addr_t *pmain_stack, addr_t *pf_stack, int *px, bool *prun, int *pans){
    
    hoh_debug("addr of main_stack in F : " << (int)(pmain_stack) << " " << (int)(*pmain_stack));
    addr_t& main_stack = *pmain_stack; // boilerplate: to ease the transition from existing code
    addr_t& f_stack    = *pf_stack;
    int &x = *px;
    int &answer = *pans;
    bool &running = *prun;

    answer = 1;
    for(int i=1; i<=x; ++i){
        answer *= i;
        hoh_debug("yo peeps, i = " << i << ", answer = " << answer);
        hoh_debug("main_stack " << main_stack << "\n");
        stack_saverestore(f_stack, main_stack);
    }
    running = false;
    stack_saverestore(f_stack, main_stack);
    // while(true){
    //     running = false;
    //     // ret = temp;
    //     stack_saverestore(f_stack, main_stack);
    // }
}

void fiber_fibbo(addr_t *pmain_stack, addr_t *pf_stack, int *px, bool *prun, int *pans)
{
    addr_t& main_stack = *pmain_stack; // boilerplate: to ease the transition from existing code
    addr_t& f_stack    = *pf_stack;
    int &x = *px;
    int &answer = *pans;
    bool &running = *prun;

    answer = 1;
    int a = 1;
    int c;
    for(int i=1; i<=x; ++i){
        c = a;
        a = answer;
        answer += c;
        hoh_debug("yo peeps, i = " << i << ", answer = " << answer);
        hoh_debug("main_stack " << main_stack << "\n");
        stack_saverestore(f_stack, main_stack);
    }
    running = false;
    stack_saverestore(f_stack, main_stack);    
}

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
        // hoh_debug("f_stack of " << f.curr_fiber << " is : " << stackptrs[f.curr_fiber]);
        f.curr_fiber = (f.curr_fiber+1)%6;
        found = (f.running[f.curr_fiber] || f.started[f.curr_fiber]);
    }
    // current decided! now ->
    int i = f.curr_fiber;
    f.f_stack = &stackptrs[i];

    if (f.started[i])
    {
        hoh_debug("stackptr size : " << stackptrs_size);
        f.started[i] = false;
        f.running[i] = true;
        hoh_debug("f " << i << " started is true!");
        stack_init5(*f.f_stack, arrays + i*4096, 4096, (i<3)? &fiber_facto : &fiber_fibbo, 
            &*f.main_stack, &*f.f_stack, &f.x[i], &f.running[i], &f.answer[i]);        
    }
}
