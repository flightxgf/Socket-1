/*
	Content:Linux Network Programming Using Socket in server 
	Author:scottshi
    Date:2016/08/31
*/

#include <netinet/in.h> //for sockaddr_in
#include <sys/types.h>  //for socket
#include <sys/socket.h> //for socket
#include <stdio.h>      //for printf
#include <stdlib.h>     //for exit
#include <string.h>     //for bzero
#include <unistd.h>     //for close

#define HELLO_WORLD_SERVER_PORT 6666
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512

int main(int argc,char** argv)
{
	printf("...Server Start...\n");
	//Initialize server socket,including address protocal,host and port
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);

	//Use internet stream protocal(TCP) to create server socket
	int server_socket = socket(PF_INET,SOCK_STREAM,0);
	if(server_socket < 0)
	{
		printf("Create Server Socket Failed!\n");
		exit(1);
	}else{
		int opt = 1;
		setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	}

	//Bind socket with its socket_addr
	if(bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))
	{
		printf("Server Bind Port:%d Failed!\n", HELLO_WORLD_SERVER_PORT);
		exit(1);
	}

	//Listen request
	if(listen(server_socket,LENGTH_OF_LISTEN_QUEUE))
	{
		printf("Server Listen Failed!\n");
		exit(1);
	}

	while(1)
	{
		struct sockaddr_in client_addr;
		socklen_t length = sizeof(client_addr);

		int new_server_socket = accept(server_socket,(struct sockaddr*)&client_addr,&length);
		if(new_server_socket < 0)
		{
			printf("Server Accept Failed!\n");
			break;
		}
		char buffer[BUFFER_SIZE];
		bzero(buffer,BUFFER_SIZE);
		length = recv(new_server_socket,buffer,BUFFER_SIZE,0);
		if(length < 0)
		{
			printf("Server Receive Data Failed!\n");
			break;
		}
		char file_name[FILE_NAME_MAX_SIZE+1];
		bzero(file_name,FILE_NAME_MAX_SIZE+1);
		strncpy(file_name,buffer,strlen(buffer)>FILE_NAME_MAX_SIZE? FILE_NAME_MAX_SIZE:strlen(buffer));
		printf("%s\n",file_name);

		FILE* fp = fopen(file_name,"r");
		if(NULL == fp)
		{
			printf("File:\t%s Not Found\n", file_name);
		}else{
			bzero(buffer,BUFFER_SIZE);
			int file_block_length = 0;
			while((file_block_length = fread(buffer,sizeof(char),BUFFER_SIZE,fp)) > 0)
			{
				printf("file_block_length=%d\n", file_block_length);
				if(send(new_server_socket,buffer,file_block_length,0) < 0)
				{
					printf("Send File:\t%s Failed!\n", file_name);
					break;
				}
				bzero(buffer,BUFFER_SIZE);
			}
			fclose(fp);
			printf("File:\t%s Transfer Finished!\n", file_name);
		}
		close(new_server_socket);
	}
	close(server_socket);
	
	printf("...Server End...");
	return 0;
}