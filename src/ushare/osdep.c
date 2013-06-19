/*
 * osdep.c : GeeXboX uShare OS independant helpers.
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

#include <stdafx.h>

#if (defined(__unix__) || defined(unix)) && !defined(USG)
#include <sys/param.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "osdep.h"

#ifdef _MSC_VER
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Iphlpapi.h>
#include <tchar.h>
#endif

#if (defined(BSD) || defined(__FreeBSD__) || defined(__APPLE__) || defined(_MSC_VER))
char *
strndup (const char *s, size_t n)
{
  size_t len;
  char *sdup = NULL;

  if (!s)
    return NULL;

  len = strlen (s);
  len = n < len ? n : len;
  sdup = (char *) malloc (len + 1);

  if (sdup)
  {
    memcpy (sdup, s, len);
    sdup[len] = '\0';
  }

  return sdup;
}

void putline(char* dir,FILE *stream)
{
	fputs(dir,stream);
}

ssize_t
getline (char **lineptr, size_t *n, FILE *stream)
{
  static char line[256];
  char *ptr;
  ssize_t len;

  if (!lineptr || !n)
    return -1;

  if (ferror (stream))
    return -1;

  if (feof (stream))
    return -1;

  fgets (line, 256, stream);
  ptr = strchr (line, '\n');

  if (ptr)
    *ptr = '\0';

  len = strlen (line);
  if ((len + 1) < 256)
  {
    ptr = realloc (*lineptr, 256);
    if (!ptr)
      return -1;

    *lineptr = ptr;
    *n = 256;
  }
  strcpy (*lineptr, line);

  return len;
}

#ifdef _MSC_VER


int strcasecmp( char *s1 ,char *s2)
{
	while (*s1 != '\0' && tolower(*s1) == tolower(*s2))
    {
		s1++;
		s2++;
    }
	
	return tolower(*(unsigned char *) s1) - tolower(*(unsigned char *) s2);
}

char* optarg;

int getopt_long (int argc, char *const *argv, const char *shortopts,
				 const struct option *longopts, int *longind)
{
	return 0;
}


osip_list_t dir_file_list;
int dir_file_count = 0;

int scandir(const char *dirname, struct dirent ***namelist,
      int (*filter)(const struct dirent *),
      int (*compar)(const struct dirent **, const struct dirent **))
{
  struct dirent *result = 0;
   struct dirent **namlist;
  int namelistlen = 0;
  int num_enrties= 0;

  DIR *dir = opendir(dirname);

  if (NULL == dir)
  {
    return -1;
  }

  // count number of enrties
    while(readdir (dir) != NULL)
    {
      num_enrties++;
    }


  rewinddir(dir);

  namlist = (struct dirent **) malloc(num_enrties * sizeof(struct dirent *));

  if (0 == namlist) // empty dir
  {
    closedir(dir);
    return namelistlen;
  }

  if(NULL != dir)
  {
    while(readdir (dir) != NULL)
    {
		result = (struct dirent *) malloc(sizeof(struct dirent) + strlen(dir->ent.d_name) + 1);
      strcpy(result->d_name, dir->ent.d_name);
      if (filter)
      {
        if (filter(result))
        {
          namlist[namelistlen] = result;
          namelistlen++;
        }
      }
      else
      {
        namlist[namelistlen] = result;
        namelistlen++;
      }
    }
  }

  //qdirsort(namlist, namelistlen, compar); //todo

  *namelist = namlist;

  closedir(dir);

  return namelistlen;
}

/*
int scandir_tolist1(char* container_dir, osip_list_t* list)
{
	WIN32_FIND_DATAA FindFileData;
	WIN32_FIND_DATAW FindFileDataW;
	char firstfilename[512];
	char dir_name[512];
	int count = 0;
	wchar_t wcch1[] = {L"."};
	wchar_t wcch2[] = {L".."};

	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError;
	
	strcpy(dir_name, container_dir);

	strcpy(firstfilename, dir_name);
	strcat(firstfilename, "\\*");	
	hFind = FindFirstFileA(firstfilename, &FindFileData);
	

	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return (0);
	} 
	else 
	{		
		if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			//CString csFileName = m_csDirectoryName + "\\" + FindFileData.cFileName;
			dirent* ent = (dirent*)malloc(sizeof(dirent));
			osip_list_add(list, ent, -1);

			ent->
			
			ent->mode = 1;
			ent->fullpath = (char*)malloc(strlen(dir_name) + 2);
			strcpy(ent->fullpath, dir_name);
			ent->filename = (char*)malloc(strlen(FindFileData.cFileName) + 2);
			strcpy(ent->filename, FindFileData.cFileName);
			count++;

		}
		
		else if(strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0
			&& (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		{
			dirent* ent = (dirent*)malloc(sizeof(dirent));

			osip_list_add(list, ent, -1);

			ent->mode = 0;
			ent->fullpath = (char*)malloc(strlen(dir_name) + 2);
			strcpy(ent->fullpath, dir_name);

			ent->filename = (char*)malloc(strlen(FindFileData.cFileName) + 2);
			strcpy(ent->filename, FindFileData.cFileName);

			count++;

		}
		else if(strcmp(FindFileData.cFileName, ".") == 0 
			&& (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{

		}
		while (FindNextFileW(hFind, &FindFileDataW) != 0) 
		{
			if(!(FindFileDataW.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				//CString csFileName = m_csDirectoryName + "\\" + FindFileData.cFileName;
				//m_csaFilesInDirectory.Add(csFileName);
				char szANSIString[512];
				char szpath[512];
				wchar_t fullpathname[1024];
				long lLen = 0;
				int x = 0;

				dirent* ent = (dirent*)malloc(sizeof(dirent));
				osip_list_add(list, ent, -1);

				ent->mode = 1;
				ent->fullpath = (char*)malloc(strlen(dir_name) + 2);
				strcpy(ent->fullpath, dir_name);

				strcpy(szpath, dir_name);
				strcat(szpath, "\\");
				
				lLen = MultiByteToWideChar(CP_ACP,0,szpath,strlen(szpath),fullpathname,sizeof(fullpathname));   
				fullpathname[lLen]   = '\0';
				wcscat(fullpathname,FindFileDataW.cFileName);
				x = wcslen(fullpathname);
				ent->filepathname = (wchar_t*)malloc(512 * sizeof(wchar_t));
				wcscpy(ent->filepathname, fullpathname);

				WideCharToMultiByte(CP_ACP,0,FindFileDataW.cFileName,-1,szANSIString,sizeof(szANSIString),NULL,NULL);
				ent->filename = (char*)malloc(strlen(szANSIString) + 2);
				strcpy(ent->filename, szANSIString);

				ent->hsize = FindFileDataW.nFileSizeHigh;
				ent->lsize = FindFileDataW.nFileSizeLow;
 				count++;
			}
			else if(wcscmp(FindFileDataW.cFileName, wcch1) != 0 && wcscmp(FindFileDataW.cFileName, wcch2) != 0
     				&& (FindFileDataW.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ))
			{
				char szANSIString[512];
				char szpath[512];
				wchar_t fullpathname[1024];
				long lLen = 0;
				

				dirent* ent = (dirent*)malloc(sizeof(dirent));

				osip_list_add(list, ent, -1);

				ent->mode = 0;
				ent->fullpath = (char*)malloc(strlen(dir_name) + 2);
				strcpy(ent->fullpath, dir_name);
				
				strcpy(szpath, dir_name);
				strcat(szpath, "\\");

				lLen = MultiByteToWideChar(CP_ACP,0,szpath,strlen(szpath),fullpathname,sizeof(fullpathname));   
				fullpathname[lLen]   = '\0';
				wcscat(fullpathname,FindFileDataW.cFileName);
				
				ent->filepathname = (wchar_t*)malloc(512 * sizeof(wchar_t));
				wcscpy(ent->filepathname, fullpathname);
		
				WideCharToMultiByte(CP_ACP,0,FindFileDataW.cFileName,-1,szANSIString,sizeof(szANSIString),NULL,NULL);
				
				ent->filename = (char*)malloc(strlen(szANSIString) + 2);
				strcpy(ent->filename, szANSIString);

				count++;
			}

			else if(wcscmp(FindFileDataW.cFileName,  wcch1) == 0 
				&& (FindFileDataW.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{

			}
		}
		
		dwError = GetLastError();
		FindClose(hFind);
		if (dwError != ERROR_NO_MORE_FILES) 
		{
			OutputDebugStringW(L"Error\n ");
			return 0;
		}
	}
	return count;	
}
int scandir_tolist2(char* container_dir)

{
	WIN32_FIND_DATAA FindFileData;
	char firstfilename[512];
	char dir_name[512];

	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError;
	
	int count = 0;

	strcpy(dir_name, container_dir);

	strcpy(firstfilename, dir_name);
	strcat(firstfilename, "\\*");	
	hFind = FindFirstFileA(firstfilename, &FindFileData);
	

	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return (0);
	} 
	else 
	{		
		if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			//CString csFileName = m_csDirectoryName + "\\" + FindFileData.cFileName;
			dirent* ent = (dirent*)malloc(sizeof(dirent));
			osip_list_add(&dir_file_list, ent, -1);
			
			ent->mode = 1;
			ent->fullpath = (char*)malloc(strlen(dir_name) + 2);
			strcpy(ent->fullpath, dir_name);

			ent->filename = (char*)malloc(strlen(FindFileData.cFileName) + 2);
			strcpy(ent->filename, FindFileData.cFileName);

			count++;

		}
		
		else if(strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0
			&& (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		{
			char temp[512] = {0};
			strcpy(temp, dir_name);
			strcat(dir_name, "\\");
			strcat(dir_name, FindFileData.cFileName);
			scandir_tolist2(dir_name);
			strcpy(dir_name, temp);

		}
		else if(strcmp(FindFileData.cFileName, ".") == 0 
			&& (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			dirent* ent = (dirent*)malloc(sizeof(dirent));
			osip_list_add(&dir_file_list, ent, -1);

			ent->mode = 0;
			ent->fullpath = (char*)malloc(strlen(dir_name) + 2);
			strcpy(ent->fullpath, dir_name);
			ent->filename = (char*)malloc(strlen(dir_name) + 2);
			strcpy(ent->filename, dir_name);

			count++;
		}
		while (FindNextFileA(hFind, &FindFileData) != 0) 
		{
			if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				//CString csFileName = m_csDirectoryName + "\\" + FindFileData.cFileName;
				//m_csaFilesInDirectory.Add(csFileName);

				dirent* ent = (dirent*)malloc(sizeof(dirent));
				osip_list_add(&dir_file_list, ent, -1);

				ent->mode = 1;
				ent->fullpath = (char*)malloc(strlen(dir_name) + 2);
				strcpy(ent->fullpath, dir_name);
				ent->filename = (char*)malloc(strlen(FindFileData.cFileName) + 2);
				strcpy(ent->filename, FindFileData.cFileName);

				count++;
			}
			else if(strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0
				&& (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ))
			{

				char temp[512] = {0};
				strcpy(temp, dir_name);
				strcat(dir_name, "\\");
				strcat(dir_name, FindFileData.cFileName);
				scandir_tolist2(dir_name);
				strcpy(dir_name, temp);
			}

			else if(strcmp(FindFileData.cFileName, ".") == 0 
				&& (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				dirent* ent = (dirent*)malloc(sizeof(dirent));
				osip_list_add(&dir_file_list, ent, -1);

				ent->mode = 0;
				ent->fullpath = (char*)malloc(strlen(dir_name) + 2);
				strcpy(ent->fullpath, dir_name);
				ent->filename = (char*)malloc(strlen(dir_name) + 2);
				strcpy(ent->filename, dir_name);

				count++;
			}
		}
		
		dwError = GetLastError();
		FindClose(hFind);
		if (dwError != ERROR_NO_MORE_FILES) 
		{
			OutputDebugString(L"Error\n ");
			return 0;
		}
	}
	return count;
}
*/

