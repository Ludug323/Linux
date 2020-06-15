#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
#include<sys/types.h>

int main(){
	int server_sock, client_sock;
	struct sockaddr_un server_addr;
	struct sockaddr_un client_addr;
	int server_len,client_len;
	
	// Create server
	unlink("server_socket");
	server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
	server_addr.sun_family  = AF_UNIX;
	strcpy(server_addr.sun_path,"server_socket");
	server_len = sizeof(server_addr);

	//bind server and give value
	bind(server_sock, (struct sockaddr *)&server_addr, server_len);
	//server_sock max qunee = 5
	listen(server_sock, 5);

	//when server get the client,do transfer accept (
	while(1){
		int num;
		printf("waiting connection.\n");

		client_len = sizeof(client_addr);
		client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
		
		//read client data
		read(client_sock, &num, 1);
		printf("num :%d\n",num);
		num++;
		//transfer data to client
		write(client_sock, &num, 1);

		close(client_sock);
	}
	return 0;
}
