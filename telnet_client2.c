#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <time.h>
#include<unistd.h>

int main(int argc, char **argv)
{
	int  sockfd, n, bytes_written, bytes_read;
	char recvline[100], sent_data[100], received_data[1000];
	struct sockaddr_in servaddr;

	if ( argc != 2 )
	{
		printf("Usage : gettime <IP address>");
		exit(1);
	}

	/* Create a TCP socket */
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("socket"); exit(2);
	}

	/* Specify server’s IP address and port */
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(10000); /* daytime server port */

	if ( inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0 )
	{
		perror("inet_pton"); exit(3);
	}

	/* Connect to the server */
	if ( connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))
		< 0 )
	{
		perror("connect"); exit(4);
	}

	do
	{
		printf("\ntelnet> ");
		fgets(sent_data, 100, stdin);					//Get input from keyboard including whitespace

		//Send to server section
		if ( strncmp(sent_data, "exit", 4) == 0 )			
		{
			exit(0);
		}
		else
		{
			bytes_written = write(sockfd, sent_data, strlen(sent_data));
			if ( bytes_written == -1 )
				perror("\nError sending data");
		}

		//Receive from server section
		bytes_read = read(sockfd, received_data, 1000);
		if ( bytes_read == -1 )
			perror("\nError reading data");

		printf("%s", received_data);
	}
	while ( 1 );

	close(sockfd);
}

