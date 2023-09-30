#include "game.h"

static Map create_map() {
  Map map = calloc(sizeof(int **), N_TILE);
  if (!map)
    exit(1);
  for (int y = 0; y != N_TILE; y++) {
    map[y] = calloc(sizeof(int) * N_TILE, N_TILE);
    if (!map[y])
      exit(1);
  }
  return map;
}

static void free_map(Map map) {
  for (int y = 0; y != N_TILE; y++)
    free(map[y]);
  free(map);
}

static int get_nei(Map map, int y, int x) {
  int nei = 0;
  for (int ty = y - 1; ty != y + 2; ty++) {
    for (int tx = x - 1; tx != x + 2; tx++) {
      if (ty < 0 || ty >= N_TILE || tx < 0 || tx >= N_TILE ||
          (tx == x && ty == y))
        continue;
      nei += map[ty][tx];
    }
  }
  return nei;
}

Game *create_game(SDL_Renderer *renderer) {
  Game *g = calloc(sizeof(Game), 1);
  if (!g)
    exit(1);
  g->renderer = renderer;
  g->running = 1;
  g->update = 1;
  g->map = create_map();
  g->speed = 10;
  return g;
}

void update_game(Game *g) {
  g->iteration++;
  g->population = 0;
  Map new_map = create_map();
  for (int y = 0; y != N_TILE; y++) {
    for (int x = 0; x != N_TILE; x++) {
      int nei = get_nei(g->map, y, x);
      if (nei == 3)
        new_map[y][x] = 1;
      else if (g->map[y][x] && nei == 2)
        new_map[y][x] = 1;
      g->population += new_map[y][x];
    }
  }
  free_map(g->map);
  g->map = new_map;
}

void free_game(Game *g) {
  free_map(g->map);
  free(g);
}

void reset_map(Game *g) {
  free_map(g->map);
  g->map = create_map();
}

void random_map(Game *g) {
  for (int y = 0; y != N_TILE; y++)
    for (int x = 0; x != N_TILE; x++)
      g->map[y][x] = rand() % 2;
}

void rotate_pattern(Game *g) {
  if (!g->to_place)
    return;
  int **map = calloc(sizeof(int *), g->to_place_length);
  for (int y = 0; y != g->to_place_length; y++)
    map[y] = calloc(sizeof(int), g->to_place_height);
  for (int y = 0; y != g->to_place_height; y++)
    for (int x = 0; x != g->to_place_length; x++)
      map[x][g->to_place_height - y - 1] = g->to_place[y][x];
  for (int y = 0; y != g->to_place_height; y++)
    free(g->to_place[y]);
  free(g->to_place);
  g->to_place = map;
}
