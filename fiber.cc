#include "labs/fiber.h"



// void f(addr_t *main_stack, addr_t *f_stack){
//     hoh_debug("entered function");
//     stack_saverestore(*f_stack, *main_stack);
// }

void shell_step_fiber(shellstate_t& shellstate, addr_t& main_stack, addr_t& f_stack, addr_t f_array, uint32_t f_arraysize){
    
    fiberstate &f = shellstate.fs;

    // for(int i=0; i<6; ++i) if(f.started[i]){
    //     f.started[i] = false;
    //     f.running[i] = true;
    //     hoh_debug("f " << i << " started is true!");
    //     stack_init5(f.f_stack, f_array, f_arraysize, (i<3)? &fiber_fibbo : &fiber_facto, 
    //         &main_stack, &f.f_stack, &f.x[i], &f.running[i], &f.answer[i]);
    // }
    f.main_stack = &main_stack;

    int &curr = f.curr_fiber;
    if(f.running[curr]){
        stack_saverestore(*f.main_stack, *f.f_stack);
        if(!f.running[curr]){
            f.total_fiber -= 1;
            shellstate.insert_answer_fiber(f.answer[curr], curr);
        }
    }

    // if(shellstate.fs.started){
    //     shellstate.fs.started = false;
    //     shellstate.fs.running = true;
    //     hoh_debug("f started is true!");
    //     hoh_debug("main_stack " << (int)(&main_stack) << (int)main_stack);
    //     stack_init5(f_stack, f_array, f_arraysize, &fiber_facto, 
    //         &main_stack, &f_stack, &shellstate.fs.x, &shellstate.fs.running, &shellstate.fs.answer);
        
    //     // stack_init2(f_stack, &f_array, f_arraysize, &f,
    //     //     &main_stack, &f_stack);
    // }
    // if(shellstate.fs.running){
    //     hoh_debug("f running!");
    //     stack_saverestore(main_stack, f_stack);
    //     hoh_debug("f came out!");
    //     if(!shellstate.fs.running){
    //         hoh_debug("f ended! \n" << shellstate.fs.answer);
    //         shellstate.insert_answer_fiber(shellstate.fs.answer);
    //     }
    // }
}

