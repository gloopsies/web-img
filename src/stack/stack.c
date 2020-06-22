#include "stack.h"

struct stack *stack_new() {
  struct stack *new_stack = malloc(sizeof(*new_stack));
  new_stack->elements = NULL;
  new_stack->len = 0;

  return new_stack;
}

int stack_push(struct stack *stack, char *data) {
  struct stack_element *new_element = malloc(sizeof(*new_element));
  if (!new_element) return 1;

  new_element->data = data;
  new_element->next = stack->elements;
  stack->elements = new_element;
  ++stack->len;

  return 0;
}

char *stack_pop(struct stack *stack) {
  if (!stack->elements) return NULL;

  struct stack_element *pop_element = stack->elements;
  char *data = pop_element->data;
  stack->elements = pop_element->next;
  --stack->len;
  free(pop_element);
  return data;
}

void stack_free(struct stack *stack) {
  while (stack_pop(stack)) {
  }
  free(stack);
}

void stack_print(struct stack *stack) {
  struct stack_element *elem = stack->elements;
  while (elem) {
    printf("%s\n", elem->data);
    elem = elem->next;
  }
  printf("end\n");
}