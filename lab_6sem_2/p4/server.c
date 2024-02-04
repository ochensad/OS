#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>

#include <arpa/inet.h>
#include <netdb.h>

#include <poll.h>

#define BUF_SIZE 50
#define CLIENT_NUM 5

#define SOCKET_ADDR "localhost"
#define SOCKET_PORT 9877

#define OK 0
#define MAX_CLIENTS_COUNT 10

static int master_sd;
static int clients[MAX_CLIENTS_COUNT];

int cleanup()
{
    close(master_sd);
    exit(EXIT_FAILURE);
}

void sigint_handler(int signum)
{
    cleanup();
    exit(OK);
}

int main(int argc, char** argv) 
{
    if (argc != 2)
    {
        perror("args not enough\n");
        return EXIT_FAILURE;
    }
    short port = (short) atoi(argv[1]);

	if ((signal(SIGINT, sigint_handler) == SIG_ERR)) {
        perror("Can't attach handler\n");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Server is listening.\nTo stop server press Ctrl + C.\n");

    if ((master_sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Failed to create socket");
        return EXIT_FAILURE;
    }

    fcntl(master_sd, F_SETFL, O_NONBLOCK);

    struct sockaddr_in addr = {
            .sin_family = AF_INET,
            .sin_addr.s_addr = INADDR_ANY,
            .sin_port = htons(port)
    };

    if (bind(master_sd, (struct sockaddr *) &addr, sizeof addr) < 0)
    {
        cleanup();
    }

    if (listen(master_sd, MAX_CLIENTS_COUNT) < 0)
    {
        cleanup();
    }

    char buf[BUF_SIZE];
	int k = 0;
	struct pollfd pfd[CLIENT_NUM + 1];
    pfd[0].fd = master_sd;
    pfd[0].events = POLLIN;

    while (1)
	{
		int rc = poll(pfd, k + 1, 10000);
        if (rc == -1)
        {
            perror("poll() failed");
            return EXIT_FAILURE;
        }

		if (pfd[0].revents & POLLIN) 
		{
			struct sockaddr_in sa;
			int asize = sizeof(sa);
			pfd[0].revents = 0;

            pfd[k++].fd = accept(master_sd, (struct sockaddr*) &sa, (socklen_t*) &asize);
			if (pfd[k].fd < 0)
			{
                k--;
			    perror("accept() failed");
			    return EXIT_FAILURE;
			}
			pfd[k].events = POLLIN;

			printf("\nClient connected №%d %s:%d\n", k - 1, inet_ntoa(sa.sin_addr), ntohs(sa.sin_port));
		} 
		else
		{
			for (int i = 1; i <= k ; i++)
			{
				if (pfd[i].revents & POLLIN)
				{
					pfd[i].revents = 0;

					if (read(pfd[i].fd, buf, sizeof(buf)) == 0)
                    {
                        printf("\nClient %d disconnected\n", i - 1);
                        pfd[i].fd = 0;
                        continue;
                    }
					printf("\nServer got message: %s\n", buf);

					sprintf(buf, "%s %d", buf, getpid());
					write(pfd[i].fd, buf, sizeof(buf));
					printf("\nServer sent message: %s\n", buf);
				}
			}
		}
	}
}