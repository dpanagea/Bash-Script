#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include "../headers/pipes.h"
#define MAX_LEN 256

int main(int argc, char* argv[])
{
    int numMonitors, bufferSize, sizeOfBloom, i;
    char* input_dir;
    if(argc != 9)
    {
        printf("Wrong number of arguments\n");
        return -1;
    }
    for(i=1; i < argc; i+=2)
    {
        if(strcmp(argv[i], "-m") == 0)
            numMonitors = atoi(argv[i+1]);
        else if(strcmp(argv[i], "-b") == 0)
            bufferSize = atoi(argv[i+1]);
        else if(strcmp(argv[i], "-s") == 0)
            sizeOfBloom = atoi(argv[i+1]);
        else if(strcmp(argv[i], "-i") == 0)
        {
            input_dir = (char*)malloc(strlen(argv[i+1]) + 1);
            strcpy(input_dir, argv[i+1]);
        }
        else
        {
            printf("Given unacceptable flag. Try again. \n");
            return -1;
        }
    }

    DIR* fd;
    DIR* sfd;
    struct dirent* input;
    char* path = (char*) malloc (MAX_LEN * sizeof(char));
    char* spath = (char*) malloc (MAX_LEN * sizeof(char));

    int numfolders = 0;
    fd = opendir(input_dir);
    if (fd == NULL)
    {
        printf("Can't open given dir \n");
        return 1;
    }
    
    while ((input = readdir(fd)))
    {
        if( (strcmp(input->d_name, ".") != 0) && (strcmp(input->d_name, "..") != 0) )
            numfolders++;
    }
    rewinddir(fd);
    pid_t pid;
    if(numfolders < numMonitors)
        numMonitors = numfolders;
    int monitors[numMonitors];
    char* TMpipes[numMonitors];
    char* Mpipes[numMonitors];
    int fdTM[numMonitors];
    int fdM[numMonitors];
    char* message;

    for( i=0; i < numMonitors; i++)
    {
        char* args[2];
        pid = fork();
        switch (pid) 
        {
            case -1: 
                perror("Error in fork");
                return -1;
            case 0:
                args[0] = "./Monitor";
                args[1] = NULL;
                execvp(args[0], &args[0]);
            default:
                monitors[i] = pid;
                TMpipes[i] = (char*)malloc(sizeof(char)*MAX_LEN);
        //        Mpipes[i] = (char*)malloc(sizeof(char)*MAX_LEN);
                sprintf(TMpipes[i], "TMpipe%d", pid); // named pipe that TM writes, and M reads
        //        sprintf(Mpipes[i], "Mpipes%d", pid); // named pipe that M writes and TM receives

                createPipe(TMpipes[i]);
                fdTM[i] = opWritePipe(TMpipes[i]); //the fd of the pipe TM writes

                message = calloc(sizeof(char), (bufferSize + 1));
                sprintf(message,"%d", bufferSize);
                writeToPipe(fdTM[i], message, bufferSize+1);
                

        }
    }

    int stat_val;

    while(1)
    {
        pid = wait(&stat_val);
        if (pid < 0)
            break;

        for(i=0; i<numMonitors; i++)
        {
            if(pid == monitors[i])
            {
                printf("Monitor #%d with pid %d is killed. \n",i, pid);
            }
        }
        if(!WIFEXITED(stat_val))
            printf("Monitor %d -> terminated abnormally \n", pid);
    }


    free(message);
    for(i=0; i<numMonitors; i++)
    {
        free(TMpipes[i]);
    //    free(Mpipes[i]);
    }
    return 0;
}