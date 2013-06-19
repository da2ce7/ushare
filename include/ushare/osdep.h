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

#ifdef _WIN32
	typedef int bool;

	#ifndef true
	#define true 1
	#endif
	#ifndef false
	#define false 0
	#endif
#else
	#include <stdbool.h>
#endif


#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#ifdef _WIN32
#include <getopt_win.h>
#else
#include <getopt.h>
#endif

#ifdef _WIN32
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef int socklen_t;
#endif

#ifdef _WIN32
#ifndef S_ISREG
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#endif

#ifndef S_ISDIR
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif
#endif


#ifdef _MSC_VER

#include "stdio.h"
#include "osip_list.h"

#define vsnprintf _vsnprintf
#define snprintf _snprintf

typedef size_t ssize_t;

typedef unsigned int uint32_t;

typedef int socklen_t;

#define strtok_r strtok_s;

int strcasecmp( char *s1 ,char *s2);

#ifndef PATH_MAX
#define  PATH_MAX 512
#endif

#define SYSCONFDIR "C:\\"

#define no_argument 0
#define required_argument 1

#endif


#if (defined(BSD) || defined(__FreeBSD__))



char *strndup (const char *s, size_t n);
ssize_t getline (char **lineptr, size_t *n, FILE *stream);
#endif

#if defined (_MSC_VER)

char *strndup (const char *s, size_t n);

ssize_t getline (char **lineptr, size_t *n, FILE *stream);
void putline(char* dir,FILE *stream);

//typedef struct {
//	unsigned char mode; //0 is directory, 1 is file
//	char* fullpath;
//	char* filename;
//	unsigned long hsize; 
//	unsigned long lsize;
//	wchar_t *filepathname;
//}dirent;

int scandir(const char *dir, struct dirent ***namelist,
      int (*filter)(const struct dirent *),
      int (*compar)(const struct dirent **, const struct dirent **));

extern osip_list_t dir_file_list;

//int scandir_tolist1(char* container_dir, osip_list_t* list); // not recursive 
//int scandir_tolist2(char* container_dir); // recursive 
void cleardir_inlist(osip_list_t* list);

int guess_default_ip (int family, char *address, int size);
int get_macaddr_by_ipaddr(const char* ipaddr, char* macaddr);

#endif


#ifdef _WIN32

#endif

#endif /* _OS_DEP_H_ */
