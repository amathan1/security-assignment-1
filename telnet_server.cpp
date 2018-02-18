#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <netinet/ip.h>

using namespace std;

int main()
{
	int   listenfd, connfd, clilen;
	struct sockaddr_in servaddr, cliaddr;
	char buff[100];
	time_t ticks;
	
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(10000);

	bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	/* Convert socket to a listening socket – max 100 pending clients*/
	listen(listenfd, 100);

	for ( ;; )
	{
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

		//printf("hello1\n");

		/* Retrieve system time */
		ticks = time(NULL);
	}
	return 0;
}