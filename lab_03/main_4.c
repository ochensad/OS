#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void CheckStatus(int stat_loc)
{
    if(WIFEXITED(stat_loc))
    {
        printf("exited with code %d\n", WEXITSTATUS(stat_loc));
    }
    else if (WIFSIGNALED(stat_loc))
    {
        printf("aborted by signal # %d\n", WTERMSIG(stat_loc));
    }
    else if (WIFSTOPPED(stat_loc))
    {
        printf("stopped by signal # %d\n", WSTOPSIG(stat_loc));
    }
}

int main(void)
{
    int stat_loc;
    pid_t ch_pid[2];
    int fd[2];
    char *message[2] = {"hdkdfjbnsdjibhisdbfdvalksmdf", " ksjbdfvijd"};

    if (pipe(fd) == -1)
    {
        perror("Can't pipe.\n");
        exit(1);
    }
    
    for (int i = 0; i < 2; i++)
    {
        if ((ch_pid[i] = fork()) == -1)
        {
            perror("Can't fork.\n");
            exit(1);
        }
        else if(ch_pid[i] == 0)
        {
			printf("\n|Child process|\n");
            printf("PID = %d, PPID = %d, GROUP = %d\n", getpid(), getppid(), getpgrp());
            close(fd[0]);
            write(fd[1], message[i], strlen(message[i]));
            return 0;
        }
        else
		{
			printf("\n|Parent process|\n");
		    printf("PID = %d, GROUPID = %d, Child = %d\n", getpid(), getpgrp(), ch_pid[i]);
        }
    }
    
	for(int i = 0; i < 2; i++)
	{
        close(fd[1]);
        read(fd[0], message[i], strlen(message[i]));
        printf("\nRECEIVE messages: %s\n", message[i]);
        if ((ch_pid[i] = waitpid(ch_pid[i], &stat_loc, 0)) == -1)
        {
            perror("Can't waitpid.");
            exit(1);
        }
        printf("\n|Process PID: %d|", ch_pid[i]);
        CheckStatus(stat_loc);
	};
	return 0;
}