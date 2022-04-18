#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_CHARS 256

int main(int argc, char const *argv[])
{
  pid_t pid, ret;
  int status;
  char command[MAX_COMMAND_CHARS];

  // remove ./ from executable name
  char str2[sizeof(argv[0])];
  strcpy(str2, argv[0]+2);

  while(1) {

    printf("%s> ", str2);
    if (scanf("%s", command) == EOF ) {
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
      execlp(command, command, NULL);
      perror("Error in exec");
      exit(1);
    }
    else {
      // parent process
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
