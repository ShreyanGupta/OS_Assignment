#pragma once
#include "util/config.h"
#include "util/debug.h"
#include <stdlib.h>

#include "labs/deque.h"

struct Deque;

struct fiberstate{
    bool started[6];
    bool running[6];
    int x[6];
    int answer[6];
    int total_fiber;
    int curr_fiber; // +1 mod 6 for scheduling
    addr_t f_stack;
    addr_t main_stack;
};

struct shellstate_t{
    Deque line;     // Command history
    Deque renderline; // Lines to be rendered
    char *curr_cmd; // PointerCurrent command
    int curr_pos;   // current posiiotn of cursor
    int end_pos;    // end position of current line
    int line_pos;   // Index of current command in Command History
    bool execute;   // if there's some command to be executed
    int num_key;    // number of keys pressed
    int cursor_color; // color of cursor

    bool coroutine_run;     // is coroutine running?
    long long coroutine_x;  // parameter to be passed

    fiberstate fs;  // fiber state
    void insert_answer(int ans);
    void insert_answer_fiber(int ans, int curr);
};

struct renderstate_t{
	const Deque* line;
	int curr_pos; // cursor!
	int num_key;
    int cursor_color;
    bool coroutine_run;
    bool fiber_run;
};

void shell_init(shellstate_t& state);
void shell_update(uint8_t scankey, shellstate_t& stateinout);
void shell_step(shellstate_t& stateinout);
void shell_render(const shellstate_t& shell, renderstate_t& render);

bool render_eq(const renderstate_t& a, const renderstate_t& b);
void render(const renderstate_t& state, int w, int h, addr_t display_base);

long long fibbo(long long x);
long long facto(long long x);

    // Helper functions :
void int_to_string(int x, char *y2);
void copy_string(char *to, const char *from);
char memcmp1(char* s1, char* s2, int len);
