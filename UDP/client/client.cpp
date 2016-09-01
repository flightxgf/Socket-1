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
	printf("...Client Start...\n");
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(SERVER_PORT);

	int client_socket = socket(AF_INET,SOCK_DGRAM,0);
	if(client_socket < 0)
	{
		printf("Create socket failed!\n");
		exit(1);
	}

	char file_name[FILE_NAME_MAX_SIZE+1];
	bzero(file_name,FILE_NAME_MAX_SIZE+1);
	printf("Please input the request file name:\t");
	scanf("%s",file_name);
	
	char buffer[BUFFER_SIZE];
	bzero(buffer,BUFFER_SIZE);
	strncpy(buffer,file_name,strlen(file_name)>FILE_NAME_MAX_SIZE? FILE_NAME_MAX_SIZE:strlen(file_name));

	if(sendto(client_socket,buffer,BUFFER_SIZE,0,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0)
	{
		printf("Send file name failed!\n");
		exit(1);
	}
	close(client_socket);
	
	printf("...Client End...\n");
	return 0;
}