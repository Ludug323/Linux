#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
/* 1106104145 顏繹庭 四電三甲 */

int main()
{

    int sockfd, len;
    struct sockaddr_in address;
    int results, select;
    int i, MSIZE = 4;
    int a[4] = {3, 1, 6, 5};
    int b[4] = {9, 2, 7, 1};
    int c[4];
    int a_max, b_max;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = 9734;
    len = sizeof(address);

    results = connect(sockfd, (struct sockaddr *)&address, len);
    if (results == -1)
    {
        perror("error client\n");
        exit(1);
    }

    write(sockfd, &MSIZE, sizeof(int));
    printf("(1)陣列加總 (2)顯示陣列中的最大值 (3)將陣列排序\n");
    scanf("%d", &select);
    write(sockfd, &select, sizeof(int));
    write(sockfd, a, MSIZE * sizeof(int));
    write(sockfd, b, MSIZE * sizeof(int));

    switch (select)
    {
    case 1:
        read(sockfd, c, MSIZE * sizeof(int));
        for (i = 0; i < MSIZE; i++)
        {
            printf("c[%d] = %d\n", i, c[i]);
        }
        break;
    case 2:
        read(sockfd, &a_max, sizeof(int));
        read(sockfd, &b_max, sizeof(int));
        printf("a_max = %d, b_max = %d\n", a_max, b_max);
        break;
    case 3:
        read(sockfd, a, MSIZE * sizeof(int));
        read(sockfd, b, MSIZE * sizeof(int));
        for (i = 0; i < MSIZE; i++)
        {
            printf("a[%d] = %d, b[%d] = %d\n", i, a[i], i, b[i]);
        }
        break;
    default:
        perror("error read!\n");
        break;
    }
    close(sockfd);
}