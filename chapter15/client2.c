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
    int sockfd int len;
    struct sockaddr_in address;
    int result;
    int i, A[5], B[5], C[5];
    //char ch = 'A';
    int MSIZE = 5;
    /*  Create a socket for the client.  */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /*  Name the socket, as agreed with the server.  */

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = 9734;
    len = sizeof(address);

    /*  Now connect our socket to the server's socket.  */

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if (result == -1)
    {
        perror("oops: client2");
        exit(1);
    }

    /*  We can now read/write via sockfd.  */

    for (i = 0; i < MSIZE; i++)
    {
        A[i] = 1;
        B[i] = 2;
    }
    write(sockfd, &MSIZE, sizeof(int));

    write(sockfd, A, MSIZE * sizeof(int));
    write(sockfd, B, MSIZE * sizeof(int));

    //C = (int *)malloc(MSIZE * (sizeof(int)));
    read(sockfd, C, MSIZE * sizeof(int));
    for (i = 0; i < MSIZE; i++)
    {
        printf("C[%d] from server = %d\n", i, C[i]);
    }
    close(sockfd);
    exit(0);
}
