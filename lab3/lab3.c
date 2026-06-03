#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HISTORY_SIZE 5 // Store only the last 5 inputs

void add_history(char *history[], int *count, const char *input) {
  // Make a separate copy because getline() reuses the same buffer
  char *copy = malloc(strlen(input) + 1);
  if (copy == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }
  strcpy(copy, input);
  if (*count < HISTORY_SIZE) {
    history[*count] = copy;
    (*count)++;
  } else {
    // Remove the oldest input before shifting
    free(history[0]);
    // Shift all inputs one position to the left
    for (int i = 1; i < HISTORY_SIZE; i++) {
      history[i - 1] = history[i];
    }
    history[HISTORY_SIZE - 1] = copy;
  }
}

void print_history(char *history[], int count) {
  for (int i = 0; i < count; i++) {
    printf("%s\n", history[i]);
  }
}

void free_history(char *history[], int count) {
  for (int i = 0; i < count; i++) {
    free(history[i]);
  }
}

int main(void) {
  char *history[HISTORY_SIZE] = {NULL};
  int count = 0;
  // getline() will allocate and resize this buffer
  char *line = NULL;
  size_t len = 0;
  while (1) {
    printf("Enter input: ");
    ssize_t num_chars = getline(&line, &len, stdin);
    if (num_chars == -1) {
      break;
    }
    // Remove the newline character from getline()
    if (num_chars > 0 && line[num_chars - 1] == '\n') {
      line[num_chars - 1] = '\0';
    }
    add_history(history, &count, line);
    // "print" is also saved in history before printing
    if (strcmp(line, "print") == 0) {
      print_history(history, count);
    }
  }
  free(line);
  free_history(history, count);
  return 0;
}
