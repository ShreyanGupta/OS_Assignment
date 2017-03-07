#pragma once

#include "labs/shell.h"
#include "util/coroutine.h"

// state for your coroutine implementation:
struct f_t{

    //insert your code here
    int x;
    int curr;
    long long answer;
};


void c_facto(coroutine_t &, f_t &, bool &);
void shell_step_coroutine(shellstate_t& shellstate, coroutine_t& f_coro, f_t& f_locals);


