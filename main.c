/*
 *  xwbserver: web server implementation on C.
 *
 *  Copyright (C) 2016 Jaime Arrocha
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include "utils.h"

int main(int argc, char **argv)
{
	/* server socket variables */
	int listenfd, connfd, port;
	int optval = 1;
	struct sockaddr_in svaddr, claddr;
	socklen_t sin_size = sizeof(claddr);

	/* variables to handle http process */
	struct web_fl webfile;
	char buff[DATLEN], method[DATLEN], uri[DATLEN], ver[DATLEN];
	
	/* zeroing structs */
	memset(&svaddr, 0, sizeof(svaddr));
	memset(&claddr, 0, sizeof(claddr));

	/* handling webserver arguments */
	if (argc != 3)
		usage(argv[0]);
	port = atoi(argv[1]);

	/* initialize web file struct */
	strcpy(webfile.file_name, argv[2]);
	webfile.stat_ct = 0;
	webfile.dyn_ct = 0;
	
	/* initializing server address socket */
	svaddr.sin_family = AF_INET;
	svaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	svaddr.sin_port = htons((unsigned short)port);
	
	/* defining listening socket descriptor */
	if ((listenfd = socket(svaddr.sin_family, SOCK_STREAM, 0)) < 0 )
		error_msg("error on socket()");
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, 
				sizeof(optval)) < 0)
		error_msg("error on setsockopt()");
	
	/* bind socket to IP address */
	if (bind(listenfd,(struct sockaddr *) &svaddr, 
				sizeof(struct sockaddr_in)) < 0)
		error_msg("error on bind()");

	/* listen on socket */
	if ((listen(listenfd, LISTENQ)) < 0)
		error_msg("error on listen()");
	
	/* main procedure */
	while(1) {
		if ((connfd = accept(listenfd, (struct sockaddr *) &claddr,
					&sin_size)) < 0)
			error_msg("error on accept()");

		printf("server: got connection from %s port %d\n",
				inet_ntoa(claddr.sin_addr),
				ntohs(claddr.sin_port));
		
		/* Read request line and headers */
		recv_msg(connfd, buff, DATLEN);

		sscanf(buff, "%s %s %s", method, uri, ver);
		printf("method: %s\nuri: %s\nver: %s\n\n", method, uri, ver);
		if (matches("GET", method) != 0) {
			errno = ENOSYS;
			error_msg("501");
		}
		
		/* get content type */
		if(get_ct_type(&webfile, uri) != 0)
			error_msg("error getting content type");

		/* get file stats */
		get_file_stats(&webfile);

		/* serve static content */
		serve_static(connfd, &webfile);
	}



	return(EXIT_SUCCESS);

}
