#ifndef _H_WMCODER_H_
#define _H_WMCODER_H_

#include "wmdefine.h"

class CProtocol;
class CData;

const UINT32 WM_DEFAULT_STREAM_SIZE = 1024 * 1024;

class CWMCoder
{
public:
	CWMCoder(UINT32 dwStreamSize = WM_DEFAULT_STREAM_SIZE) : m_oStream(dwStreamSize)
	{

	}

	CWMCoder(const CWMCoder& roCoder) : m_oStream(roCoder.m_oStream)
	{

	}

	CWMCoder& operator = (const CWMCoder& roCoder)
	{
		if (this != &roCoder)
		{
			m_oStream = roCoder.m_oStream;
		}

		return this;
	}

	virtual ~CWMCoder()
	{

	}

	bool Encode(const CData* poPtcHead, const char* pcProtocol, INT32 nPtcLen, char* pcPkg, INT32* pnPkgLen);
	bool Encode(const CData* poPtcHead,const CProtocol* poProtocol, char* pcPkg, INT32* pnPkgLen);

	bool Decode(const char* pcPkg, INT32 nPkgLen, CProtocol* poProtocol);
	bool DecodeProtocolHead(const char* pcPkg, INT32 nPkgLen, CData& roPtcHead, INT32& nHeadLen);
	bool GetProtocolAttr(const char* pcPkg, INT32 nPkgLen, UINT32& rdwType, UINT32& rdwSize);

protected:
	OctetsStream m_oStream;
};

#endif