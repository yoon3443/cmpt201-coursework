// lab1.c May 19th
// Jungyoon(Yoon) Kim, jka282@sfu.ca
#define _POSIX_C_SOURCE 200809L // According to man page getline is available after POSIX >= 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
  char *buff = NULL; // No memory here
  size_t len = 0;    // buffer size
  while (1) {        // ask user for new string until user presses enter
    printf("Please enter some text: ");
    ssize_t num_char = getline(&buff, &len, stdin); // memory saved to heap
    if (num_char == -1) {                           // error handling
      perror("getline failed");
      free(buff);
      exit(EXIT_FAILURE); // quit
    }
    if (num_char == 1) { // When detected string is only \n (enter)
      break;             // break loop when user presses enter
    }
    printf("Tokens:\n");
    char *saveptr = NULL;                        // initialize
    char *tok = strtok_r(buff, " \n", &saveptr); // save first token
    while (tok != NULL) {
      printf(" %s\n", tok);
      tok = strtok_r(NULL, " \n", &saveptr); // Continue tokenizing
    }
  }
  free(buff);
  return 0;
}
