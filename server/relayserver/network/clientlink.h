#ifndef _H_CLITENT_H_
#define _H_CLITENT_H_

#include "protocol/protocolheader.h"
#include "kernel/netlink.h"

class CClientLink : public CWMNetLink
{
	DECLARE_SINGLETON(CClientLink);
private:
	CClientLink(){}

	virtual ~CClientLink(){}

public:
	virtual void OnConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID);
	virtual void OnReceive(UINT32 dwConnID, UINT32 dwPtcType, INT32 nPtcLen) const;
	virtual void OnSend(UINT32 dwConnID, UINT32 dwPtcType, INT32 nPtcLen) const;

	virtual EWMPeerType GetPeerType() const 
	{
		return WM_NET_CLIENT;
	}

	bool Send(UINT32 dwConnID, const CProtocol& roProtocol) const
	{
		return _Send(dwConnID, NULL, roProtocol);
	}

	bool SendAll(const CProtocol& roProtocol) const
	{
		bool bError = true;
		CSetConnID::const_iterator iter;

		for (iter = m_oSetConnID.begin(); iter != m_oSetConnID.end(); ++iter)
		{
			bError = bError && _Send(*iter, NULL, roProtocol);
		}

		return bError;
	}
};
#endif