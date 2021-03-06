/*
 * http.h : GeeXboX uShare Web Server handler header.
 * Originally developped for the GeeXboX project.
 * Parts of the code are originated from GMediaServer from Oskar Liljeblad.
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

#ifndef _HTTP_H_
#define _HTTP_H_

#include <upnp/upnp.h>
#include <upnp/upnptools.h>

void http_setcallbaks();

#ifdef _WIN32
bool httpGetDataFile_char(IN char const * const strFilename, OUT wchar_t const **const wstrFilePath);
bool httpGetDataFile(IN wchar_t const * const wstrFilename, OUT wchar_t const **const wstrFilePath);
#else
#endif

VDCallback_GetInfo *callbackGetInfo;
VDCallback_Open *callbackOpen;
VDCallback_Read *callbackRead;
VDCallback_Write *callbackWrite;
VDCallback_Seek *callbackSeek;
VDCallback_Close *callbackClose;

#endif /* _HTTP_H_ */
