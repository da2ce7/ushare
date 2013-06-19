/*
 * trace.c : GeeXboX uShare log facility.
 * Originally developped for the GeeXboX project.
 * Copyright (C) 2005-2007 Alexis Saettler <asbin@asbin.org>
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

#include <stdafx.h>

#include <stdio.h>
#include <stdarg.h>

#ifdef _WIN32
#else
#include <syslog.h>
#endif


#include "ushare_config.h"
#include "trace.h"
#include "ushare.h"

extern struct ushare_t *ut;


// copy form Open-Transactions
#ifdef _WIN32
void print_log (log_level level, const char *fmt, va_list * pvl)
{
	{
		if(NULL == fmt) return;
		if(NULL == pvl) return;
	}
	// ------------------
	{
		int size=0;
		int nsize=0;
		char * buffer = NULL;
		va_list args = NULL;

		va_list args_2 = *pvl;  //windows only.

		args = *pvl;
		size = _vscprintf(fmt,args) + 1;

		// ------------------------------------    
		buffer = (char*)malloc(size+100);
		memset(buffer,'\0',size+100);
		if(NULL == buffer) return;

		nsize = vsnprintf_s(buffer,size,size,fmt,args_2);

		if(0 > nsize) return;

		if (size <= nsize) 
		{
			size  = nsize+1;
			free(buffer); buffer = NULL;
			buffer = (char*)malloc(size+100);
			memset(buffer,'\0',size+100);

			if(NULL == buffer) return;
			memset(buffer,'\0', size+100);

			nsize = vsnprintf_s(buffer,size,size,fmt,*pvl);
			va_end(args);
			va_end(args_2);

			// ------------------------------------
		if(0 > nsize) return;
		}
		if(nsize >= size) return;
		// ------------------------------------
		{
			FILE *output = level == ULOG_ERROR ? stderr : stdout;
			fprintf_s(output,buffer);
		}
		free(buffer); buffer = NULL;
		return;
	}
}


#else
void
print_log (log_level level, const char *format, ...)
{
	if(NULL != format) return;

    // ------------------
	int size=0;
	int nsize=0;
	char * buffer = NULL;
	va_list args;

#ifdef _WIN32
	va_list args_2 = *pvl;  //windows only.

	args = *pvl;
	size = _vscprintf(fmt,args) + 1;
#else
#endif

  bool is_daemon = ut ? ut->daemon : false;
  bool is_verbose = ut ? ut->verbose : false;

  if (!format)
    return;

  if (!is_verbose && level >= ULOG_VERBOSE)
    return;


  va_start (va, format);



  if (is_daemon || !_WIN32)
  {
#ifndef _WIN32
    int flags = LOG_DAEMON;
    flags |= level == ULOG_ERROR ? LOG_ERR : LOG_NOTICE;
    vsyslog (flags, format, va);
#endif
  }
  else
  {
    FILE *output = level == ULOG_ERROR ? stderr : stdout;
	if (NULL != va) vfprintf (output, format, va);
  }
  va_end (va);
}

#endif

void start_log (void)
{
#ifdef _WIN32
#else
  openlog (PACKAGE_NAME, LOG_PID, LOG_DAEMON);
#endif
}