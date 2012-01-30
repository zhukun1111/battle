#include "pch.h"
#include "netcontroler.h"
#include "netreqmgr.h"


INSTANCE_SINGLETON(CNetControler)

CNetControler::CNetControler()
	:m_bInit(false),
	m_hNextCtrlHandle(0),
	m_pRecvBuf(NULL)
{

}

CNetControler::~CNetControler()
{

}

bool CNetControler::Init( void )
{
	if (true == m_bInit)
	{
		return false;
	}

	CNetEvtMgr::CreateInstance();
	CNetReqMgr::CreateInstance();

	if (false == CNetEvtMgr::Instance()->Init())
	{
		LOG("Init CNetEvtMgr failed");
		return false;
	}

	if (false == CNetReqMgr::Instance()->Init())
	{
		LOG("Init CNetReqMgr failed");
		return false;
	}

	//这里启动底层网络，他们在不断往Pipe读写数据

	m_pRecvBuf = new char[WMNET_MAX_PKG_LEN];
	if (NULL == m_pRecvBuf)
	{
		LOG("new RecvBuf[%d] failed", WMNET_MAX_PKG_LEN);
		return false;
	}

	m_bInit = true;
	return true;
}

void CNetControler::Uninit( void )
{
	CNetEvtMgr::DestroyInstance();
	CNetReqMgr::DestroyInstance();
}

WMHANDLE CNetControler::_GetNextHandle()
{
	return  ++m_hNextCtrlHandle;
}

void CNetControler::_ProConnEvt( SConnectEvt* pstEvent )
{
	SCtrlHandle* pstHandle = _GetCtrlHandle(pstEvent->m_hHandle);
	if (NULL == pstHandle)
	{
		//Close(pstEvent->m_dwConnID);
	}
	else
	{
		SConnInfo stConnInfo;
		stConnInfo.m_dwConnID = pstEvent->m_dwConnID;
		stConnInfo.m_hNetHandle = pstEvent->m_hHandle;
		stConnInfo.m_bInitial = pstEvent->m_bInitial;
		stConnInfo.m_dwLocalIP = pstEvent->m_dwLocalIP;
		stConnInfo.m_dwRemoteIP = pstEvent->m_dwRemoteIP;
		stConnInfo.m_wLocalPort = pstEvent->m_wLocalPort;
		stConnInfo.m_wRemotePort = pstEvent->m_wRemotePort;
		stConnInfo.m_poNetProcessor = pstHandle->m_poNetProcessor;

		_AddConnInfo(stConnInfo);
		//pstHandle->m_poNetProcessor->OnConnect(stConnInfo.m_hNetHandle, stConnInfo.m_dwConnID, WM_SUCCESS, 0, pstHandle->m_stUserData);
	}
}

void CNetControler::_ProcRecvEvt( SRecvEvt* pstEvent )
{
	WMASSERT(pstEvent->m_nLen <= (UINT32)WMNET_MAX_PKG_LEN);

	if (pstEvent->m_pData != NULL)
	{
		memcpy(m_pRecvBuf, pstEvent->m_pData, pstEvent->m_nLen);
		delete [] pstEvent->m_pData;
	}
	else
	{
		if (false == CNetEvtMgr::Instance()->ReadData(m_pRecvBuf, pstEvent->m_nLen))
		{
			LOG("PopFront RecvData Len %d failed", pstEvent->m_nLen);
			return;
		}
	}

	SConnInfo* pstConnInfo = _GetConnInfo(pstEvent->m_dwConnID);
	if (NULL == pstConnInfo)
	{
		return;
	}

	pstConnInfo->m_poNetProcessor->OnRecv(pstConnInfo->m_dwConnID, m_pRecvBuf, pstEvent->m_nLen, pstConnInfo->m_stUserData);
}

bool CNetControler::_AddCtrlHandle( SCtrlHandle& rstCtrlHandle )
{
	if (_GetCtrlHandle(rstCtrlHandle.m_hNetHandle) != NULL)
	{
		return false;
	}

	m_oMapCtrlHandle[rstCtrlHandle.m_hNetHandle] = rstCtrlHandle;
	return true;
}

bool CNetControler::_RemoveCtrlHandle( WMHANDLE hNetHandle )
{
	CMapCtrlHandle::iterator it = m_oMapCtrlHandle.find(hNetHandle);
	if (it == m_oMapCtrlHandle.end())
	{
		return false;
	}

	m_oMapCtrlHandle.erase(it);
}

SCtrlHandle* CNetControler::_GetCtrlHandle( WMHANDLE hNetHandle )
{
	CMapCtrlHandle::iterator it = m_oMapCtrlHandle.find(hNetHandle);
	if(it == m_oMapCtrlHandle.end())
	{
		return NULL;
	}

	return &it->second;
}

bool CNetControler::_AddConnInfo( const SConnInfo& rstConnInfo )
{
	if (_GetConnInfo(rstConnInfo.m_dwConnID) != NULL)
	{
		LOG("ConnID %u exist, replace it", rstConnInfo.m_dwConnID);
	}

	m_oMapConnInfo[rstConnInfo.m_dwConnID] = rstConnInfo;
	return true;
}

bool CNetControler::_RemoveConnInfo( UINT32 dwConnID )
{
	CMapConnInfo::iterator it = m_oMapConnInfo.find(dwConnID);
	if (it == m_oMapConnInfo.end())
	{
		return false;
	}

	m_oMapConnInfo.erase(it);
	return true;
}

SConnInfo* CNetControler::_GetConnInfo( UINT32 dwConnID )
{
	CMapConnInfo::iterator it = m_oMapConnInfo.find(dwConnID);
	if (it == m_oMapConnInfo.end())
	{
		return NULL;
	}

	return &it->second;
}

