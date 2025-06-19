#include "pattern.h"

static enum Type get_type(const char *s) {
  if (!strcmp(s, "Block\n"))
    return Block;
  else if (!strcmp(s, "Oscillator\n"))
    return Oscillator;
  else if (!strcmp(s, "Glider\n"))
    return Glider;
  else if (!strcmp(s, "Gun\n"))
    return Gun;
  else if (!strcmp(s, "Spaceship\n"))
    return Spaceship;
  else if (!strcmp(s, "Eater\n"))
    return Eater;
  else if (!strcmp(s, "Spacefiller\n"))
    return Spacefiller;
  return Oscillator;
}

static void get_map(Pattern *pat, FILE *f) {
  char *buff = NULL;
  size_t ms = 0;
  pat->map = calloc(pat->height, sizeof(int *));
  for (int y = 0; y != pat->height; y++) {
    pat->map[y] = calloc(pat->length, sizeof(int));
    getline(&buff, &ms, f);
    for (int x = 0; x != pat->length; x++)
      pat->map[y][x] = buff[x] == '1';
  }
  free(buff);
}

static Pattern *parse_file(FILE *f) {
  if (!f)
    exit(1);
  Pattern *pat = malloc(sizeof(Pattern));
  char *buff = NULL;
  int size;
  size_t ms = 0;
  size = getline(&buff, &ms, f);
  pat->name = malloc(sizeof(char) * (size + 2));
  strcpy(pat->name, buff);
  pat->name[strlen(pat->name) - 1] = '\0';
  getline(&buff, &ms, f);
  pat->type = get_type(buff);
  getline(&buff, &ms, f);
  pat->height = atoi(buff);
  getline(&buff, &ms, f);
  pat->length = atoi(buff);
  get_map(pat, f);
  free(buff);
  return pat;
}

static Pattern *get_pattern(const char *fname) {
  char *f_name = calloc(strlen(fname) + 12, sizeof(char));
  strcat(f_name, "patterns/");
  strcat(f_name, fname);
  FILE *f = fopen(f_name, "r");
  free(f_name);
  return parse_file(f);
}

Storage **load_patterns() {
  Storage **storage = calloc(NB_PATTERNS_TYPE, sizeof(Storage *));
  for (int i = 0; i != NB_PATTERNS_TYPE; i++) {
    storage[i] = calloc(1, sizeof(Storage));
    storage[i]->type = i;
  }
  DIR *d;
  struct dirent *dir;
  d = opendir("patterns");
  if (!d)
    exit(1);
  while ((dir = readdir(d))) {
    if (dir->d_type == 8) {
      printf("%s\n", dir->d_name);
      Pattern *pat = get_pattern(dir->d_name);
      Storage *sto = storage[pat->type];
      sto->length++;
      sto->pat = realloc(sto->pat, sizeof(Pattern *) * sto->length);
      sto->pat[sto->length - 1] = pat;
    }
  }
  closedir(d);
  return storage;
}

void free_storage(Storage **storage) {
  for (int i = 0; i != NB_PATTERNS_TYPE; i++) {
    for (int l = 0; l != storage[i]->length; l++) {
      for (int y = 0; y != storage[i]->pat[l]->height; y++)
        free(storage[i]->pat[l]->map[y]);
      free(storage[i]->pat[l]->name);
      free(storage[i]->pat[l]->map);
      free(storage[i]->pat[l]);
    }
    free(storage[i]->pat);
    free(storage[i]);
  }
  free(storage);
}
