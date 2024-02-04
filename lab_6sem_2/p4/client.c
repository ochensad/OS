#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>

#include <arpa/inet.h>
#include <netdb.h>

#define BUF_SIZE 256
#define SOCKET_ADDR "localhost"
#define SOCKET_PORT 9877

#define OK 0
static int master_sd;

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

    master_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (master_sd == -1) {
        perror("Failed to create socket");
        return EXIT_FAILURE;
    }
    if ((signal(SIGINT, sigint_handler) == SIG_ERR)) {
        perror("Can't attach handler\n");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "To stop client press Ctrl + C.\n");


    struct sockaddr_in addr = {
            .sin_family = AF_INET,
            .sin_addr.s_addr = INADDR_ANY,
            .sin_port = htons(port)
    };
    if (connect(master_sd, (struct sockaddr *) &addr, sizeof addr) < 0) {
        perror("Failed to connect");
        return EXIT_FAILURE;
    }
    char buf[BUF_SIZE];
    snprintf(buf, BUF_SIZE, "Client: my pid is %d", getpid());

    if (send(master_sd, buf, strlen(buf) + 1, 0) < 0)
    {
        perror("Failed to sendto");
        return EXIT_FAILURE;
    }
    printf("Client sent: %s\n", buf);
    while(1)
    {
        unsigned len;
        if (recv(master_sd, buf, BUF_SIZE, 0) < 0)
        {
            perror("Failed to recv");
            return EXIT_FAILURE;
        }
        printf("Client got: %s\n", buf);
    }
}