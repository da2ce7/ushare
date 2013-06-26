/*
 * trace.h : GeeXboX uShare log facility headers.
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

#ifndef _TRACE_H_
#define _TRACE_H_

typedef enum {
  ULOG_NORMAL = 1,
  ULOG_ERROR = 2,
  ULOG_VERBOSE = 3,
} log_level;

#ifdef _MSC_VER
void print_log (log_level level, const char *fmt, va_list * pvl);
#else
void print_log (log_level level, const char *format, ...)
  __attribute__ ((format (printf, 2, 3)));
#endif


void start_log (void);

/* log_info
 * Normal print, to replace printf
 */
#ifdef _WIN32
__inline void log_info(const char *fmt,...) {
	va_list vl;
    va_start(vl, fmt);
	print_log (ULOG_NORMAL,fmt,&vl);
    va_end(vl);
}
#else 
#define log_info(s, str...) {          \
  print_log (ULOG_NORMAL, (s), ##str); \
  }
#endif

/* log_error
 * Error messages, output to stderr
 */
#ifdef _WIN32
__inline void log_error(const char *fmt,...) {
	va_list vl;
    va_start(vl, fmt);
	print_log (ULOG_ERROR,fmt,&vl);
	va_end(vl);
}
#else 
#define log_error(s, str...) {        \
  print_log (ULOG_ERROR, (s), ##str); \
  }
#endif

/* log_verbose
 * Output only in verbose mode
 */
#ifdef _WIN32
__inline void log_verbose(const char *fmt,...) {
	va_list vl;
    va_start(vl, fmt);
	print_log (ULOG_VERBOSE,fmt,&vl);
	va_end(vl);
}
#else 
#define log_verbose(s, str...) {        \
  print_log (ULOG_VERBOSE, (s), ##str); \
  }
#endif

#endif /* _TRACE_H_ */
