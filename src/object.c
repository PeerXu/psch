#include "object.h"

/* #include <stdio.h> */
/* #include <stdlib.h> */
/* #include <string.h> */
/* #include <ctype.h> */

/* #define __DEBUG 1 */

/* #define HEAP_SIZE 10240 */
/* #define STRING_BUFF_SIZE 2048 */
/* #define MAX_SYMBOL_LENGTH 128 */

/* #define ERR_OUT_OF_MEMORY "out of memory" */
/* #define ERR_SYMBOL "error symbol" */
/* #define ERR_CELL_TYPE "error cell type" */

/* #define err_log(err_code) printf("%s\n", (err_code)) */
/* #define debug_log(msg) printf("%s\n", (msg)) */
/* #define debug_malloc_log(t, s) printf("cell type: %s\ncell: %s\n", (t), (s)) */

/* #define ltrim(p) while (' ' == *(p)) { ++(p); } */
/* #define rtrim(p) while (' ' == *(p)) { --(p); } */

/* enum _cell_type { */
/*   T_NIL=0, */
/*   T_FALSE, */
/*   T_TRUE, */
/*   T_MAX_CONSTANT, */
/*   T_NUMBER, */
/*   T_STRING, */
/*   T_PAIR, */
/*   T_SYMBOL */
/* }; */

/* enum _num_type { */
/*   NT_INTEGER=0, */
/*   NT_DOUBLE=1 */
/* }; */

/* struct _string { */
/*   char _data[STRING_BUFF_SIZE]; */
/*   unsigned int _length; */
/* }; */

/* struct _number { */
/*   unsigned int _type; */
/*   union { */
/*     long _l; */
/*     double _d; */
/*   } _data; */
/* }; */

/* struct _pair { */
/*   struct { */
/*     struct cell *_car; */
/*     struct cell *_cdr; */
/*   } _data; */
/* }; */

/* struct _symbol { */
/*   char _data[MAX_SYMBOL_LENGTH]; */
/* }; */

/* struct _object { */
/* }; */

/* struct cell { */
/*   unsigned int _type; */
/*   union { */
/*     struct _symbol *_sym; */
/*     struct _string *_str; */
/*     struct _number *_num; */
/*     struct _pair *_pair; */
/*     struct _object *_nil; */
/*     struct _object *_true; */
/*     struct _object *_false; */
/*   } _data; */
/* }; */

/* #define CELL_DATA(x) ((x)->_data) */
/* #define CELL_TYPE(x) ((x)->_type) */

extern struct _object constant_object[T_MAX_CONSTANT];
extern struct cell constant_cell[T_MAX_CONSTANT];
/* struct cell constant_cell[T_MAX_CONSTANT] = {  */
/*   {T_NIL, constant_object[T_NIL]}, */
/*   {T_FALSE, constant_object[T_FALSE]}, */
/*   {T_TRUE, constant_object[T_TRUE]} */
/* }; */

/* struct cell heap[2][HEAP_SIZE]; */
/* int free_register; */
/* int scan_register; */
/* int current_heap_register; */
/* int free_heap_register; */

/* #define CURRENT_HEAP heap[current_heap_register] */
/* #define FREE_HEAP heap[free_heap_register] */

/* int is_atom(char *s); */
/* int is_symbol(char *s); */
/* int is_string(char *s); */
/* int is_number(char *s); */
/* int is_pair(char *s); */
/* int is_nil(char *s); */
/* int is_true(char *s); */
/* int is_false(char *s); */

/* int cell_is_atom(struct cell *cell); */
/* int cell_is_symbol(struct cell *cell); */
/* int cell_is_string(struct cell *cell); */
/* int cell_is_number(struct cell *cell); */
/* int cell_is_pair(struct cell *cell); */
/* int cell_is_nil(struct cell *cell); */
/* int cell_is_true(struct cell *cell); */
/* int cell_is_false(struct cell *cell); */

/* struct { */
/*   int (*is_atom)(struct cell *); */
/*   int (*is_string)(struct cell *); */
/*   int (*is_number)(struct cell *); */
/*   int (*is_symbol)(struct cell *); */
/*   int (*is_pair)(struct cell *); */
/*   int (*is_nil)(struct cell *); */
/*   int (*is_true)(struct cell *); */
/*   int (*is_false)(struct cell *); */
/* } cell_method = { */
/*   cell_is_atom, */
/*   cell_is_string, */
/*   cell_is_number, */
/*   cell_is_symbol, */
/*   cell_is_pair, */
/*   cell_is_nil, */
/*   cell_is_true, */
/*   cell_is_false */
/* }; */

