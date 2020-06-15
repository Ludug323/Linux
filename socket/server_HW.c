#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
/* 1106104145 顏繹庭 四電三甲 */

void sorted(int msize, int arr[]);
int main()
{
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_in server_address, client_address;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = 9734;

    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

    listen(server_sockfd, 5);
    while (1)
    {
        int MSIZE;
        int *a, *b, *c;
        int i, j, select;
        int a_max = 0;
        int b_max = 0;

        printf("server waiting...\n");

        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);

        read(client_sockfd, &MSIZE, sizeof(int));
        printf("Get client MSIZE data: %d\n", MSIZE);
        read(client_sockfd, &select, sizeof(int));

        a = (int *)malloc(MSIZE * sizeof(int));
        b = (int *)malloc(MSIZE * sizeof(int));
        c = (int *)malloc(MSIZE * sizeof(int));

        read(client_sockfd, a, MSIZE * sizeof(int));
        read(client_sockfd, b, MSIZE * sizeof(int));
        if (fork() == 0)
        {
            switch (select)
            {
            case 1:
                for (i = 0; i < MSIZE; i++)
                {
                    c[i] = a[i] + b[i];
                }
                write(client_sockfd, c, MSIZE * sizeof(int));
                break;
            case 2:
                for (i = 0; i < MSIZE; i++)
                {
                    if (a_max < a[i])
                        a_max = a[i];
                    if (b_max < b[i])
                        b_max = b[i];
                }
                write(client_sockfd, &a_max, sizeof(int));
                write(client_sockfd, &b_max, sizeof(int));
                break;
            case 3:
                sorted(MSIZE, a);
                sorted(MSIZE, b);
                write(client_sockfd, a, MSIZE * sizeof(int));
                write(client_sockfd, b, MSIZE * sizeof(int));
                break;
            default:
                printf("Select error!\n");
                break;
            }
            free(a);
            free(b);
            free(c);
        }
        else
            close(client_sockfd);
    }
}
void sorted(int msize, int arr[])
{
    int i, j;
    for (i = 0; i < msize; i++)
    {
        for (j = 0; j < i; j++)
        {
            if (arr[j] > arr[i])
            {
                int temp = 0;
                temp = arr[j];
                arr[j] = arr[i];
                arr[i] = temp;
            }
        }
    }
}