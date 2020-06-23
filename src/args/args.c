#include "args.h"

#include <stdlib.h>
#include <string.h>

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;

  switch (key) {
    case 'o':
      free(arguments->out_dir);
      arguments->out_dir = strdup(arg);
      break;
    case 'f': {
      char *temp1 = arg;
      char *temp2;
      do {
        temp2 = strchr(temp1, ' ');
        if (temp2) {
          char *new_file = malloc((temp2 - temp1 + 1) * sizeof(char));
          strncpy(new_file, temp1, temp2 - temp1);
          new_file[temp2 - temp1] = '\0';

          while (temp2 && *temp2 == ' ') ++temp2;
          stack_push(arguments->out_names, new_file);
        } else {
          stack_push(arguments->out_names, strdup(temp1));
        }
        temp1 = temp2;
      } while (temp1);
      break;
    }
    case 't':
      free(arguments->tab);
      arguments->tab = strdup(arg);
      break;
    case 'w':
      arguments->width = atoi(arg);
      if (!arguments->width)
        fprintf(stderr, "Invalid width %s. Skipping...\n", arg);
      break;
    case 'h':
      arguments->height = atoi(arg);
      if (!arguments->width)
        fprintf(stderr, "Invalid height %s. Skipping...\n", arg);
      break;
    case USE_HEIGHT:
      arguments->use_height = 1;
      break;
    case 'n':
      arguments->no_compression = 1;
      break;
    case ARGP_KEY_ARG:
      stack_push(arguments->in_files, strdup(arg));
      break;
    case ARGP_KEY_END:
      if (state->arg_num <= 0) {
        fputs("At least 1 picture needed\n", stderr);
        exit(2);
      }
      if (stack_length(arguments->in_files) <
          stack_length(arguments->out_names)) {
        printf("Input: %d, out: %d\n", stack_length(arguments->in_files),
               stack_length(arguments->out_names));
        fputs("More output names then input files provided\n", stderr);
      }
      break;
    default:
      return ARGP_ERR_UNKNOWN;
      exit(1);
  }

  return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

struct arguments *arguments_get(int argc, char **argv) {
  struct arguments *arguments;
  if (!(arguments = (struct arguments *)malloc(sizeof(struct arguments))))
    return NULL;

  arguments->in_files = stack_new();
  arguments->out_dir = strdup(".");
  arguments->out_names = stack_new();
  arguments->tab = strdup("\t");
  arguments->width = 0;
  arguments->height = 0;
  arguments->use_height = 0;
  arguments->no_compression = 0;

  argp_parse(&argp, argc, argv, 0, 0, arguments);

  if (arguments->use_height && (arguments->height || arguments->width)) {
    arguments->use_height = 0;
    fputs("--use-height is ignored when used with static sizes!\n", stderr);
  }

  return arguments;
}

void arguments_free(struct arguments *arguments) {
  stack_free(arguments->in_files);
  stack_free(arguments->out_names);
  free(arguments->out_dir);

  free(arguments);
}