#include <stdio.h>
char *version = "0.0.1";

#define prompt "*>"
#define BUFFSIZE 1024

void repl();
int read(char *buff);
int syntax_check(char *expr);
int parse_expr(char *expr);

void repl() {
  char buff[BUFFSIZE];
  printf(prompt);
  while(read(buff)) {
    if (!syntax_check(buff)) {
      printf("[ERR]: syntax error\n");
    }
    else {
      printf("%s\n", buff);
    }
    printf(prompt);
  }
}

int read(char *buff) {
  int size = 0;
  char *buf = buff;
  char c;

  for (c = getchar(); c != '\n'; c = getchar()) {
    buf[size++] = c;
  }
  buf[size] = '\0';
  return size;
}

int syntax_check(char *expr) {
  int expr_count=0;
  int string_count=0;
  char *p = expr;

  while (*p != '\0') {
    if (string_count) {
      if (*p == '"') {
	--string_count;
      }
    }
    else if (*p == '"') {
      ++string_count;
    }
    else if (*p == '(') {
      ++expr_count;
    }
    else if (*p == ')') {
      --expr_count;
    }
    ++p;
  }

  if (expr_count || string_count) {
    return 0;
  }
  return 1;
}

int main(void) {
  printf("pscsh -- scheme interpreter\n");
  printf("version: %s\n", version);
  repl();
  return 0;
}
