#include "p3150020-p3150103-mysh.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define TOKENSIZE 64

int spawn_process(int inFd, int outFd, char* command){
  pid_t pid,ret;
  int status;
  pid = fork();
  if (pid < 0){
    perror("fork");
    exit(1);
  }
  if(pid == 0) {

    if (inFd != 0)
    {
      dup2(inFd, 0);
      close(inFd);
    }
    if (outFd != 1) {
      dup2(outFd, 1);
      close(outFd);
    }

    int nArgs;
    char** args = getArguments(command, &nArgs);
    // for (int i = 0; i < nArgs; ++i)
    // {
    //   fprintf(stdout, "args[%d]: %s\n", i, args[i]);
    // }
    // execute command with extra args
    char str1[] = ">", str2[] = ">>", str3[] = "<";
    char** newArgs = malloc(nArgs* sizeof(char*));
    int i = 0;
    for (; i < nArgs; ++i)
    {
      //printf("DEBUGGGGG%s\n", args[i]);
      if (strcmp(args[i], str1) == 0){
        // redirect output to i+1, truncate
        // printf("DEBUG >: %s %s\n",args[i], args[i+1] );
        int file = open(args[i+1], O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
        if (file < 0) {
          perror("open file wronly");
          exit(1);
        }
        close(1);
        if (dup(file) < 0) { // 1: redirect stdout
          perror("redirect output");
          exit(1);
        }
        // skip next arg
        i++;
        //args[i] = NULL;
      }
      if (strcmp(args[i], str2) == 0){
        // redirect output to i+1, append
        // printf("DEBUG >>: %s %s\n",args[i], args[i+1] );

        int file = open(args[i+1], O_APPEND | O_WRONLY);
        if (file < 0) {
          perror("open file append");
          exit(1);
        }
        close(1);
        if (dup(file) < 0) { // 1: redirect stdout
          perror("redirect output append");
          exit(1);
        }
        // skip next arg
        i++;
        // args[i] = NULL;
      }
      if (strcmp(args[i], str3) == 0){
        // redirect input from i+1
        int file = open(args[i+1], O_RDONLY);
        if (file < 0) {
          perror("open file rdonly");
          exit(1);
        }
        close(0);
        if (dup(file) < 0) {
          perror("redirect input");
          exit(1);
        }
        // skip next arg
        i++;
      }
      else {
        newArgs[i] = malloc(sizeof(args[i]));
        strcpy(newArgs[i],args[i]);
      }
    }
    newArgs[i] = NULL;
    ret = execvp(newArgs[0], newArgs);
    perror("Error in exec");
    exit(1);
  }
  else {
    ret = waitpid(pid, &status, 0);
      if (ret == -1){
        perror("Error in wait");
        exit(1);
      }
  }
  return ret;
}

char ** getArguments(char* line, int* count) {
  // line: \n terminated.
  int bufsize = TOKENSIZE, pos = 0;
  char** arguments = malloc(bufsize * sizeof(char*));
  char *arg;
  if (!arguments){
    perror("allocation error");
    exit(1);
  }

  arg = strtok(line," \t\r\n\a");
  // arguments[0]: program to execute
  while(arg != NULL)
  {
    arguments[pos] = arg;
    pos++;
    if(pos >= bufsize) {
      bufsize += TOKENSIZE;
      arguments = realloc(arguments, bufsize * sizeof(char*));
      if(!arguments){
        perror("reallocation error");
        exit(1);
      }
    }
    // arguments[i]: next parameter
    arg = strtok(NULL, " \t\r\n\a");
  }

  strtok(arguments[pos-1],"\n");
  arguments[pos] = NULL;
  *count = pos;
  return arguments;
}

char** splitPipes(char* line, int* countPipes) {
  int bufsize = TOKENSIZE, pos = 0;
  char** cmds = malloc(bufsize * sizeof(char*));
  char *cmd;
  if (!cmds){
    perror("allocation error");
    exit(1);
  }

  cmd = strtok(line,"|");
  // arguments[0]: program to execute
  while(cmd != NULL)
  {
    cmds[pos] = cmd;
    pos++;
    if(pos >= bufsize) {
      bufsize += TOKENSIZE;
      cmds = realloc(cmds, bufsize * sizeof(char*));
      if(!cmds){
        perror("reallocation error");
        exit(1);
      }
    }
    // arguments[i]: next parameter
    cmd = strtok(NULL, "|");
  }

  strtok(cmds[pos-1],"\n");
  cmds[pos] = NULL;
  *countPipes = pos;
  return cmds;
}
