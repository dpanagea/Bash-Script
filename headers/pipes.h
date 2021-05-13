#ifndef PIPES_H
#define PIPES_H
#include <sys/stat.h>
#include <stdio.h>

void createPipe(char*);

int opReadPipe(char*);
void readFromPipe(int, char*, size_t); //int fd, char* msg, size_t buffersize

int opWritePipe(char*);
void writeToPipe(int, char*, size_t);


#endif
