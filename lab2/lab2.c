#define _POSIX_C_SOURCE 200809L // getline()
#include <stdio.h>              //printf, getline
#include <stdlib.h>             //exit(), free()
#include <string.h>
#include <sys/types.h> //pid_t
#include <sys/wait.h>  //waitpid()
#include <unistd.h>    // fork(), execl()

int main(void) {
  char *line = NULL; // pointer for user input
  size_t len = 0;    // initialize buffer
  while (1) {        // run forever
    printf("Enter programs to run.\n");
    printf("> ");
    // read user input
    ssize_t nread = getline(&line, &len, stdin);
    // getline failed
    if (nread == -1) {
      break;
    }
    // remove trailing newline character
    if (nread > 0 && line[nread - 1] == '\n') {
      line[nread - 1] = '\0';
    }
    // break when user types exit
    if (strcmp(line, "exit") == 0) {
      break;
    }
    // break when user input only enter
    if (line[0] == '\0') {
      break;
    }
    // create child process
    pid_t pid = fork();
    // fork failed
    if (pid == -1) {
      perror("fork failed");
      continue;
    }
    if (pid == 0) {
      execl(line, line, (char *)NULL);
      printf("Exec failure\n");
      exit(EXIT_FAILURE);
    }
    int status;
    waitpid(pid, &status, 0);
  }
  free(line);
  return 0;
}
