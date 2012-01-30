#ifndef _H_NETWORKTHRD_H_
#define _H_NETWORKTHRD_H_
#include "singleton.h"
#include "wmthread.h"
#include "netreqmgr.h"


class CNetworkThrd : public IWMThread
{
	CNetworkThrd(void);
	virtual ~CNetworkThrd(void);
public:
	DECLARE_SINGLETON(CNetworkThrd)

	virtual void WMAPI Run();

	bool Start();
	void Stop();

protected:
	bool _Init();
	void _MainLoop();
	void _Uninit();

	bool _ProcNetwork();
	void _ProcRequest();

	void _ProcStartListenReq(SStartListenReq* pstReq);
	void _ProcStopListenReq(SStopListenReq* pstReq);
	void _ProcConnReq(SConnReq* pstReq);
	void _ProcSendReq(SSendReq* pstReq);

protected:
	bool				m_bTerminate;
	WMHANDLE			m_hThread;
	IWMThreadCtrl*		m_poThrdCtrl;

	struct event_base*	m_poEventBase;
	
};

#endif