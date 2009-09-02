// DicomFile.cpp : Implementation of CDicomFile

#include "stdafx.h"
#include "DicomFile.h"
#include "DicomThumbnail.h"
#include "DicomStringFormat.h"
#include "support.h"
#include <time.h>
#include "License.h"

// CDicomFile

DicomTagList CDicomFile::s_tags(TEXT("tooltip"));

CDicomFile::CDicomFile()
: m_dcmFileFormat(0)
{
	SHGetMalloc(&m_pMalloc);
}

CDicomFile::~CDicomFile()
{
	delete m_dcmFileFormat;
}

HRESULT STDMETHODCALLTYPE CDicomFile::GetDateStamp( 
	/* [out] */ FILETIME *pDateStamp)
{
	try
	{
		HANDLE file = CreateFile(m_szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
		GetFileTime(file, NULL, NULL, pDateStamp);
		CloseHandle(file);
		return S_OK;
	}
	catch (...)
	{
		return E_FAIL;
	}
}

HRESULT STDMETHODCALLTYPE CDicomFile::GetLocation( 
	/* [size_is][out] */ LPWSTR pszPathBuffer,
	/* [in] */ DWORD cch,
	/* [unique][out][in] */ DWORD *pdwPriority,
	/* [in] */ const SIZE *prgSize,
	/* [in] */ DWORD dwRecClrDepth,
	/* [out][in] */ DWORD *pdwFlags)
{
	try
	{
		StrCpyN(pszPathBuffer, m_szFile, cch);
		if (pdwPriority)
		{
			*pdwPriority = IEI_PRIORITY_MIN;
		}
		if (prgSize)
		{
			m_thumbnailSize = *prgSize;
		}
		else
		{
			m_thumbnailSize.cx = 96;
			m_thumbnailSize.cy = 96;
		}
		m_colorDepth = dwRecClrDepth;
		*pdwFlags = IEIFLAG_ASYNC | IEIFLAG_CACHE;
		return E_PENDING;
		// *pdwFlags = IEIFLAG_CACHE;
		// return NOERROR;
	}
	catch (...)
	{
		return E_FAIL;
	}
}

HRESULT STDMETHODCALLTYPE CDicomFile::Extract( 
	/* [out] */ HBITMAP *phBmpThumbnail)
{
	try
	{
		License license;
		if (!license.isLicenseValid())
		{
			return E_NOTIMPL;
		}

		try
		{
			DicomThumbnail::Create(getDcmFileFormat(), phBmpThumbnail, m_thumbnailSize);
			return NOERROR; 
		}
		catch (...)
		{
			return E_INVALIDARG;
		}
	}
	catch (...)
	{
		return E_FAIL;
	}
}

HRESULT STDMETHODCALLTYPE CDicomFile::IsDirty( void)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDicomFile::Load( 
	/* [in] */ LPCOLESTR pszFileName,
	/* [in] */ DWORD dwMode)
{
	try
	{
		USES_CONVERSION;
		_tcscpy(m_szFile, OLE2T((WCHAR*)pszFileName)); 
		ATLTRACE(m_szFile);
		ATLTRACE(_T("\n"));
		return S_OK;    
	}
	catch (...)
	{
		return E_FAIL;
	}
}

HRESULT STDMETHODCALLTYPE CDicomFile::Save( 
	/* [unique][in] */ LPCOLESTR pszFileName,
	/* [in] */ BOOL fRemember)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDicomFile::SaveCompleted( 
	/* [unique][in] */ LPCOLESTR pszFileName)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDicomFile::GetCurFile( 
	/* [out] */ LPOLESTR *ppszFileName)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDicomFile::GetClassID( 
	/* [out] */ CLSID *pClassID)
{
	return E_NOTIMPL;
}

HRESULT CDicomFile::Run()
{
	return E_PENDING;
}

HRESULT CDicomFile::Kill(BOOL fWait )
{
	return NOERROR;
}

HRESULT CDicomFile::Suspend()
{
	return E_NOTIMPL;
}

HRESULT CDicomFile::Resume()
{
	return E_NOTIMPL;
}

ULONG CDicomFile::IsRunning()
{
	return 1;
}

HRESULT CDicomFile::GetInfoTip(DWORD dwFlags, WCHAR **ppwszTip)
{
	try
	{
		USES_CONVERSION;
		DicomTagList::TagCollection const & tags = s_tags.getTags();

		CComBSTR bstrInfo;
		bstrInfo.Append(TEXT(""));
		License license;
		if (license.isLicenseValid())
		{
			DcmFileFormat& file = getDcmFileFormat();
			if (file.error().good())
			{
				for (size_t i=0; i < tags.size(); ++i)
				{
					OFString value;
					if (file.getDataset()->findAndGetOFString(tags[i], value).good())
					{
						bstrInfo.Append(A2W(DcmTag(tags[i]).getTagName()));
						bstrInfo.Append(TEXT(": "));
						bstrInfo.Append(A2W(value.c_str()));
						bstrInfo.Append(TEXT("\n"));
					}
				}
			}
			else
			{
				bstrInfo.Append(TEXT("Not a DICOM file."));
			}
		}
		else
		{
			bstrInfo.Append(TEXT("License is not valid.\n"));
		}

		// bstrInfo.Append(TEXT("\n"));
		// bstrInfo.Append(TEXT("DICOM Shell (c) andreas.grimme@gmx.net\n"));

		if (license.hasExpirationDate())
		{
			TCHAR timeStr[512];
			time_t expT = license.expirationDate();
			tm* ptm = gmtime(&expT);
			wcsftime(timeStr, 512, TEXT("%#c"), ptm);
			bstrInfo.Append(TEXT("Evaluation period will expire "));
			bstrInfo.Append(timeStr);
			bstrInfo.Append(TEXT("\n"));
		}

		*ppwszTip = (WCHAR*) m_pMalloc->Alloc((bstrInfo.Length() +1) * sizeof(WCHAR));
		if (*ppwszTip)
		{
			wcscpy(*ppwszTip, (WCHAR*)(BSTR)bstrInfo);
		}
		return S_OK;
	}
	catch (...)
	{
		return E_FAIL;
	}
}

HRESULT CDicomFile::GetInfoFlags(DWORD *pdwFlags)
{
	return E_NOTIMPL;
}

HRESULT CDicomFile::getFileTimes(
								 LPFILETIME lpCreationTime,
								 LPFILETIME lpLastAccessTime,
								 LPFILETIME lpLastWriteTime)
{
	try
	{
		HANDLE file = CreateFile(m_szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
		GetFileTime(file, lpCreationTime, lpLastAccessTime, lpLastWriteTime);
		CloseHandle(file);
		return S_OK;
	}
	catch (...)
	{
		return E_FAIL;
	}
}

DcmFileFormat& CDicomFile::getDcmFileFormat()
{
	USES_CONVERSION;

	if (!m_dcmFileFormat)
	{
		m_dcmFileFormat = new DcmFileFormat();
		m_dcmFileFormat->loadFile(W2A(m_szFile));
	}
	return *m_dcmFileFormat;
}

STDMETHODIMP CDicomFile::Open(BSTR path)
{
	wcscpy(m_szFile, path);
	return S_OK;
}

STDMETHODIMP CDicomFile::Invoke(DISPID dispidMember, REFIID riid,
	LCID lcid, WORD wFlags, DISPPARAMS* pdispparams, VARIANT* pvarResult,
	EXCEPINFO* pexcepinfo, UINT* puArgErr)
{
	if (dispidMember)
	{
		return IDispatchImplStandard::Invoke(dispidMember, riid,lcid, wFlags, pdispparams, pvarResult,pexcepinfo, puArgErr);
	}
	else
	{
		return S_OK;
	}
}
