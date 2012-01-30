#ifndef _H_WMSYSTEM_H
#define  _H_WMSYSTEM_H

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "kernel/wmdefine.h"
#include "kernel/wmthread.h"
#include "kernel/wmlock.h"
#include "kernel/wmtime.h"
#include "kernel/tickcount.h"

#ifdef WIN32
	typedef DWORD WM_THREADID;
	typedef HANDLE WM_THREADHANDLE;
	#define WM_GetCurrentThreadId GetCurrentThreadId
	#define WM_ThreadEqual(srcId, dstId) ((DWORD)srcId == (DWORD)dstId)
#else
	typedef pthread_t WM_THREADID;
	typedef pthread_t WM_THREADHANDLE;
	#define WM_GetCurrentThreadId pthread_self
	#define WM_ThreadEqual(srcId, dstId) (pthread_equal(srcId, dstId))
#endif

struct STimeVal
{
	UINT32 m_dwSec;
	UINT32 m_dwMilisec;
};

//void WMAPI WM_GetTickCount(CTickCount& roTickCount);
#endif