/*
 *  utils.c: implements common functions fo xwbserver.
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
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "utils.h"

/* prints error messages */
void error_msg(const char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(EXIT_FAILURE);
}

/* receive msg function */
int recv_msg(int fd, char *buff)
{
	//char eol = EOL;
	ssize_t nrecv = 0;

	while (nrecv <= 0) {
		nrecv = recv(fd, buff, DATLEN, 0);

		if (nrecv < 0) {
			if (errno != EINTR)
				error_msg("recv() error");
		} else if (nrecv == 0)
			return 1;
		else
			nrecv = 0;
	}

	return 1;
}

/* send msg function */
void send_msg(int fd, char *buff)
{
	size_t nleft = strlen(buff);
	ssize_t nsend = 0;

	/* send content */
	while (nleft > 0) {
		if ((nsend = send(fd, buff, nleft, 0)) <= 0 ) {
			if ( nsend < 0 && errno == EINTR)
				nsend = 0;
			else
				error_msg("error with send()");
		}
		nleft -= nsend;
		buff += nsend;
	}
}

/* print http error messages */
void error_http(int fd, struct web_fl *webfile, char *err_cd, char *msg)
{
	
}

/* compares two strings */
int matches(const char* cmd, const char* pattern)
{
	int len = strlen(cmd);

	if(len > strlen(pattern))
		return -1;
	return memcmp(pattern, cmd, len);
}

/* get content type: dynamic or static */
int get_ct_type(struct web_fl *webfile, char *uri)
{
	if(!strstr(uri, "cgi-bin")) {
		if (uri[strlen(uri) - 1] == '/') {
			webfile->stat_ct = 1;			
			webfile->dyn_ct = 0;
			return(EXIT_SUCCESS);			
		}
	} else {
		webfile->stat_ct = 0;			
		webfile->dyn_ct = 1;
		return(EXIT_SUCCESS);			
	}

	return(EXIT_FAILURE);
}

/* get file stats */
void get_file_stats(struct web_fl *webfile)
{
	struct stat filestat;

	if (stat(webfile->file_name, &filestat) < 0) {
		printf("File: %s\n", webfile->file_name);
		error_msg("error on stat()");
	} else
		webfile->file_size = filestat.st_size;

	if (strstr(webfile->file_name, ".html"))
		strcpy(webfile->file_type, "text/html");
	else if (strstr(webfile->file_name, ".gif"))
		strcpy(webfile->file_type, "image/gif");
	else if (strstr(webfile->file_name, ".jpg"))
		strcpy(webfile->file_type, "image/jpeg");
	else
		strcpy(webfile->file_type, "text/plain");
}

/* serve static content */
void serve_static(int connfd, struct web_fl *webfile)
{
	int filefd;
	char *addr;
	char buff[DATLEN];
	/* send() variables */
	size_t nleft;
	ssize_t nsend;

	sprintf(buff, "HTTP/1.0 200 OK\r\n");
	sprintf(buff, "%sServer: xWeb Server\r\n", buff);
	sprintf(buff, "%sContent-length: %d\r\n", buff, webfile->file_size);
	sprintf(buff, "%sContent-type: %s\r\n\r\n", buff, webfile->file_type);
	
	send_msg(connfd, buff);
	
	if ((filefd = open(webfile->file_name, O_RDONLY, 0)) < 0)
		error_msg("error opening web file");
	read(filefd, buff, webfile->file_size);
	send_msg(connfd, buff);
	
	//if ((addr = mmap(0, webfile->file_size, PROT_READ, MAP_PRIVATE, 
	//				filefd, 0)) == ((void *) -1))
	//	error_msg("error on mmap()");
	
	// close file desciptor 
	//if (close(filefd) < 0)
	//	error_msg("error closing filefd");
	
	close(filefd);
}

/* usage function */
void usage(char *argv)
{
	printf("Usage: %s [PORT] [INDEX PAGE]\n", argv);
	exit(EXIT_SUCCESS);
}
