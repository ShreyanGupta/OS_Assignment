#pragma once
#include "util/config.h"
#include "util/debug.h"
#include <stdlib.h>

#include "labs/deque.h"

struct Deque;

struct shellstate_t{
    Deque line;
    Deque renderline;
    char *curr_cmd;
    int curr_pos;
    int end_pos;
    int line_pos;
    bool execute;
    int num_key;
    int cursor_color;
    // bool enter_pressed;
    // char output[1<<9];
    // bool render_clear;
    // bool print_output;
};

struct renderstate_t{
	const Deque* line;
	int curr_pos; // cursor!
	int num_key;
    int cursor_color;
};

void shell_init(shellstate_t& state);
void shell_update(uint8_t scankey, shellstate_t& stateinout);
void shell_step(shellstate_t& stateinout);
void shell_render(const shellstate_t& shell, renderstate_t& render);

bool render_eq(const renderstate_t& a, const renderstate_t& b);
void render(const renderstate_t& state, int w, int h, addr_t display_base);

long long fibbo(long long x);
long long facto(long long x);


void int_to_string(int x, char *y2);
void copy_string(char *to, const char *from);
char memcmp1(char* s1, char* s2, int len);
