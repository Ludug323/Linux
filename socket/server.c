#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>

int main()
{
    int client_sockfd, server_sockfd;
    int client_len, server_len;
    struct sockaddr_un server_address, client_address;

    //Remove any old socket and create an un name socket for the server.
    unlink("server_socket");
    server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, "server_socket");
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

    listen(server_sockfd, 5);
    while (1)
    {
        char ch;
        printf("server waiting\n");

        //Accept a connection.
        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);

        //We can now read/write to client on client sockfd.
        read(client_sockfd, &ch, 1);
        printf("client data : %c\n", ch);
        ch++;
        write(client_sockfd, &ch, 1);
        close(client_sockfd);
    }
}