#include "stdafx.h"
#include "support.h"

void convert(const char* src, STRRET* dst)
{
	dst->uType = STRRET_CSTR;
	strncpy(dst->cStr, src, _MAX_PATH);
}

void convert(unsigned long value, STRRET* dst)
{
	char str[_MAX_PATH];
	itoa(value, str, 10);
	convert(str, dst);
}

void convert(const char* src, PROPVARIANT *dst)
{
	// todo
}

void convert(PROPVARIANT* src, STRRET* dst)
{
	// todo
}

void getFileTimes(
	LPCTSTR m_szFile,
	LPFILETIME lpCreationTime,
	LPFILETIME lpLastAccessTime,
	LPFILETIME lpLastWriteTime)
{
	HANDLE file = CreateFile(m_szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
	if (!file)
	{
		throw 0;
	}
	GetFileTime(file, lpCreationTime, lpLastAccessTime, lpLastWriteTime);
	CloseHandle(file);
}

void convert(FILETIME const& s, LARGE_INTEGER& d)
{
	d.HighPart = s.dwHighDateTime;
	d.LowPart = s.dwLowDateTime;
}

int compare(FILETIME const& a, FILETIME const& b)
{
	LARGE_INTEGER ia, ib;
	convert(a, ia);
	convert(b, ib);
	if (ia.QuadPart < ib.QuadPart)
	{
		return -1;
	}
	else if (ia.QuadPart == ib.QuadPart)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
