#ifndef _H_NETLINK_H_
#define _H_NETLINK_H_

#include "wmdefine.h"

class CData;
class CNetProcessor;
class IWMNetWork;
class CProtocol;

typedef std::set<UINT32> CSetConnID;

class CWMNetLink
{
public:
	CWMNetLink(void) : m_poNetProcessor(NULL), m_poNetwork(NULL)
	{
	}
	virtual ~CWMNetLink(){}

	virtual void OnConnect(UINT32 dwConnID) = 0;
	virtual void OnClose(UINT32 dwConnID) = 0;
	virtual void OnReceive(UINT32 dwConnID, UINT32 dwPtcType, INT32 nPtcLen) = 0;

	virtual EWMPeerType GetPeerType() const = 0;
	bool Close(UINT32 dwConnID);

	void AddConnID(UINT32 dwConnID)
	{
		m_oSetConnID.insert(dwConnID);
	}

	void DelConnID(UINT32 dwConnID)
	{
		m_oSetConnID.erase(dwConnID);
	}

	const CSetConnID& GetConnIDSet() const { return m_oSetConnID; }

	bool GetLocalAddr(UINT32 dwConnID, UINT32& rdwIP, UINT16& rwPort);
	bool GetRemoteAddr(UINT32 dwConnID, UINT32& rdwIP, UINT16& rwPort);

	bool Init(CNetProcessor* poNetProcessor, IWMNetWork* poNetwork);

	virtual bool GetProtocolHead(const char* pcPkg, INT32 nPkgLen, CData** ppoPtcHead, INT32& nHeadLen) const
	{
		WMASSERT(NULL != ppoPtcHead);

		*ppoPtcHead = NULL;
		nHeadLen = 0;
		return true;
	}

protected:
	CNetProcessor*	m_poNetProcessor;
	IWMNetWork*		m_poNetwork;

	CSetConnID		m_oSetConnID;

protected:
	bool _Send(UINT32 dwConnID, const CData* poPtcHead, const CProtocol& roProtocol) const;
	bool _Send(UINT32 dwConnID, const CData* poPtcHead, const char* pcProtocol, INT32 nPtcLen) const;
	bool _DecodeProtocolHead(const char* pcPkg, INT32 nPkgLen, CData& roPtcHead, INT32& nHeadLen) const;
	bool _CallRpc(UINT32 dwConnID, UINT32 dwRpcType, const CData* poPtcHead, const CData& roArgument, UINT32* pdwRpcID = NULL);
};

#endif