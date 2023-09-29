#pragma once
#include "game.h"
#include "global.h"
#include "menu.h"
#include <SDL.h>

SDL_Renderer *window_init();
void render_map(Game *g);
void update_loop(Game *g, Menu *m);
