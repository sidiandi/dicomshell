#include "stdafx.h"
#include <dcmtk/dcmdata/dcdict.h>
#include <dcmtk/dcmdata/dcdicent.h>
#include "DicomTagList.h"
#include "support.h"

namespace
{
	DcmTagKey defaultTags[] =
	{
		DCM_PatientsName,
		DCM_PatientsBirthDate,
		DCM_PatientsSex,

		DCM_StudyDescription,
		DCM_SeriesDescription,
		DCM_SeriesNumber,
		DCM_InstanceNumber,
		DCM_Rows,
		DCM_Columns,

		DCM_TransferSyntaxUID,
		DCM_SOPClassUID,
		DCM_Modality,
		DCM_ProtocolName,
		DCM_SequenceName,
		DCM_SoftwareVersions,
		DCM_SpecificCharacterSet
	};
}

extern HINSTANCE g_hInstance;

DicomTagList::DicomTagList(LPCTSTR key)
: m_valid(false)
, m_key(key)
{
	ZeroMemory(&m_dicTime, sizeof(m_dicTime));
}

DicomTagList::~DicomTagList(void)
{
}

DicomTagList::TagCollection const& DicomTagList::getTags()
{
	USES_CONVERSION;

	try
	{
		TCHAR mfn[_MAX_PATH];
		GetModuleFileName(g_hInstance, mfn, _MAX_PATH);
		m_dicPath = mfn;
		m_dicPath.append(TEXT("\\..\\"));
		m_dicPath.append(m_key);
		m_dicPath.append(TEXT(".dic"));

		FILETIME writeTime;
		getFileTimes(m_dicPath.c_str(), 0, 0, &writeTime);
		if (compare(m_dicTime, writeTime) < 0)
		{
			m_dicTime = writeTime;
			m_valid = false;
		}
	}
	catch (...)
	{
	}
	
	if (!m_valid)
	{
		m_tags.clear();
		DcmDataDictionary dic(false, false);
		if (dic.loadDictionary(W2A(m_dicPath.c_str())))
		{
			DcmHashDictIterator iter(dic.normalBegin());
			DcmHashDictIterator end(dic.normalEnd());
			for (; iter != end; ++iter)
			{
				m_tags.push_back(**iter);
			}
		}
		else
		{
			m_tags = TagCollection(defaultTags, defaultTags + sizeof(defaultTags) / sizeof(*defaultTags));
		}
		m_valid = true;
	}
	return m_tags;
}
