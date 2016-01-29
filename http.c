/*
 *  http.c: handles http code behavior
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
#include <sys/types.h>
#include "utils.h"
#include "http.h"

void http_404(int, struct web_fl *);
void http_200(int, struct web_fl *);

struct http_codes {
	char *code;
	void (*func)(int, struct web_fl *);
} hcodes[] = {
	{ "200", http_200},
	{ "404", http_404},
	{ 0 }
};

void call_http(char *call, int fd, struct web_fl *webfile)
{
	struct http_codes *hc;

	for (hc = hcodes; hc->code; hc++) {
		if (matches(call, hc->code) == 0)
			hc->func(fd, webfile);
	}
}

void http_200(int fd, struct web_fl *webfile)
{
	char buff[DATLEN];

	sprintf(buff, "HTTP/1.0 200 OK\r\n");
	sprintf(buff, "%sServer: xWebserver\r\n", buff);
	sprintf(buff, "%sContent-length:%d\r\n", buff, webfile->file_size);
	sprintf(buff, "%sContent-type:%s\r\n\r\n", buff, webfile->file_type);
    
	send_msg(fd, buff);
}

void http_404(int fd, struct web_fl *webfile)
{
	char buff[DATLEN];

	sprintf(buff, "HTTP/1.0 404 NOT FOUND\r\n");
	sprintf(buff, "%sServer: xWebserver\r\n", buff);
	sprintf(buff, "%s<html><head><title>404 Not Found</title></head>"
		,buff);
	sprintf(buff, "%s<body><h1>URL not found</h1></body></html>\r\n", buff);
    
	send_msg(fd, buff);
}
