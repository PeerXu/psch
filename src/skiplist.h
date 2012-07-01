#ifndef __SKIPLIST_H
#define __SKIPLIST_H 1

#define __DEBUG 1

#define MAX_NUMBER_OF_LEVELS 16
#define MAX_LEVEL (MAX_NUMBER_OF_LEVELS-1)

typedef char* key_type;
typedef int value_type;

typedef struct _level {
  struct _node *forward;
  int cur_level;
} *level;

typedef struct _node {
  key_type key;
  value_type value;

  int height;

  level levels;
} *node;

typedef struct _skiplist {
  int height;
  node head;
} skiplist;

int skl_init();
skiplist *skl_new_list();
value_type skl_find(skiplist *l, key_type k);
int skl_insert(skiplist *l, key_type k, value_type v);
value_type skl_delete(skiplist *l, key_type k);
int skl_free_list(skiplist *l);

#ifdef __DEBUG
int random_level(int max);
node _skl_new_node(int l, key_type k, value_type v);
int skl_print_list(skiplist *l);
#endif

extern node NIL;

#define NIL_KEY (char *)NULL
#define NIL_VALUE (int)-1

#define HEAD_KEY (char *)""
#define HEAD_VALUE (int)-1

#define key_gt(a, b) ((b) && strcmp((a), (b)) == 1)
#define key_lt(a, b) (!(b) || strcmp((a), (b)) == -1)
#define key_eq(a, b) ((a) && (b) && strcmp((a), (b)) == 0)

#define new_level(l) (struct _level *)malloc((l)*sizeof(struct _level))

#endif
