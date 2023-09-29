#include "game.h"
#include "menu.h"
#include "pattern.h"
#include "window.h"

int main() {
  Storage **storage = load_patterns();
  Menu *m = init_win(storage);
  m->storage = storage;
  Game *g = create_game(window_init());
  update_loop(g, m);
  close_win(m);
  free_game(g);
  free_storage(storage);
  return 0;
}
