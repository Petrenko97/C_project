#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#include "p3150020-p3150103-mysh.h"
#define MAX_COMMAND_CHARS   256
#define MAX_ARGUMENT_CHARS  512
#define MAX_ARGUMENTS       20

int main(int argc, char const *argv[])
{
  pid_t pid, ret;
  int status;
  char *command = NULL;
  size_t len = 0;
  int stdin_copy = dup(0);
  int stdout_copy = dup(1);
// remove ./ from executable name
  char str2[sizeof(argv[0])];
  strcpy(str2, argv[0]+2);

  //για αρχη, μεμονωμένες εντολές, σε επανάληψη
  while(1) {

    printf("%s> ", str2);
    command = NULL;
    len = 0;
    fflush(stdin);
    if (getline(&command, &len, stdin) == -1) {
      printf("Exiting...\n");
      exit(0);
    }
    int nPipes;
    char** commands = splitPipes(command, &nPipes);


    if (nPipes < 2) {
      spawn_process(0,1,commands[0]);
    }
    else if (nPipes < 3) {
      int in, fd[2];

      in = 0;

      pipe(fd);
      spawn_process(in, fd[1], commands[0]);
      close(fd[1]);
      in = fd[0];

      if (in != 0) {
        dup2(in, 0);
      }

      spawn_process(in, 1, commands[1]);

      // revert stdin and stdout to normal
      dup2(stdin_copy, 0);
      dup2(stdout_copy, 1);
      close(stdin_copy);
      close(stdout_copy);
    }
    else {
        printf("Too many pipes.\n");
        printf("We support up to two commands to be piped.\n");
    }


  }
}
