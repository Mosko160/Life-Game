#pragma once
#include "global.h"
#include <SDL.h>
#include <stdio.h>

typedef int **Map;

typedef struct {
  SDL_Renderer *renderer;
  Map map;
  int iteration;
  int population;
  int running;
  int update;
  int speed;
  int **to_place;
  int to_place_height;
  int to_place_length;
} Game;

Game *create_game(SDL_Renderer *renderer);
void update_game(Game *g);
void free_game(Game *g);
void reset_map(Game *g);
void random_map(Game *g);
