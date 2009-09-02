#include "StdAfx.h"
#include "Pidl.h"

static LPMALLOC g_pMalloc = 0;

Pidl::Pidl(void)
: _pidl(0)
{
}

Pidl::Pidl(LPCITEMIDLIST pidl)
: _pidl(ILClone(pidl))
{
}

Pidl::~Pidl(void)
{
	Free();
}

Pidl const& Pidl::operator=(LPCITEMIDLIST pidl)
{
	Free();
	_pidl = ILClone(pidl);
	Trace();
	return *this;
}

void Pidl::Copy(LPITEMIDLIST* pidl) const
{
	Trace();
	*pidl = ILClone(_pidl);
}

void Pidl::Free()
{
	if (_pidl)
	{
		ILFree(_pidl);
	}
}

CComBSTR Pidl::ToString() const
{
	CComBSTR s;
	if (_pidl)
	{
		for (SHITEMID* i = &_pidl->mkid; i->cb; i = (SHITEMID*)(((LPBYTE)i)+i->cb))
		{
			s.Append(TEXT("item"));
			s.Append(TEXT("\n"));
		}
	}
	else
	{
		s.Append(_TEXT("<undefined>"));
	}
	return s;
}

CComBSTR Pidl::Path() const
{
	WCHAR path[_MAX_PATH];
	SHGetPathFromIDList(_pidl, path);
	return CComBSTR(path);
}

void Pidl::Trace() const
{
	
	ATLTRACE(Path());
}

void Pidl::SetData(void* data, size_t count)
{
	Free();
	
	// Allocate enough memory for the PIDL to hold data structure 
    // plus the terminator
	size_t total = count + 2 * sizeof(USHORT);

	if (!g_pMalloc)
	{
		SHGetMalloc(&g_pMalloc);
	}

	_pidl = (LPITEMIDLIST) g_pMalloc->Alloc(total);
	_pidl->mkid.cb = count + sizeof(USHORT);
	memcpy(& _pidl->mkid.abID, data, count);
	LPITEMIDLIST n = ILGetNext(_pidl);
	n->mkid.cb = 0;
}

void Pidl::GetData(void* data, size_t count)
{
	if (_pidl->mkid.cb == count + sizeof(USHORT))
	{
		memcpy(data, & _pidl->mkid.abID, count);
	}
	else
	{
		ZeroMemory(data, count);
	}
}

bool Pidl::IsChild() const
{
	return true;
}
