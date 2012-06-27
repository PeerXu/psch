#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef __OBJECT_H
#define __OBJECT_H 1

#define __DEBUG 1

#define HEAP_SIZE 10240
#define STRING_BUFF_SIZE 2048
#define MAX_SYMBOL_LENGTH 128

#define ERR_OUT_OF_MEMORY "out of memory"
#define ERR_SYMBOL "error symbol"
#define ERR_CELL_TYPE "error cell type"

#define err_log(err_code) printf("%s\n", (err_code))
#define debug_log(msg) printf("%s\n", (msg))
#define debug_malloc_log(t, s) printf("cell type: %s\ncell: %s\n", (t), (s))

#define ltrim(p) while (' ' == *(p)) { ++(p); }
/* #define rtrim(p) while (' ' == *(p)) { --(p); } */

enum _cell_type {
  T_NIL=0,
  T_FALSE,
  T_TRUE,
  T_MAX_CONSTANT,
  T_NUMBER,
  T_STRING,
  T_PAIR,
  T_SYMBOL
};

enum _num_type {
  NT_INTEGER=0,
  NT_DOUBLE=1
};

struct _string {
  char _data[STRING_BUFF_SIZE];
  unsigned int _length;
};

struct _number {
  unsigned int _type;
  union {
    long _l;
    double _d;
  } _data;
};

struct _pair {
  struct {
    struct cell *_car;
    struct cell *_cdr;
  } _data;
};

struct _symbol {
  char _data[MAX_SYMBOL_LENGTH];
};

struct _object {
};

struct cell {
  unsigned int _type;
  union {
    struct _symbol *_sym;
    struct _string *_str;
    struct _number *_num;
    struct _pair *_pair;
    struct _object *_constant;
  } _data;
};

#define CELL_DATA(x) ((x)->_data)
#define CELL_TYPE(x) ((x)->_type)

struct _object constant_object[T_MAX_CONSTANT];
struct cell constant_cell[T_MAX_CONSTANT];
/* struct cell constant_cell[T_MAX_CONSTANT] = { */
/*   {T_NIL, constant_object[T_NIL]}, */
/*   {T_FALSE, constant_object[T_FALSE]}, */
/*   {T_TRUE, constant_object[T_TRUE]} */
/* }; */

struct cell heap[2][HEAP_SIZE];
int free_register = 0;
int scan_register = 0;
int current_heap_register = 0;
int free_heap_register = 1;
#define CURRENT_HEAP heap[current_heap_register]
#define FREE_HEAP heap[free_heap_register]

int is_atom(char *s);
int is_symbol(char *s);
int is_string(char *s);
int is_number(char *s);
int is_pair(char *s);
int is_nil(char *s);
int is_true(char *s);
int is_false(char *s);

int cell_is_atom(struct cell *cell);
int cell_is_symbol(struct cell *cell);
int cell_is_string(struct cell *cell);
int cell_is_number(struct cell *cell);
int cell_is_pair(struct cell *cell);
int cell_is_nil(struct cell *cell);
int cell_is_true(struct cell *cell);
int cell_is_false(struct cell *cell);

struct _cell_method {
  int (*is_atom)(struct cell *);
  int (*is_string)(struct cell *);
  int (*is_number)(struct cell *);
  int (*is_symbol)(struct cell *);
  int (*is_pair)(struct cell *);
  int (*is_nil)(struct cell *);
  int (*is_true)(struct cell *);
  int (*is_false)(struct cell *);
} cell_method = {
  cell_is_atom,
  cell_is_string,
  cell_is_number,
  cell_is_symbol,
  cell_is_pair,
  cell_is_nil,
  cell_is_true,
  cell_is_false
};

struct cell *mk_cell(char *s);
int free_cell(struct cell *cell);
struct _symbol *mk_symbol(char *s);
int free_symbol(struct _symbol *sym);
struct _string *mk_string(char *s);
int free_string(struct _string *str);
struct _number *mk_number(char *s);
int free_number(struct _number *num);
struct _pair *mk_pair(char *s);
int free_pair(struct _pair *pair);

int petty_print_expr(char *s);
int init_constant_cell();

#endif
