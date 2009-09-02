#pragma once

void convert(const char* src, STRRET* dst);
void convert(unsigned long, STRRET* dst);
void convert(const char* src, PROPVARIANT *dst);
void convert(PROPVARIANT* src, STRRET* dst);
void getFileTimes(
	LPCTSTR fileName,
	LPFILETIME lpCreationTime,
	LPFILETIME lpLastAccessTime,
	LPFILETIME lpLastWriteTime);
int compare(FILETIME const& a, FILETIME const& b);
