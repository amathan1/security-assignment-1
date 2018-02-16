#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
	int   listenfd, connfd, clilen, n, bytes_written, bytes_read, ret_val;
	int port = atoi(argv[1]);
	struct sockaddr_in servaddr, cliaddr;
	char buff[100], data_received[100], sent_data[1000];
	char *last_token, *token;
	time_t ticks;
	FILE *in = NULL;

	if ( argc != 2 )
	{
		printf("Usage : telnetserv <port number > 2000>");
		exit(1);
	}

	/* Create a TCP socket */
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	/* Initialize server's address and well-known port */
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);   /* daytime server */

										/* Bind server’s address and port to the socket */
	bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	/* Convert socket to a listening socket – max 100 pending clients*/
	listen(listenfd, 100);

	for ( ; ; )
	{
		/* Wait for client connections and accept them */
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

		do
		{
			//Receive from client section
			bytes_read = read(connfd, data_received, 1000);
			if ( bytes_read == -1 )
				perror("\nError reading data");
			data_received[bytes_read] = '\0';
			
			if ( (strncmp(data_received, "ls", 2)) == 0 )				//If ls command is received from client
			{
				in = popen("ls | xargs", "r");
				fgets(sent_data, 1000, in);
			}
			else if ( (strncmp(data_received, "pwd", 3)) == 0 )			//If pwd command is received from client
			{
				in = popen(data_received, "r");
				fgets(sent_data, 1000, in);
			}
			else if ( (strncmp(data_received, "cd", 2)) == 0 )			//If cd command is received from client
			{
				last_token = NULL;
				token = strtok(data_received, " \t\n");
				while ( token )
				{
					last_token = token;
					token = strtok(NULL, " \t\n");
				}
				ret_val = chdir(last_token);
				if ( ret_val == -1 )
					strcpy(sent_data, "Error changing directory");
				else
					strcpy(sent_data, "");
			}
			else if ( (strncmp(data_received, "rmdir", 5)) == 0 )			//If rmdir command is received from client
			{
				last_token = NULL;
				token = strtok(data_received, " \t\n");
				while ( token )
				{
					last_token = token;
					token = strtok(NULL, " \t\n");
				}
				ret_val = rmdir(last_token);
				if ( ret_val == -1 )
					strcpy(sent_data, "Error removing directory");
				else
					strcpy(sent_data, "");
			}
			else if ( (strncmp(data_received, "mkdir", 5)) == 0 )			//If mkdir command is received from client
			{
				last_token = NULL;
				token = strtok(data_received, " \t\n");
				while ( token )
				{
					last_token = token;
					token = strtok(NULL, " \t\n");
				}
				ret_val = mkdir(last_token, ACCESSPERMS);
				if ( ret_val == -1 )
					strcpy(sent_data, "Error making directory");
				else
					strcpy(sent_data, "");
			}
			else
			{
				strcpy(sent_data, "Command not supported ");
			}

			//Reply to client section
			bytes_written = write(connfd, sent_data, 1000);
			if ( bytes_written == -1 )
				perror("\nError sending data");

		}
		while ( bytes_read != 0 );
		
		/* Close the connection */
		close(connfd);
	}
}

