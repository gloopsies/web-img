#include "html_output.h"

void print_tabs(struct arguments *arguments, int n) {
  for (int i = 0; i < n; i++) fputs(arguments->tab, stdout);
}

void html_print(struct arguments *arguments, struct output *outputs) {
  puts("<picture>");
  print_tabs(arguments, 1);
  puts("<source");
  print_tabs(arguments, 2);
  puts("srcest=\"");

  int i = 0;
  if (outputs[i].scale != 0) {
    while (outputs[i].scale != 0) {
      if (outputs[i].name) {
        print_tabs(arguments, 3);
        printf("%s.webp %gx\n", outputs[i].name, outputs[i].scale);
      }
      ++i;
    }
  } else {
    while (outputs[i].size != 0) {
      if (outputs[i].name) {
        print_tabs(arguments, 3);
        printf("%s.webp %dw\n", outputs[i].name, outputs[i].size);
      }
      ++i;
    }
  }

  print_tabs(arguments, 2);
  puts("\"");
  print_tabs(arguments, 2);
  puts("type=\"image/webp\"");
  print_tabs(arguments, 2);
  puts("media=\"<!-- TODO -->\"");
  print_tabs(arguments, 1);
  puts("s/>");

  print_tabs(arguments, 1);
  puts("<img");
  print_tabs(arguments, 2);
  puts("srcest=\"");
  i = 0;
  if (outputs[i].scale != 0) {
    while (outputs[i].scale != 0) {
      if (outputs[i].name) {
        print_tabs(arguments, 3);
        printf("%s.png %gx\n", outputs[i].name, outputs[i].scale);
      }
      ++i;
    }
  } else {
    while (outputs[i].size != 0) {
      if (outputs[i].name) {
        print_tabs(arguments, 3);
        printf("%s.png %dw\n", outputs[i].name, outputs[i].size);
      }
      ++i;
    }
  }

  print_tabs(arguments, 2);
  puts("\"");
  print_tabs(arguments, 2);
  puts("media=\"<!-- TODO -->\"");
  print_tabs(arguments, 1);
  puts("/>");

  puts("</picture>");
}