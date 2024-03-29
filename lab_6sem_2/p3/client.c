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

    struct sockaddr from;
    from.sa_family = AF_UNIX;
    char bcn[BUF_SIZE];
    snprintf(bcn, BUF_SIZE, "c%d.s", getpid());
    strcpy(from.sa_data, bcn);

    struct sockaddr to;
    to.sa_family = AF_UNIX;
    strcpy(to.sa_data, SOCKET_NAME);

    if (bind(sockfd, &from, strlen(from.sa_data) + 1 + sizeof(from.sa_family)) < 0)
    {
        perror("Failed to bind socket");
        return EXIT_FAILURE;
    }

    char buf[BUF_SIZE];
    snprintf(buf, BUF_SIZE, "%d", getpid());

    if (sendto(sockfd, buf, strlen(buf) + 1, 0, &to, strlen(to.sa_data) + 1 + sizeof(to.sa_family)) == -1)
    {
        printf("Client: Failed to send message");
        close(sockfd);
        unlink(bcn);
        return EXIT_FAILURE;
    }
    printf("Client sent: %s\n", buf);

    unsigned len;
    if (recvfrom(sockfd, buf, sizeof(buf), 0, &from, &len) <= 0)
    {
        perror("Clinet: Failed to recv");
        close(sockfd);
        unlink(bcn);
        return EXIT_FAILURE;
    } 
    printf("Client got: %s\n", buf);

    close(sockfd);
    unlink(bcn);
    return OK;
}