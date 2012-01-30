#include "kernel/pch.h"
#include "buffmgr.h"

INSTANCE_SINGLETON(CBufMgr);

char* CBufMgr::CreateBuf(UINT32 dwSize)
{
	char* pRet = NULL;
	CMapBuf::iterator itBuf = m_oMapBuf.find(dwSize);
	if (itBuf == m_oMapBuf.end() || itBuf->second.empty())
	{
		pRet = new char[dwSize];
		if (NULL == pRet)
		{
			LOG("new char[%u] failed", dwSize);
			return NULL;
		}

		SBufInfo stInfo;
		stInfo.m_bInUse = true;
		stInfo.m_dwSize = dwSize;
		m_oMapBufInfo[pRet] = stInfo;
	}
	else
	{
		pRet = *(itBuf->second.end() - 1);
		itBuf->second.pop_back();
		CMapBufInfo::iterator itBufInfo = m_oMapBufInfo.find(pRet);
		if (itBufInfo != m_oMapBufInfo.end())
		{
			ASSERT( (dwSize == itBufInfo->second.m_dwSize) && (false == itBufInfo->second.m_bInUse) );
			itBufInfo->second.m_bInUse = true;
		}
		else
		{
			ASSERT(false);
		}
	}

	return pRet;
}

void CBufMgr::ReleaseBuf( char* pBuf)
{
	CMapBufInfo::iterator it = m_oMapBufInfo.find(pBuf);
	if (it == m_oMapBufInfo.end())
	{
		LOG("Can not find buf 0x%p", pBuf);
		return;
	}

	SBufInfo& rstInfo = it->second;
	if (false == rstInfo.m_bInUse)
	{
		LOG("Buf Ox%p already released", pBuf);
		return;
	}

	rstInfo.m_bInUse =  false;
	UINT32 dwSize = it->second.m_dwSize;

	CMapBuf::iterator itBuf = m_oMapBuf.find(dwSize);
	if (itBuf != m_oMapBuf.end())
	{
		itBuf->second.push_back(pBuf);
	}
	else
	{
		m_oMapBuf.insert(std::pair<UINT32, CVecBuf>(dwSize, CVecBuf(1, pBuf)));
	}
}

CBufMgr::~CBufMgr()
{
	for (CMapBuf::iterator it = m_oMapBuf.begin(); it != m_oMapBuf.end(); it++)
	{
		CVecBuf& oVecBuf = it->second;
		for (CVecBuf::iterator itVec = oVecBuf.begin(); itVec != oVecBuf.end(); ++itVec)
		{
			delete [] *itVec;
		}
	}
}

CBufMgr::CBufMgr()
{

}