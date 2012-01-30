#ifndef _H_PTCC2RREQ_LOGIN_H_
#define _H_PTCC2RREQ_LOGIN_H_

#include "protocol/wmprotocol.h"
#include "kernel/wmpeer.h"

#define PTCC2RREQ_LOGIN_NULL_IMPLEMENTATION \ 
	void PtcC2RReq_Login::Process(UINT32 dwConnID){}

const UINT32 TYPE_PTCC2RREQ_LOGIN = 1000;

class PtcC2RReq_Login : public CProtocol
{
public:
	UINT8 m_btSlot;

public:
	PtcC2RReq_Login(void) : CProtocol(TYPE_PTCC2RREQ_LOGIN) {}

	explicit PtcC2RReq_Login(UINT8 btSlot) : CProtocol(TYPE_PTCC2RREQ_LOGIN),
		m_btSlot(btSlot)
	{

	}

	PtcC2RReq_Login(const PtcC2RReq_Login& rhs) : CProtocol(rhs),
		m_btSlot(rhs.m_btSlot)
	{

	}

	virtual CProtocol* Clone(void) const { return new PtcC2RReq_Login(*this);}

	virtual bool SizePolicy(size_t nsize) { return  nsize <= (UINT32)1; }

	virtual void Process(const IWMPeer& roWMPeer)
	{
		const char* pszLocalName = CWMConfigure::Instance()->GetLocalName();
		const CWMPeer& roPeer = dynamic_cast<const CWMPeer&>(roWMPeer);
		Process(roPeer.GetPeerID());
	}

	void Process(UINT32 dwConnID);

protected:
	virtual OctetsStream& _Marshal(OctetsStream& os) const
	{
		os << m_btSlot;

		return os;
	}

	virtual const OctetsStream& _Unmarshal(const OctetsStream& os)
	{
		os >> m_btSlot;

		return os;
	}

};
#endif