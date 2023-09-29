#include "menu.h"
#include <curses.h>

static void display_home(Menu *m) {
  wclear(m->menuWin);
  box(m->menuWin, 0, 0);
  mvwprintw(m->menuWin, 2, 4, "1. Pause");
  mvwprintw(m->menuWin, 2, 28, "2. Random");
  mvwprintw(m->menuWin, 2, 54, "3. Reset");
  mvwprintw(m->menuWin, 2, 80, "4. Speed");
  mvwprintw(m->menuWin, 2, 104, "5. Patterns");
  curs_set(0);
  wrefresh(m->menuWin);
}

static void display_speed(Menu *m, Game *g) {
  wclear(m->menuWin);
  box(m->menuWin, 0, 0);
  mvwprintw(m->menuWin, 2, 57, "< %d >", g->speed);
  curs_set(0);
  wrefresh(m->menuWin);
}

static void display_pattern_info(Menu *m) {
  wclear(m->patternInfoWin);
  box(m->patternInfoWin, 0, 0);
  if (!m->storage[m->category]->length)
    return;
  Pattern *pat = m->storage[m->category]->pat[m->index_pattern];
  mvwprintw(m->patternInfoWin, 2, 3, "Name: %s", pat->name);
  mvwprintw(m->patternInfoWin, 3, 3, "Dimension: %dx%d", pat->length,
            pat->height);
  for (int y = 0; y != pat->height; y++)
    for (int x = 0; x != pat->length; x++)
      mvwprintw(m->patternInfoWin, y + 5, x + 3, pat->map[y][x] ? "#" : " ");
  wrefresh(m->patternInfoWin);
}

static void display_category(Menu *m) {
  wclear(m->patternWin);
  int i = 2;
  for (int index = 0; index != m->storage[m->category]->length; index++)
    mvwprintw(m->patternWin, i++, 5, "%s",
              m->storage[m->category]->pat[index]->name);
  mvwprintw(m->patternWin, 2 + m->index_pattern, 2, "->");
  box(m->patternWin, 0, 0);
  if (m->focus == PATTERN) {
    mvwprintw(m->patternWin, 0, 6, " Select ");
    wrefresh(m->patternWin);
  }
  wrefresh(m->patternWin);
  display_pattern_info(m);
}

static void display_category_name(Menu *m) {
  wclear(m->categoryWin);
  mvwprintw(m->categoryWin, 2, 5, "Block");
  mvwprintw(m->categoryWin, 3, 5, "Oscillator");
  mvwprintw(m->categoryWin, 4, 5, "Glider");
  mvwprintw(m->categoryWin, 2 + m->index_categ, 2, "->");
  box(m->categoryWin, 0, 0);
  if (m->focus == CATEGORY)
    mvwprintw(m->categoryWin, 0, 6, " Select ");
  wrefresh(m->categoryWin);
  display_category(m);
}

static void display_patterns_win(Menu *m) {
  box(m->patternsWin, 0, 0);
  wrefresh(m->patternsWin);
  display_category_name(m);
}

Menu *init_win(Storage **storage) {
  Menu *m = calloc(sizeof(Menu), 1);
  m->mainWin = initscr();
  m->menuWin = subwin(m->mainWin, 5, 120, 50, 40);
  m->patternsWin = subwin(m->mainWin, 46, 201, 2, 5);
  m->categoryWin = subwin(m->patternsWin, 46, 20, 2, 5);
  m->patternWin = subwin(m->patternsWin, 46, 20, 2, 24);
  m->patternInfoWin = subwin(m->patternsWin, 46, 163, 2, 43);
  m->storage = storage;
  refresh();
  display_home(m);
  return m;
}

void close_win(Menu *m) {
  delwin(m->mainWin);
  endwin();
  free(m);
}

static void home_key_pressed(Game *g, Menu *m, int key) {
  switch (key) {
  case SDLK_1:
    g->running = !g->running;
    break;
  case SDLK_2:
    random_map(g);
    break;
  case SDLK_3:
    reset_map(g);
    break;
  case SDLK_4:
    m->status = SPEED;
    display_speed(m, g);
    break;
  case SDLK_5:
    m->status = PATTERNS;
    m->focus = CATEGORY;
    display_patterns_win(m);
    break;
  }
}

static void speed_key_pressed(Game *g, Menu *m, int key) {
  switch (key) {
  case SDLK_LEFT:
    if (g->speed > 1)
      g->speed--;
    display_speed(m, g);
    break;
  case SDLK_RIGHT:
    g->speed++;
    display_speed(m, g);
    break;
  case SDLK_ESCAPE:
    m->status = HOME;
    display_home(m);
    break;
  }
}

static void choose_pattern(Game *g, Menu *m) {
  Pattern *pat = m->storage[m->category]->pat[m->index_pattern];
  g->to_place = pat->map;
  g->to_place_height = pat->height;
  g->to_place_length = pat->length;
}

static void patterns_key_pressed(Game *g, Menu *m, int key) {
  switch (key) {
  case SDLK_UP:
    if (m->focus) {
      if (m->index_pattern)
        m->index_pattern--;
      display_category(m);
    } else {
      if (m->index_categ)
        m->index_categ--;
      m->category = m->index_categ;
      display_category_name(m);
    }
    break;
  case SDLK_DOWN:
    if (m->focus) {
      if (m->index_pattern != m->storage[m->category]->length - 1)
        m->index_pattern++;
      display_category(m);
    } else {
      if (m->index_categ != NB_PATTERNS_TYPE - 1)
        m->index_categ++;
      m->category = m->index_categ;
      display_category_name(m);
    }
    break;
  case SDLK_RETURN:
    if (m->focus == CATEGORY) {
      m->focus = PATTERN;
      mvwprintw(m->patternWin, 0, 6, " Select ");
      box(m->categoryWin, 0, 0);
      wrefresh(m->categoryWin);
      wrefresh(m->patternWin);
    } else
      choose_pattern(g, m);
    break;
  case SDLK_ESCAPE:
    if (m->focus == PATTERN) {

      m->index_pattern = 0;
      m->focus = CATEGORY;
      mvwprintw(m->categoryWin, 0, 6, " Select ");
      box(m->patternWin, 0, 0);
      wrefresh(m->categoryWin);
      wrefresh(m->patternWin);
    } else {
      m->status = HOME;
      wclear(m->patternsWin);
      wrefresh(m->patternsWin);
    }
    break;
  }
  if (g || m || key)
    return;
  return;
}

void key_pressed(Game *g, Menu *m, int key) {
  switch (m->status) {
  case HOME:
    return home_key_pressed(g, m, key);
    break;
  case SPEED:
    return speed_key_pressed(g, m, key);
  case PATTERNS:
    return patterns_key_pressed(g, m, key);
  }
}
