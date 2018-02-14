#include <stdio.h>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <time.h>

int main(int argc, char **argv)
{
	int   listenfd, connfd, clilen, n;
	struct sockaddr_in servaddr, cliaddr;
	char buff[100], data_received[100], sent_data[100];
	time_t ticks;

	/* Create a TCP socket */
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	/* Initialize server's address and well-known port */
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(10000);   /* daytime server */

										/* Bind server�s address and port to the socket */
	bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	/* Convert socket to a listening socket � max 100 pending clients*/
	listen(listenfd, 100);

	for ( ; ; )
	{
		/* Wait for client connections and accept them */
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

		do
		{
			//Receive from client section
			n = read(connfd, data_received, 100);
			data_received[n] = '\0';
			printf("%s\n", data_received);
			/*if ( (strncmp(data_received, "ls", 2)) == 0 )
			{
				printf("\nYOU SUCK");
			}*/

			//Reply to client section

		}
		while ( n != 0 );
		
		/* Close the connection */
		close(connfd);
	}
}
