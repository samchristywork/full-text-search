#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TOKEN_SIZE 256

int number_of_results;

struct tree_t {
  int c;
  struct tree_t *parent;
  int instances[256];
  struct tree_t *children[256];
} tree;

struct tree_t **allocation_array;
size_t allocation_array_size = 0;
int allocation_array_index = 0;

int recursive_print(struct tree_t *t, int len) {
  if (t->parent) {
    len = recursive_print(t->parent, len);
  }
  putchar(t->c);
  return len + 1;
}

void report(struct tree_t *t, FILE *f) {
  for (int i = 0;; i++) {
    if (t->instances[i] == 0) {
      break;
    }
    number_of_results++;
    int len = recursive_print(t, -1);
    printf("\t%d\t%d\t", t->instances[i], len);
    fseek(f, t->instances[i] - len, SEEK_SET);
    char buf[80];
    int ret = fread(buf, 1, 80, f);
    for (int i = 0; i < ret; i++) {
      if (buf[i] == '\n') {
        buf[i] = '$';
      }
    }
    buf[ret] = 0;
    printf("%s", buf);
    printf("\n");
  }
  for (int i = 0; i < 256; i++) {
    if (t->children[i] != NULL) {
      report(t->children[i], f);
    }
  }
}

void find(struct tree_t *t, char *str, FILE *f) {
  for (int i = 0; i < strlen(str); i++) {
    if (!t->children[(int)str[i]]) {
      return;
    }
    t = t->children[(int)str[i]];
  }
  report(t, f);
}

int main(int argc, char *argv[]) {
  allocation_array_size = 1;
  allocation_array = malloc(sizeof(struct tree_t *));

  char token_buffer[MAX_TOKEN_SIZE + 1];
  bzero(token_buffer, MAX_TOKEN_SIZE + 1);

  FILE *f = fopen(argv[1], "rb");
  fseek(f, 0L, SEEK_END);
  size_t file_size = ftell(f);
  rewind(f);
  if (!f) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  int token_buffer_index = 0;
  size_t file_index = -1;

  while (1) {
    int c = fgetc(f);
    file_index++;
    if (file_index % 1000000 == 0) {
      printf("%f\n", (float)file_index * 100 / file_size);
    }
    if (c == -1) {
      break;
    }
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
      if (c >= 'A' && c <= 'Z') {
        c -= 'A';
        c += 'a';
      }
      if (token_buffer_index > MAX_TOKEN_SIZE) {
        fprintf(stderr, "ERROR: Max token size exceeded.\n");
        exit(EXIT_FAILURE);
      }
      token_buffer[token_buffer_index] = c;
      token_buffer[token_buffer_index + 1] = 0;
      token_buffer_index++;
    } else {
      if (token_buffer_index > 0) {

        struct tree_t *cur_pos = &tree;
        for (int i = 0; i < token_buffer_index; i++) {
          int c = token_buffer[i];
          if (cur_pos->children[c] == NULL) {
            cur_pos->children[c] = malloc(sizeof(struct tree_t));
            memset(cur_pos->children[c], 0, sizeof(struct tree_t));
            cur_pos->children[c]->c = c;
            cur_pos->children[c]->parent = cur_pos;

            if (allocation_array_index + 1 > allocation_array_size) {
              allocation_array_size *= 2;
              allocation_array =
                  realloc(allocation_array,
                          allocation_array_size * sizeof(struct tree_t *));
            }

            allocation_array[allocation_array_index] = cur_pos->children[c];
            allocation_array_index++;
          }
          cur_pos = cur_pos->children[c];
        }
        for (int i = 0; i < 256; i++) {
          if (cur_pos->instances[i] == 0) {
            cur_pos->instances[i] = file_index;
            break;
          }
        }
      }
      token_buffer_index = 0;
    }
  }

  char input_buffer[256];
  while (1) {
    bzero(input_buffer, 256);
    int ret = scanf("%s", input_buffer);
    if (ret == -1) {
      break;
    }
    number_of_results = 0;

    struct timespec start_ts;
    struct timespec end_ts;

    clock_gettime(CLOCK_REALTIME, &start_ts);
    find(&tree, input_buffer, f);
    clock_gettime(CLOCK_REALTIME, &end_ts);
    unsigned long start_ns = (unsigned long)start_ts.tv_sec * 1000000000 +
                             (unsigned long)start_ts.tv_nsec;
    unsigned long end_ns = (unsigned long)end_ts.tv_sec * 1000000000 +
                           (unsigned long)end_ts.tv_nsec;
    double elapsed = end_ns - start_ns;
    int count = 0;
    while (elapsed > 1000) {
      elapsed /= 1000.;
      count++;
      if (count == 3) {
        break;
      }
    }
    char *time_labels[4] = {"nanoseconds", "microseconds", "milliseconds",
                            "seconds"};

    printf("Found %d results in %f %s.\n", number_of_results, elapsed,
           time_labels[count]);
  }

  for (int i = 0; i < allocation_array_index; i++) {
    free(allocation_array[i]);
  }
  free(allocation_array);
  fclose(f);
}
