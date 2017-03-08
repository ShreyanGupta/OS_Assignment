#include "labs/fiber.h"

void fiber_facto(addr_t *pmain_stack, addr_t *pf_stack, int *xx, bool *run, int *ans){
    addr_t& main_stack = *pmain_stack; // boilerplate: to ease the transition from existing code
    addr_t& f_stack    = *pf_stack;
    int x = *xx;
    int &answer = *ans;
    bool &running = *run;
    answer = 1;
    for(int i=1; i<=x; ++i){
        answer *= i;
        hoh_debug("yo peeps, i = " << i);
        stack_saverestore(f_stack, main_stack);
    }
    // running = false;
    // stack_saverestore(f_stack, main_stack);
    while(true){
        running = false;
        // ret = temp;
        stack_saverestore(f_stack, main_stack);
    }

}

void shell_step_fiber(shellstate_t& shellstate, addr_t& main_stack, addr_t& f_stack, addr_t f_array, uint32_t f_arraysize){
    fiberstate &f = shellstate.fs;
    if(f.started){
        f.started = false;
        f.running = true;
        hoh_debug("f started is true! \n");
        stack_init5(f_stack, &f_array, f_arraysize, &fiber_facto, 
            &main_stack, &f_stack, &f.x, &f.running, &f.answer);
    }
    if(f.running){
        hoh_debug("f running! \n");
        stack_saverestore(main_stack, f_stack);
        if(!f.running){
            hoh_debug("f ended! \n");
            shellstate.insert_answer_fiber(f.answer);
        }
    }
}

