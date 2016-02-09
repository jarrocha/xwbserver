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

#define LISTENQ 5
#define DATLEN 8192
#define FILEDAT 256

struct web_fl {
	int file_size;
	int stat_ct;
	int dyn_ct;
	char file_type[FILEDAT];
	char file_name[FILEDAT];
};

struct wb_req {
	int trx_fd;
	char method[FILEDAT];
	char uri[FILEDAT];
	char ver[FILEDAT];
};


ssize_t recv_msg(int, char *, size_t);
size_t send_msg(int, char *);
void error_http(int, struct web_fl *, char *, char *);
void error_msg(const char *);
int get_ct_type(struct web_fl *, char *);
void get_file_stats(struct web_fl *);
void serve_rq(int, struct web_fl *);
int matches(const char *, const char *);
void usage(char *);

#endif
