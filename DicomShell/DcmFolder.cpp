#include "StdAfx.h"
#include "DcmFolder.h"
#include "Pidl.h"

PidlData::PidlData()
: magic(s_magic)
{
}

bool PidlData::IsValid() const
{
	return magic == s_magic;
}

ULONG PidlData::s_magic = 0x79fe72aa;

DcmFolder::DcmFolder(void)
: _type(Invalid)
, _void(0)
{
}

DcmFolder::~DcmFolder(void)
{
}

DcmFolder::Type DcmFolder::GetType() const
{
	return _type;
}

void DcmFolder::Set(DcmSequenceOfItems* sequence)
{
	_type = SequenceOfItems;
	_sequence = sequence;
}

size_t DcmFolder::GetItemCount() const
{
	switch (GetType())
	{
	case SequenceOfItems:
		return GetSequenceOfItems()->card();
		break;
	case Item:
		return GetItem()->card();
		break;
	default:
		return 0;
		break;
	}
}

DcmFolder DcmFolder::GetChild(LPCITEMIDLIST pidl) const
{
	DcmFolder c(*this);
	for (LPCITEMIDLIST i = pidl; i->mkid.cb; i = ILGetNext(i))
	{
		PidlData d;
		Pidl(i).GetData(d);
		if (d.IsValid())
		{
			switch(c.GetType())
			{
			case SequenceOfItems:
				c.Set(c.GetSequenceOfItems()->getItem(d.index));
				break;
			case Item:
				c.Set(c.GetItem()->getElement(d.index));
				break;
			default:
				break;
			}
		}
	}
	return c;
}

DcmElement* DcmFolder::GetElement() const
{
	switch (GetType())
	{
	case Element:
		return _element;
	case SequenceOfItems:
		return _sequence;
	default:
		throw 0;
	}
}

DcmItem* DcmFolder::GetItem() const
{
	switch (GetType())
	{
	case Item:
		return _item;
	default:
		throw 0;
	}
}

DcmSequenceOfItems* DcmFolder::GetSequenceOfItems() const
{
	switch (GetType())
	{
	case SequenceOfItems:
		return _sequence;
	default:
		throw 0;
	}
}

void DcmFolder::Set(DcmItem* item)
{
	if (!item)
	{
		throw 0;
	}
	_type = Item;
	_item = item;
}

void DcmFolder::Set(DcmElement* element)
{
	DcmSequenceOfItems* s = dynamic_cast<DcmSequenceOfItems*>(element);
	if (s)
	{
		_type = SequenceOfItems;
		_sequence = s;
	}
	else
	{
		_type = Element;
		_element = element;
	}
}

bool DcmFolder::HasType(Type t) const
{
	Type at = GetType();
	switch (t)
	{
	case Element:
		return (at == Element || at == SequenceOfItems);
	default:
		return at == t;
	}
}

DcmFolder::DcmFolder(DcmFolder const& src)
: _type(src._type)
, _void(src._void)
{
}

DcmFolder const & DcmFolder::operator=(DcmFolder const& src)
{
	_type = src._type;
	_void = src._void;
	return *this;
}