void cleardir_inlist(osip_list_t* list)
{
	while (!osip_list_eol (list, 0))
	{
		dirent* ent = (dirent*)osip_list_get(list, 0);
		osip_list_remove (list, 0);
		if(ent->d_ino == 0)
		{
			OutputDebugStringW(L"Directory, name is : ");
		}
		else
		{
			OutputDebugStringW(L"File, name is : ");
		}
		OutputDebugStringA(ent->d_name);
		OutputDebugStringW(L"\n");
		free(ent->d_name);
		free(ent);
     }
}

int guess_default_ip(int family, char *address, int size)
{
	SOCKET sock;
	SOCKADDR_STORAGE local_addr;
	int local_addr_len;
	struct addrinfo *addrf = NULL;
	WORD	wVersionRequested;
	WSADATA wsaData;
	
	wVersionRequested = MAKEWORD( 1, 0 );
				
	if (WSAStartup(wVersionRequested,&wsaData)!=0) 
	{
		return FALSE;
	}
	address[0] = '\0';
	sock = socket(family, SOCK_DGRAM, 0);
	
	if(family == AF_INET)
    {
		getaddrinfo("217.12.3.11",NULL,NULL,&addrf);
    }
	else if(family == AF_INET6)
    {
		getaddrinfo("2001:238:202::1",NULL,NULL,&addrf);
    }
	
	if (addrf==NULL)
	{
		closesocket(sock);
		freeaddrinfo(addrf);
		_snprintf(address, size, (family == AF_INET) ? "127.0.0.1" : "::1" );
		return -1;
	}
	
	if(WSAIoctl(sock,SIO_ROUTING_INTERFACE_QUERY, addrf->ai_addr, addrf->ai_addrlen,
		&local_addr, sizeof(local_addr), &local_addr_len, NULL, NULL) != 0)
    {
		closesocket(sock);
		freeaddrinfo(addrf);
		_snprintf(address, size, (family == AF_INET) ? "127.0.0.1" : "::1" );
		return -1;
    }
	
	closesocket(sock);
	freeaddrinfo(addrf);
	
	if(getnameinfo((const struct sockaddr*)&local_addr,
		local_addr_len,address, size, NULL, 0, NI_NUMERICHOST))
    {
		_snprintf(address, size, (family == AF_INET) ? "127.0.0.1" : "::1" );
		return -1;
    }
	
	return 0;
}


