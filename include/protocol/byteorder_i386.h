#ifndef _H_BYTEORDER_I386_H_
#define _H_BYTEORDER_I386_H_

#if defined BYTE_ORDER_BIG_ENDIAN
	#define byteorder_16(val) (val)
	#define byteorder_32(val) (val)
	#define byteorder_64(val) (val)

#elif defined __GUNC__

	inline UINT16 byteorder_16(UINT16 val)	
	{
		register UINT16 v;
		__asm__("xchg %b0, %h0" : "=q"(v) : "0"(val));
		return v;
	}

	inline UINT32 byteorder_32(UINT32 val)	
	{
		register UINT32 v;
		__asm__("bswap %0" : "=r"(v) : "0"(val));
		return v;
	}

	inline UINT64 byteorder_64(UINT64 val)	
	{
		union
		{
			UINT64 __ll;
			UINT32 __l[2];
		}i, o;

		i.__ll = val;
		o.__l[0] = byteorder_32(i.__l[1]);
		o.__l[1] = byteorder_32(i.__l[0]);
		return o.__ll;
	}

#elif defined WIN32
	inline UINT64 byteorder_16(UINT16 val)
	{
		__asm ror val, 8
		return val;
	}
	
	inline UINT64 byteorder_32(UINT32 val)	
	{
		__asm mov eax, val
		__asm bswap eax
		__asm mov val, eax
		return val;
	}

	inline UINT64 byteorder_64(UINT64 val)	
	{
		union
		{
			UINT64 __ll;
			UINT32 __l[2];
		}i, o;

		i.__ll = val;
		o.__l[0] = byteorder_32(i.__l[1]);
		o.__l[1] = byteorder_32(i.__l[0]);

		return o.__ll;
	}
#endif
#endif