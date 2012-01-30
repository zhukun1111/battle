#ifndef _H_WMINTERFACE_H_
#define _H_WMINTERFACE_H_

class IWMPeer
{
public:
	virtual ~IWMPeer(void){}
	virtual void Destroy()
	{
		delete this;
	}

	virtual IWMPeer* Clone(void) const = 0;
};

class CProtocol;
class IWMSender
{
public:
	virtual ~IWMSender(){}
	virtual bool Send(const IWMPeer& roWMPeer, const CProtocol& roProtocol) = 0;
	virtual bool Send(const IWMPeer& roWMPeer, UINT32 dwPtcType, const char* pcProtocol, INT32 nPtcLen) = 0;
};
#endif 