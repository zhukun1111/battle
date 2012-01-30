#include "pch.h"
#include "kernel/threadstubmgr.h"

CThreadStubMgr* g_poThreadStubMgr = NULL;

IWMThreadCtrl* WMAPI WM_GetThreadStubMgr(UINT32 dwVersion)
{
	if (NULL == g_poThreadStubMgr)
	{
		g_poThreadStubMgr = new CThreadStubMgr;
	}

	if (g_poThreadStubMgr != NULL)
	{
		g_poThreadStubMgr->AddRef();
	}

	return static_cast<IWMThreadCtrl*>(g_poThreadStubMgr);
}

WMHANDLE WMAPI CThreadStubMgr::Begin( IWMThread* poThread )
{
	++m_hNextHandle;

	CThreadStub* poStub = new CThreadStub;
	if (NULL == poStub)
	{
		return INVALID_WMHANDLE;
	}

	if (false == poStub->Begin(m_hNextHandle, poThread))
	{
		delete poStub;
		return INVALID_WMHANDLE;
	}

	m_oStubMap[m_hNextHandle] = poStub;
	return m_hNextHandle;
}

bool WMAPI CThreadStubMgr::Kill( WMHANDLE hThread )
{
	CThreadStub* poStub = _FindStub(hThread);
	if (NULL == poStub)
	{
		return false;
	}

	return poStub->Kill();
}

bool WMAPI CThreadStubMgr::Suspend( WMHANDLE hThread )
{
	CThreadStub* poStub = _FindStub(hThread);
	if (NULL == poStub)
	{
		return false;
	}

	return poStub->Suspend();
}

bool WMAPI CThreadStubMgr::Resume( WMHANDLE hThread )
{
	CThreadStub* poStub = _FindStub(hThread);
	if (NULL == poStub)
	{
		return false;
	}

	return poStub->Resume();
}

bool WMAPI CThreadStubMgr::WaitFor( WMHANDLE hThread, UINT32 dwTime )
{
	CThreadStub* poStub = _FindStub(hThread);
	if (NULL == poStub)
	{
		return false;
	}

	return poStub->WaitFor();
}

bool WMAPI CThreadStubMgr::CloseHandle( WMHANDLE hThread )
{
	CThreadStubMap::iterator it = m_oStubMap.find(hThread);
	if (it == m_oStubMap.end())
	{
		return false;
	}

	CThreadStub * poStub = it->second;
	if (NULL == poStub)
	{
		assert(false);
		return false;
	}

	m_oStubMap.erase(it);
	delete poStub;

	return true;
}

UINT32 WMAPI CThreadStubMgr::GetThreadID( WMHANDLE hThread )
{
	CThreadStub* poStub = _FindStub(hThread);
	if (NULL == poStub)
	{
		return false;
	}

	return poStub->GetThreadID();
}

void WMAPI CThreadStubMgr::Release()
{
	if (--m_nRef <= 0)
	{
		delete this;
	}
}

CThreadStub* CThreadStubMgr::_FindStub( WMHANDLE hThread )
{
	CThreadStubMap::iterator it = m_oStubMap.find(hThread);
	if (it == m_oStubMap.end())
	{
		return NULL;
	}

	return it->second;
}

void CThreadStubMgr::AddRef()
{
	m_nRef++;
}

CThreadStubMgr::CThreadStubMgr()
{
	m_nRef = 0;
	m_hNextHandle = 0;
}

CThreadStubMgr::~CThreadStubMgr()
{

}
