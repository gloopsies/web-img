#include "stack.h"

struct stack_element {
  void *data;
  struct stack_element *next;
};

typedef struct stack {
  struct stack_element *elements;
  int len;
} Stack;

Stack *stack_new() {
  Stack *new_stack = malloc(sizeof(*new_stack));
  new_stack->elements = NULL;
  new_stack->len = 0;

  return new_stack;
}

int stack_push(Stack *stack, void *data) {
  struct stack_element *new_element = malloc(sizeof(*new_element));
  if (!new_element) return 1;

  new_element->data = data;
  new_element->next = stack->elements;
  stack->elements = new_element;
  ++stack->len;

  return 0;
}

void *stack_pop(Stack *stack) {
  if (!stack->elements) return NULL;

  struct stack_element *pop_element = stack->elements;
  char *data = pop_element->data;
  stack->elements = pop_element->next;
  --stack->len;
  free(pop_element);
  return data;
}

void stack_free(Stack *stack) {
  char *temp = stack_pop(stack);
  while (temp) {
    free(temp);
  }
  free(stack);
}

int stack_length(Stack *stack) { return stack->len; }

void stack_print(Stack *stack) {
  struct stack_element *elem = stack->elements;
  while (elem) {
    printf("%s\n", (char *)elem->data);
    elem = elem->next;
  }
  printf("end\n");
}