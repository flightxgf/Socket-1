/*
	Content:Linux Network Programming Using Socket(UDP) in server 
	Author:scottshi
    Date:2016/08/31
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <stdarg.h>
#include <string.h>

#define SERVER_PORT 8000
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512

int main()
{
	printf("...Server Start...\n");
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(SERVER_PORT);

	int server_socket = socket(AF_INET,SOCK_DGRAM,0);
	if(server_socket < 0)
	{
		printf("Create socket failed!\n");
		exit(1);
	}

	if(bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))
	{
		printf("Server bind port:%d failed!\n",SERVER_PORT);
		exit(1);
	}

	while(1)
	{
		struct sockaddr_in client_addr;
		socklen_t length = sizeof(client_addr);
		
		char buffer[BUFFER_SIZE];
		bzero(buffer,BUFFER_SIZE);

		if(recvfrom(server_socket,buffer,BUFFER_SIZE,0,(struct sockaddr*)&client_addr,&length) == -1)
		{
			printf("Receive data failed!\n");
			exit(1);
		}
		printf("buffer length:%d\n", strlen(buffer));
		char file_name[FILE_NAME_MAX_SIZE+1];
		bzero(file_name,FILE_NAME_MAX_SIZE+1);
		strncpy(file_name,buffer,strlen(buffer)>FILE_NAME_MAX_SIZE? FILE_NAME_MAX_SIZE:strlen(buffer));
		printf("%s\n", file_name);
	}
	close(server_socket);
	printf("...Server End...\n");
	return 0;
}