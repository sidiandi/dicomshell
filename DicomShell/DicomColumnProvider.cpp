// DicomColumnProvider.cpp : Implementation of CDicomColumnProvider

#include "stdafx.h"
#include "DicomColumnProvider.h"

// CDicomColumnProvider

DicomTagList CDicomColumnProvider::s_tags(TEXT("columns"));

CDicomColumnProvider::CDicomColumnProvider()
{
}

HRESULT CDicomColumnProvider::Initialize(LPCSHCOLUMNINIT psci)
{
	try
	{
		CopyMemory(&m_init, psci, sizeof(m_init));
		m_columns = & s_tags.getTags();
		return S_OK;
	}
	catch (...)
	{
		return E_FAIL;
	}
}

HRESULT CDicomColumnProvider::GetColumnInfo(DWORD dwIndex, SHCOLUMNINFO *psci)
{
	try
	{
		USES_CONVERSION;
		DicomTagList::TagCollection const& columns = *m_columns;

		if (dwIndex < columns.size())
		{
			psci->scid.fmtid = CLSID_DicomColumnProvider;
			psci->scid.pid = dwIndex;
			psci->vt = VT_I4;
			psci->fmt = LVCFMT_LEFT;
			psci->cChars = 20;
			psci->csFlags = SHCOLSTATE_TYPE_STR | SHCOLSTATE_SECONDARYUI;
			lstrcpyW(psci->wszTitle, A2W(DcmTag(columns[dwIndex]).getTagName()));
			lstrcpyW(psci->wszDescription, A2W(DcmTag(columns[dwIndex]).getTagName()));
			return S_OK;
		}
		else
		{
			return S_FALSE;
		}
	}
	catch (...)
	{
		return E_FAIL;
	}
}

bool CDicomColumnProvider::selectFile(const char* path)
{
	std::string fileName(path);
	if (fileName == m_currentFileName)
	{
		return true;
	}
	else
	{
		try
		{
			m_currentFileName = std::string();
			m_currentFileName = fileName;
			m_currentFile.setReadMode(ERM_fileOnly);
			OFCondition status = m_currentFile.loadFile(fileName.c_str(),
				EXS_Unknown,
				EGL_noChange,
				DCM_MaxReadLength,
				ERM_fileOnly);
			if (status.good())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		catch (...)
		{
			return false;
		}
	}
}

HRESULT CDicomColumnProvider::GetItemData(LPCSHCOLUMNID pscid, LPCSHCOLUMNDATA pscd, VARIANT *pvarData)
{
	try
	{
		USES_CONVERSION;
		DicomTagList::TagCollection const& columns = *m_columns;
		if (pscid->fmtid == CLSID_DicomColumnProvider)
		{
			if (pscid->pid < columns.size())
			{
				if (selectFile(W2A(pscd->wszFile)))
				{
					OFString value;
					if (m_currentFile.getDataset()->findAndGetOFString(columns[pscid->pid], value).good())
					{
						CComVariant v(A2W(value.c_str()));
						v.Detach(pvarData);
						return S_OK;
					}
				}
			}
		}
		return S_FALSE;
	}
	catch (...)
	{
		return E_FAIL;
	}
}
