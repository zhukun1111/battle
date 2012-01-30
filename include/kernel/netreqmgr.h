#ifndef _H_NETREQMGR_H_
#define _H_NETREQMGR_H_

#include "wmdefine.h"
#include "singleton.h"
#include "mempipe.h"


enum ENetReqType
{
	NETREQ_INVALID = 0,
	NETREQ_START_LISTEN = 1,
	NETREQ_STOP_LISTEN,
	NETREQ_CONNECT,
	NETREQ_SEND,
};

struct SStartListenReq
{
	WMHANDLE	m_hCtrlHandle;
	INT32		m_nListenSock;
	UINT32		m_dwRecvBufSize;
	UINT32		m_dwSendBufSize;
};

struct SStopListenReq
{
	WMHANDLE	m_hCtrlHandle;
};

struct SConnReq
{
	WMHANDLE	m_hCtrlHandle;
	UINT32		m_dwIP;
	UINT16		m_wPort;
	UINT32		m_dwRecvBufSize;
	UINT32		m_dwSendBufSize;
};

struct SSendReq
{
	UINT32	m_dwConnID;
	INT32	m_nLen;
	char*	m_pData;
};

struct SNetRequest
{
	ENetReqType	nType;
	union
	{
		SStartListenReq	stStartListen;
		SStopListenReq	stStopListen;
		SConnReq		stConn;
		SSendReq		stSend;
	}stUn;
};

typedef std::list<SNetRequest> CListNetRequest;

class CNetReqMgr
{
	CNetReqMgr(void);
	virtual ~CNetReqMgr(void);

public:

	DECLARE_SINGLETON(CNetReqMgr);

	bool Init();
	void WriteNetReq(SNetRequest& rstReq);
	void WriteSendData(UINT32 dwConnID, const char* pcData, INT32 nLen);

	inline bool ReadNetReq(SNetRequest& rstReq)
	{
		if (m_oBufQueue.size() != 0)
		{
			rstReq = m_oBufQueue.front();
			m_oBufQueue.pop_front();
			return true;
		}
		return m_oSendPipe.Read((char*)&rstReq, sizeof(rstReq));
	}

	inline bool ReadData(char* pcBuf, INT32 nLen)
	{
		while (false == m_oSendPipe.Read(pcBuf, nLen));	
		return true;
	}

	inline void ReadReqToBuffer()
	{
		for (INT32 i = 0; i < WMNET_TEMPBUF_QUEUE_LEN; i++)
		{
			SNetRequest stTmp;

			if (false == m_oSendPipe.Read((char*)&stTmp, sizeof(stTmp)))
			{
				return;
			}

			if (NETREQ_SEND == stTmp.nType)
			{
				stTmp.stUn.stSend.m_pData = new char[stTmp.stUn.stSend.m_nLen];
				if (NULL == stTmp.stUn.stSend.m_pData)
				{
					LOG("new char[%d] fail", stTmp.stUn.stSend.m_nLen);
					m_oSendPipe.Discard(stTmp.stUn.stSend.m_nLen);		//相当于服务器丢失消息，不管了
					continue;
				}
				if (false == ReadData(stTmp.stUn.stSend.m_pData, stTmp.stUn.stSend.m_nLen))
				{
					LOG("Read SendData Len %d failed", stTmp.stUn.stSend.m_nLen);
					delete [] stTmp.stUn.stSend.m_pData;
					continue;
				}
			}

			m_oBufQueue.push_back(stTmp);
		}
	}

protected:
	MemPipe		m_oSendPipe;
	CListNetRequest	m_oBufQueue;
};
#endif
