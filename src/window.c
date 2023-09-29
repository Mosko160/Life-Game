#include "window.h"

SDL_Renderer *window_init() {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *win = SDL_CreateWindow("The life game", SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, W_SIZE, W_SIZE,
                                     SDL_WINDOW_SHOWN);
  if (!win)
    exit(2);
  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, 0);
  if (!ren)
    exit(2);
  return ren;
}

void render_map(Game *g) {
  for (int y = 0; y != N_TILE; y++) {
    for (int x = 0; x != N_TILE; x++) {
      SDL_Rect rect = {S_TILE * x, S_TILE * y, S_TILE, S_TILE};
      if (g->map[y][x])
        SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 0);
      else
        SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 0);
      SDL_RenderFillRect(g->renderer, &rect);
    }
  }
}

static void click(Game *g, SDL_Event evt) {
  int x = evt.button.x / 9;
  int y = evt.button.y / 9;
  if (g->to_place) {
    for (int py = 0; py != g->to_place_height; py++)
      for (int px = 0; px != g->to_place_length; px++)
        g->map[y + py][x + px] = g->to_place[py][px];
    g->to_place = NULL;
  } else
    g->map[y][x] = !g->map[y][x];
}

static void event_listener(Game *g, Menu *m) {
  SDL_Event evt;
  SDL_PollEvent(&evt);
  switch (evt.type) {
  case SDL_QUIT:
    g->update = 0;
    break;
  case SDL_KEYDOWN:
    key_pressed(g, m, evt.key.keysym.sym);
    break;
  case SDL_MOUSEBUTTONUP:
    click(g, evt);
    break;
  }
}

static void set_pattern_rect(Game *g) {
  int px, py;
  SDL_GetMouseState(&px, &py);
  px = px / 9;
  py = py / 9;
  for (int y = 0; y != g->to_place_height; y++) {
    for (int x = 0; x != g->to_place_length; x++) {
      SDL_Rect rect = {S_TILE * (px + x), S_TILE * (py + y), S_TILE, S_TILE};
      if (g->to_place[y][x])
        SDL_SetRenderDrawColor(g->renderer, 255, 0, 0, 0);
      else
        SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 0);
      SDL_RenderFillRect(g->renderer, &rect);
    }
  }
}

void update_loop(Game *g, Menu *m) {
  int rate = 0;
  while (g->update) {
    if (!rate && g->running)
      update_game(g);
    render_map(g);
    event_listener(g, m);
    if (g->to_place)
      set_pattern_rect(g);
    SDL_RenderPresent(g->renderer);
    SDL_Delay(FRAME_INTERVAL);
    rate = (rate + 1) % (FPS / g->speed);
  }
}
