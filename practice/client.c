#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<sys/types.h>
#include<unistd.h>

int main(){
	int sockfd, len_sockfd;
	struct sockaddr_un socket_address;
	int num = 5;
	int con_result;
	//Create a socket for the client.
	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	socket_address.sun_family = AF_UNIX;
	strcpy(socket_address.sun_path, "server_socket");
	len_sockfd = sizeof(socket_address);
	
	//Now connect out socket for the server's socket
	con_result = connect(sockfd, (struct sockaddr *)&socket_address, len_sockfd);
	if(con_result == -1){
		perror("error connection\n");
		exit(EXIT_FAILURE);
	}
	//if connection success,write/read data for the socket 
	write(sockfd, &num, sizeof(int));
	printf("waiting for server data\n");
	sleep(1);
	read(sockfd, &num, sizeof(int));
	printf("new data is %d\n",num);

	close(sockfd);
}
