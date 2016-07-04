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

/* prototypes */
static void thr_term(struct st_trx *);
static void get_file_stats(struct st_trx *);

/* exit error messages */
void error_msg(const char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(EXIT_FAILURE);
}

/* recoverable error messages */
void client_msg(const char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
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
int get_ct_type(struct st_trx *wb_trx, char *uri)
{
	if(!strstr(uri, "cgi-bin")) {
		if (uri[strlen(uri) - 1] == '/') {
			wb_trx->stat_ct = 1;			
			wb_trx->dyn_ct = 0;
			return(EXIT_SUCCESS);			
		}
	} else {
		wb_trx->stat_ct = 0;			
		wb_trx->dyn_ct = 1;
		return(EXIT_SUCCESS);			
	}

	return(EXIT_FAILURE);
}

/* get file stats */
static void get_file_stats(struct st_trx *wb_trx)
{
	struct st_trx *ptrx = wb_trx;
	struct stat filestat;
	
	if (matches(ptrx->uri,"/") == 0)
		strcat(ptrx->file_name, "/index.html");
	else
		strcat(ptrx->file_name, ptrx->uri);
		
	printf("Filename: %s\n", ptrx->file_name);
	
	if ((ptrx->file_fd = open(ptrx->file_name, O_RDONLY, 0)) < 0) {
		printf("File not found\n");
		call_http("404", ptrx->trx_fd, ptrx);
		thr_term(ptrx);
	} else {
		if (stat(ptrx->file_name, &filestat) < 0) {
			printf("File: %s\n", ptrx->uri);
			error_msg("error on stat()");
		} else
			ptrx->file_size = filestat.st_size;

		if (strstr(ptrx->file_name, ".html"))
			strcpy(ptrx->file_type, "text/html");
		else if (strstr(ptrx->file_name, ".gif"))
			strcpy(ptrx->file_type, "image/gif");
		else if (strstr(ptrx->file_name, ".jpg"))
			strcpy(ptrx->file_type, "image/jpeg");
		else
			strcpy(ptrx->file_type, "text/plain");
	}
}

/* serve static content */
void serve_rq(struct st_trx *wb_trx)
{
	char *addr;
	
	/* get file stats */
	get_file_stats(wb_trx);
		
	if ((addr = mmap(0, wb_trx->file_size, PROT_READ, MAP_PRIVATE, 
			 wb_trx->file_fd, 0)) == ((void *) -1))
		error_msg("error on mmap()");
	
	if (close(wb_trx->file_fd) < 0)
		error_msg("error closing filefd");
	
	call_http("200", wb_trx->trx_fd, wb_trx);
	send_msg(wb_trx->trx_fd, addr);

	if (munmap(addr, wb_trx->file_size) < 0)
		error_msg("munmap() error");
	/* freeing web transaction */
	free(wb_trx);
}

/* thread terminating function */
static void thr_term(struct st_trx *ptrx)
{
	void *val = NULL;

	free(ptrx);
	pthread_exit(val);		
}

/* usage function */
void usage(char *argv)
{
	printf("Usage: %s [PORT] [DIRECTORY]\n", argv);
	exit(EXIT_SUCCESS);
}
