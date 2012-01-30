#ifndef _H_NETLIN_H_ 
#define _H_NETLIN_H_

#include "wmnetwork.h"
#include "netevtmgr.h"
#include "netreqmgr.h"
#include <map>

struct SCtrlHandle
{
	WMHANDLE		m_hNetHandle;
	bool			m_bInitial;
	UINT32			m_dwIP;
	UINT16			m_wPort;
	IWMNetworkProcessor* m_poNetProcessor;
	SWMUserData		m_stUserData;
};

struct SConnInfo
{
	void Reset()
	{
		m_dwConnID		= 0;
		m_hNetHandle	= INVALID_WMHANDLE;
		m_bInitial		= false;
		m_dwLocalIP		= 0;
		m_dwRemoteIP	= 0;
		m_wLocalPort	= 0;
		m_wRemotePort	= 0;
		m_poNetProcessor = NULL;
		m_stUserData.m_dwUserData = 0;
		m_stUserData.m_pUserPtr = NULL;
	}

	UINT32			m_dwConnID;
	WMHANDLE		m_hNetHandle;
	bool			m_bInitial;
	UINT32			m_dwLocalIP;
	UINT32			m_dwRemoteIP;
	UINT16			m_wLocalPort;
	UINT16			m_wRemotePort;
	IWMNetworkProcessor* m_poNetProcessor;
	SWMUserData		m_stUserData;
};

typedef std::map<WMHANDLE, SCtrlHandle> CMapCtrlHandle;
typedef std::map<UINT32, SConnInfo> CMapConnInfo;

class CNetControler : public IWMNetWork
{
	CNetControler();
	virtual ~CNetControler();

public:
	DECLARE_SINGLETON(CNetControler);

	virtual WMHANDLE WMAPI Connect(UINT32 dwRemoteIP, UINT16 wRemotePort, IWMNetworkProcessor* poNetProcessor, UINT32 dwSendBufSize = WM_DEFAULT_SENDBUF_SIZE , UINT32 dwRecvBufSize = WM_DEFAULT_RECVBUF_SIZE);
	virtual WMHANDLE WMAPI Listen(UINT32 dwLocalIP, UINT16 wLocalPort, IWMNetworkProcessor* poNetProcessor, bool bReuseAddr, UINT32 dwSendBufSize, UINT32 dwRecvBufSize);
	virtual bool WMAPI Send(UINT32 dwConnID, const char* pcData, INT32 nLen);
	virtual bool WMAPI ProcessMsg(INT32 nCount);

	bool Init(void);
	void Uninit(void);

protected:
	WMHANDLE _GetNextHandle();

	bool _AddCtrlHandle(SCtrlHandle& rstCtrlHandle);
	bool _RemoveCtrlHandle(WMHANDLE hNetHandle);
	SCtrlHandle* _GetCtrlHandle(WMHANDLE hNetHandle);

	bool _AddConnInfo(const SConnInfo& rstConnInfo);
	bool _RemoveConnInfo(UINT32 dwConnID);
	SConnInfo* _GetConnInfo(UINT32 dwConnID);

	inline void _ProConnEvt(SConnectEvt* pstEvent);
	inline void _ProcRecvEvt(SRecvEvt* pstEvent);

protected:
	bool				m_bInit;
	WMHANDLE			m_hNextCtrlHandle;
	CMapCtrlHandle		m_oMapCtrlHandle;
	CMapConnInfo		m_oMapConnInfo;
	char*				m_pRecvBuf;
};

#endif
