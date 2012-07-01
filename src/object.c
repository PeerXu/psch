#include "object.h"

/* extern struct _object constant_object[T_MAX_CONSTANT]; */
/* extern struct cell constant_cell[T_MAX_CONSTANT]; */
/* extern struct cell heap[2][HEAP_SIZE]; */
/* extern int free_register; */
/* extern int scan_register; */
/* extern int current_heap_register; */
/* extern int free_heap_register; */
/* extern struct _cell_method cell_method; */


/* ===== private method ===== */

int init_constant_cell() {
  int i;

#ifdef __DEBUG
  debug_log("initialize constant cell...");
#endif

  for( i = 0; i < T_MAX_CONSTANT; ++i) {
    CELL_TYPE(constant_cell+i) = i;
    CELL_DATA(constant_cell+i)._constant = &constant_object[i];
  }

  return 0;
}

int init_register() {
#ifdef __DEBUG
  debug_log("initialize register...");
#endif

  free_register = 0;
  scan_register = 0;
  current_heap_register = 0;
  free_heap_register = 1;

  return 0;
}

int init_interface() {
#ifdef __DEBUG
  debug_log("initialize interface...");
#endif

  cell_method.is_atom = cell_is_atom;
  cell_method.is_string = cell_is_string;
  cell_method.is_number = cell_is_number;
  cell_method.is_symbol = cell_is_symbol;
  cell_method.is_pair = cell_is_pair;
  cell_method.is_nil = cell_is_nil;
  cell_method.is_true = cell_is_true;
  cell_method.is_false = cell_is_false;

  return 0;
}

int init() {
  init_constant_cell();
  init_register();
  init_interface();
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
    while (' ' != *ps && ')' != *ps) {
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
  /* buff[length - 1] = '\0'; */
  buff[length] = '\0';

  cell = mk_cell(buff);

  return cell;
}

struct cell *mk_pair_cdr(char *s) {
  struct cell *cell = NULL;
  char buff[STRING_BUFF_SIZE];
  int start, end, length;

  get_pair_cdr_index(s, &start, &end);
  if (end <= start) {
    strcpy(buff, "()\0");
  } else {
    length = end - start;
    buff[0] = '(';
    strncpy(buff+1, s+start, length);
    buff[length+1] = ')';
    buff[length+2] = '\0';
  }

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
    if (is_nil(s)) {
      cell = &constant_cell[T_NIL];
#ifdef __DEBUG
      debug_malloc_log("nil", s);
#endif
    } else {
      CELL_TYPE(cell) = T_PAIR;
      CELL_DATA(cell)._pair = mk_pair(s);
#ifdef __DEBUG
      debug_malloc_log("pair", s);
#endif
    }
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
