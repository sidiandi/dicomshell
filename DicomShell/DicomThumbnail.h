#pragma once
#include "DicomTagList.h"

class DcmFileFormat;

class DicomThumbnail
{
public:
	static void Create(DcmFileFormat& file, HBITMAP* thumbnail, SIZE const& size);
	static void CreateTextThumbnail(DcmFileFormat& file, HBITMAP* thumbnail, SIZE const& size);
private:
	static void Create(LPWSTR path, HBITMAP* thumbnail, SIZE const& size);
	static DicomTagList s_tags;
};
