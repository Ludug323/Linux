/*  Make the necessary includes and set up the variables.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    /*  Create an unnamed socket for the server.  */

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /*  Name the socket.  */

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = 9734;
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

    /*  Create a connection queue and wait for clients.  */

    listen(server_sockfd, 5);
    while (1)
    {
        //char ch;
        int MSIZE, i, *A, *B, *C;
        printf("server waiting\n");

        /*  Accept a connection.  */

        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);

        /*  We can now read/write to client on client_sockfd.  */

        read(client_sockfd, &MSIZE, sizeof(int));
        printf("MSIZE = %d\n", MSIZE);

        A = (int *)malloc(MSIZE * sizeof(int));
        B = (int *)malloc(MSIZE * sizeof(int));
        C = (int *)malloc(MSIZE * sizeof(int));

        read(client_sockfd, A, MSIZE * sizeof(int));
        for (i = 0; i < MSIZE; i++)
        {
            printf("A[%d] = %d\n", i, A[i]);
        }

        read(client_sockfd, B, MSIZE * sizeof(int));
        for (i = 0; i < MSIZE; i++)
            printf("B[%d] = %d\n", i, B[i]);

        for (i = 0; i < MSIZE; i++)
        {
            C[i] = A[i] + B[i];
            printf("C[%d] = %d\n", i, C[i]);
        }
        write(client_sockfd, C, MSIZE * sizeof(int));

        close(client_sockfd);
        free(A);
        free(B);
        free(C);
    }
}