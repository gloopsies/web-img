#ifndef __stack__
#define __stack__

#include <stdio.h>
#include <stdlib.h>

struct stack_element {
  char *data;
  struct stack_element *next;
};

struct stack {
  struct stack_element *elements;
  int len;
};

struct stack *stack_new();
int stack_push(struct stack *, char *);
char *stack_pop(struct stack *);
void stack_free(struct stack *);
void stack_print(struct stack *);

#endif