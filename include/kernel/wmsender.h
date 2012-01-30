#ifndef _H_WMSENDER_H_
#define _H_WMSENDER_H_

#include "wminterface.h"

const UINT32 WM_DEFAULT_SENDBUF_SIZE = 1 * 1024 * 1024;

class CWMCoder;
class IWMNetWork;
class CProtocol;
class IWMPeer;

class CWMSender : public IWMSender
{
public:

	explicit CWMSender(CWMCoder* poCoder, IWMNetWork* poNetwork, UINT32 dwSendSize = WM_DEFAULT_SENDBUF_SIZE):
	m_poCoder(poCoder), m_poNetwork(poNetwork), m_nSize(dwSendSize) 
	{
		WMASSERT(NULL != poCoder);
		WMASSERT(NULL != poNetwork);
		m_pSendBuf = new char[m_nSize];
		WMASSERT(m_pSendBuf != NULL);
	}

	CWMSender(const CWMSender& roSender):
	m_poCoder(roSender.m_poCoder), m_poNetwork(roSender.m_poNetwork), m_nSize(roSender.m_nSize) 
	{
		WMASSERT(NULL != poCoder);
		WMASSERT(NULL != poNetwork);
		m_pSendBuf = new char[m_nSize];
		WMASSERT(m_pSendBuf != NULL);
	}

	CWMSender& operator= (const CWMSender& roSender)
	{
		if (this != &roSender)
		{
			if (m_nSize != roSender.m_nSize)
			{
				delete[] m_pSendBuf;
				m_nSize = roSender.m_nSize;
				m_pSendBuf = new char[m_nSize];
				WMASSERT(NULL != m_pSendBuf);
			}

			m_poCoder = roSender.m_poCoder;
			m_poNetwork = roSender.m_poNetwork;
		}

		return this;
	}

	virtual ~CWMSender()
	{
		delete[] m_pSendBuf;
	}

	virtual bool Send(const IWMPeer& roWMPeer, const CProtocol& roProtocol);
	virtual bool Send(const IWMPeer& roWMPeer, const char* pcProtocol, INT32 nPtcLen);

protected:
	CWMCoder*		m_poCoder;
	IWMNetWork*		m_poNetwork;

	char*			m_pSendBuf;
	INT32			m_nSize;
};

#endif