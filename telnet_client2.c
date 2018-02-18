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
#include <unistd.h>
#include <netdb.h>

int main(int argc, char **argv)
{
	int  sockfd, n, bytes_written, bytes_read, i;
	int port = atoi(argv[2]);
	char recvline[100], sent_data[100], received_data[1000], ip[100];
	struct sockaddr_in servaddr;
	char *hostname, *ip_address;
	struct hostent *he;
	struct in_addr **addr_list;

	if ( argc != 3 )					//If client isn't invoked using the correct sequence of commands
	{
		printf("Usage : ./telnetcli <server hostname> <port number>");
		exit(1);
	}

	hostname = argv[1];
	if ( (he = gethostbyname(hostname)) == NULL )				//If no hostname is obtained
	{
		// get the host info
		herror("gethostbyname");
		return 1;
	}

	addr_list = (struct in_addr **) he->h_addr_list;

	for ( i = 0; addr_list[i] != NULL; i++ )
	{
		strcpy(ip, inet_ntoa(*addr_list[i]));
	}

	/* Create a TCP socket */
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("socket"); exit(2);
	}

	//printf("\n%s", argv[2]);
	/* Specify server’s IP address and port */
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port); /* daytime server port */

	if ( inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0 )
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