int get_macaddr_by_ipaddr(const char* ipaddr, char* macaddr)
{
	IP_ADAPTER_INFO AdapterInfo[16];			// Allocate information for up to 16 NICs
	DWORD dwBufLen = sizeof(AdapterInfo);		// Save the memory size of buffer
	
	DWORD dwStatus = GetAdaptersInfo(			// Call GetAdapterInfo
		AdapterInfo,							// [out] buffer to receive data
		&dwBufLen);								// [in] size of receive data buffer
	
	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;// Contains pointer to current adapter info
	do {
		unsigned char *MACData = pAdapterInfo->Address;
		printf("MAC Address: %02X-%02X-%02X-%02X-%02X-%02X\n", 
			MACData[0], MACData[1], MACData[2], MACData[3], MACData[4], MACData[5]);
		printf("IP Address: %s\n", pAdapterInfo->IpAddressList.IpAddress.String);
		if(strcmp(pAdapterInfo->IpAddressList.IpAddress.String, ipaddr) == 0)
		{	
			sprintf(macaddr, "%02x%02x%02x%02x%02x%02x" ,
				MACData[0], MACData[1], MACData[2], MACData[3], MACData[4], MACData[5]);
			return 0;
		}
		pAdapterInfo = pAdapterInfo->Next;		// Progress through linked list
	}
	while(pAdapterInfo);
	return -1;
}

#endif
#endif /* (defined(BSD) || defined(__FreeBSD__) || defined(__APPLE__)) */
