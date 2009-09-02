// to debug this under a new explorer instance setup the following debugging info
// Command: "explorer.exe"
// Command Arguments: "::{E48ABDCD-0DE7-4C5A-91EB-13D6E81506F0}"

#include "stdafx.h"
#include "DicomShell.h"
#include "resource.h"

#include "memutils.h"
#include "ComObjectWithRef.h"

#include "IDListUtils.h"
#include "pidl.h"
#include <objbase.h>
#include "DicomFile.h"
#include "support.h"
#include "DcmFolder.h"

//------------------------------------------------------------------------------
// CDicomFolder
//------------------------------------------------------------------------------

// shell data source object

class ATL_NO_VTABLE CDicomFolder :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CDicomFolder, &CLSID_DicomFolder>,
    public IObjectWithSiteImpl<CDicomFolder>,
    public IPersistFolder2,
    public IShellFolder2
{
public:
    CDicomFolder()
	{
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_DICOMFOLDER)

    BEGIN_COM_MAP(CDicomFolder)
        COM_INTERFACE_ENTRY2(IPersist, IPersistFolder2)
        COM_INTERFACE_ENTRY2(IPersistFolder, IPersistFolder2)
        COM_INTERFACE_ENTRY(IPersistFolder2)
        COM_INTERFACE_ENTRY2(IShellFolder, IShellFolder2)
        COM_INTERFACE_ENTRY(IShellFolder2)
        COM_INTERFACE_ENTRY(IObjectWithSite)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease()
    {
    }

    // IPersist
    IFACEMETHODIMP GetClassID(__out CLSID *pclsid);

    // IPersistFolder
    IFACEMETHODIMP Initialize(PCIDLIST_ABSOLUTE pidl);

    // IPersistFolder2
    IFACEMETHODIMP GetCurFolder(__out PIDLIST_ABSOLUTE *ppidl);

    // IShellFolder
    IFACEMETHODIMP ParseDisplayName(HWND hwnd, __in IBindCtx *pbc, __in PWSTR pszDisplayName, __inout ULONG *pchEaten, __deref_out PIDLIST_RELATIVE *ppidl, __inout ULONG *pdwAttributes);
    IFACEMETHODIMP EnumObjects(HWND hwnd, SHCONTF grfFlags, __deref_out IEnumIDList **ppenmIDList);
    IFACEMETHODIMP BindToObject(PCUIDLIST_RELATIVE pidl, __in IBindCtx *pbc, REFIID riid, __deref_out void **ppv);
    IFACEMETHODIMP BindToStorage(PCUIDLIST_RELATIVE pidl, __in IBindCtx *pbc, REFIID riid, __deref_out void **ppv);
    IFACEMETHODIMP CompareIDs(LPARAM lParam, PCUIDLIST_RELATIVE pidl1, PCUIDLIST_RELATIVE pidl2);
    IFACEMETHODIMP CreateViewObject(HWND hwndOwner, REFIID riid, __deref_out void **ppv);
    IFACEMETHODIMP GetAttributesOf(UINT cidl, __in_ecount_opt(cidl) PCUITEMID_CHILD_ARRAY rgpidl, __inout SFGAOF *rgfInOut);
    IFACEMETHODIMP GetUIObjectOf(HWND hwndOwner, UINT cidl, __in_ecount_opt(cidl) PCUITEMID_CHILD_ARRAY rgpidl, REFIID riid, __reserved UINT *rgfReserved, __deref_out void **ppv);
    IFACEMETHODIMP GetDisplayNameOf(PCUITEMID_CHILD pidl, SHGDNF uFlags, __out STRRET *psrName);
    IFACEMETHODIMP SetNameOf(HWND hwnd, PCUITEMID_CHILD pidl, LPCWSTR pszName, SHGDNF uFlags, __deref_out_opt PITEMID_CHILD *ppidlOut);

	// IShellFolder2
    IFACEMETHODIMP GetDefaultSearchGUID(__out GUID *pguid);
    IFACEMETHODIMP EnumSearches(__deref_out IEnumExtraSearch **ppenum);
    IFACEMETHODIMP GetDefaultColumn(DWORD dwRes, __out ULONG *plSort, __out ULONG *plDisplay);
    IFACEMETHODIMP GetDefaultColumnState(UINT iColumn, __out SHCOLSTATEF *pcsFlags);
    IFACEMETHODIMP GetDetailsEx(PCUITEMID_CHILD pidl, const PROPERTYKEY *pkey, __out VARIANT *pvar);
    IFACEMETHODIMP GetDetailsOf(__in_opt PCUITEMID_CHILD pidl, UINT iColumn, __out SHELLDETAILS *psd);
    IFACEMETHODIMP MapColumnToSCID(UINT iColumn, __out PROPERTYKEY *pkey);

    // IContextMenuCB
    IFACEMETHODIMP CallBack(__in_opt IShellFolder *psf, HWND hwndOwner, __in_opt IDataObject *pdtobj, UINT uiMsg, WPARAM wParam, LPARAM lParam);

private:
	// folder view callback object, used to customize the view behavior of a data source instance

    class CViewCallback : 
        public CComObjectRootEx<CComSingleThreadModel>, 
        public IShellFolderViewCB
    {
    public:
        CViewCallback()
        {
        }

        BEGIN_COM_MAP(CViewCallback)
			COM_INTERFACE_ENTRY_IID(IID_IShellFolderViewCB, IShellFolderViewCB)
        END_COM_MAP()

        DECLARE_PROTECT_FINAL_CONSTRUCT()

        static HRESULT s_CreateInstance(CDicomFolder *prf, REFIID riid, __deref_out void **ppv)
        {
            *ppv = NULL;
            CComObjectWithRef<CViewCallback> *psprfvcb;
            HRESULT hr = CComObjectWithRef<CViewCallback>::CreateInstance(&psprfvcb);
            if (SUCCEEDED(hr))
            {
                hr = psprfvcb->_Initialize(prf);
                if (SUCCEEDED(hr))
                {
                    hr = psprfvcb->QueryInterface(riid, ppv);
                }
                psprfvcb->Release();
            }
            return hr;
        }

        HRESULT FinalConstruct()
        {
            return S_OK;
        }

        void FinalRelease()
        {
            if (_prf)
            {
                _prf->Release();
            }
        }

        // IShellFolderViewCB
        IFACEMETHODIMP MessageSFVCB(UINT uiMsg, WPARAM wParam, LPARAM lParam)
        {
            HRESULT hr;
            switch (uiMsg)
            {
            case SFVM_GETNOTIFY:
                *(PCIDLIST_ABSOLUTE *)wParam = (PIDLIST_ABSOLUTE)_spidl;  // evil weak reference
                *(LONG *)lParam = SHCNE_UPDATEITEM;
                hr = S_OK;

            default:
                hr = E_FAIL;
            }
            return hr;
        }

    private:
        HRESULT _Initialize(CDicomFolder *prf)
        {
            HRESULT hr = prf->GetCurFolder(&_spidl);
            if (SUCCEEDED(hr))
            {
                _prf = prf;
                _prf->AddRef();
            }
            return hr;
        }

    private:
        CComMemPtr<ITEMIDLIST_ABSOLUTE> _spidl;
        CDicomFolder *_prf;
    };

private:
	// Property helpers
    static const struct ColumnInfo
    {
		PROPERTYKEY const& key;
        SHCOLSTATEF scsf;
        const char* title;
		int cx;
		HRESULT (CDicomFolder::*pfnGetProperty)(PCUITEMID_CHILD, STRRET*);
    } c_rgColumnInfo[];

	HRESULT _GetDisplayName(PCUITEMID_CHILD pidl, __out STRRET*);
	HRESULT _GetTag(PCUITEMID_CHILD pidl, __out STRRET*);
	HRESULT _GetVR(PCUITEMID_CHILD pidl, __out STRRET*);
	HRESULT _GetVM(PCUITEMID_CHILD pidl, __out STRRET* ret);
	HRESULT _GetValue(PCUITEMID_CHILD pidl, __out STRRET*);

private:
	HRESULT CreateFolder(DcmFolder& folder, REFIID riid, void **ppv);

	DcmElement* GetElement(PCUITEMID_CHILD pidl);
	DcmItem* GetItem(PCUITEMID_CHILD pidl);

    Pidl _pidl; // namespace location
	CComObject<CDicomFile>* _dicomFile;
	DcmFolder _folder;
	std::vector<Pidl> _childPidl;
};

