/*
 * Implementation of common functions
 */

#ifndef _COMMON_H_
#define _COMMON_H_

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

void error_msg(const char *);
int get_ct_type(struct web_fl *, char *);
void get_file_stats(struct web_fl *);
void serve_static(int, struct web_fl *);
int matches(const char *, const char *);
void usage(char *);

#endif
