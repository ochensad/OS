#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    pid_t ch_pid[2];

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
            return 0;
        }
        else
		{
			printf("\n|Parent process|\n");
		    printf("PID = %d, GROUPID = %d, Child = %d\n", getpid(), getpgrp(), ch_pid[i]);
        }
    }
    return 0;
}