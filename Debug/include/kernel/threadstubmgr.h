#ifndef _H_THREADSTUBMGR_H
#define _H_THREADSTUBMGR_H

#include "threadstub.h"

typedef std::map<WMHANDLE, CThreadStub*> CThreadStubMap;

class CThreadStubMgr : public IWMThreadCtrl
{
public:
	CThreadStubMgr();
	virtual ~CThreadStubMgr();

	virtual WMHANDLE WMAPI Begin(IWMThread* poThread);
	virtual bool WMAPI Kill(WMHANDLE hThread);
	virtual bool WMAPI Suspend(WMHANDLE hThread);
	virtual bool WMAPI Resume(WMHANDLE hThread);
	virtual bool WMAPI WaitFor(WMHANDLE hThread, UINT32 dwTime);
	virtual bool WMAPI CloseHandle(WMHANDLE hTHread);
	virtual UINT32 WMAPI GetThreadID(WMHANDLE hThread);
	virtual void WMAPI Release();

	void AddRef();

protected:
	CThreadStub* _FindStub(WMHANDLE hThread);

protected:
	INT32			m_nRef;
	WMHANDLE		m_hNextHandle;
	CThreadStubMap	m_oStubMap;
};

#endif