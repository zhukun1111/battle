#include "protocol/wmprotocol.h"
#include "protocol/wmcoder.h"
#include "kernel/wmnetwork.h"
#include "kernel/wmpeer.h"
#include "kernel/wmsender.h"


bool CWMSender::Send( const IWMPeer& roWMPeer, const CProtocol& roProtocol )
{
	WMASSERT(NULL != m_poCoder);
	INT32 nPkgLen = m_nSize;

	const CWMPeer& roPeer = static_cast<const CWMPeer&>(roWMPeer);
	if (roPeer.GetPeerType() >= WM_BUS_BASE)
	{
		if (false == m_poCoder->Encode(roPeer.GetProtocolHead(), &roProtocol, m_pSendBuf, &nPkgLen))
		{
			return false;
		}

		//WMASSERT(NULL != m_poBus);
		//return m_poBus->send(roPeer.GetPeerID(), m_pSendBuf, nPkgLen);
	}
	else
	{
		if (false == m_poCoder->Encode(roPeer.GetProtocolHead(), &roProtocol, m_pSendBuf, &nPkgLen))
		{
			return false;
		}

		WMASSERT(NULL != m_poNetwork);
		return m_poNetwork->Send(roPeer.GetPeerID(), m_pSendBuf, nPkgLen);
	}
}

bool CWMSender::Send( const IWMPeer& roWMPeer, const char* pcProtocol, INT32 nPtcLen )
{
	WMASSERT(NULL != m_poCoder);
	INT32 nPkgLen = m_nSize;

	const CWMPeer& roPeer = static_cast<const CWMPeer&>(roWMPeer);
	if (roPeer.GetPeerType() >= WM_BUS_BASE)
	{
		if (false == m_poCoder->Encode(roPeer.GetProtocolHead(),pcProtocol, nPtcLen,  m_pSendBuf, &nPkgLen))
		{
			return false;
		}

		//WMASSERT(NULL != m_poBus);
		//return m_poBus->send(roPeer.GetPeerID(), m_pSendBuf, nPkgLen);
	}
	else
	{
		if (false == m_poCoder->Encode(roPeer.GetProtocolHead(), pcProtocol, nPtcLen,  m_pSendBuf, &nPkgLen))
		{
			return false;
		}

		WMASSERT(NULL != m_poNetwork);
		return m_poNetwork->Send(roPeer.GetPeerID(), m_pSendBuf, nPkgLen);
	}
}
