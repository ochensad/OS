#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

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
    const char *coms[2] = {"./3_1", "./3_2"};

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
            if (execlp(coms[i], coms[i], NULL) == -1)
            {
                perror("Can't exec.\n");
                exit(1);
            }
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
        if ((ch_pid[i] = waitpid(ch_pid[i], &stat_loc, 0)) == -1)
        {
            perror("Can't waitpid.");
            exit(1);
        }
        printf("\n|Process PID: %d|", ch_pid[i]);
        CheckStatus(stat_loc);
	}
	return 0;
}