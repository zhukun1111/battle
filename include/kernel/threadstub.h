#ifndef _H_THREADSTUB_H
#define _H_THREADSTUB_H
#include "wmsystem.h"

class IWMThread;
class CThreadStub
{
public:
	CThreadStub();
	virtual ~CThreadStub();

	bool Begin(WMHANDLE hHandle, IWMThread* poThread);
	bool Kill();
	bool Suspend();
	bool Resume();
	bool WaitFor();
	UINT32 GetThreadID();

protected:
	static UINT32 WMAPI _StaticThrdFunc(void* pArg);

protected:
	WMHANDLE	m_hHandle;
	WM_THREADHANDLE m_dwThreadId;
	IWMThread*	m_poThread;

};

#endif