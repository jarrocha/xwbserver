/*
:q
 * Implementation of common functions
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#define LISTENQ 5
#define DATLEN 8192
#define FILEDAT 256
#define EOL "\r\n"

struct web_fl {
	int file_size;
	int stat_ct;
	int dyn_ct;
	char file_type[FILEDAT];
	char file_name[FILEDAT];
	char method[FILEDAT];
	char uri[FILEDAT];
	char ver[FILEDAT];	
};

int recv_msg(int, char *);
void send_msg(int, char *);
void error_http(int, struct web_fl *, char *, char *);
void error_msg(const char *);
int get_ct_type(struct web_fl *, char *);
void get_file_stats(struct web_fl *);
void serve_static(int, struct web_fl *);
int matches(const char *, const char *);
void usage(char *);

#endif
