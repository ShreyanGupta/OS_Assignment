#include "labs/coroutine.h"

void c_facto(coroutine_t &f_coro, f_t &f_locals, bool &done){
    int &x = f_locals.x;
    int &curr = f_locals.curr;
    long long &answer = f_locals.answer;

    h_begin(f_coro);

    answer = 1;
    for(curr=1; curr<=x; ++curr){
        answer *= curr;
        h_yield(f_coro);
    }

    done = true;
    f_locals.curr = 0;
    coroutine_reset(f_coro);
    // h_end(f_coro);
}


void shell_step_coroutine(shellstate_t& shellstate, coroutine_t& f_coro, f_t& f_locals){

    //insert your code here

    if(!shellstate.coroutine_run) return;
    bool done = false;
    f_locals.x = shellstate.coroutine_x;
    // f_locals.curr = 1;
    hoh_debug("x = " << f_locals.x << ", curr = " << f_locals.curr);
    c_facto(f_coro, f_locals, done);
    if(done){
        shellstate.insert_answer(f_locals.answer);
    }

    // hoh_debug("leaving cor, going to fiber");

}


