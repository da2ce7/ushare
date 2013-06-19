// OTLIB
// Open Transactions Pre-Compiled Headers File

#pragma once


#ifndef EXPORT
#define EXPORT
#endif
#include <export_wrapper.h>

#ifdef _WIN32
#include <winsock_wrapper.h>
#endif

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#ifdef _WIN32
#include <dirent_win.h>
#else
#include <dirent.h>
#endif

#include <ushare_config.h>

#include <osdep.h>