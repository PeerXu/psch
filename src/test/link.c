#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* key_type;
typedef int value_type;

#define key_gt(a, b) (strcmp((a), (b)) == 1)
#define key_lt(a, b) (strcmp((a), (b)) == -1)
#define key_eq(a, b) (strcmp((a), (b)) == 0)
#define new_node() (node *)malloc(sizeof(node))

#define KEY(n) ((n)->key)
#define VALUE(n) ((n)->value)

typedef struct _node {
  key_type key;
  value_type value;
  struct _node *next;
}node;

node *new_list() {
  node *head=NULL;
  head = new_node();
  head->next = NULL;
  return head;
}

value_type find(node *head, key_type k) {
  node *p = head->next;
  while (p && !key_eq(k, p->key)) p = p->next;
  return p && p->value;
}

int insert(node *head, key_type k, value_type v) {
  node *p = head, *np, *cur = new_node();
  cur->key = k;
  cur->value = v;

  while (p->next && key_lt(KEY(p->next), k)) {
    p = p->next;
  }
  np = p->next;
  
  p->next = cur;
  cur->next = np;
  
  return 0;
}

value_type delete(node *head, key_type k) {
  value_type v;
  node *p = head;
  node *fp = NULL;

  while (p->next && !key_eq(KEY(p->next), k)) {
    p = p->next;
  }

  if (!p->next) {
    return (value_type)NULL;
  }

  fp = p->next;
  p->next = fp->next;

  free(fp);

  return 0;
}

int _print_link(node *list) {
  if (list) {
    printf("(key:%10s, value: %10d)\n", list->key, list->value);
  } else {
    return 0;
  }
  return _print_link(list->next);
}

int print_link(node *list) {
  return _print_link(list->next);
}

int main(void) {
  node *list = new_list();
  insert(list, "3", 3);
  insert(list, "1", 1);
  insert(list, "5", 5);
  print_link(list);
  printf("delete:\n");
  delete(list, "3");
  print_link(list);
  return 0;
}
