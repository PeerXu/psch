#include <stdio.h>

int main(void) {
  char buff[1024];
  int size = 0;
  char c;
  for (c = getchar(); c != '\n'; c = getchar()) {
    buff[size++] = c;
  }
  buff[size] = '\0';
  printf("expr: %s, size: %d\n", buff, size);
  return 0;
}