bool WMAPI CNetControler::ProcessMsg( INT32 nCount )
{
	if (nCount <= 0)
	{
		nCount = 0x7fffffff;
	}

	while (true)
	{
		SNetEvent stEvent;
		if (false == CNetEvtMgr::Instance()->ReadNetEvt(stEvent))
		{
			return false;
		}

		switch(stEvent.m_nType)
		{
		case NETEVT_RECV:
			_ProcRecvEvt(&stEvent.stUn.m_stRecv);
			break;
		case NETREQ_CONNECT:
			_ProConnEvt(&stEvent.stUn.m_stConn);
			break;
		default:
			WMASSERT(false);
			break;
		}

		if (--nCount <= 0)
		{
			break;
		}
	}

	return true;
}

bool WMAPI CNetControler::Send( UINT32 dwConnID, const char* pcData, INT32 nLen )
{
	if (nLen <= 0 || nLen > WMNET_MAX_PKG_LEN)
	{
		return false;
	}

	if (NULL == pcData)
	{
		return false;
	}

	SConnInfo* pstConnInfo = _GetConnInfo(dwConnID);
	if (NULL == pstConnInfo)
	{
		return false;
	}

	CNetReqMgr::Instance()->WriteSendData(dwConnID, pcData, nLen);
	return true;
}

WMHANDLE WMAPI CNetControler::Connect( UINT32 dwRemoteIP, UINT16 wRemotePort, IWMNetworkProcessor* poNetProcessor, UINT32 dwSendBufSize , UINT32 dwRecvBufSize )
{
	LOG("Connect to %s:%u", inet_ntoa((in_addr&)dwRemoteIP), wRemotePort);

	SCtrlHandle stCtrlHandle		= {0};
	stCtrlHandle.m_bInitial			= true;
	stCtrlHandle.m_hNetHandle		= _GetNextHandle();
	stCtrlHandle.m_dwIP				= dwRemoteIP;
	stCtrlHandle.m_wPort			= wRemotePort;
	stCtrlHandle.m_poNetProcessor	= poNetProcessor;

	if (false == _AddCtrlHandle(stCtrlHandle))
	{
		LOG("Handle %u Already Exist", stCtrlHandle.m_hNetHandle);
		return INVALID_WMHANDLE;
	}

	SNetRequest stReq;
	stReq.nType					= NETREQ_CONNECT;
	SConnReq& rstConnReq		= stReq.stUn.stConn;
	rstConnReq.m_hCtrlHandle	= stCtrlHandle.m_hNetHandle;
	rstConnReq.m_dwIP			= dwRemoteIP;
	rstConnReq.m_wPort			= wRemotePort;
	rstConnReq.m_dwRecvBufSize	= dwRecvBufSize;
	rstConnReq.m_dwSendBufSize	= dwSendBufSize;

	CNetReqMgr::Instance()->WriteNetReq(stReq);

	return stCtrlHandle.m_hNetHandle;
}

WMHANDLE WMAPI CNetControler::Listen( UINT32 dwLocalIP, UINT16 wLocalPort, IWMNetworkProcessor* poNetProcessor, bool bReuseAddr, UINT32 dwSendBufSize, UINT32 dwRecvBufSize )
{
	LOG("Listen at %s:%u", inet_ntoa((in_addr&)dwLocalIP), wLocalPort);

	INT32 hSocket = socket(AF_INET, SOCK_STREAM,  0);

	if (bReuseAddr)
	{
		INT32 nReuse = 1;
		setsockopt(hSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nReuse, sizeof(nReuse));
	}

	SNetRequest stReq;
	SStartListenReq& rstListenReq = stReq.stUn.stStartListen;
	
	SCtrlHandle stCtrlHandle = {0};
	sockaddr_in stAddr = {0};
	stAddr.sin_family = AF_INET;
	if (0 == dwLocalIP)
	{
		stAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		stAddr.sin_addr.s_addr = dwLocalIP;
	}

	stAddr.sin_port = htons(wLocalPort);
	
	if (bind(hSocket, (sockaddr*)&stAddr, sizeof(stAddr)) < 0)
	{
		LOG("Bind at %s:%d failed, errno %d, errmsg %s", inet_ntoa((in_addr&)dwLocalIP), wLocalPort, errno, strerror(errno));
		goto fail_ret;
	}

	if (listen(hSocket, 128) < 0)
	{
		LOG("Listen at %s:%d failed, errno %d, errmsg %s", inet_ntoa((in_addr&)dwLocalIP), wLocalPort, errno, strerror(errno));
		goto fail_ret;
	}

	stCtrlHandle.m_bInitial			= false;
	stCtrlHandle.m_hNetHandle		= _GetNextHandle();
	stCtrlHandle.m_dwIP				= dwLocalIP;
	stCtrlHandle.m_wPort			= wLocalPort;
	stCtrlHandle.m_poNetProcessor	= poNetProcessor;

	if (false == _AddCtrlHandle(stCtrlHandle))
	{
		LOG("Handle %u Already Exist", stCtrlHandle.m_hNetHandle);
		goto fail_ret;
	}

	stReq.nType						= NETREQ_START_LISTEN;
	rstListenReq.m_hCtrlHandle		= stCtrlHandle.m_hNetHandle;
	rstListenReq.m_nListenSock		= hSocket;
	rstListenReq.m_dwRecvBufSize	= dwRecvBufSize;
	rstListenReq.m_dwSendBufSize	= dwSendBufSize;

	CNetReqMgr::Instance()->WriteNetReq(stReq);

	return stCtrlHandle.m_hNetHandle;

fail_ret:
	if (hSocket != -1)
	{
		close(hSocket);
		hSocket = -1;
	}

	return INVALID_WMHANDLE;
}




