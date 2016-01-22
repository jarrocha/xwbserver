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
	struct stat filestat;
	/* send() variables */
	size_t nleft;
	ssize_t nsend;

	sprintf(buff, "HTTP/1.0 200 OK\r\n");
	sprintf(buff, "%sServer: Tiny Web Server\r\n", buff);
	sprintf(buff, "%sContent-length: %d\r\n", buff, webfile->file_size);
	sprintf(buff, "%sContent-type: %s\r\n\r\n", buff, webfile->file_type);

	/* send content */
	nleft = strlen(buff);
	nsend = 0;
	while (nleft > 0) {
		if ((nsend = send(connfd, buff, DATLEN, 0)) <= 0 ) {
			if (errno == EINTR)
				nsend = 0;
			else
				error_msg("error with send()");

		}
		nleft -= nsend;
	}
	
	/*	
	if (stat(webfile->file_name, &filestat) < 0) {
		printf("File: %s\n", webfile->file_name);
		error_msg("error on stat()");
	} else
		webfile->file_size = filestat.st_size;

	if ((filefd = open(webfile->file_name, O_RDONLY, 0)) < 0)
		error_msg("error opening web file");


	if ((addr = mmap(0, webfile->file_size, PROT_READ, MAP_PRIVATE, 
					filefd, 0)) == ((void *) -1))
		error_msg("error on mmap()");
	
	// close file desciptor 
	if (close(filefd) < 0)
		error_msg("error closing filefd");
	
	// send content 
	nleft = webfile->file_size;
	while (nleft > 0) {
		if ((nsend = send(connfd, addr, webfile->file_size, 0)) <= 0 ) {
			if (errno == EINTR)
				nsend = 0;
			else
				error_msg("error with send()");

		}
		nleft -= nsend;
	}
*/
}

/* usage function */
void usage(char *argv)
{
	printf("Usage: %s [PORT] [INDEX PAGE]\n", argv);
	exit(EXIT_SUCCESS);
}
