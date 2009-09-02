

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IDicomFile,0x46EF4C89,0x4C3A,0x4618,0xAD,0x36,0x74,0xEB,0xED,0xC2,0x90,0x3F);


MIDL_DEFINE_GUID(IID, LIBID_DicomShellLib,0xA16C99BE,0x3929,0x4BB5,0xBD,0xF8,0xF7,0xD0,0xF2,0x0A,0xBC,0xA1);


MIDL_DEFINE_GUID(CLSID, CLSID_DicomFile,0xC7DF344C,0x2496,0x4965,0x88,0x5E,0x1A,0xAC,0x8D,0x70,0x2D,0x28);


MIDL_DEFINE_GUID(CLSID, CLSID_DicomColumnProvider,0x49EA79DD,0xE386,0x46D3,0xA1,0xCE,0x29,0x2F,0x8C,0x95,0xC3,0xF8);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



