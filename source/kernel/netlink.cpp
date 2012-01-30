#include "kernel/wmnetwork.h"
#include "protocol/wmprotocol.h"
#include "protocol/wmrpc.h"
#include "netlink.h"
#include "netprocessor.h"
#include "wmpeer.h"

bool CWMNetLink::Close( UINT32 dwConnID )
{
	m_oSetConnID.erase(dwConnID);
	return m_poNetProcessor->Close(dwConnID);
}

bool CWMNetLink::GetLocalAddr( UINT32 dwConnID, UINT32& rdwIP, UINT16& rwPort )
{
	return m_poNetwork->GetConnLocalAddr(dwConnID, rdwIP, rwPort);
}

bool CWMNetLink::GetRemoteAddr( UINT32 dwConnID, UINT32& rdwIP, UINT16& rwPort )
{
	return m_poNetwork->GetConnRemoteAddr(dwConnID, rdwIP, rwPort);
}

bool CWMNetLink::Init( CNetProcessor* poNetProcessor, IWMNetWork* poNetwork )
{
	m_poNetProcessor = poNetProcessor;
	m_poNetwork = poNetwork;
}

bool CWMNetLink::_Send( UINT32 dwConnID, const CData* poPtcHead, const CProtocol& roProtocol ) const
{
	return  m_poNetProcessor->Send(dwConnID, poPtcHead, roProtocol);
}

bool CWMNetLink::_Send( UINT32 dwConnID, const CData* poPtcHead, const char* pcProtocol, INT32 nPtcLen ) const
{
	return m_poNetProcessor->Send(dwConnID, poPtcHead, pcProtocol, nPtcLen);
}

bool CWMNetLink::_DecodeProtocolHead( const char* pcPkg, INT32 nPkgLen, CData& roPtcHead, INT32& nHeadLen ) const
{
	return m_poNetProcessor->DecodeProtocolHead(pcPkg, nPkgLen, roPtcHead, nHeadLen);
}

bool CWMNetLink::_CallRpc( UINT32 dwConnID, UINT32 dwRpcType, const CData* poPtcHead, const CData& roArgument, UINT32* pdwRpcID /*= NULL*/ )
{
	CWMPeer oPeer("", WM_NET_BASE, dwConnID, 0, poPtcHead);
	//return CRpc::CallRpc(oPeer, dwRpcType, roArgument, pdwRpcID)
}
