#include "StdAfx.h"
#include "DicomThumbnail.h"
#include "dcmtk/dcmimage/diregist.h"   /* required to support color images */

DicomTagList DicomThumbnail::s_tags(TEXT("imagetext"));

HBITMAP createHbitmap(SIZE const& size)
{
	const int bitCount = 24;
	
	BITMAPINFO bmi;
	ZeroMemory(&bmi.bmiHeader,sizeof(BITMAPINFOHEADER)); 
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = size.cx;
	bmi.bmiHeader.biHeight = size.cy;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = bitCount;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = ((((bmi.bmiHeader.biWidth * bmi.bmiHeader.biBitCount) + 31) & ~31) >> 3) * bmi.bmiHeader.biHeight; 
	bmi.bmiHeader.biXPelsPerMeter = 0;
	bmi.bmiHeader.biYPelsPerMeter = 0;
	bmi.bmiHeader.biClrUsed = 0;
	bmi.bmiHeader.biClrImportant;

	void* data = 0;
	HBITMAP bitmap = CreateDIBSection(0, &bmi, DIB_RGB_COLORS, &data, NULL, 0);
	return bitmap;
}

HBITMAP asHbitmap(DicomImage& dicomImage)
{
	const int bitCount = 24;
	
	BITMAPINFO bmi;
	ZeroMemory(&bmi.bmiHeader,sizeof(BITMAPINFOHEADER)); 
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = dicomImage.getWidth();
	bmi.bmiHeader.biHeight = dicomImage.getHeight();
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = bitCount;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = ((((bmi.bmiHeader.biWidth * bmi.bmiHeader.biBitCount) + 31) & ~31) >> 3) * bmi.bmiHeader.biHeight; 
	bmi.bmiHeader.biXPelsPerMeter = 0;
	bmi.bmiHeader.biYPelsPerMeter = 0;
	bmi.bmiHeader.biClrUsed = 0;
	bmi.bmiHeader.biClrImportant;

	void* data = 0;
	HBITMAP bitmap = CreateDIBSection(0, &bmi, DIB_RGB_COLORS, &data, NULL, 0);
	dicomImage.createWindowsDIB(data, bmi.bmiHeader.biSizeImage, 0, bitCount, 1, 1);
	return bitmap;
}

bool extractIconImageSequence(DcmDataset* dataset, HBITMAP* thumbnail, SIZE const& size)
{
	DcmSequenceOfItems* iconImageSequence;
	if (!dataset->findAndGetSequence(DCM_IconImageSequence, iconImageSequence).good())
	{
		return false;
	}
	DcmItem* icon = iconImageSequence->getItem(0);
	DicomImage image(icon, EXS_Unknown);
	bool scale = false;
	if (scale)
	{
		DicomImage* tn = image.createScaledImage((unsigned long)size.cx, (unsigned long)size.cy, 2);
		*thumbnail = asHbitmap(*tn);
		delete tn;
	}
	else
	{
		*thumbnail = asHbitmap(image);
	}
	return true;
}

void DicomThumbnail::Create(LPWSTR path, HBITMAP* thumbnail, SIZE const& size)
{
	USES_CONVERSION;
	DcmFileFormat file;
	OFCondition s = file.loadFile(W2A(path));
	if (!s.good()) throw s;
	Create(file, thumbnail, size);
}

void DicomThumbnail::Create(DcmFileFormat& file, HBITMAP* thumbnail, SIZE const& size)
{
	USES_CONVERSION;

	if (extractIconImageSequence(file.getDataset(), thumbnail, size))
	{
	}
	else
	{
	    E_TransferSyntax xfer = file.getDataset()->getOriginalXfer();
	    unsigned long opt_compatibilityMode = CIF_MayDetachPixelData | CIF_TakeOverExternalDataset;
		DicomImage sourceImage(file.getDataset(), xfer);
		EI_Status status = sourceImage.getStatus();
		if (status == EIS_Normal)
		{
			sourceImage.setWindow(0);
			DicomImage* tn = sourceImage.createScaledImage((unsigned long)size.cx, (unsigned long)size.cy, 2);
			if (!tn) throw EIS_InvalidDocument;
			*thumbnail = asHbitmap(*tn);
			delete tn;
		}
		else
		{
			CreateTextThumbnail(file, thumbnail, size);
		}
		return;
	}
}

bool appendValue(CComBSTR& out, DcmDataset& dataset, DcmTagKey const& tag)
{
	USES_CONVERSION;
	OFString value;
	if (dataset.findAndGetOFString(tag, value).good())
	{
		out.Append(A2W(value.c_str()));
		return true;
	}
	else
	{
		return false;
	}
}

void DicomThumbnail::CreateTextThumbnail(DcmFileFormat& file, HBITMAP* thumbnail, SIZE const& size)
{
	HBITMAP bm = createHbitmap(size);
	HDC dc = CreateCompatibleDC(::GetDC(0));
	//create a new bitmap and select it in the memory dc
	HGDIOBJ obj = SelectObject(dc,bm);

	DcmDataset& dataset = * file.getDataset();

	CComBSTR info;

	/*
	DicomTagList::TagCollection const & tags = s_tags.getTags();

	for (size_t i=0; i < tags.size(); ++i)
	{
		if (appendValue(info, dataset, tags[i]))
		{
			info.Append(TEXT("\n"));
		}
	}
	*/

	info.Append(TEXT("DICOM "));
	appendValue(info, dataset, DCM_Modality);
	info.Append(TEXT("\n"));
	appendValue(info, dataset, DCM_PatientsName);
	info.Append(TEXT("\n"));
	appendValue(info, dataset, DCM_PatientsSex);
	info.Append(TEXT("\n"));
	appendValue(info, dataset, DCM_PatientsBirthDate);

	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = r.left + size.cy;
	r.bottom = r.top + size.cy;

	::SetTextColor(dc, RGB(0xff, 0xff, 0xff));
	::SetBkColor(dc, RGB(0x00, 0x00, 0x00));

	//choose the font
	HGDIOBJ hfnt, hOldFont; 
 
    hfnt = GetStockObject(ANSI_VAR_FONT); 
    hOldFont = SelectObject(dc, hfnt);

	DrawText(dc, (WCHAR*)(BSTR)info, -1, &r, DT_WORDBREAK);

	//final cleanup
    SelectObject(dc, hOldFont); 
	SelectObject(dc,obj);
	DeleteDC(dc);

	*thumbnail = bm;
}

