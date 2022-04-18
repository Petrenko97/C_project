#include <string.h>

char ** getArguments(char* line, int* count);
int spawn_process(int inFd, int outFd, char* command);
char** splitPipes(char* line, int* countPipes);