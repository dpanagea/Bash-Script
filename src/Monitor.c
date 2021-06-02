#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../headers/pipes.h"
#define MAX_LEN 256

int main(int argc, char* argv[])
{
    char* message;

    pid_t pid = getpid();
    char* ReadPipe;
    int fdRead;

    ReadPipe = (char*)malloc(sizeof(char)*(MAX_LEN + 1));
    sprintf(ReadPipe, "TMpipe%d", pid);
    createPipe(ReadPipe);
    fdRead = opReadPipe(ReadPipe);
    
    readFromPipe(fdRead, message, 256);

    return 0;
}