OBJECT_ENTRY_AUTO(__uuidof(DicomFolder), CDicomFolder)

// IPersist
STDMETHODIMP CDicomFolder::GetClassID(__out CLSID *pclsid)
{
	*pclsid = CLSID_DicomFolder;
    return S_OK;
}

// IPersistFolder
STDMETHODIMP CDicomFolder::Initialize(PCIDLIST_ABSOLUTE pidl)
{
	_pidl = pidl;
	HRESULT hr = CComObject<CDicomFile>::CreateInstance(&_dicomFile);
	CComPtr<IPersistFile> persistFile;
	_dicomFile->QueryInterface(&persistFile);
	_bstr_t path(_pidl.Path());
	persistFile->Load((LPCOLESTR)path, STGM_READ);
	_folder.Set(&_dicomFile->getDcmFileFormat());
	_dicomFile->AddRef();
	return S_OK;
}

// IPersistFolder2
STDMETHODIMP CDicomFolder::GetCurFolder(__out PIDLIST_ABSOLUTE *ppidl)
{
    _pidl.Copy(ppidl);
	return S_OK;
}

// IShellFolder
STDMETHODIMP CDicomFolder::ParseDisplayName(HWND hwnd, __in IBindCtx *pbc, __in PWSTR pszDisplayName, __inout ULONG *pchEaten, __deref_out PIDLIST_RELATIVE *ppidl, __inout ULONG *pdwAttributes)
{
	return E_NOTIMPL;
}

