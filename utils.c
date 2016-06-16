/*
 *  utils.c: implements common functions for xwbserver.
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

#include "utils.h"
#include "http.h"

/* prints error messages */
void error_msg(const char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(EXIT_FAILURE);
}

/* receive msg function */
ssize_t recv_msg(int fd, char *usr_buff, size_t nbytes)
{
	size_t nleft = nbytes;
	ssize_t nrecv;
	char *recv_buff = usr_buff;
	/*
	char EOL[3] = "\r\n";
	size_t eol_len = strlen(EOL);
	int eol_matched;
	*/

	while (nleft > 0) {
		if ((nrecv = recv(fd, recv_buff, nleft, 0)) < 0) {
			if (errno == EINTR)
				nrecv = 0;
			else
				error_msg("recv() error");
		} else if ((nrecv == 0 ) || strstr(recv_buff, "\r\n"))
			break;
		/* search for EOL at the last two bytes 
		for (int i = nrecv - 2; i <= nrecv; i++) {
			if ( *(recv_buff + i) == EOL[eol_matched])
				eol_matched++;
			if (eol_matched == eol_len)
				*(recv_buff + 1 - eol_len) = '\0';
		}
		*/
		nleft -= nrecv;
		recv_buff += nrecv;
	}

	return nbytes - nleft;
	
}

/* send msg function */
size_t send_msg(int fd, char *buff)
{
	size_t nleft = strlen(buff);
	ssize_t nsend = 0;

	/* send content */
	printf("Bytes to send: %d\n", (int) nleft);
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
	if (nleft == 0)
		printf("Send success!!\n");
	return nleft;
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
void serve_rq(struct wb_req *wb_trx, struct web_fl *webfile)
{
	int filefd;
	char *addr;

	
	if ((filefd = open(webfile->file_name, O_RDONLY, 0)) < 0)
		call_http("404", wb_trx->trx_fd, webfile);
	/* get file stats */
	get_file_stats(webfile);
		
	if ((addr = mmap(0, webfile->file_size, PROT_READ, MAP_PRIVATE, 
				filefd, 0)) == ((void *) -1))
		error_msg("error on mmap()");
	
	if (close(filefd) < 0)
		error_msg("error closing filefd");
	
	call_http("200", wb_trx->trx_fd, webfile);
	send_msg(wb_trx->trx_fd, addr);

	if (munmap(addr, webfile->file_size) < 0)
		error_msg("munmap() error");
	/* freeing web transaction */
	free(wb_trx);
}

/* usage function */
void usage(char *argv)
{
	printf("Usage: %s [PORT] [INDEX PAGE]\n", argv);
	exit(EXIT_SUCCESS);
}