extern struct _cell_method cell_method;

/* struct cell *mk_cell(char *s); */
/* int free_cell(struct cell *cell); */
/* struct _symbol *mk_symbol(char *s); */
/* int free_symbol(struct _symbol *sym); */
/* struct _string *mk_string(char *s); */
/* int free_string(struct _string *str); */
/* struct _number *mk_number(char *s); */
/* int free_number(struct _number *num); */
/* struct _pair *mk_pair(char *s); */
/* int free_pair(struct _pair *pair); */

/* int petty_print_expr(char *s); */
/* int init_constant_cell(); */

/* ===== private method ===== */
int init_constant_cell() {
  int i;

  for( i = 0; i < T_MAX_CONSTANT; ++i) {
    CELL_TYPE(constant_cell+i) = i;
    CELL_DATA(constant_cell+i) = constant_object[i];
  }

  return 0;
}

int is_number_integer(char *s) {
  char *ps = s;
  for (; *ps != '\0'; ++ps) {
    if ('.' == *ps) {
      return 0;
    }
  }
  return 1;
}

struct _number *malloc_number() {
  struct _number *num = NULL;

  num = malloc(sizeof(struct _number));
  if (NULL == num) {
    err_log(ERR_OUT_OF_MEMORY);
    exit(1);
  }

  return num;
}

struct _number *mk_number_integer(char *s) {
  struct _number *num = malloc_number();
  
  num->_type = NT_INTEGER;
  num->_data._l = atol(s);

  return num;
}

struct _number *mk_number_float(char *s) {
  struct _number *num = malloc_number();

  num->_type = NT_DOUBLE;
  num->_data._d = atof(s);

  return num;
}

int is_real_pair(char *s) {
  return 0;
}

int scan_start_and_end_char(char *s, char ec) {
  int current = 0;

  do {
    ++s;
    ++current;
  } while (ec != *s);
  ++current;

  return current;
}

// get_pair_car("(+ 1 2 3)", &start, &end)) => start: 1, end: 2, result: +
int get_pair_car_index(char *s, int *start, int *end) {
  char *ps = s + 1;
  int _start = 1, _end;
  
  while (' ' == *ps) {
    ++ps;
    ++_start;
  }

  *start = _start;
  _end = _start;

  if ('"' == *ps) {
    _end += scan_start_and_end_char(ps, '"');
  } else if ('(' == *ps) {
    _end += scan_start_and_end_char(ps, ')');
  } else {
    while (' ' != *ps) {
      ++ps;
      ++_end;
    }
  }

  *end = _end;
  return 0;
}

// get_pair_cdr("(+ 1 2 3)", &start, &end) => start: 3, end: 8, result: 1 2 3
// get_pair_cdr("(1 . 2)", &start, &end) => start: 5, end: 6, result: 2
int get_pair_cdr_index(char *s, int *start, int *end) {
  char *ps;
  int _start, _end, tmp;

  get_pair_car_index(s, &tmp, &_start);
  ps = s + _start - 1;

  while (' ' == *ps) {
    ++ps;
    ++_start;
  }
  ++_start;
  *start = _start;

  _end = strlen(s) - 1;
  ps = s + _end;
 
  while(' ' == *ps) {
    --ps;
    --_end;
  }
  *end = _end;

  return 0;
}


struct cell *mk_pair_car(char *s) {
  struct cell *cell = NULL;
  char buff[STRING_BUFF_SIZE];
  int start, end, length;

  get_pair_car_index(s, &start, &end);
  length = end - start;
  strncpy(buff, s+start, length);
  buff[length - 1] = '\0';

  cell = mk_cell(buff);

  return cell;
}

struct cell *mk_pair_cdr(char *s) {
  struct cell *cell = NULL;
  char buff[STRING_BUFF_SIZE];
  int start, end, length;

  get_pair_cdr_index(s, &start, &end);
  length = end - start;
  buff[0] = '(';
  strncpy(buff+1, s+start, length);
  buff[length+1] = ')';
  buff[length+2] = '\0';

  /* sprintf(buff, "(%s)", buff); */
  cell = mk_cell(buff);

  return cell;
}

int print_number_integer(struct _number *num) {
  printf("number: integer: %ld\n", (num->_data)._l);
  return 0;
}

int print_number_double(struct _number *num) {
  printf("number: double: %lf\n", (num->_data)._d);
  return 0;
}

int gc_loop() {
  free_register = 0;
  return 0;
}

