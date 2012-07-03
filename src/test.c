#include <stdio.h>
#include <stdlib.h>

#include "object.h"
#include "skiplist.h"

#define LENGTH 10

int main(void) {
  skiplist *l;
  int i;
  char *buff[LENGTH] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};

  skl_init();

  l = skl_new_list();

  for ( i = 0; i < LENGTH; ++i) {
    skl_insert(l, (key_type)buff[i], mk_cell(buff[i]));
  }
  
  skl_print_list(l);

  skl_free_list(l);

  return 0;
}
