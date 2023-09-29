#pragma once
#include "global.h"
#define _GNU_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Type { Block = 0, Oscillator, Glider };

typedef struct {
  char *name;
  int length;
  int height;
  enum Type type;
  int **map;
} Pattern;

typedef struct {
  Pattern **pat;
  enum Type type;
  int length;
} Storage;

Storage **load_patterns();
void free_storage(Storage **storage);
