#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SOCKET_NAME "mysocket.s"
#define BUF_SIZE 256
#define OK 0

int main(void)
{
    int sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("Failed to create socket");
        return EXIT_FAILURE;
    }

    struct sockaddr to;
    to.sa_family = AF_UNIX;
    strcpy(to.sa_data, SOCKET_NAME);

    char buf[BUF_SIZE];
    snprintf(buf, BUF_SIZE, "My pid is: %d", getpid());

    if (sendto(sockfd, buf, strlen(buf) + 1, 0, &to, strlen(to.sa_data) + 1 + sizeof(to.sa_family)) == -1)
    {
        printf("Client: Failed to send message");
        close(sockfd);
        return EXIT_FAILURE;
    }
    printf("Client sent: %s\n", buf);

    close(sockfd);
    return OK;
}