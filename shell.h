#pragma once
#include "util/config.h"
#include "util/debug.h"

struct shellstate_t{
    char curr_cmd[1<<9];
    int curr_pos;
    int end_pos;
    bool execute;
    int num_key;
};

struct renderstate_t{
	char *line[25];
	int firstline;
	int lastline;
	int curr_pos; // cursor!
	int num_key;
};

void shell_init(shellstate_t& state);
void shell_update(uint8_t scankey, shellstate_t& stateinout);
void shell_step(shellstate_t& stateinout);
void shell_render(const shellstate_t& shell, renderstate_t& render);

bool render_eq(const renderstate_t& a, const renderstate_t& b);
void render(const renderstate_t& state, int w, int h, addr_t display_base);

