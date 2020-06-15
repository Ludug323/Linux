#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>

int main()
{
    int sockfd, len, result;
    struct sockaddr_un address;
    char ch = 'A';

    //create socket for the client
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    //Name the socket, as agreed with server.
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, "server_socket");
    len = sizeof(address);

    //Now connect our socket to the server's scoket
    result = connect(sockfd, (struct sockaddr *)&address, len);
    if (result == -1)
    {
        perror("error : client");
        exit(1);
    }

    write(sockfd, &ch, 1);
    read(sockfd, &ch, 1);
    printf("char from server: %c\n", ch);
    close(sockfd);
    exit(0);
}