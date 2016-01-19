/*
 * cweb_server: c web server implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include "utils.h"

const int LISTENQ = 5;

int main(int argc, char **argv)
{
	int listenfd, connfd, port;
	int optval = 1;
	struct sockaddr_in svaddr, claddr;
	
	/* zeroing structs */
	memset(&svaddr, 0, sizeof(svaddr));
	memset(&claddr, 0, sizeof(svaddr));

	/* handling webserver arguments */
	if (argc != 2)
		usage(argv[0]);
	port = atoi(argv[1]);

	/* initializing server address socket */
	svaddr.sin_family = AF_INET;
	svaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	svaddr.sin_port = htons((unsigned short)port);
	
	/* defining listening socket descriptor */
	if ((listenfd = socket(svaddr.sin_family, SOCK_STREAM, 0)) < 0 )
		error_msg("error on socket()");
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
			(const void *)&optval , sizeof(int)) < 0)
		error_msg("error on setsockopt()");
	
	/* bind socket to IP address */
	if (bind(listenfd,(struct sockaddr *) &svaddr, sizeof(svaddr)) < 0)
			error_msg("error on bind()");

	/* listen on socket */
	if ((listen(listenfd, LISTENQ)) < -1)
		error_msg("error on listen()");
	
	/* main procedure */
	while(1) {
		if ((connfd = accept(listenfd, (struct sockaddr *)&claddr,
					sizeof(claddr))) < 0)
			error_msg("error on accept()");

	}

	return(EXIT_SUCCESS);

}
