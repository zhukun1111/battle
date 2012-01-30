#include "pch.h"
#include "threadstub.h"
#include "wmthread.h"
#ifdef WIN32
#include <process.h>
#endif


CThreadStub::CThreadStub()
{
	m_hHandle = -1;
	m_dwThreadId = 0;
	m_poThread = NULL;
}

CThreadStub::~CThreadStub()
{
	Kill();
}

bool CThreadStub::Begin( WMHANDLE hHandle, IWMThread* poThread )
{
	if (m_dwThreadId != 0)
	{
		return false;
	}

	m_poThread	= poThread;

#ifdef WIN32
	m_dwThreadId = (HANDLE)_beginthreadex(NULL, 0, _StaticThrdFunc, (void*)this, 0, 0);
	
	if (NULL == m_dwThreadId)
	{
		return false;
	}

#else

	if (0 != pthread_create(&m_dwThreadId, NULL, (void *(*)(void *))_StaticThrdFunc, this))
	{
		return false;
	}
#endif

	return true;
}

UINT32 WMAPI CThreadStub::_StaticThrdFunc( void* pArg )
{
	CThreadStub* poStub = (CThreadStub*)pArg;

#ifdef WIN32
	UINT32 dwRet = 0;
	__try
	{
		poStub->m_poThread->Run();
	}
	__except(0)
	{
		dwRet = 1;
	}

	return dwRet;
#else

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

	sigset_t new_set, old_set;
	sigemptyset(&new_set);
	sigemptyset(&old_set);
	sigaddset(&new_set, SIGHUP);
	sigaddset(&new_set, SIGINT);
	sigaddset(&new_set, SIGQUIT);
	sigaddset(&new_set, SIGTERM);
	sigaddset(&new_set, SIGUSR1);
	sigaddset(&new_set, SIGUSR2);
	sigaddset(&new_set, SIGPIPE);
	
	ptread_sigmask(SIG_BLOCK, &new_set, &old_set);

	poStub->m_poThread->Run();
	return NULL;
#endif
}

bool CThreadStub::Kill()
{
	if (0 == m_dwThreadId)
	{
		return false;
	}

#ifdef WIN32
	TerminateThread(m_dwThreadId, -1);
#else
	pthread_cancel(m_dwThreadId);
#endif
}

bool CThreadStub::Suspend()
{
	return false;
}

bool CThreadStub::Resume()
{
	return false;
}

bool CThreadStub::WaitFor()
{
	if (0 == m_dwThreadId)
	{
		return false;
	}

#ifdef WIN32
	if (WaitForSingleObject(m_dwThreadId, 3000) == WAIT_OBJECT_0)
	{
		m_dwThreadId = 0;
		return true;
	}
	else
	{
		m_dwThreadId = 0;
		return false;
	}
#else
	pthread_join(m_dwThreadId, NULL);
	m_dwThreadId = 0;
	return true;
#endif
}

UINT32 CThreadStub::GetThreadID()
{
	return (UINT32)m_dwThreadId;
}