struct cell *malloc_cell() {
  if (HEAP_SIZE <= free_register) {
    gc_loop();
  }

#ifdef __DEBUG
  printf("malloc cell: free=%d\n", free_register);
#endif

  return &CURRENT_HEAP[free_register++];
}

/* ===== object malloc ===== */
struct cell *mk_cell(char *s) {
  struct cell *cell = NULL;
  
  cell = malloc_cell();
  if (NULL == cell) {
    err_log(ERR_OUT_OF_MEMORY);
    exit(1);
  }

  if (is_string(s)) {
    CELL_TYPE(cell) = T_STRING;
    CELL_DATA(cell)._str = mk_string(s);
#ifdef __DEBUG
    debug_malloc_log("string", s);
#endif
  } else if (is_number(s)) {
    CELL_TYPE(cell) = T_NUMBER;
    CELL_DATA(cell)._num = mk_number(s);
#ifdef __DEBUG
    debug_malloc_log("number", s);
#endif
  } else if (is_pair(s)) {
    CELL_TYPE(cell) = T_PAIR;
    CELL_DATA(cell)._pair = mk_pair(s);
  } else if (is_symbol(s)) {
    CELL_TYPE(cell) = T_SYMBOL;
    CELL_DATA(cell)._sym = mk_symbol(s);
#ifdef __DEBUG
    debug_malloc_log("symbol", s);
#endif
  } else {
    err_log(ERR_SYMBOL);
  }

  return cell;
}

struct _string *mk_string(char *s) {
  struct _string *str = NULL;
  int length = strlen(s);

  if (length > STRING_BUFF_SIZE) {
    length = STRING_BUFF_SIZE - 1;
  }

  str = malloc(sizeof(struct _string));
  if (NULL == str) {
    err_log(ERR_OUT_OF_MEMORY);
    exit(1);
  }

  strncpy(str->_data, s, length);
  str->_data[length] = '\0';
  str->_length = length;

  return str;
}

struct _number *mk_number(char *s) {
  struct _number *num = NULL;
  
  if (is_number_integer(s)) {
    num = mk_number_integer(s);
  } else {
    num = mk_number_float(s);
  }

  return num;
}

struct _symbol *mk_symbol(char *s) {
  struct _symbol *sym = NULL;
  int length = strlen(s);

  if (MAX_SYMBOL_LENGTH <= length) {
    err_log(ERR_SYMBOL);
    exit(1);
  }

  sym = malloc(sizeof(struct _symbol));
  if (NULL == sym) {
    err_log(ERR_OUT_OF_MEMORY);
    exit(1);
  }

  if (is_symbol(s)) {
    strncpy(sym->_data, s, length);
    sym->_data[length] = '\0';
  }
  return sym;
}

struct _pair *mk_pair(char *s) {
  struct _pair *pair = NULL;
  char *current_char_pointer = s;

  pair = malloc(sizeof(struct _pair));
  if (NULL == pair) {
    err_log(ERR_OUT_OF_MEMORY);
    exit(1);
  }

  (pair->_data)._car = mk_pair_car(s);
  (pair->_data)._cdr = mk_pair_cdr(s);
}

/* ===== object free ===== */

int free_string(struct _string *str) {
  free(str);
  return 0;
}

int free_number(struct _number *num) {
  free(num);
  return 0;
}

int free_symbol(struct _symbol *sym) {
  free(sym);
  return 0;
}

int free_pair(struct _pair *pair) {
  free_cell((pair->_data)._car);
  free_cell((pair->_data)._cdr);
  free(pair);
  return 0;
}

int free_cell(struct cell *cell) {
  switch(cell->_type) {
  case T_NUMBER:
    free_number(CELL_DATA(cell)._num);
    break;
  case T_STRING:
    free_string(CELL_DATA(cell)._str);
    break;
  case T_SYMBOL:
    free_symbol(CELL_DATA(cell)._sym);
    break;
  case T_PAIR:
    free_pair(CELL_DATA(cell)._pair);
    break;
  default:
    err_log(ERR_CELL_TYPE);
  }
  return 0;
}

/* ===== object type assert ===== */
int is_atom(char *s) {
  return is_string(s) || is_number(s) || is_symbol(s);
}

int is_string(char *s) {
  int length = strlen(s);
  if (0 == length) {
    return 0;
  }
  return '"' == s[0] && '"' == s[length - 1];
}

int is_number(char *s) {
  int _float_flag = 0;
  char *ps = s;
  while('\0' != *ps) {
    if ('.' == *ps) {
      if (0 == _float_flag) {
	_float_flag = 1;
      } else {
	return 0;
      }
    } else if (!isdigit(*ps)) {
      return 0;
    }
    ++ps;
  }
  return 1;
}

