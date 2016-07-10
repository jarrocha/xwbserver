/*
 *  Definition of common functions
 *
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

#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#define LISTENQ 5
#define DATLEN 8192
#define FILEDAT 256

struct st_trx {
	int trx_fd;
	int file_size;
	int file_fd;
	int stat_ct;
	int dyn_ct;
	char method[FILEDAT];
	char uri[FILEDAT];
	char ver[FILEDAT];
	char file_type[FILEDAT];
	char file_name[FILEDAT];
};


size_t recv_msg(int, char *, size_t);
size_t send_msg(int, char *);
void error_msg(const char *);
int get_ct_type(struct st_trx *, char *);
void serve_rq(struct st_trx *);
int matches(const char *, const char *);
void usage(char *);

#endif
