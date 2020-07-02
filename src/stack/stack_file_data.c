#include "stack_file_data.h"

void stack_file_data_free(Stack *stack) {
  struct file_data *temp = stack_pop(stack);
  while (temp) {
    file_data_free(temp);
  }
  free(stack);
}