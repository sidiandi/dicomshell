#pragma once

class DicomStringFormat
{
public:
	DicomStringFormat(ostream&);
public:
	~DicomStringFormat(void);

	void print(DcmFileFormat&);
	void print(DcmItem&);
	void print(DcmElement&);
private:
	ostream& out;
};
