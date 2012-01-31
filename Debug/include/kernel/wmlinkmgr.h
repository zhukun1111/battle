#ifndef _H_WMLINKMGR_H_
#define _H_WMLINKMGR_H_

#include "wmdefine.h"
#include "netprocessor.h"

template<typename LinkType> inline
EWMErrCode WM_NetConnect(CNetProcessor* poNetProcessor, IWMNetWork* poNetwork, INT32 nReconnCount = 0)
{
	if (NULL == poNetProcessor || NULL == poNetwork)
	{
		return WM_WRONG_PARAM;
	}

	if (false == LinkType::CreateInstance())
	{
		return WM_UNKNOWN_ERR;
	}

	if (false == LinkType::Instance()->Init(poNetProcessor, poNetwork))
	{
		return WM_UNKNOWN_ERR;
	}

	EWMPeerType nPeerType = LinkType::Instance()->GetPeerType();
	CWMConfigure* poConfigure = CWMConfigure::Instance();
	const SPeerInfo* pstPeerInfo = poConfigure->GetNextPeer(nPeerType);

	do 
	{
		UINT32 dwSendBufSize = pstPeerInfo->m_dwSendBufSize == 0 ? WM_DEFAULT_SENDBUF_SIZE : pstPeerInfo->m_dwSendBufSize;
		UINT32 dwRecvBufSize = pstPeerInfo->m_dwRecvBufSize == 0 ? WM_DEFAULT_RECVBUF_SIZE : pstPeerInfo->m_dwRecvBufSize;

		WMHANDLE hNetHandle = poNetwork->Connect(pstPeerInfo->m_dwIP, pstPeerInfo->m_wPort, poNetProcessor, dwSendBufSize, dwRecvBufSize);
		if (INVALID_WMHANDLE == hNetHandle)
		{
			return WM_UNKNOWN_ERR;
		}

		SNetLinkInfo stLinkInfo(nPeerType, NULL, LinkType::Instance(), true);
		stLinkInfo.m_dwRecvBufSize = dwRecvBufSize;
		stLinkInfo.m_dwSendBufSize = dwSendBufSize;

		poNetProcessor->Attach(hNetHandle, stLinkInfo);

	} while (NULL != (pstPeerInfo = poConfigure->GetNextPeer(nPeerType)));

	poConfigure->EndGetPeer();
	return WM_SUCCESS;
}


template<typename LinkType> inline
EWMErrCode WM_NetListen(CNetProcessor* poNetProcessor, IWMNetWork* poNetwork)
{
	if (NULL == poNetProcessor || NULL == poNetwork)
	{
		return WM_WRONG_PARAM;
	}

	if (false == LinkType::CreateInstance())
	{
		return WM_UNKNOWN_ERR;
	}

	if (false == LinkType::Instance()->Init(poNetProcessor, poNetwork))
	{
		return WM_UNKNOWN_ERR;
	}

	EWMPeerType nPeerType = LinkType::Instance()->GetPeerType();
	CWMConfigure* poConfigure = CWMConfigure::Instance();
	const SSockInfo* pstSockInfo = poConfigure->GetListenSock(nPeerType);


	UINT32 dwSendBufSize = pstPeerInfo->m_dwSendBufSize == 0 ? WM_DEFAULT_SENDBUF_SIZE : pstPeerInfo->m_dwSendBufSize;
	UINT32 dwRecvBufSize = pstPeerInfo->m_dwRecvBufSize == 0 ? WM_DEFAULT_RECVBUF_SIZE : pstPeerInfo->m_dwRecvBufSize;

	WMHANDLE hNetHandle = poNetwork->Listen(pstSockInfo->m_dwIP, pstSockInfo->m_wPort, poNetProcessor, true, dwSendBufSize, dwRecvBufSize);
	if (INVALID_WMHANDLE == hNetHandle)
	{
		return WM_UNKNOWN_ERR;
	}

	SNetLinkInfo stLinkInfo(nPeerType, NULL, LinkType::Instance(), true);
	poNetProcessor->Attach(hNetHandle, stLinkInfo);

	return WM_SUCCESS;
}


#endif