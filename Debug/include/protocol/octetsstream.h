#ifndef _H_OCTESSTREAM_H
#define _H_OCTESSTREAM_H
#endif

#include "octets.h"

template<typename U, typename V>
union TypeAliasing
{
	U u;
	V v;
	TypeAliasing(V _v) : v(_v){}
	operator U() const {return u;}
};

template<typename U, typename V>
inline U aliasing_cast(V v)
{
	TypeAliasing<U, V> data(v);
	return data;
}

class CompactUINT;
class CompactSINT;

class OctetsStream
{
public:
	OctetsStream(void) : pos(0) {}
	explicit OctetsStream(size_t size) : data(size), pos(0) {}
	OctetsStream(const Octets& o) : data(o), pos(0) {}
	OctetsStream(const OctetsStream& os) : data(os.data), pos(0) {}

	OctetsStream& operator = (const OctetsStream& os)
	{
		if (this != &os)
		{
			pos = os.pos;
			data = os.data;
		}

		return *this;
	}

	bool operator == (const OctetsStream& os) const { return data == os.data; }
	bool operator != (const OctetsStream& os) const { return data != os.data; }

	size_t size() const { return data.size(); }
	void assign(const void* beg, size_t size) { data.assign(beg, size); pos = 0; }
	void swap(OctetsStream& os) { data.swap(os.data); }

	operator Octets& () { return data; }
	operator const Octets& () const { return data; }
	size_t position() const { return pos; }

	void *begin() { return data.begin(); }
	void *end() { return data.end(); }

	const void *begin() const { return data.begin(); }
	const void *end() const { return data.end(); }

	void reserve(size_t size) { data.reserve(size); }
	void insert(void* pos, const void* beg, size_t len) { data.insert(pos, beg, len); }
	void insert(void* pos, const void* beg, const void* end) { data.insert(pos, beg, end); }
	void* erase(void* beg, void* end) { return data.erase(beg, end); }
	void clear() {data.clear(); pos = 0;}

	OctetsStream& operator << (INT8 val)				{ return _push_back(val); }
	OctetsStream& operator << (UINT8 val)				{ return _push_back(val); }
	OctetsStream& operator << (INT16 val)				{ return _push_back(byteorder_16(val)); }
	OctetsStream& operator << (UINT16 val)				{ return _push_back(byteorder_16(val)); }
	OctetsStream& operator << (INT32 val)				{ return _push_back(byteorder_32(val)); }
	OctetsStream& operator << (UINT32 val)				{ return _push_back(byteorder_32(val)); }
	OctetsStream& operator << (INT64 val)				{ return _push_back(byteorder_64(val)); }
	OctetsStream& operator << (UINT64 val)				{ return _push_back(byteorder_64(val)); }
	

	OctetsStream& operator << (char val)				{ return _push_back(val); }
	OctetsStream& operator << (bool val)				{ return _push_back(val); }
	OctetsStream& operator << (float val)				{ return _push_back(byteorder_32(aliasing_cast<UINT32>(val))); }
	OctetsStream& operator << (double val)				{ return _push_back(byteorder_32(aliasing_cast<UINT64>(val))); }

	OctetsStream& push_back(const char* src, size_t len)
	{
		data.insert(data.end(), src, len);
		return *this;
	}

	OctetsStream& operator << (const Octets& o)
	{
		_compact_uint32(static_cast<UINT32>(o.size()));
		data.insert(data.end(), o.begin(), o.end());
		return *this;
	}

	const OctetsStream& operator >> (INT8& val) const 
	{
		val = _pop_front_8();
		return *this;
	}

	const OctetsStream& operator >> (UINT8& val) const 
	{
		val = _pop_front_8();
		return *this;
	}

	const OctetsStream& operator >> (INT16& val) const 
	{
		val = _pop_front_16();
		return *this;
	}

	const OctetsStream& operator >> (UINT16& val) const 
	{
		val = _pop_front_16();
		return *this;
	}

	const OctetsStream& operator >> (INT32& val) const 
	{
		val = _pop_front_32();
		return *this;
	}

	const OctetsStream& operator >> (UINT32& val) const 
	{
		val = _pop_front_32();
		return *this;
	}


	const OctetsStream& operator >> (INT64& val) const 
	{
		val = _pop_front_64();
		return *this;
	}

	const OctetsStream& operator >> (UINT64& val) const 
	{
		val = _pop_front_64();
		return *this;
	}


