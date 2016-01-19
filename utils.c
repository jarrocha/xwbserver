/*
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

/* usage function */
void usage(char *argv)
{
	printf("Usage: %s [PORT]\n", argv);
	exit(EXIT_SUCCESS);
}
