#include "html_output.h"

void print_tabs(struct arguments *arguments, int n) {
  for (int i = 0; i < n; i++) fputs(arguments->tab, stdout);
}

void html_print(struct arguments *arguments, struct file_data file_in,
                struct output *outputs) {
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
  puts("/>");

  char *ext = img_type_to_string(file_in.type);
  if (ext) {
    print_tabs(arguments, 1);
    puts("<img");
    print_tabs(arguments, 2);
    puts("srcest=\"");
    i = 0;
    if (outputs[i].scale != 0) {
      while (outputs[i].scale != 0) {
        if (outputs[i].name) {
          print_tabs(arguments, 3);
          printf("%s.%s %gx\n", outputs[i].name, ext, outputs[i].scale);
        }
        ++i;
      }
    } else {
      while (outputs[i].size != 0) {
        if (outputs[i].name) {
          print_tabs(arguments, 3);
          printf("%s.%s %dw\n", outputs[i].name, ext, outputs[i].size);
        }
        ++i;
      }
    }

    print_tabs(arguments, 2);
    puts("\"");
    print_tabs(arguments, 2);
    printf("src=\"%s.%s\"\n", outputs[0].name, ext);
    print_tabs(arguments, 2);
    puts("media=\"<!-- TODO -->\"");
    print_tabs(arguments, 2);
    puts("alt=\"<!-- TODO -->\"");
    print_tabs(arguments, 1);
    puts("/>");
  }
  puts("</picture>");
}