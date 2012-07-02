#include <stdio.h>
#include <stdlib.h>
#include "skiplist.h"

#define LENGTH 10

int make_list(skiplist *l) {
  node n1, n3, n5, n7, n9;
  n1 = _skl_new_node(1, "1", 1);
  n3 = _skl_new_node(2, "3", 3);
  n5 = _skl_new_node(3, "5", 5);
  n7 = _skl_new_node(2, "7", 7);
  n9 = _skl_new_node(1, "9", 9);

  /* level 1 */

  (l->head->levels[0]).forward = n1;

  (n1->levels[0]).forward = n3;
  (n1->levels[0]).cur_level = 1;
  
  (n3->levels[0]).forward = n5;
  (n3->levels[0]).cur_level = 1;

  (n5->levels[0]).forward = n7;
  (n5->levels[0]).cur_level = 1;

  (n7->levels[0]).forward = n9;
  (n7->levels[0]).cur_level = 1;
  
  (n9->levels[0]).forward = NIL;
  (n9->levels[0]).cur_level = 1;

  /* level 2 */

  (l->head->levels[1]).forward = n3;

  (n3->levels[1]).forward = n5;
  (n3->levels[1]).cur_level = 2;

  (n5->levels[1]).forward = n7;
  (n5->levels[1]).cur_level = 2;

  (n7->levels[1]).forward = NIL;
  (n7->levels[1]).cur_level = 2;

  /* level 3 */

  (l->head->levels[2]).forward = n5;

  (n5->levels[2]).forward = NIL;
  (n5->levels[2]).cur_level = 3;

  return 0;
}

int main(void) {
  skiplist *l;
  int i;
  char *buff[10] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};

  skl_init();

  l = skl_new_list();

  /* make_list(l); */
  /* skl_print_list(l); */

  /* skl_insert(l, "6", 6); */
  /* skl_insert(l, "4", 4); */
  /* skl_insert(l, "2", 2); */

  for ( i = 0; i < LENGTH; ++i) {
    skl_insert(l, (key_type)buff[i], (value_type)i);
  }
  
  skl_print_list(l);

  skl_delete(l, "10");

  skl_print_list(l);

  skl_free_list(l);

  return 0;
}
