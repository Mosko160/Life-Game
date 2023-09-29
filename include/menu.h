#pragma once

#include "game.h"
#include "pattern.h"
#include <ncurses.h>
#include <stdlib.h>

enum Status { HOME, SPEED, PATTERNS };
enum Focus { CATEGORY = 0, PATTERN };

typedef struct {
  WINDOW *mainWin;
  WINDOW *menuWin;
  WINDOW *patternsWin;
  enum Status status;
  Storage **storage;
  enum Focus focus;
  WINDOW *categoryWin;
  WINDOW *patternWin;
  WINDOW *patternInfoWin;
  enum Type category;
  int index_categ;
  int index_pattern;
} Menu;

Menu *init_win(Storage **storage);
void close_win(Menu *m);
void key_pressed(Game *g, Menu *m, int key);
