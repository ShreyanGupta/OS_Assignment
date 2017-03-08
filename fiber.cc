#include "labs/fiber.h"

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

void f(addr_t *main_stack, addr_t *f_stack){
    hoh_debug("entered function");
    stack_saverestore(*f_stack, *main_stack);
}

void shell_step_fiber(shellstate_t& shellstate, addr_t& main_stack, addr_t& f_stack, addr_t f_array, uint32_t f_arraysize){
    // fiberstate &f = shellstate.fs;
    // if(f.started){
    //     f.started = false;
    //     f.running = true;
    //     hoh_debug("f started is true! \n");
    //     stack_init5(f_stack, &f_array, f_arraysize, &fiber_facto, 
    //         &main_stack, &f_stack, &f.x, &f.running, &f.answer);
    // }
    // if(f.running){
    //     hoh_debug("f running! \n");
    //     stack_saverestore(main_stack, f_stack);
    //     if(!f.running){
    //         hoh_debug("f ended! \n");
    //         shellstate.insert_answer_fiber(f.answer);
    //     }
    // }

    if(shellstate.fs.started){
        shellstate.fs.started = false;
        shellstate.fs.running = true;
        hoh_debug("f started is true!");
        hoh_debug("main_stack " << (int)(&main_stack) << (int)main_stack);
        stack_init5(f_stack, f_array, f_arraysize, &fiber_facto, 
            &main_stack, &f_stack, &shellstate.fs.x, &shellstate.fs.running, &shellstate.fs.answer);
        
        // stack_init2(f_stack, &f_array, f_arraysize, &f,
        //     &main_stack, &f_stack);
    }
    if(shellstate.fs.running){
        hoh_debug("f running!");
        stack_saverestore(main_stack, f_stack);
        hoh_debug("f came out!");
        if(!shellstate.fs.running){
            hoh_debug("f ended! \n" << shellstate.fs.answer);
            shellstate.insert_answer_fiber(shellstate.fs.answer);
        }
    }
}

