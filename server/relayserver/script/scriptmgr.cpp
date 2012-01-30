#include "utility/sharepch.h"
#include "kernel/wmhash.h"
#include "script.h"
#include "scriptmgr.h"
#include "luametatable.h"
#include "script/tolua/tolua_interface.h"

INSTANCE_SINGLETON(CScriptMgr);

CScriptMgr::CScriptMgr(): m_dwLastScriptID(-1), m_dwRunningScriptID(-1)
{

}

CScriptMgr::~CScriptMgr()
{

}

CScript* CScriptMgr::GetScript( const char* pszScriptPath )
{
	CMapScriptPath::iterator iter = m_oMapScriptPath.find(std::string(pszScriptPath));

	if (iter != m_oMapScriptPath.end())
	{
		return iter->second;
	}

	return NULL;
}

bool CScriptMgr::_LoadFile( const char* pszScriptpath, UINT32 dwScriptID )
{
	m_dwLastScriptID = dwScriptID;
	bool bStatus = false;

	try
	{
		bStatus = m_oLuaSystem.LoadFile(pszScriptpath);
	}
	catch (CLuaStateException* e)
	{
		WMASSERT(false);
	}

	m_dwLastScriptID = -1;
	WMASSERT(bStatus);
	return bStatus;
}

bool CScriptMgr::Init( const char* pszPreloadFilePath )
{
	WMASSERT(NULL != pszPreloadFilePath);
	lua_State* pLuaState = m_oLuaSystem.GetLuaState();
	tolua_script_open(pLuaState);

	SetMetaTable(pLuaState);
	return true;
}

void CScriptMgr::UnInit( void )
{
	CMapScriptID::iterator iter = m_oMapScriptID.begin();

	for (; iter != m_oMapScriptID.end(); ++iter)
	{
		delete iter->second;
	}

	m_oMapScriptID.clear();
	m_oMapScriptPath.clear();
}

bool CScriptMgr::ReloadAllScripts( void )
{
	m_dwRunningScriptID = -1;
	m_dwLastScriptID = -1;

	m_oLuaSystem.ReNewLuaState();

	CMapScriptPath::iterator iter = m_oMapScriptPath.begin();
	for (; iter != m_oMapScriptPath.end(); ++iter)
	{
		_LoadFile(iter->first.c_str(), iter->second->GetScriptID());
	}

	return true;
}

bool CScriptMgr::ExecuteString( const char* pszScriptString )
{
	bool bStatus = false;

	try
	{
		bStatus = m_oLuaSystem.ExecuteString(pszScriptString);
	}
	catch(CLuaStateException& e)
	{
		WMASSERT(false);
	}

	return bStatus;
}

CScript* CScriptMgr::LoadFile( const char* pszScriptPath )
{
	WMASSERT(pszScriptPath);

	CMapScriptPath::iterator iterPath = m_oMapScriptPath.find(std::string(pszScriptPath));
	if (iterPath != m_oMapScriptPath.end())
	{
		return iterPath->second;
	}

	UINT32 dwScriptID = hash(pszScriptPath, strlen(pszScriptPath), 0);

	CMapScriptID::iterator iterID = m_oMapScriptID.find(dwScriptID);
	if (iterID != m_oMapScriptID.end())
	{
		return NULL;
	}

	bool bStatus = _LoadFile(pszScriptPath, dwScriptID);
	if (bStatus)
	{
		CScript* poScript = new CScript(dwScriptID, pszScriptPath);
		m_oMapScriptID.insert(std::make_pair(dwScriptID, poScript));
		m_oMapScriptPath.insert(std::make_pair(std::string(pszScriptPath), poScript));
		return poScript;
	}

	return NULL;
}
