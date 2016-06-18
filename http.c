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

#include "http.h"

static void http_200(int, struct st_trx *);
static void http_404(int, struct st_trx *);
static void http_500(int, struct st_trx *);
static void http_501(int, struct st_trx *);
//static int test_method(struct wb_req *);

static struct http_codes {                                                      
        char *code;                                                             
        void (*func)(int, struct st_trx *);                                     
} hcodes[] = {
	{ "200", http_200},
	{ "404", http_404},
	{ "500", http_500},
	{ "501", http_501},
	{ 0 }
};


void call_http(char *call, int fd, struct st_trx *wb_trx)
{
	struct http_codes *hc;

	for (hc = hcodes; hc->code; hc++) {
		if (matches(call, hc->code) == 0)
			hc->func(fd, wb_trx);
	}
}

static void http_200(int fd, struct st_trx *wb_trx)
{
	char buff[DATLEN];

	sprintf(buff, "HTTP/1.0 200 OK\r\n");
	sprintf(buff, "%sServer: xWebserver\r\n", buff);
	sprintf(buff, "%sContent-length:%d\r\n", buff, wb_trx->file_size);
	sprintf(buff, "%sContent-type:%s\r\n\r\n", buff, wb_trx->file_type);
    
	send_msg(fd, buff);
}

static void http_500(int fd, struct st_trx *wb_trx)
{
	char buff[DATLEN];

	sprintf(buff, "HTTP/1.0 500 INTERNAL SERVER ERROR\r\n");
	sprintf(buff, "%sServer: xWebserver\r\n", buff);
    
	send_msg(fd, buff);
}

static void http_501(int fd, struct st_trx *wb_trx)
{
	char buff[DATLEN];

	sprintf(buff, "HTTP/1.0 501 NOT IMPLEMENTED\r\n");
	sprintf(buff, "%sServer: xWebserver\r\n", buff);
    
	send_msg(fd, buff);
}

static void http_404(int fd, struct st_trx *wb_trx)
{
	char buff[DATLEN];

	sprintf(buff, "HTTP/1.0 404 NOT FOUND\r\n");
	sprintf(buff, "%sServer: xWebserver\r\n\r\n", buff);
	sprintf(buff, "%s<html><head><title>404 Not Found</title></head>"
		,buff);
	sprintf(buff, "%s<body><h1>URL not found</h1></body></html>\r\n", buff);
    
	send_msg(fd, buff);
}
/*
static int test_method(struct wb_req *request)
{
	char *method[] = { "GET" };

	for (int i = 0; i < sizeof(method) / sizeof(method[0]); i++) {
		printf("Test against: %s\n", method[i]);
		printf("Request: %s\n", request->method);
		if (matches(request->method, method[i]) == 0)
			return 0;
	}
	return -1;
}*/
