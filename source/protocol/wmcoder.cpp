#include "pch.h"
#include "wmsystem.h"
#include "octetsstream.h"
#include "data.h"
#include "wminterface.h"
#include "wmprotocol.h"
#include "wmcoder.h"

bool CWMCoder::Encode( const CData* poPtcHead, const char* pcProtocol, INT32 nPtcLen, char* pcPkg, INT32* pnPkgLen )
{
	WMASSERT(NULL != pcProtocol && NULL != pcPkg && NULL != pnPkgLen && *pnPkgLen > 0);
	
	if (NULL == poPtcHead)
	{
		if (*pnPkgLen < nPtcLen)
		{
			LOG("Package Length is %d, shorter than %d", *pnPkgLen, nPtcLen);
			*pnPkgLen = -1;
			return false;
		}
		else
		{
			memcpy(pcPkg, pcProtocol, nPtcLen);
			*pnPkgLen = nPtcLen;
			return true;
		}
	}
	else
	{
		m_oStream.clear();
		m_oStream << *poPtcHead;

		if (static_cast<size_t>(*pnPkgLen) < nPtcLen + m_oStream.size())
		{
			LOG("Package Length is %d, shorter than %d", *pnPkgLen, nPtcLen + m_oStream.size());
			*pnPkgLen = -1;
			return false;
		}
		else
		{
			memcpy(pcPkg, m_oStream.begin(), m_oStream.size());
			m_oStream(pcPkg + m_oStream.size(), pcProtocol, nPtcLen);
			*pnPkgLen = nPtcLen + static_cast<INT32>(m_oStream.size());
			return true;
		}
	}

}

bool CWMCoder::Encode( const CData* poPtcHead,const CProtocol* poProtocol, char* pcPkg, INT32* pnPkgLen )
{
	WMASSERT(NULL != poProtocol && NULL != pcPkg && NULL != pnPkgLen && *pnPkgLen > 0);

	if (NULL == poPtcHead)
	{
		return poProtocol->Encode(m_oStream, pcPkg, pnPkgLen);
	}
	else
	{
		m_oStream.clear();
		m_oStream << *poPtcHead;

		if (static_cast<size_t>(*pnPkgLen) < m_oStream.size())
		{
			LOG("Package Length is %d, shorter than %d", *pnPkgLen, nPtcLen + m_oStream.size());
			*pnPkgLen = -1;
			return false;
		}
		else
		{
			INT32 nHeadLen = static_cast<INT32>(m_oStream.size());
			memcpy(pcPkg, m_oStream.begin(), nHeadLen);

			*pnPkgLen -= nHeadLen;

			bool bStatus = poProtocol->Encode(m_oStream, pcPkg + nHeadLen, pnPkgLen);
			if (false == bStatus)
			{
				return false;
			}
			else
			{
				*pnPkgLen += nHeadLen;
				return true;
			}
		}
	}
}

bool CWMCoder::Decode( const char* pcPkg, INT32 nPkgLen, CProtocol* poProtocol )
{
	WMASSERT(NULL != pcPkg && nPkgLen > 0 && NULL != poProtocol);
	return poProtocol->Decode(m_oStream, pcPkg, nPkgLen);
}

bool CWMCoder::DecodeProtocolHead( const char* pcPkg, INT32 nPkgLen, CData& roPtcHead, INT32& nHeadLen )
{
	m_oStream.assign(pcPkg, nPkgLen);

	m_oStream >> roPtcHead;

	nHeadLen = static_cast<INT32>(m_oStream.position());
	return true;
}

bool CWMCoder::GetProtocolAttr( const char* pcPkg, INT32 nPkgLen, UINT32& rdwType, UINT32& rdwSize )
{
	m_oStream.assign(pcPkg, static_cast<INT32>(nPkgLen) > 8 ? 8 : nPkgLen);

	CompactUINT oCompactType(rdwType);
	CompactSINT oCompactSize(rdwSize);

	m_oStream >> oCompactType >> oCompactSize;

	return true;
}
