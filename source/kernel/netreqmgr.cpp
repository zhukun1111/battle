#include "kernel/pch.h"
#include "kernel/netreqmgr.h"
#include "kernel/netevtmgr.h"

INSTANCE_SINGLETON(CNetReqMgr);


CNetReqMgr::CNetReqMgr( void )
{

}

CNetReqMgr::~CNetReqMgr( void )
{

}

bool CNetReqMgr::Init()
{
	if (false == m_oSendPipe.Init(WMNET_REQ_PIPE_SIZE))
	{
		LOG("Init SendPipe failed");
		return false;
	}

	return true;
}

void CNetReqMgr::WriteNetReq( SNetRequest& rstReq )
{
	while (false == m_oSendPipe.Write((char*)&rstReq, sizeof(rstReq)))
	{
		LOG("Write Req to SendPipe Full, Sleep and try again");
#ifdef WIN32
		Sleep(10);
#else
		usleep(10);
#endif
	}
}


void CNetReqMgr::WriteSendData( UINT32 dwConnID, const char* pcData, INT32 nLen )
{
	SNetRequest stReq;
	stReq.nType = NETREQ_SEND;
	stReq.stUn.stSend.m_dwConnID	= dwConnID;
	stReq.stUn.stSend.m_nLen		= nLen;
	stReq.stUn.stSend.m_pData		= NULL;

	WriteNetReq(stReq);

	while(false == m_oSendPipe.Write(pcData, nLen))
	{
		LOG("Write Data to SendPipe failed, sleep and try again");
#ifdef WIN32
		Sleep(10);
#else
		usleep(10);
#endif
	}
}
