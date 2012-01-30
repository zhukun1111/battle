#include "pch.h"
#include "networkthrd.h"

INSTANCE_SINGLETON(CNetworkThrd);

CNetworkThrd::CNetworkThrd( void )
{
	m_bTerminate	= false;
	m_poThrdCtrl	= NULL;
	m_hThread		= INVALID_WMHANDLE;
}

CNetworkThrd::~CNetworkThrd( void )
{

}

void WMAPI CNetworkThrd::Run()
{
	_MainLoop();
	_Uninit();
}

bool CNetworkThrd::Start()
{
	if (false == _Init())
	{
		return false;
	}

	WMASSERT(NULL == m_poThrdCtrl);
	m_poThrdCtrl = WM_GetThreadCtrl();
	if (m_poThrdCtrl == NULL)
	{
		LOG("Start GetThredCtrl failed");
		return false;
	}

	m_hThread = m_poThrdCtrl->Begin(this);
	if (INVALID_WMHANDLE == m_hThread)
	{
		LOG("m_poThrdCtrl->Begin failed");
		return false;
	}

	return true;
}

void CNetworkThrd::Stop()
{
	WMASSERT(m_poThrdCtrl != NULL);
	m_bTerminate = true;
	m_poThrdCtrl->WaitFor(m_hThread, 1000);

	m_poThrdCtrl->Release();
	m_poThrdCtrl = NULL;
}

bool CNetworkThrd::_Init()
{

	return true;
}

void CNetworkThrd::_Uninit()
{

}

void CNetworkThrd::_MainLoop()
{
	while (!m_bTerminate)
	{
		if (false == _ProcNetwork())
		{
			break;
		}

		_ProcRequest();
	}
}

bool CNetworkThrd::_ProcNetwork()
{

	return true;
}

void CNetworkThrd::_ProcRequest()
{
	SNetRequest stReq;
	while(CNetReqMgr::Instance()->ReadNetReq(stReq))
	{
		switch (stReq.nType)
		{
		case NETREQ_SEND:
			_ProcSendReq(&stReq.stUn.stSend);
			break;
		case NETREQ_CONNECT:
			_ProcConnReq(&stReq.stUn.stConn);
			break;
		case NETREQ_START_LISTEN:
			_ProcStartListenReq(&stReq.stUn.stStartListen);
			break;
		case NETREQ_STOP_LISTEN:
			_ProcStopListenReq(&stReq.stUn.stStopListen);
			break;
		default:
			WMASSERT(false);
			break;
		}
	}
}

void CNetworkThrd::_ProcStartListenReq( SStartListenReq* pstReq )
{

}

void CNetworkThrd::_ProcStopListenReq( SStopListenReq* pstReq )
{

}

void CNetworkThrd::_ProcConnReq( SConnReq* pstReq )
{

}

void CNetworkThrd::_ProcSendReq( SSendReq* pstReq )
{

}


