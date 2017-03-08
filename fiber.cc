#include "labs/fiber.h"

void fiber_facto(addr_t &main_stack, addr_t &f_stack, int xx, bool &running, int &answer){
    int x=xx;
    answer = 1;
    for(int i=0; i<=x; ++i){
        answer *= i;
        stack_saverestore(f_stack, main_stack);
    }
    running = false;

    // while(true){
    //     done = true;
    //     ret = temp;
    //     stack_saverestore(f_stack, main_stack);
    // }

}

void shell_step_fiber(shellstate_t& shellstate, addr_t& main_stack, addr_t& f_stack, addr_t f_array, uint32_t f_arraysize){
    fiberstate &f = shellstate.fs;
    if(f.started){
        f.started = false;
        f.running = true;
        // stackinit
        // fiber_facto()
        stack_init5(f_stack, f_array, f_arraysize, &fiber_facto, 
            main_stack, f_stack, f.x, f.running, f.answer);
    }
    if(f.running){
        stack_saverestore(main_stack, f_stack);
        if(!f.running){
            shellstate.insert_answer_fiber(f.answer);
        }
    }
}