class CopyPidlPolicy
{
public:
    static void init(PITEMID_CHILD *ppidl)
    {
    }
    static void destroy(PITEMID_CHILD *ppidl)
    {
        ILFree(*ppidl);
    }
    static HRESULT copy(PITEMID_CHILD *ppidlDst, const Pidl *ppidlSrc)
    {
		ppidlSrc->Copy(ppidlDst);
		return S_OK;
    }
};

STDMETHODIMP CDicomFolder::EnumObjects(HWND hwnd, SHCONTF grfFlags, __deref_out IEnumIDList **ppenmIDList)
{
	HRESULT hr;
	typedef CComEnumOnSTL<IEnumIDList, &__uuidof(IEnumIDList), PITEMID_CHILD, CopyPidlPolicy, std::vector<Pidl> > EnumIDList;
	_childPidl.resize(_folder.GetItemCount());
	for (size_t i = 0; i < _childPidl.size(); ++i)
	{
		PidlData d;
		d.index = (ULONG) i;
		_childPidl[i].SetData(d);
	}

	CComObject<EnumIDList>* pspenm;
	hr = CComObject<EnumIDList>::CreateInstance(&pspenm);
	pspenm->AddRef();
	hr = pspenm->Init(NULL, _childPidl);
	hr = pspenm->QueryInterface(IID_PPV_ARGS(ppenmIDList));
	pspenm->Release();
	return hr;
}

HRESULT CDicomFolder::CreateFolder(DcmFolder& folder, REFIID riid, void **ppv)
{
    HRESULT hr = E_NOINTERFACE;
	CComObject<CDicomFolder>* f;
	hr = CComObject<CDicomFolder>::CreateInstance(&f);
	f->AddRef();
	f->_dicomFile = this->_dicomFile;
	if (folder.HasType(DcmFolder::Invalid))
	{
		throw 0;
	}
	f->_folder = folder;
	f->_dicomFile->AddRef();
	hr = f->QueryInterface(riid, ppv);
	f->Release();
	return hr;
}

STDMETHODIMP CDicomFolder::BindToObject(PCUIDLIST_RELATIVE pidl, __in IBindCtx *pbc, REFIID riid, __deref_out void **ppv)
{
    *ppv = NULL;
    HRESULT hr = E_NOINTERFACE;
	
    if (riid == __uuidof(IShellFolder) || riid == __uuidof(IShellFolder2))
    {
		hr = CreateFolder(_folder.GetChild(pidl), riid, ppv);
    }
	return hr;
}

