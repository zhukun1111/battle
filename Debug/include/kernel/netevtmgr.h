#ifndef _H_NETEVTMGR_H_
#define _H_NETEVTMGR_H_

#include "wmdefine.h"
#include "singleton.h"
#include "mempipe.h"


enum ENetEvtType
{
	NETEVT_INVALID		= 0,
	NETEVT_CONNECT,
	NETEVT_RECV,
};

struct SConnectEvt
{
	UINT32		m_dwSockID;
	UINT32		m_dwConnID;
	UINT32		m_dwLocalIP;
	UINT32		m_dwRemoteIP;
	UINT16		m_wLocalPort;
	UINT16		m_wRemotePort;
	bool		m_bInitial;
	WMHANDLE	m_hHandle;
};

struct SRecvEvt
{
	UINT32	m_dwConnID;
	UINT32	m_nLen;
	char*	m_pData;
};

struct SNetEvent
{
	ENetEvtType m_nType;
	union
	{
		SConnectEvt		m_stConn;
		SRecvEvt		m_stRecv;
	}stUn;
};

//typedef std::list<SNetEvent> CListNetEvent;

class CNetEvtMgr
{
	CNetEvtMgr(void);
	virtual ~CNetEvtMgr(void);

public:
	DECLARE_SINGLETON(CNetEvtMgr)

	bool Init();

	inline bool ReadNetEvt(SNetEvent& rstEvent)
	{
		return m_oRecvPipe.Read((char*)&rstEvent, sizeof(rstEvent));
	}

	inline bool ReadData(char* pcBuf, INT32 nLen)
	{
		while(false == m_oRecvPipe.Read(pcBuf, nLen));
		return true;
	}

	inline void PushConnEvt(UINT32 dwConnID, bool bInitial, WMHANDLE hHandle,
							UINT32 dwLocalIP, UINT16 wLocalPort, UINT32 dwRemoteIP, UINT16 wRemotePort)
	{
		SNetEvent stEvent;

		stEvent.m_nType			= NETEVT_CONNECT;
		SConnectEvt& rstConnEvt	= stEvent.stUn.m_stConn;
		rstConnEvt.m_dwConnID	= dwConnID;
		rstConnEvt.m_bInitial	= bInitial;
		rstConnEvt.m_hHandle	= hHandle;
		rstConnEvt.m_dwLocalIP	= dwLocalIP;
		rstConnEvt.m_wLocalPort	= wLocalPort;
		rstConnEvt.m_dwRemoteIP	= dwRemoteIP;
		rstConnEvt.m_wRemotePort = wRemotePort;

		_WriteNetEvt(stEvent);
	}

	inline void PushRecvEvt(UINT32 dwConnID, const char* pcData, INT32 nLen);

protected:
	void _WriteNetEvt(SNetEvent& rstEvt);

protected:
	MemPipe		m_oRecvPipe;
};

#endif
