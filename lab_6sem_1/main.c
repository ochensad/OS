#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#define BUF_SIZE 512


int main(void)
{
    pid_t ch_pid[3];

    int sockets[2];
    char buf[BUF_SIZE];
    char buf_m[BUF_SIZE];
    snprintf(buf_m, BUF_SIZE, "%d", getpid());

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0) 
    {
        perror("socketpair() failed");
        exit(1);
    }

    for (int i = 0; i < 3; i++)
    {
        if ((ch_pid[i] = fork()) == -1)
        {
            perror("Can't fork.\n");
            exit(1);
        }
        else if(ch_pid[i] == 0)
        {
            close(sockets[1]);
            snprintf(buf_m, BUF_SIZE, "%d", getpid());
            write(sockets[0], buf_m, sizeof(buf_m));
            printf("\n|Child %d process write message: %s\n",i ,buf_m);
            sleep(1);
            read(sockets[0], buf, sizeof(buf));
            printf("\n|Child %d process read message: %s\n",i, buf);
            close(sockets[0]);
            return 0;
        }
        else
		{
            //close(sockets[0]);
            read(sockets[1], buf, sizeof(buf));
            snprintf(buf_m, BUF_SIZE, "%s %d", buf,getpid());
            printf("\n|Parent process read message: %s\n", buf);
            write(sockets[1], buf_m, sizeof(buf_m));
            printf("\n|Parent process write message: %s\n", buf_m);
            //close(sockets[1]);
        }
    }
    close(sockets[0]);
    close(sockets[1]);
    return 0;
}