int is_symbol(char *s) {
  return s && strlen(s) >= 1 && ',' != *s && '#' != *s;
}

int is_pair(char *s) {
  int length = strlen(s);
  return length !=0 && '(' == s[0] && ')' == s[length - 1];
}

int is_nil(char *s) {
  char *ps = s;
  ltrim(ps);

  if ('\'' == *ps) {
    ++ps;
  }

  if ('(' != *ps) {
    return 0;
  }
  ++ps;

  while(' ' == *ps) {
    ++ps;
  }
  
  if (')' != *ps) {
    return 0;
  }

  return 1;
}

int cell_is_atom(struct cell *cell) {
  return cell_is_string(cell) || cell_is_number(cell) || cell_is_symbol(cell);
}

int cell_is_string(struct cell *cell) {
  return CELL_TYPE(cell) == T_STRING;
}

int cell_is_number(struct cell *cell) {
  return CELL_TYPE(cell) == T_NUMBER;
}

int cell_is_pair(struct cell *cell) {
  return CELL_TYPE(cell) == T_PAIR;
}

int cell_is_nil(struct cell *cell) {
  return CELL_TYPE(cell) == T_NIL;
}

int cell_is_true(struct cell *cell) {
  return CELL_TYPE(cell) == T_TRUE;
}

int cell_is_false(struct cell *cell) {
  return CELL_TYPE(cell) == T_FALSE;
}

int cell_is_symbol(struct cell *cell) {
  return CELL_TYPE(cell) == T_SYMBOL;
}

/* ===== object print ===== */
int print_cell(struct cell *cell) {
  
  return 0;
}

int print_string(struct _string *str) {
  printf("string: %s\n", str->_data);
  return 0;
}

int print_number(struct _number *num) {
  if (NT_INTEGER == num->_type) {
    print_number_integer(num);
  } else if (NT_DOUBLE == num->_type) {
    print_number_double(num);
  }
  return 0;
}

/* void test_func__get_pair_any_index(char *string) { */
/*   char car_buff[1024], cdr_buff[1024]; */
/*   int car_start, car_end, cdr_start, cdr_end; */

/*   get_pair_car_index(string, &car_start, &car_end); */
/*   strncpy(car_buff, string + car_start, car_end - car_start); */
/*   car_buff[car_end-car_start] = '\0'; */

/*   get_pair_cdr_index(string, &cdr_start, &cdr_end); */
/*   strncpy(cdr_buff, string + cdr_start, cdr_end - cdr_start); */
/*   cdr_buff[cdr_end-cdr_start] = '\0'; */

/*   printf("(car %s): %s, start:%d, end:%d\n", string, car_buff, car_start, car_end); */
/*   printf("(cdr %s): %s, start:%d, end:%d\n", string, cdr_buff, cdr_start, cdr_end); */
/* } */

void test_func__is_nil(char *s) {
  printf("is_nil(%s)=%d\n", s, is_nil(s));
}

/* int main(void) { */
/*   /\* mk_cell("abc"); *\/ */
/*   /\* mk_cell("123.321"); *\/ */
/*   /\* mk_cell("\"hello, world\""); *\/ */
/*   /\* mk_cell("(+ 1 2)"); *\/ */

/*   /\* char *string = "((+ 1 2) 1 2 3)"; *\/ */
/*   /\* char buff[1024]; *\/ */
/*   /\* int start, end; *\/ */
/*   /\* get_pair_car_index(string, &start, &end); *\/ */
/*   /\* strncpy(buff, string + start, end - start); *\/ */
/*   /\* buff[end-start] = '\0'; *\/ */
/*   /\* printf("(car %s): %s, start:%d, end:%d\n", string, buff, start, end); *\/ */

/*   /\* test_func__get_pair_any_index("(+ 1 2 3)"); *\/ */
/*   /\* test_func__get_pair_any_index("(\"abc efg\" 1 2 3)"); *\/ */
/*   /\* test_func__get_pair_any_index("((1 2 3 4) 1 2 3 4 5)"); *\/ */

/*   test_func__is_nil("'()"); */
/*   test_func__is_nil("()"); */
/*   test_func__is_nil("(+ 1 2 3)"); */
/*   test_func__is_nil("'(+ 1 2 3)"); */
/*   test_func__is_nil("  '(   )"); */
/*   test_func__is_nil("  '(   )   "); */

/*   return 0; */
/* } */
