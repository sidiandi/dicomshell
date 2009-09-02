

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Wed Sep 02 19:38:04 2009
 */
/* Compiler settings for .\DicomShell.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __DicomShell_h__
#define __DicomShell_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDicomFile_FWD_DEFINED__
#define __IDicomFile_FWD_DEFINED__
typedef interface IDicomFile IDicomFile;
#endif 	/* __IDicomFile_FWD_DEFINED__ */


#ifndef __DicomFile_FWD_DEFINED__
#define __DicomFile_FWD_DEFINED__

#ifdef __cplusplus
typedef class DicomFile DicomFile;
#else
typedef struct DicomFile DicomFile;
#endif /* __cplusplus */

#endif 	/* __DicomFile_FWD_DEFINED__ */


#ifndef __DicomColumnProvider_FWD_DEFINED__
#define __DicomColumnProvider_FWD_DEFINED__

#ifdef __cplusplus
typedef class DicomColumnProvider DicomColumnProvider;
#else
typedef struct DicomColumnProvider DicomColumnProvider;
#endif /* __cplusplus */

#endif 	/* __DicomColumnProvider_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "shobjidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IDicomFile_INTERFACE_DEFINED__
#define __IDicomFile_INTERFACE_DEFINED__

/* interface IDicomFile */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IDicomFile;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("46EF4C89-4C3A-4618-AD36-74EBEDC2903F")
    IDicomFile : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Open( 
            /* [in] */ BSTR path) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDicomFileVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDicomFile * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDicomFile * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDicomFile * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDicomFile * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDicomFile * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDicomFile * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDicomFile * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Open )( 
            IDicomFile * This,
            /* [in] */ BSTR path);
        
        END_INTERFACE
    } IDicomFileVtbl;

    interface IDicomFile
    {
        CONST_VTBL struct IDicomFileVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDicomFile_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDicomFile_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDicomFile_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDicomFile_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDicomFile_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDicomFile_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDicomFile_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDicomFile_Open(This,path)	\
    (This)->lpVtbl -> Open(This,path)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDicomFile_Open_Proxy( 
    IDicomFile * This,
    /* [in] */ BSTR path);


void __RPC_STUB IDicomFile_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDicomFile_INTERFACE_DEFINED__ */



#ifndef __DicomShellLib_LIBRARY_DEFINED__
#define __DicomShellLib_LIBRARY_DEFINED__

/* library DicomShellLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_DicomShellLib;

EXTERN_C const CLSID CLSID_DicomFile;

#ifdef __cplusplus

class DECLSPEC_UUID("C7DF344C-2496-4965-885E-1AAC8D702D28")
DicomFile;
#endif

EXTERN_C const CLSID CLSID_DicomColumnProvider;

#ifdef __cplusplus

class DECLSPEC_UUID("49EA79DD-E386-46D3-A1CE-292F8C95C3F8")
DicomColumnProvider;
#endif
#endif /* __DicomShellLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


