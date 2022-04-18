#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

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

  //για αρχη, μεμονωμένες εντολές, σε επανάληψη
  while(1) {

    printf("%s>", str2);
    if (getline(&command, &len, stdin) == -1) {
      printf("Exiting...\n");
      exit(0);
    }

    ret = spawn_process(0,1,command);

  }
}
