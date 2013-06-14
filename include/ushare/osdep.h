/*
 * osdep.h : GeeXboX uShare OS independant helpers headers.
 * Originally developped for the GeeXboX project.
 * Copyright (C) 2005-2007 Benjamin Zores <ben@geexbox.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef _OS_DEP_H_
#define _OS_DEP_H_

#ifdef _MSC_VER

#include "stdio.h"
#include "osip_list.h"

#define vsnprintf _vsnprintf
#define snprintf _snprintf

typedef signed int ssize_t;

typedef unsigned int uint32_t;

typedef int socklen_t;

typedef int bool;
#define true 1
#define false 0

int strcasecmp( char *s1 ,char *s2);

#ifndef PATH_MAX
#define  PATH_MAX 512
#endif

#define SYSCONFDIR "F:\\DLNA\\ushare_z\\ushare_bindebug"

struct option{
	const char* name;
	int has_arg;
	int* flag;
	int val;
};
#define no_argument 0
#define required_argument 1


extern char* optarg;

int getopt_long (int argc, char *const *argv, const char *shortopts,
		        const struct option *longopts, int *longind);


#endif


#if (defined(BSD) || defined(__FreeBSD__))

#include <unistd.h>

char *strndup (const char *s, size_t n);
ssize_t getline (char **lineptr, size_t *n, FILE *stream);
#endif

#if defined (_MSC_VER)

char *strndup (const char *s, size_t n);

ssize_t getline (char **lineptr, size_t *n, FILE *stream);
void putline(char* dir,FILE *stream);

typedef struct {
	unsigned char mode; //0 is directory, 1 is file
	char* fullpath;
	char* filename;
	unsigned long hsize; 
	unsigned long lsize;
	wchar_t *filepathname;
}dirent;

extern osip_list_t dir_file_list;

int scandir_tolist1(char* container_dir, osip_list_t* list); // not recursive 
int scandir_tolist2(char* container_dir); // recursive 
void cleardir_inlist(osip_list_t* list);

int guess_default_ip (int family, char *address, int size);
int get_macaddr_by_ipaddr(const char* ipaddr, char* macaddr);

#endif

#endif /* _OS_DEP_H_ */
