//#include "wmprotocol.h"
#include "netprocessor.h"
#include "netlink.h"
#include "wmpeer.h"

bool CNetProcessor::Send( UINT32 dwConnID, const CData* poPtcHead, const CProtocol& roProtocol ) const
{
	CWMPeer oPeer("", WM_NET_BASE, dwConnID, 0, poPtcHead);
	return m_poSender->Send(oPeer, roProtocol);
}

bool CNetProcessor::Send( UINT32 dwConnID, const CData* poPtcHead, const char* pcProtocol, INT32 nPtcLen ) const
{
	CWMPeer oPeer("", WM_NET_BASE, dwConnID, 0, poPtcHead);
	return m_poSender->Send(oPeer, pcProtocol, nPtcLen);
}

bool CNetProcessor::DecodeProtocolHead( const char* pcPkg, INT32 nPkgLen, CData& roPtcHead, INT32& nHeadLen ) const
{
	return m_poCoder->DecodeProtocolHead(pcPkg, nPkgLen, roPtcHead, nHeadLen);
}

bool CNetProcessor::Close( UINT32 dwConnID )
{
	CMapConnIDLink::iterator it = m_oMapConnIDLink.find(dwConnID);
	if (m_oMapConnIDLink.end() != it)
	{
		if (true == it->second.m_bInitial)
		{
			m_oMapHandleLink.erase(it->second.m_hLinkHandle);
		}
		
		m_oMapConnIDLink.erase(it);
		return m_poNetwork->Close(dwConnID);
	}
	else
	{
		return m_poNetwork->Close(dwConnID);
	}
}

void WMAPI CNetProcessor::OnConnect( WMHANDLE hNetHandle, UINT32 dwConnID, EWMErrCode nWMErrCode, const SWMUserData& rstHandledata )
{
	CMapHandleLink::iterator it = m_oMapHandleLink.find(hNetHandle);
	if (m_oMapHandleLink.end() != it)
	{
		if (true == it->second.m_bInitial)
		{
			m_poNetwork->CloseHandle(hNetHandle);
		}

		if (WM_SUCCESS == nWMErrCode)
		{
			it->second.m_hLinkHandle = hNetHandle;
			m_oMapConnIDLink.insert(std::make_pair(dwConnID, it->second));
			SNetLinkInfo* pstLinkInfo = _GetNetLinkInfo(dwConnID);
			pstLinkInfo->m_dwConnID = dwConnID;

			SWMUserData stUserdata;
			stUserdata.m_dwUserData = dwConnID;
			stUserdata.m_pUserPtr = pstLinkInfo;
			m_poNetwork->SetConnUserData(dwConnID, stUserdata);

			it->second.m_poNetLink->OnConnect(dwConnID);
			it->second.m_poNetLink->AddConnID(dwConnID);
		}
	}
}

void WMAPI CNetProcessor::OnRecv( UINT32 dwConnID, const char* pcPkg, INT32 nLen, const SWMUserData& rstConnData )
{
	SNetLinkInfo* pstLinkInfo = static_cast<SNetLinkInfo*>(rstConnData.m_pUserPtr);
	CWMNetLink* poNetLink = pstLinkInfo->m_poNetLink;

	INT32 nHeadLen = 0;
	CData* poProtocolHead = NULL;
	if (false == poNetLink->GetProtocolHead(pcPkg, nLen, &poProtocolHead, nHeadLen))
	{
		LOG("Get protocol head from ConnID %u failed", dwConnID);
		return;
	}

	if (NULL != poProtocolHead)
	{
		pcPkg += nHeadLen;
		nLen -= nHeadLen;
	}

	UINT32 dwType, dwSize;
	if (false == m_poCoder->GetProtocolAttr(pcPkg, nLen, dwType, dwSize))
	{
		return ;
	}

	CProtocol* poProtocol = CProtocol::GetProtocol(dwType);

	if (false == poProtocol->SizePolicy(dwSize))
	{
		return;
	}

	poNetLink->OnReceive(dwConnID, dwType, nLen);

	m_poCoder->Decode(pcPkg, nLen, poProtocol);

	CWMPeer oPeer("fff", pstLinkInfo->m_nPeerType, dwConnID, 0, poProtocolHead);
	poProtocol->Process(oPeer);
	return;
}

void WMAPI CNetProcessor::OnClose( UINT32 dwConnID, EWMErrCode nWMErrCode, const SWMUserData& rstConnData )
{

}
