// DicomFile.h : Declaration of the CDicomFile

#pragma once
#include "resource.h"       // main symbols
#include "DicomShell.h"
#include "DicomTagList.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// CDicomFile

class ATL_NO_VTABLE CDicomFile :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CDicomFile, &CLSID_DicomFile>,
	public IDispatchImpl<IDicomFile, &IID_IDicomFile, &LIBID_DicomShellLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IExtractImage2, &IID_IExtractImage2, &LIBID_ShellObjects>,
	public IDispatchImpl<IPersistFile, &IID_IPersistFile>,
	IQueryInfo,
	IRunnableTask
{
public:
	CDicomFile();
	virtual ~CDicomFile();

	DECLARE_REGISTRY_RESOURCEID(IDR_DICOMFILE)

	BEGIN_COM_MAP(CDicomFile)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IDicomFile)

		COM_INTERFACE_ENTRY(IExtractImage2)
		COM_INTERFACE_ENTRY(IExtractImage)
		COM_INTERFACE_ENTRY(IPersistFile)
		COM_INTERFACE_ENTRY_IID(IID_IRunnableTask, IRunnableTask)
		COM_INTERFACE_ENTRY_IID(IID_IQueryInfo, IQueryInfo)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	// IDicomFile implementation
	typedef IDispatchImpl<IDicomFile, &IID_IDicomFile, &LIBID_DicomShellLib, /*wMajor =*/ 1, /*wMinor =*/ 0> IDispatchImplStandard;
	STDMETHOD(Open)(BSTR path);

	STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid,
		LCID lcid, WORD wFlags, DISPPARAMS* pdispparams, VARIANT* pvarResult,
		EXCEPINFO* pexcepinfo, UINT* puArgErr);

	// IExtractImage2 implementation

	virtual HRESULT STDMETHODCALLTYPE GetDateStamp( 
        /* [out] */ FILETIME *pDateStamp);

    virtual HRESULT STDMETHODCALLTYPE GetLocation( 
        /* [size_is][out] */ LPWSTR pszPathBuffer,
        /* [in] */ DWORD cch,
        /* [unique][out][in] */ DWORD *pdwPriority,
        /* [in] */ const SIZE *prgSize,
        /* [in] */ DWORD dwRecClrDepth,
        /* [out][in] */ DWORD *pdwFlags);
    
    virtual HRESULT STDMETHODCALLTYPE Extract( 
        /* [out] */ HBITMAP *phBmpThumbnail);

	// IPersistFile implementation
	
	virtual HRESULT STDMETHODCALLTYPE IsDirty( void);
        
    virtual HRESULT STDMETHODCALLTYPE Load( 
        /* [in] */ LPCOLESTR pszFileName,
        /* [in] */ DWORD dwMode);
    
    virtual HRESULT STDMETHODCALLTYPE Save( 
        /* [unique][in] */ LPCOLESTR pszFileName,
        /* [in] */ BOOL fRemember);
    
    virtual HRESULT STDMETHODCALLTYPE SaveCompleted( 
        /* [unique][in] */ LPCOLESTR pszFileName);
    
    virtual HRESULT STDMETHODCALLTYPE GetCurFile( 
        /* [out] */ LPOLESTR *ppszFileName);

	virtual HRESULT STDMETHODCALLTYPE GetClassID( 
		/* [out] */ CLSID *pClassID);

    // IRunnableTask implementation

	STDMETHOD (Run)(THIS);
    STDMETHOD (Kill)(THIS_ BOOL fWait );
    STDMETHOD (Suspend)(THIS);
    STDMETHOD (Resume)(THIS);
    STDMETHOD_(ULONG, IsRunning)(THIS);

    // IQueryInfo implementation
	
    STDMETHOD(GetInfoTip)(THIS_ DWORD dwFlags, WCHAR **ppwszTip);
    STDMETHOD(GetInfoFlags)(THIS_ DWORD *pdwFlags);

	// other methods

	DcmFileFormat& getDcmFileFormat();

private:

	SCODE getText(
		istream& in,
		/* [out][in] */ ULONG *pcwcBuffer,
		/* [size_is][out] */ WCHAR *awcBuffer);

	TCHAR m_szFile[MAX_PATH];
	SIZE m_thumbnailSize;
	DWORD m_colorDepth;
	LPMALLOC m_pMalloc;

	HRESULT getFileTimes(
		LPFILETIME lpCreationTime,
		LPFILETIME lpLastAccessTime,
		LPFILETIME lpLastWriteTime);

	DcmFileFormat* m_dcmFileFormat;
	static DicomTagList s_tags;
};

OBJECT_ENTRY_AUTO(__uuidof(DicomFile), CDicomFile)
