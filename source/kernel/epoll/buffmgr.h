#ifndef _H_BUFMGR_H
#define _H_BUFMGR_H

#include "kernel/singleton.h"
#include <vector>
#include <map>

class CBufMgr
{
	struct SBufInfo
	{
		bool m_bInUse;
		UINT32 m_dwSize;
	};

	typedef std::vector<char*> CVecBuf;
	typedef std::map<UINT32, CVecBuf> CMapBuf;
	typedef std::map<char*, SBufInfo> CMapBufInfo;

	CBufMgr(void);
	virtual ~CBufMgr(void);

public:
	DECLARE_SINGLETON(CBufMgr);
	char* CreateBuf(UINT32 dwSize);
	void ReleaseBuf(char* pBuf);

protected:
	CMapBufInfo m_oMapBufInfo;
	CMapBuf	m_oMapBuf;
};

#endif