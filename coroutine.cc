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
    h_end(f_coro);
}


void shell_step_coroutine(shellstate_t& shellstate, coroutine_t& f_coro, f_t& f_locals){

    //insert your code here
    c_facto(f_coro, f_locals, done);
}