STDMETHODIMP CDicomFolder::BindToStorage(PCUIDLIST_RELATIVE pidl, __in IBindCtx *pbc, REFIID riid, __deref_out void **ppv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDicomFolder::CompareIDs(LPARAM lParam, PCUIDLIST_RELATIVE pidl1, PCUIDLIST_RELATIVE pidl2)
{
	PidlData d1;
	PidlData d2;
	Pidl(pidl1).GetData(d1);
	Pidl(pidl1).GetData(d2);
	return (HRESULT)(d1.index - d2.index);
}

STDMETHODIMP CDicomFolder::CreateViewObject(HWND hwndOwner, REFIID riid, __deref_out void **ppv)
{
    *ppv = NULL;
    HRESULT hr = E_NOINTERFACE;
    if (riid == __uuidof(IShellView))
    {
        SFV_CREATE sfvc = {0};
        sfvc.cbSize = sizeof(sfvc);
        hr = QueryInterface(IID_PPV_ARGS(&sfvc.pshf));
        if (SUCCEEDED(hr))
        {
            hr = CViewCallback::s_CreateInstance(
				this, 
				IID_IShellFolderViewCB,
				IID_PPV_ARGS_Helper(&sfvc.psfvcb));
            if (SUCCEEDED(hr))
            {
                CComPtr<IShellView> spsv;
                hr = SHCreateShellFolderView(&sfvc, &spsv);
                if (SUCCEEDED(hr))
                {
                    hr = spsv->QueryInterface(riid, ppv);
                }
                sfvc.psfvcb->Release();
            }
            sfvc.pshf->Release();
        }
    }
    return hr;
}

STDMETHODIMP CDicomFolder::GetAttributesOf(UINT cidl, __in_ecount_opt(cidl) PCUITEMID_CHILD_ARRAY rgpidl, __inout SFGAOF *rgfInOut)
{
	SFGAOF out = 0;
	out |= 
		SFGAO_FOLDER |
		SFGAO_HASSUBFOLDER |
		// SFGAO_BROWSABLE |
		0;
	for (UINT i = 0; i < cidl; ++i)
	{
	}
	*rgfInOut = out & *rgfInOut;
	return S_OK;
}

STDMETHODIMP CDicomFolder::GetUIObjectOf(HWND hwndOwner, UINT cidl, __in_ecount_opt(cidl) PCUITEMID_CHILD_ARRAY rgpidl, REFIID riid, __reserved UINT *rgfReserved, __deref_out void **ppv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDicomFolder::GetDisplayNameOf(PCUITEMID_CHILD pidl, SHGDNF uFlags, __out STRRET *psrName)
{
	DcmElement* e = GetElement(pidl);
	if (e)
	{
		DcmTag t(e->getTag());
		convert(t.getTagName(), psrName);
		return S_OK;
	}
	DcmItem* i = GetItem(pidl);
	if (i)
	{
		Pidl p(pidl);
		PidlData d;
		p.GetData(d);
		stringstream o;
		o << d.index;
		convert(o.str().c_str(), psrName);
		return S_OK;
	}
	return E_NOINTERFACE;
}

STDMETHODIMP CDicomFolder::SetNameOf(HWND hwnd, PCUITEMID_CHILD pidl, LPCWSTR pszName, SHGDNF uFlags, __deref_out_opt PITEMID_CHILD *ppidlOut)
{
	return E_NOTIMPL;
}

// IShellFolder2
STDMETHODIMP CDicomFolder::GetDefaultSearchGUID(__out GUID *pguid)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDicomFolder::EnumSearches(__deref_out IEnumExtraSearch **ppenum)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDicomFolder::GetDefaultColumn(DWORD dwRes, __out ULONG *plSort, __out ULONG *plDisplay)
{
    *plSort = 0;
    *plDisplay = 0;
    return S_OK;
}

const PROPERTYKEY PKEY_Name = { 0 , 0};

const CDicomFolder::ColumnInfo CDicomFolder::c_rgColumnInfo[] =
{
	{ PKEY_Name, SHCOLSTATE_ONBYDEFAULT, "Name", 16, &CDicomFolder::_GetDisplayName },
	{ PKEY_Name, SHCOLSTATE_ONBYDEFAULT, "Tag", 16, &CDicomFolder::_GetTag },
	{ PKEY_Name, SHCOLSTATE_ONBYDEFAULT, "VR", 4, &CDicomFolder::_GetVR },
	{ PKEY_Name, SHCOLSTATE_ONBYDEFAULT, "VM", 4, &CDicomFolder::_GetVM },
	{ PKEY_Name, SHCOLSTATE_ONBYDEFAULT, "Value", 128, &CDicomFolder::_GetValue },
};

DcmElement* CDicomFolder::GetElement(PCUITEMID_CHILD pidl)
{
	DcmFolder c = _folder.GetChild(pidl);
	if (c.HasType(DcmFolder::Element))
	{
		return c.GetElement();
	}
	else
	{
		return 0;
	}
}

DcmItem* CDicomFolder::GetItem(PCUITEMID_CHILD pidl)
{
	DcmFolder c = _folder.GetChild(pidl);
	if (c.HasType(DcmFolder::Item))
	{
		return c.GetItem();
	}
	else
	{
		return 0;
	}
}

HRESULT CDicomFolder::_GetDisplayName(PCUITEMID_CHILD pidl, __out STRRET* ret)
{
	DcmElement* e = GetElement(pidl);
	if (e)
	{
		DcmTag t(e->getTag());
		convert(t.getTagName(), ret);
		return S_OK;
	}
	return E_NOINTERFACE;
}

HRESULT CDicomFolder::_GetVR(PCUITEMID_CHILD pidl, __out STRRET* ret)
{
	DcmElement* e = GetElement(pidl);
	if (e)
	{
		DcmTag t(e->getTag());
		convert(t.getVR().getVRName(), ret);
		return S_OK;
	}
	return E_NOINTERFACE;
}

HRESULT CDicomFolder::_GetVM(PCUITEMID_CHILD pidl, __out STRRET* ret)
{
	DcmElement* e = GetElement(pidl);
	if (e)
	{
		convert(e->getVM(), ret);
		return S_OK;
	}
	return E_NOINTERFACE;
}

HRESULT CDicomFolder::_GetTag(PCUITEMID_CHILD pidl, __out STRRET* ret)
{
	DcmElement* e = GetElement(pidl);
	if (e)
	{
		DcmTag t(e->getTag());
		convert(t.toString().c_str(), ret);
	    return S_OK;
	}
	return E_NOINTERFACE;
}

HRESULT CDicomFolder::_GetValue(PCUITEMID_CHILD pidl, __out STRRET* ret)
{
	DcmElement* e = GetElement(pidl);
	if (e)
	{
		OFString str;
		e->getOFString(str,0);
		convert(str.c_str(), ret);
		return S_OK;
	}
	return E_NOINTERFACE;
}

STDMETHODIMP CDicomFolder::GetDefaultColumnState(UINT iColumn, __out SHCOLSTATEF *pcsFlags)
{
    HRESULT hr = E_INVALIDARG;
    if (iColumn < ARRAYSIZE(c_rgColumnInfo))
    {
		ColumnInfo const & ci = c_rgColumnInfo[iColumn];
        *pcsFlags |= c_rgColumnInfo[iColumn].scsf;
		hr = S_OK;
	}
    return hr;
}

STDMETHODIMP CDicomFolder::GetDetailsEx(PCUITEMID_CHILD pidl, const PROPERTYKEY *pkey, __out VARIANT *pvar)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDicomFolder::GetDetailsOf(__in_opt PCUITEMID_CHILD pidl, UINT iColumn, __out SHELLDETAILS *psd)
{
    HRESULT hr = E_INVALIDARG;
    if (iColumn < ARRAYSIZE(c_rgColumnInfo))
    {
		ColumnInfo const & ci = c_rgColumnInfo[iColumn];
		psd->fmt = LVCFMT_LEFT;
		if (pidl)
		{
			(this->*ci.pfnGetProperty)(pidl, &psd->str);
		}
		else
		{
			psd->cxChar = ci.cx;
			convert(ci.title, &psd->str);
		}
		hr = S_OK;
	}
    return hr;
}

STDMETHODIMP CDicomFolder::MapColumnToSCID(UINT iColumn, __out PROPERTYKEY *pkey)
{
    HRESULT hr = E_INVALIDARG;
    if (iColumn < ARRAYSIZE(c_rgColumnInfo))
    {
        *pkey = c_rgColumnInfo[iColumn].key;
        hr = S_OK;
    }
    return hr;
}


// IContextMenuCB
STDMETHODIMP CDicomFolder::CallBack(__in_opt IShellFolder *psf, HWND hwndOwner, __in_opt IDataObject *pdtobj, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	return E_NOTIMPL;
}

