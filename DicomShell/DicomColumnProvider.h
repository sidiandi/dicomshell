// DicomColumnProvider.h : Declaration of the CDicomColumnProvider

#pragma once
#include "resource.h"       // main symbols
#include "DicomShell.h"
#include "DicomTagList.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CDicomColumnProvider

class ATL_NO_VTABLE CDicomColumnProvider :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDicomColumnProvider, &CLSID_DicomColumnProvider>,
	public IColumnProvider
{
public:
	CDicomColumnProvider();

DECLARE_REGISTRY_RESOURCEID(IDR_DICOMCOLUMNPROVIDER)


BEGIN_COM_MAP(CDicomColumnProvider)
	COM_INTERFACE_ENTRY_IID(IID_IColumnProvider, IColumnProvider)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

    // IColumnProvider methods
    STDMETHOD (Initialize)(THIS_ LPCSHCOLUMNINIT psci);
    STDMETHOD (GetColumnInfo)(THIS_ DWORD dwIndex, SHCOLUMNINFO *psci);
    STDMETHOD (GetItemData)(THIS_ LPCSHCOLUMNID pscid, LPCSHCOLUMNDATA pscd, VARIANT *pvarData);

private:
	bool selectFile(const char* path);

	SHCOLUMNINIT m_init;
	std::string m_currentFileName;
	DcmFileFormat m_currentFile;
	static DicomTagList s_tags;
	DicomTagList::TagCollection const* m_columns;
};

OBJECT_ENTRY_AUTO(__uuidof(DicomColumnProvider), CDicomColumnProvider)
