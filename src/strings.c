// #include "strings.h"
#include <stdio.h>
#include <stdlib.h>

char *getFilename(const char *path) {
  char *filename = malloc(100 * sizeof(char));
  int len = sizeof(path) / sizeof(char);
  int filename_it = 0;
  while (*path != '/') {
    // putchar(filename[filename_it]);
    putchar(path[len]);
    filename[filename_it++] = path[len--];
  }
  return filename;
}

int main(int argc, char *argv[]) {
  char *path = "data/data/com.termux";
  char *name = getFilename(path);
  puts(name);
  return EXIT_SUCCESS;
}
