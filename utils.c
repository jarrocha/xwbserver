/*
 * Implementation of common functions
 */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

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

void usage(char *argv)
{
	printf("Usage: %s [PORT]\n", argv);
	exit(EXIT_SUCCESS);
}
