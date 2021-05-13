#include "../headers/pipes.h"
#include <errno.h> 
#include <unistd.h>
#include <stdlib.h> 
#include <fcntl.h>

void createPipe(char* str)
{
    if(mkfifo(str, 0666) == -1 && errno != EEXIST)
    {
        perror("creation pipe");
        exit(6);
    }
}

int opReadPipe(char* str)
{
    int fd = -1;
    if( (fd=open(str, O_RDONLY)) < 0 )
    {
        perror("Problem at open pipe for reading");
        exit(3);
    }
    return fd;
}

void readFromPipe(int fd, char* msg, size_t buf)
{
    if( read(fd, msg, buf) < 0 )
    {
        perror("Problem at reading from pipe");
        exit(5);
    }
}

int opWritePipe(char* str)
{
    int fd = -1;
    if( (fd=open(str, O_WRONLY)) < 0 )
    {
        perror("Problem at open pipe for writing");
        exit(1);
    }
    return fd;
}

void writeToPipe(int fd, char* msg, size_t buf)
{
    if( write(fd, msg, buf) < 0)
    {
        perror("Problem at writing to pipe");
        exit(2);
    }
}

