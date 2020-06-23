#ifndef __stack__
#define __stack__

#include <stdio.h>
#include <stdlib.h>

typedef struct stack Stack;

Stack *stack_new();
int stack_push(Stack *stack, char *data);
char *stack_pop(Stack *stack);
void stack_free(Stack *stack);
int stack_length(Stack *stack);
void stack_print(Stack *stack);

#endif