	const OctetsStream& operator >> (char& val) const 
	{
		val = _pop_front_8();
		return *this;
	}

	const OctetsStream& operator >> (bool& val) const 
	{
		val = _pop_front_8() ? true : false;
		return *this;
	}


	const OctetsStream& operator >> (float& val) const 
	{
		UINT32 tmp = _pop_front_32();
		val = aliasing_cast<float>(tmp);
		return *this;
	}

	const OctetsStream& operator >> (double& val) const 
	{
		UINT64 tmp = _pop_front_64();
		val = aliasing_cast<double>(tmp);
		return *this;
	}

	const OctetsStream& pop_front(void* dest, size_t len) const
	{
		if (pos + len > data.size())
		{
			WMASSERT("can't pop any more data\n");
			return this;
		}

		memcpy(dest, static_cast<const char*>(data.begin()) + pos, len);
		pos += static_cast<UINT32>(len);
		return *this;
	} 

	const OctetsStream& operator >> (Octets& o) const
	{
		UINT32 len;
		_compact_uint32(len);

		if (pos + len > data.size())
		{
			WMASSERT("can't pop any more data\n");
			return this;
		}

		o.assign(static_cast<const char*>(data.begin()) + pos, len);
		pos += len;
		return *this;
	}

	bool eos() const { return pos == data.size(); }

protected:
	enum { MAXSPARE = 16384 };
	Octets data;
	mutable UINT32 pos;
	mutable UINT32 tranpos;

	template<typename T> OctetsStream& _push_back(T val)
	{
		data.insert(data.end(), &val, sizeof(val));
	}

	template<typename T> void _pop_front(T& val) const
	{
		if (pos + sizeof(val) > data.size())
		{
			WMASSERT("can't pop any more data\n");
			return;
		}

		val = *(T*)(static_cast<const char*>(data.begin()) + pos );
		pos += sizeof(val);
	}

	UINT8 _pop_front_8(void) const
	{
		UINT8 val;
		_pop_front(val);
		return val;
	}

	UINT16 _pop_front_16(void) const
	{
		UINT16 val;
		_pop_front(val);
		return byteorder_16(val);
	}

	UINT16 _pop_front_32(void) const
	{
		UINT16 val;
		_pop_front(val);
		return byteorder_32(val);
	}

	UINT16 _pop_front_64(void) const
	{
		UINT16 val;
		_pop_front(val);
		return byteorder_64(val);
	}

	OctetsStream& _compact_uint32(UINT32);
	const OctetsStream& _uncompact_uint32(UINT32& val) const;

	OctetsStream& _compact_sint32(UINT32);
	const OctetsStream& _uncompact_sint32(INT32& val) const;

	friend OctetsStream& operator << (OctetsStream& os, const CompactUINT& _compact);
	friend const OctetsStream& operator >> (const OctetsStream& os, CompactUINT& _compact);

	friend OctetsStream& operator << (OctetsStream& os, const CompactSINT& _compact);
	friend const OctetsStream& operator >> (const OctetsStream& os, CompactSINT& _compact);
};

class CompactUINT
{
	const UINT32* data;
public:
	explicit CompactUINT(const UINT32& val): data(&val) {}

	friend OctetsStream& operator << (OctetsStream& os, const CompactUINT& _compact);
	friend const OctetsStream& operator >> (const OctetsStream& os, CompactUINT& _compact);
};

inline OctetsStream& operator << (OctetsStream& os, const CompactUINT& _compact)
{
	return os._compact_uint32(*_compact.data);
}

inline OctetsStream& operator << (OctetsStream& os, CompactUINT& _compact)
{
	return os._uncompact_uint32(const_cast<UINT32&>(*_compact.data));
}

class CompactSINT
{
	const INT32* data;
public:
	explicit CompactSINT(const INT32& val): data(&val) {}

	friend OctetsStream& operator << (OctetsStream& os, const CompactSINT& _compact);
	friend const OctetsStream& operator >> (const OctetsStream& os, CompactSINT& _compact);
};

inline OctetsStream& operator << (OctetsStream& os, const CompactSINT& _compact)
{
	return os._compact_sint32(*_compact.data);
}

inline OctetsStream& operator << (OctetsStream& os, CompactSINT& _compact)
{
	return os._uncompact_sint32(const_cast<UINT32&>(*_compact.data));
}