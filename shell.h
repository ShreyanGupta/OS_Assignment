#pragma once
#include "util/config.h"
#include "util/debug.h"
#include <stdlib.h>

#include "labs/deque.h"

struct shellstate_t{
    Deque line;
    char *curr_cmd;
    int curr_pos;
    int end_pos;
    int line_pos;
    bool execute;
    int num_key;
    bool cursor_on_curr_cmd;
    char output[1<<9];
};

struct renderstate_t{
	Deque Lines;
	int curr_pos; // cursor!
	int num_key;
	char output[1<<9];
	bool cursor_on_curr;
};

void shell_init(shellstate_t& state);
void shell_update(uint8_t scankey, shellstate_t& stateinout);
void shell_step(shellstate_t& stateinout);
void shell_render(const shellstate_t& shell, renderstate_t& render);

bool render_eq(const renderstate_t& a, const renderstate_t& b);
void render(const renderstate_t& state, int w, int h, addr_t display_base);

long long fibbo(long long x);
long long facto(long long x);
