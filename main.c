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

#include "utils.h"
#include "http.h"
#include "thread.h"

/* pthread functions */
void *thr_func(void *);

/* initializing mutex lock */
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

/* static globals */
static int listenfd;
static char fname[FILEDAT];	/* holds webserver directory */

int main(int argc, char **argv)
{
	/* server socket variables */
	int x;
	int port;
	int nthreads;
	int optval = 1;
	struct sockaddr_in svaddr;
	
	/* handling webserver arguments */
	if (argc != 4)
		usage(argv[0]);
	port = atoi(argv[1]);		/* !!No bounds checking */
	strcpy(fname, argv[2]);		/* !!No bounds checking */

	/* thread structure initialization */
	nthreads = atoi(argv[3]);
	struct thread *thr_ctl = malloc(sizeof(struct thread) * nthreads);
	if (!thr_ctl)
		error_msg("error allocating thr_ctl");

	/* initializing server address socket */
	svaddr.sin_family = AF_INET;
	svaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	svaddr.sin_port = htons((unsigned short) port);
	
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
	

	/* pre-threading procedure */
	for (x = 0; x < nthreads; x++) {
		if((pthread_create(&thr_ctl[x].tid, NULL, thr_func,(void *)&x))
			!= 0)
			error_msg("Thread create error");
		if ((pthread_detach(thr_ctl[x].tid)) != 0)
			error_msg("Thread detach error");
	}

	for(;;)
		pause();

	return(EXIT_SUCCESS);
}


void *thr_func(void *arg)
{
	/* variables to handle http process */
	char buff[DATLEN];
	struct st_trx *wb_trx;
	struct sockaddr_in claddr;
	socklen_t sin_size = sizeof(claddr);

	/* main procedure */
	for(;;) {
		wb_trx = malloc((sizeof(struct st_trx)));
		if (!wb_trx)
			error_msg("error allocating wb_trx");	
		
		/* initialize web file struct */
		strcpy(wb_trx->file_name, fname);
		wb_trx->stat_ct = 0;
		wb_trx->dyn_ct = 0;
		
		pthread_mutex_lock(&mtx);	
		if ((wb_trx->trx_fd = accept(listenfd,
					    (struct sockaddr *) &claddr,
					    &sin_size)) < 0)
			error_msg("error on accept()");
		pthread_mutex_unlock(&mtx);	

		printf("server: got connection from %s port %d\n",
				inet_ntoa(claddr.sin_addr),
				ntohs(claddr.sin_port));
		
		/* Read request line and headers */
		recv_msg(wb_trx->trx_fd, buff, DATLEN);

		sscanf(buff, "%s %s %s", wb_trx->method, wb_trx->uri,
		       wb_trx->ver);
		printf("method: %s\nuri: %s\nver: %s\n\n", wb_trx->method, 
		       wb_trx->uri, wb_trx->ver);
		
		serve_rq(wb_trx);
	}

	return(EXIT_SUCCESS);

}
