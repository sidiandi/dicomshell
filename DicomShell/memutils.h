//------------------------------------------------------------------------------
// MemUtils.h
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
// Smart pointers and other memory helpers
//
//------------------------------------------------------------------------------

#pragma once

#include <atlcore.h>
#include <atlbase.h>
#include <atlmem.h>
#include <strsafe.h>

template<typename T, class Allocator = CCRTAllocator>
class CMemPtr :
    public CHeapPtr<T, Allocator>
{
public:
    HRESULT Allocate(size_t nElements = 1) throw()
    {
        return CHeapPtr<T, Allocator>::Allocate(nElements) ? S_OK : E_OUTOFMEMORY;
    }

    HRESULT AllocateBytes(size_t nBytes) throw()
    {
        return CHeapPtr<T, Allocator>::AllocateBytes(nBytes) ? S_OK : E_OUTOFMEMORY;
    }

    HRESULT Reallocate(size_t nElements = 1) throw()
    {
        return CHeapPtr<T, Allocator>::Reallocate(nElements) ? S_OK : E_OUTOFMEMORY;
    }

    HRESULT ReallocateBytes(size_t nBytes = 1) throw()
    {
        return CHeapPtr<T, Allocator>::ReallocateBytes(nBytes) ? S_OK : E_OUTOFMEMORY;
    }
};

// uses COM Heap API (CoTaskMemAlloc)
template <typename T>
class CComMemPtr : public CMemPtr<T, CComAllocator>
{
};

// uses LocalXXX Heap API
template <typename T>
class CLocalMemPtr : public CMemPtr<T, CLocalAllocator>
{
};

// uses GlobalXXX heap API
template <typename T>
class CGlobalMemPtr : public CMemPtr<T, CGlobalAllocator>
{
};

inline HRESULT CoAllocStringLen(PCWSTR pszSource, size_t cch, __deref_out_ecount(cch+1) LPWSTR *ppsz)
{
    *ppsz = NULL;

    size_t cchTotal;
    HRESULT hr = S_OK;
	cchTotal = cch + 1;
    if (SUCCEEDED(hr))
    {
        CComMemPtr<WCHAR> spsz;
        hr = spsz.Allocate(cchTotal);
        if (SUCCEEDED(hr))
        {
            hr = StringCchCopyNW(spsz, cchTotal, pszSource, cch);
            if (SUCCEEDED(hr))
            {
                *ppsz = spsz.Detach();
            }
        }
    }
    return hr;
}
