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

#ifdef __cplusplus
extern "C" {
#endif
#ifdef _MSC_VER
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Iphlpapi.h>
#include <tchar.h>
#include <Userenv.h>
#endif
#ifdef __cplusplus
}
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

	size_t len_dername = (strlen(dirname) +1) * sizeof(char *);
	char * strDirBuf = (char *) malloc(len_dername);


	size_t outlen = trimwhitespace(strDirBuf,len_dername,dirname);

	DIR *dir = opendir(strDirBuf);

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


#ifdef _WIN32

// inspired from http://ntcoder.com/bab/tag/expandenvironmentstringsforuser/
LONG Windows_ExpandEnvironmentStrings    (wchar_t const  *const strEnvironmentStrings, wchar_t const **const out_ExpandedString)
{
	LPWSTR  aEnvStrBuf = (LPWSTR)malloc((wcslen(strEnvironmentStrings)+1) * sizeof(LPWSTR));
	LPCWSTR strEnvStrCopy = wcscpy(aEnvStrBuf,strEnvironmentStrings);

	size_t lBuffSize2 = (MAX_PATH +1) * sizeof(LPWSTR);
	LPWSTR aHomeDirBuf2 = (LPWSTR)malloc(lBuffSize2);;


	if(NULL == strEnvStrCopy) return -1;

	{


		{
			// We need a process with query permission set
			HANDLE hToken = 0;
			if(!OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hToken )) return false;

			// Expand Environment Strings
			if(!ExpandEnvironmentStringsForUserW(hToken,strEnvStrCopy,aHomeDirBuf2,lBuffSize2)) return false;

			// Close handle opened via OpenProcessToken
			CloseHandle( hToken );
		}
		{
			LPWSTR  aExpStrBuf = (LPWSTR)malloc((wcslen(aHomeDirBuf2)+1) * sizeof(LPWSTR));
			LPCWSTR strExpStrCopy = wcscpy(aExpStrBuf,aHomeDirBuf2); // make a copy
			*out_ExpandedString = strExpStrCopy;
		}
		free (aHomeDirBuf2);
	}
	free(aEnvStrBuf); // cleanup copy.

	return true;
}




LONG Windows_GetInstallFolderFromRegistry(wchar_t const **const out_InstallFolderPath)
{
	HKEY hKey=0;
	LONG lRes = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Open-Transactions", 0, KEY_READ, &hKey);
	bool bExistsAndSuccess = (lRes == ERROR_SUCCESS);
	bool bDoesNotExistsSpecifically = (lRes == ERROR_FILE_NOT_FOUND);

	wchar_t const **const strValueOfBinDir = NULL;
	LONG lRes2 = WindowsRegistryTools_GetStringRegKey(hKey, L"Path", L"bad", strValueOfBinDir);

	if (bExistsAndSuccess)
	{
		*out_InstallFolderPath = *strValueOfBinDir;
		return true;
	}
	return false;
}

LONG Windows_GetAppDataFolderFromRegistry(wchar_t const **const out_AppDataFolderPath)
{
	HKEY hKey=0;
	LSTATUS lRes = RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\\Windows\\\CurrentVersion\\Explorer\\User Shell Folders", 0, KEY_READ, &hKey);
	BOOL bExistsAndSuccess = (lRes == ERROR_SUCCESS);
	BOOL bDoesNotExistsSpecifically = (lRes == ERROR_FILE_NOT_FOUND);

	wchar_t const *strValueOfAppDir = NULL;
	WindowsRegistryTools_GetStringRegKey(hKey, L"AppData", L"bad", &strValueOfAppDir);

	if (bExistsAndSuccess)
	{
		*out_AppDataFolderPath = strValueOfAppDir;
		return true;
	}
	return false;
}


LONG WindowsRegistryTools_GetDWORDRegKey (HKEY hKey, wchar_t const *const strValueName, DWORD const nDefaultValue,   DWORD          *const out_nValue)
{
	DWORD dwBufferSize = (sizeof(DWORD));
	DWORD nResult = (0);
	LONG nError = RegQueryValueExW
		(hKey,
		strValueName,
		0,
		NULL,
		(LPBYTE)(&nResult),
		&dwBufferSize);

	*out_nValue = nDefaultValue;
	if (ERROR_SUCCESS == nError)
	{
		*out_nValue = nResult;
	}
	return nError;
}


LONG WindowsRegistryTools_GetBoolRegKey  (HKEY hKey, wchar_t const *const strValueName, bool    const bDefaultValue,   bool           *const out_bValue)
{
	const DWORD nDefValue = ((bDefaultValue) ? 1 : 0);
	DWORD nResult = nDefValue;
	LONG nError = WindowsRegistryTools_GetDWORDRegKey(hKey, strValueName, nDefValue, &nResult);

	*out_bValue = bDefaultValue;
	if (ERROR_SUCCESS == nError)
	{
		bool bResult = (nResult != 0) ? true : false;
		*out_bValue = bResult;
	}
	return nError;
}


LONG WindowsRegistryTools_GetStringRegKey(HKEY hKey, wchar_t const *const strValueName, wchar_t const *const strDefaultValue, wchar_t const **const out_strValue)
{
	size_t nBufSize = 512 * sizeof(LPBYTE);
	LPBYTE szBuffer = (LPBYTE) malloc(nBufSize);
	ULONG nError = 0;
	DWORD dwBufSize = nBufSize;
	memset(szBuffer,'/0',nBufSize);

	nError = RegQueryValueExW(hKey, strValueName, 0, NULL, szBuffer, &dwBufSize);

	*out_strValue = strDefaultValue;
	if (ERROR_SUCCESS == nError)
	{
		wchar_t const*const strBuffer = (wchar_t *)realloc(szBuffer,nBufSize);
		*out_strValue = strBuffer;
	}
	else
	{
		free(szBuffer);
	}
	return nError;
}

#endif



// from http://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way

size_t trimwhitespace(char *out, size_t len, const char *str)
{
	if(len == 0)
		return 0;

	{
		const char *end = NULL;
		size_t out_size = 0;

		// Trim leading space
		while(isspace(*str)) str++;

		if(*str == 0)  // All spaces?
		{
			*out = 0;
			return 1;
		}

		// Trim trailing space
		end = str + strlen(str) - 1;
		while(end > str && isspace(*end)) end--;
		end++;

		// Set output size to minimum of trimmed string length and buffer size minus 1
		out_size = (end - str) < len-1 ? (end - str) : len-1;

		// Copy trimmed string and add null terminator
		memcpy(out, str, out_size);
		out[out_size] = 0;

		return out_size;
	}
}

