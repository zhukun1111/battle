#ifndef _H_WMPEER_H_
#define _H_WMPEER_H_

#include "wmdefine.h"
#include "wminterface.h"
#include "data.h"

class CWMPeer : public IWMPeer
{
public:
	CWMPeer(const char* pszPeerName, EWMPeerType nPeerType, UINT32 dwPeerID, UINT32 dwLine = 0, const CData* poPtcHead = NULL)
		:m_strPeerName(pszPeerName), m_nPeerType(nPeerType), m_dwPeerID(dwPeerID), m_dwLine(dwLine), m_poPtcHead(poPtcHead)
	  {}

	  virtual void Destroy()
	  {
		  _GetPeerList().push_back(this);
	  }

	  virtual IWMPeer* Clone(void) const
	  {
		  CListPeer& roListPeer = _GetPeerList();
		  
		  if (true == roListPeer.empty())
		  {
			  return new CWMPeer(this);
		  }
		  else
		  {
			  CWMPeer* poPeer = *(roListPeer.begin());
			  roListPeer.erase(roListPeer.begin());
			  *poPeer = *this;
			  return poPeer;
		  }
	  }

	  EWMPeerType GetPeerType() const
	  {
		  return m_nPeerType;
	  }

	  UINT32 GetPeerID() const
	  {
		  return m_dwPeerID;
	  }

	  UINT32 GetLine() const
	  {
		  WMASSERT(m_nPeerType > WM_BUS_BASE);
		  return m_dwLine; 
	  }

	  const char* GetPeerName() const
	  {
		  return m_strPeerName.c_str();
	  }

	  const CData* GetProtocolHead() const
	  {
		  return m_poPtcHead;
	  }

	  CData* GetProtocolHead()
	  {
		  return m_poPtcHead;
	  }



protected:
	std::string		m_strPeerName;
	EWMPeerType		m_nPeerType;

	UINT32			m_dwPeerID;
	UINT32			m_dwLine;

	const CData*	m_poPtcHead;

	typedef std::list<CWMPeer*> CListPeer;

	static CListPeer& _GetPeerList()
	{
		static CListPeer soListPeer;
		return soListPeer;
	}
};

#endif