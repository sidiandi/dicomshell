//------------------------------------------------------------------------------
// IDListUtils.h
//------------------------------------------------------------------------------
// File provided for Microsoft Shared Source.
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
//------------------------------------------------------------------------------
//
// Normalized IDList functions
//
//------------------------------------------------------------------------------

#pragma once

//  IID_PPV_ARGS(ppType)
//      ppType is the variable of type IType that will be filled
//
//      RESULTS in:  IID_IType, ppvType
//      will create a compiler error if wrong level of indirection is used.
//

template<typename T> void** IID_PPV_ARGS_Helper(T** pp)
{
    // make sure everyone derives from IUnknown
    static_cast<IUnknown*>(*pp);
    return reinterpret_cast<void**>(pp);
}

#define IID_PPV_ARGS(ppType) __uuidof(**(ppType)), IID_PPV_ARGS_Helper(ppType)

#define IFACEMETHODIMP virtual STDMETHODIMP  
#define PCIDLIST_ABSOLUTE LPCITEMIDLIST
#define PIDLIST_RELATIVE LPITEMIDLIST 
#define PIDLIST_ABSOLUTE  LPITEMIDLIST 
#define PCUIDLIST_RELATIVE LPCITEMIDLIST 
#define PCUITEMID_CHILD_ARRAY LPCITEMIDLIST* 
#define PCUITEMID_CHILD LPCITEMIDLIST 
#define PITEMID_CHILD LPITEMIDLIST 
#define PROPERTYKEY SHCOLUMNID
#define ITEMIDLIST_ABSOLUTE ITEMIDLIST
#define PCUIDLIST_ABSOLUTE LPCITEMIDLIST
#define PCITEMID_CHILD LPCITEMIDLIST

inline HRESULT CloneIDList(PCUIDLIST_RELATIVE pidl, __deref_out PIDLIST_RELATIVE *ppidlOut)
{
    HRESULT hr = E_INVALIDARG;
    if (pidl)
    {
        *ppidlOut = ILClone(pidl);
        hr = *ppidlOut ? S_OK : E_OUTOFMEMORY;
    }
    return hr;
}

inline HRESULT CloneFullIDList(PCUIDLIST_ABSOLUTE pidl, __deref_out PIDLIST_ABSOLUTE *ppidlOut)
{
    return CloneIDList(pidl, reinterpret_cast<PIDLIST_RELATIVE *>(ppidlOut));
}

inline HRESULT CloneChildIDList(PCITEMID_CHILD pidl, __deref_out PITEMID_CHILD *ppidlOut)
{
    *ppidlOut = ILClone(pidl);
    return *ppidlOut ? S_OK : E_OUTOFMEMORY;
}

inline HRESULT CombineIDLists(PCIDLIST_ABSOLUTE pidl1, PCUIDLIST_RELATIVE pidl2, __out PIDLIST_ABSOLUTE *ppidlOut)
{
    *ppidlOut = ILCombine(pidl1, pidl2);
    return *ppidlOut ? S_OK : E_OUTOFMEMORY;
}

inline HRESULT CloneFirstIDList(PCUIDLIST_RELATIVE pidl, __deref_out PITEMID_CHILD *ppidl)
{
    *ppidl = ILCloneFirst(pidl);
    return *ppidl ? S_OK : E_OUTOFMEMORY;
}

__inline HRESULT ResultFromShort(int i)
{ 
    return MAKE_HRESULT(SEVERITY_SUCCESS, 0, (USHORT)(i));
}

// use for extracting a comparison value from return value of IShellFolder::CompareIDs()
__inline short ShortFromResult(HRESULT hr)
{
    return (short)HRESULT_CODE(hr);
}

// use to creating an HRESULT to be returned from IShellFolder::CompareIDs() from a comparison value
__inline HRESULT CompareResultFromInt(int i)
{
    // Preserves the value's sign when casting to short
    return ResultFromShort((USHORT)(i < 0 ? -1 : (i > 0 ? 1 : 0)));
}

