#ifndef _H_WMTHREAD_H
#define _H_WMTHREAD_H

#include "kernel/wmdefine.h"

const UINT32 WM_THREAD_VERSION = 1;

#define WM_INFINITE 0xffffffff

class IWMThread
{
public:
	virtual WMAPI ~IWMThread() {}
	virtual void WMAPI Run() = 0;
};

class IWMThreadCtrl
{
public:
	virtual WMAPI ~IWMThreadCtrl() {}
	virtual WMHANDLE WMAPI Begin(IWMThread* poThread) = 0;
	virtual bool WMAPI Kill(WMHANDLE hThread) = 0;
	virtual bool WMAPI Suspend(WMHANDLE hThread) = 0;
	virtual bool WMAPI Resume(WMHANDLE hThread) = 0;
	virtual bool WMAPI WaitFor(WMHANDLE hThread, UINT32 dwTime = WM_INFINITE) = 0;
	virtual bool WMAPI CloseHandle(WMHANDLE hTHread) = 0;
	virtual UINT32 WMAPI GetThreadID(WMHANDLE hThread) = 0;
	virtual void WMAPI Release() = 0;
};

IWMThreadCtrl* WMAPI WM_GetThreadCtrl(UINT32 dwVersion = WM_THREAD_VERSION);
typedef IWMThreadCtrl* (WMAPI *PFN_WM_GetThreadCtrl)(UINT32 dwVersion);


#endif