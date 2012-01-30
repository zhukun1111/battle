#ifndef _H_NETPROCESSOR_H_
#define _H_NETPROCESSOR_H_

#include "wmdefine.h"
#include "wmnetwork.h"

class CData;
class CProtocol;
class IWMNetWork;
class CWMNetLink;

struct SNetLinkInfo
{
	SNetLinkInfo(EWMPeerType nPeerType, const CWMNetLink* poNetLink) : m_nPeerType(nPeerType), m_poNetLink(poNetLink),
		m_hLinkHandle(0), m_dwNetID(0), m_dwConnID(0), m_dwRecvProtocol(0)
	{

	}

	EWMPeerType			m_nPeerType;
	CWMNetLink*			m_poNetLink;
	WMHANDLE			m_hLinkHandle;
	UINT32				m_dwNetID;
	UINT32				m_dwConnID;
	UINT32				m_dwRecvProtocol;

	UINT32				m_dwSendBufSize;
	UINT32				m_dwRecvBufSize;

	bool operator < (const SNetLinkInfo& rhs) const{ return m_dwNetID < rhs.m_dwNetID; }
};

class CNetProcessor : public IWMNetworkProcessor
{
public:
	CNetProcessor(IWMNetWork* poNetwork): m_poNetwork(poNetwork)
	{
		WMASSERT(NULL != poNetwork);
	}

	CNetProcessor(const CNetProcessor& roNetProcessor) : m_poNetwork(roNetProcessor.m_poNetwork){}
	virtual ~CNetProcessor() {}

	void Attach(WMHANDLE hNetHandle, const SNetLinkInfo& rstLinkInfo)
	{
		SNetLinkInfo rstLinkInfoCopy(rstLinkInfo);

		if (0 == rstLinkInfoCopy.m_dwNetID)
		{
			rstLinkInfoCopy.m_dwNetID = _GetNextNetID();
		}

		CMapHandleLink::iterator it = m_oMapHandleLink.find(hNetHandle);
		if (m_oMapHandleLink.end() == it)
		{
			rstLinkInfoCopy.m_dwNetID = hNetHandle;
			m_oMapHandleLink.insert(std::make_pair(hNetHandle, rstLinkInfoCopy));
		}
		else
		{
			it->second = rstLinkInfoCopy;
		}
	}

	bool Send(UINT32 dwConnID, const CData* poPtcHead, const CProtocol& roProtocol) const;
	bool Send(UINT32 dwConnID, const CData* poPtcHead, const char* pcProtocol, INT32 nPtcLen) const;

	bool DecodeProtocolHead(const char* pcPkg, INT32 nPkgLen, CData& roPtcHead, INT32& nHeadLen) const;

	bool Close(UINT32 UINT32 dwConnID);

	virtual void WMAPI OnConnect(WMHANDLE hNetHandle, UINT32 dwConnID, EWMErrCode nWMErrCode, const SWMUserData& rstHandledata);
	virtual void WMAPI OnClose(UINT32 dwConnID, EWMErrCode nWMErrCode, const SWMUserData& rstConnData);
	virtual void WMAPI OnRecv(UINT32 dwConnID, const char* pcPkg, INT32 nLen, const SWMUserData& rstConnData);

	UINT32 _GetNextNetID()
	{
		return ++m_dwNextNetID;
	}

protected:
	IWMNetWork*			m_poNetwork;
	CWMCoder*			m_poCoder;
	IWMSender*			m_poSender;

	typedef std::map<WMHANDLE, SNetLinkInfo> CMapHandleLink;
	typedef std::map<UINT32, SNetLinkInfo> CMapConnIDLink;

	CMapHandleLink			m_oMapHandleLink;
	CMapConnIDLink			m_oMapConnIDLink;

	UINT32					m_dwNextNetID;
protected:
	SNetLinkInfo* _GetNetLinkInfo(UINT32 dwConnID)
	{
		CMapConnIDLink::iterator it = m_oMapConnIDLink.find(dwConnID);
		if (m_oMapConnIDLink.end() == it)
		{
			return NULL;
		}

		return &it->second;
	}
};

#endif