#include "stdafx.h"
#include "resource.h"
#include "DicomShell.h"

class CDicomShellModule : public CAtlDllModuleT< CDicomShellModule >
{
public :
	DECLARE_LIBID(LIBID_DicomShellLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DICOMSHELL, "{EF93610C-517E-466B-BEEE-50AAA82C490C}")
};

CDicomShellModule _AtlModule;


#ifdef _MANAGED
#pragma managed(push, off)
#endif

HINSTANCE g_hInstance;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	g_hInstance = hInstance;
	
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		// register RLE decompression codec
		DcmRLEDecoderRegistration::registerCodecs();
		DJDecoderRegistration::registerCodecs(); // register JPEG codecs
		break;
	case DLL_PROCESS_DETACH:
		// deregister RLE decompression codec
		DcmRLEDecoderRegistration::cleanup();
		// deregister JPEG decompression codecs
		DJDecoderRegistration::cleanup();
		break;
	}

    return _AtlModule.DllMain(dwReason, lpReserved); 
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}
// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}

