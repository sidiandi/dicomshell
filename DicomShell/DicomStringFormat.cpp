#include "StdAfx.h"
#include "DicomStringFormat.h"

DicomStringFormat::DicomStringFormat(ostream& a_out)
: out(a_out)
{
}

DicomStringFormat::~DicomStringFormat(void)
{
}

void DicomStringFormat::print(DcmFileFormat& f)
{
	for (unsigned long i=0; i< f.card(); ++i)
	{
		print(*f.getItem(i));
	}
}

void DicomStringFormat::print(DcmItem& item)
{
	for (unsigned long i = 0; i < item.card(); ++i)
	{
		print(*item.getElement(i));
	}
}

void DicomStringFormat::print(DcmElement& e)
{
	OFString s;
	for (unsigned long i = 0; i < e.getVM(); ++i)
	{
		e.getOFString(s, i);
		out << s << std::endl;
	}
}

