#include "kernel/pch.h"
#include "protocol/octetsstream.h"

OctetsStream& OctetsStream::_compact_uint32( UINT32 val)
{
	if (val < 0x80 ) return _push_back(static_cast<UINT8>(val));
	else if (val < 0x40000) return _push_back(byteorder_16(val | 0x8000));
	else if (val < 0x200000000) return _push_back(byteorder_32(val|0xc0000000));

	_push_back(static_cast<UINT8>(0xe0));
	return _push_back(byteorder_32(val));
	
	return this;
}

const OctetsStream& OctetsStream::_uncompact_uint32( UINT32& val ) const
{
	if (pos == data.size())
	{
		LOG("pos = data.size()");
		WMASSERT(false);
		return *this;
	}

	switch(*(static_cast<const UINT8*>(data.begin()) + pos) & 0xe0)
	{
	case 0xe0:
		_pop_front_8();
		val = _pop_front_32();
		return *this;
	case 0xc0:
		val = _pop_front_32() & ~0xc0000000;
		return *this;
	case 0xa0:
	case 0x80:
		val = _pop_front_16() & ~0x8000;
		return *this;
	}

	val = _pop_front_8();
	return this;
}

OctetsStream& OctetsStream::_compact_sint32( UINT32 val)
{
	if (val >= 0)
	{
		if(val < 0x40) return _push_back(static_cast<UINT8>(val));
		else if (val < 0x2000) return _push_back(byteorder_16(val | 0x8000));
		else if (val < 0x10000000) return _push_back(byteorder_32(val | 0xc0000000));
		_push_back(static_cast<UINT8>(0xe0));
		return _push_back(byteorder_32(val));
	}

	if (-val > 0)
	{
		if(val < 0x40) return _push_back(static_cast<UINT8>(val));
		else if (val < 0x2000) return _push_back(byteorder_16(val | 0x8000));
		else if (val < 0x10000000) return _push_back(byteorder_32(val | 0xc0000000));
		_push_back(static_cast<UINT8>(0xe0));
		return _push_back(byteorder_32(val));
	}

	_push_back(static_cast<UINT8>(0xf0));
	return _push_back(byteorder_32(val));
	return this;
}

const OctetsStream& OctetsStream::_uncompact_sint32( INT32& val ) const
{	
	if (pos == data.size())
	{
		LOG("pos = data.size()");
		WMASSERT(false);
		return *this;
	}

	switch(*(static_cast<const UINT8*>(data.begin()) + pos) & 0xf0)
	{
	case 0xf0:
		_pop_front_8();
		val = -static_cast<INT32>(_pop_front_32());
		return *this;
	case 0xe0:
		_pop_front_8();
		val = _pop_front_32();
		return *this;
	case 0xd0:
		val = -static_cast<INT32>(_pop_front_32() & ~0xd0000000);
		return *this;
	case 0xc0:
		val = _pop_front_32() & ~0xc000000;
		return *this;
	case 0xb0:
	case 0xa0:
		val = -(_pop_front_16() & ~0x8000);
		return *this;
	case 0x90:
	case 0x80:
		val = _pop_front_16() & ~0x8000;
		return *this;
	case 0x70:
	case 0x60:
	case 0x50:
	case 0x40:
		val = -(_pop_front_8() & ~0x40);
		return *this;
	}

	val = _pop_front_8();
	return this;
}
