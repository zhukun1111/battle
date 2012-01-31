#ifndef _H_PROTOCOL_H_
#define _H_PROTOCOL_H_

#include <kernel/wmdefine.h>

class IWMPeer;

class CProtocol
{
public:
	UINT32 m_dwType;

public:
	explicit CProtocol(UINT32 dwType) : m_dwType(dwType)
	{
	}

	virtual ~CProtocol()
	{
	}

	friend OctetsStream& operator << (OctetsStream& os, const CProtocol& roProtocol);
	friend OctetsStream& operator >> (const OctetsStream& os, CProtocol& roProtocol);

	virtual void Process(const IWMPeer& roWMPeer) = 0;

	virtual CProtocol* Clone() const = 0;

	virtual void Reset(){}

	virtual void Destroy()
	{
		delete this; 
	}

	virtual bool SizePolicy(size_t size) const = 0;

	bool Encode(OctetsStream& os, char* pcPkg, INT32* pnLength) const;
	bool Decode(OctetsStream& os, const char* pcPkg, INT32 nLength);

	static CProtocol* GetProtocol(UINT32 dwType);
	static void Register(void);

protected:
	typedef std::map<UINT32, CProtocol*> CMapProtocol;

	static CMapProtocol& _ProtocolMap()
	{
		static CMapProtocol s_oMapProtocol;
		return s_oMapProtocol;
	}

protected:
	virtual OctetsStream& _Marshal(OctetsStream& os) const = 0;
	virtual const OctetsStream& _Unmarshal(const OctetsStream& os) = 0;
};

inline OctetsStream& operator << (OctetsStream& os, const CProtocol& roProtocol)
{
	os << CompactUINT(roProtocol.m_dwType);

	size_t dwTypeEnd = os.size();
	roProtocol._Marshal(os);

	size_t dwDataEnd = os.size();
	size_t dwdataLen = dwDataEnd - dwTypeEnd;
	os << CompactUINT(dwdataLen);

	char acBuf[sizeof(UINT32)];
	size_t dwSizeLen = os.size() - dwdataLen;
	char* pcBegin = static_cast<char*>(os.begin());

	memcpy(acBuf, pcBegin + dwDataEnd, dwSizeLen);
	memmove(pcBegin + dwTypeEnd + dwSizeLen, pcBegin + dwTypeEnd);
	memcpy(pcBegin + dwTypeEnd, acBuf, dwSizeLen);
	
	return os;
}

inline OctetsStream& operator >> (const OctetsStream& os, CProtocol& roProtocol)
{
	UINT32 dwSize;
	CompactUINT oCompactType(roProtocol.m_dwType);
	CompactUINT oCompactSize(dwSize);

	os >> oCompactType >> oCompactSize;
	return roProtocol._Unmarshal(os);
}
#endif