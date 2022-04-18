#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
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

// remove ./ from executable name
  char str2[sizeof(argv[0])];
  strcpy(str2, argv[0]+2);

  while(1) {

    printf("%s>", str2);
    if (getline(&command, &len, stdin) == -1) {
      printf("Exiting...\n");
      exit(0);
    }


    pid = fork();
    if (pid < 0) {
      perror("Error in fork");
      exit(1);
    }

    if (pid == 0){
      // child process
      int nArgs;
      char** args = getArguments(command, &nArgs);
      // execute command with extra args

      execvp(args[0], args);
      perror("Error in exec");
      exit(1);
    }
    else {
      // parent process
      // printf("Created child with pid: %d\n", pid);
      // must wait for child
      ret = waitpid(pid, &status, 0);
      if (ret == -1){
        perror("Error in wait");
        exit(1);
      }
      printf("\n");
    }

  }
}
