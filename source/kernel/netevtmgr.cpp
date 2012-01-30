#include "kernel/pch.h"
#include "kernel/netreqmgr.h"
#include "kernel/netevtmgr.h"

INSTANCE_SINGLETON(CNetEvtMgr);

CNetEvtMgr::CNetEvtMgr( void )
{

}

CNetEvtMgr::~CNetEvtMgr( void )
{

}


bool CNetEvtMgr::Init()
{
	if (false == m_oRecvPipe.Init(WMNET_EVT_PIPE_SIZE))
	{
		LOG("Init Recv Pipe failed");
		return false;
	}

	return true;
}

void CNetEvtMgr::PushRecvEvt( UINT32 dwConnID, const char* pcData, INT32 nLen )
{
	SNetEvent stEvent;

	stEvent.m_nType		= NETEVT_RECV;
	stEvent.stUn.m_stRecv.m_dwConnID = dwConnID;
	stEvent.stUn.m_stRecv.m_nLen = nLen;
	stEvent.stUn.m_stRecv.m_pData = NULL;

	_WriteNetEvt(stEvent);

	while (false == m_oRecvPipe.Write(pcData, nLen))
	{
		LOG("Write Data to RecvPipe failed, sleep and try again");
		CNetReqMgr::Instance()->ReadReqToBuffer();
#ifdef WIN32
		Sleep(10);
#else
		usleep(10);
#endif
	}
}

void CNetEvtMgr::_WriteNetEvt( SNetEvent& rstEvt )
{
	while (false == m_oRecvPipe.Write((char*)&rstEvt, sizeof(rstEvt)))
	{
		LOG("Write Evt to RecvPipe failed, sleep and try again");
		CNetReqMgr::Instance()->ReadReqToBuffer();
#ifdef WIN32
		Sleep(10);
#else
		usleep(10);
#endif
	}
}
