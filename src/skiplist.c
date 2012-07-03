#include "skiplist.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

node NIL;

int random_level(int max) {
  int k = 1;
  while((rand() % 2) && k < max) k++;
  return k;
}

key_type _skl_new_key(key_type k) {
  int len;
  key_type r = NULL;

  if (!k) {
    return (key_type)NULL;
  }

  len = strlen(k);
  len = len < KEY_BUFF_SIZE ? len : KEY_BUFF_SIZE;

  r = malloc(sizeof(char) * KEY_BUFF_SIZE);
  strncpy(r, k, len);
  r[len] = '\0';

  return r;
}

node _skl_new_node(int l, key_type k, value_type v) {
  int len;
  node n;
  n = malloc(sizeof(struct _node));
  n->height = l;

  /* n->key = k; */
  n->key = _skl_new_key(k);

  n->value = v;
  n->levels = new_level(l);
  return n;
}

int _skl_free_node(node n) {
  if (NULL == n) {
    return 1;
  }

  if(n->key) free(n->key);
  n->key = NULL;

  if(n->levels) free(n->levels);
  n->levels = NULL;

  free(n);
  return 0;
}

int skl_init() {
  int i;

  srand(time(NULL));

  NIL = _skl_new_node(MAX_NUMBER_OF_LEVELS, NIL_KEY, NIL_VALUE);
  for (i=0; i<MAX_NUMBER_OF_LEVELS; i++) {
    (NIL->levels[i]).cur_level = i;
    (NIL->levels[i]).forward = NULL;
  }

  return 0;
}

skiplist *skl_new_list() {
  skiplist *list;
  node pn;
  int i;

  pn = _skl_new_node(MAX_NUMBER_OF_LEVELS, HEAD_KEY, HEAD_VALUE);
  for (i = 0; i < MAX_NUMBER_OF_LEVELS; ++i) {
    (pn->levels[i]).cur_level = i+1;
    (pn->levels[i]).forward = NIL;
  }

  list = (struct _skiplist *)malloc(sizeof(struct _skiplist));
  list->height = MAX_NUMBER_OF_LEVELS;
  list->head = pn;
  
  return list;
}

node _skl_find_element_iter(node n, int l, key_type k) {
  if (0 > l || n == NIL) {
    return NIL;
  }

  if (key_eq(n->key, k)) {
    return n;
  }

  if ((n->levels[l-1]).forward != NIL && !key_lt(k, ((n->levels[l-1]).forward)->key)) {
    return _skl_find_element_iter((n->levels[l-1]).forward, l, k);
  } else {
    return _skl_find_element_iter(n, l - 1, k);
  }
}

node skl_find_element(skiplist *l, key_type k) {
  return _skl_find_element_iter(l->head, l->height, k);
}

value_type skl_find(skiplist *l, key_type k) {
  node n = skl_find_element(l, k);
  return NIL == n ? NULL : n->value;
}

node _skl_find_element_lt(node n, int l, key_type k) {
  node ret;
  if (NIL == n) {
    return NIL;
  }
  if ((n->levels[l]).forward != NIL && !key_lt(((n->levels[l]).forward)->key, k)) {
    return n;
  }
  ret = _skl_find_element_lt((n->levels[l]).forward, l, k);
  return ret != NIL ? ret : n;
}

int _skl_update_level_insert(skiplist *list, int level, node n) {
  node prev_node = _skl_find_element_lt(list->head, level, n->key);

  (n->levels[level]).forward = (prev_node->levels[level]).forward;
  (prev_node->levels[level]).forward = n;
  (prev_node->levels[level]).cur_level = level;

  return 0;
}

int skl_insert(skiplist *l, key_type k, value_type v) {
  int i;
  node n = _skl_new_node(random_level(l->height), k, v);

  for (i = 0; i < n->height; ++i) {
    _skl_update_level_insert(l, i, n);
  }

  return 0;
}

int _skl_update_level_delete(skiplist *list, int level, node n) {
  node prev_node = _skl_find_element_lt(list->head, level, n->key);

  (prev_node->levels[level]).forward = (n->levels[level]).forward;

  return 0;
}

value_type skl_delete(skiplist *l, key_type k) {
  int i;
  node n = skl_find_element(l, k);

  if (NIL == n) {
    return (value_type)-1;
  }

  for (i = 0; i < n->height; ++i) {
    _skl_update_level_delete(l, i, n);
  }

  _skl_free_node(n);

  return 0;
}

int _skl_free_list_iter(node n) {
  if (NIL == n) {
    return 0;
  }

  _skl_free_list_iter((n->levels[0]).forward);
  _skl_free_node(n);

  return 0;
}

int skl_free_list(skiplist *l) {
  return _skl_free_list_iter(l->head);
}

#ifdef __DEBUG
int _skl_print_list_level(node n, int level) {
  node p;
  printf("%3d: ", level);
  for (p = n; p != NIL; p = (p->levels[level-1]).forward) {
    printf("(%s, 0x%08x), ", p->key, p->value);
  }
  printf("\n");
  return 0;
}

int skl_print_list(skiplist *l) {
  int i;
  node p;

  for (i = l->height; i > 0; --i) {
    if (!((l->head->levels[i-1]).forward == NIL)) {
      _skl_print_list_level(l->head, i);
    }
  }
  
  return 0;
}
#endif
