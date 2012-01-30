#include "script.h"
#include "scriptmgr.h"

bool CScript::CallFunction( const char* pszFuncName, bool bHasReturn, const char* pszArgFormat, ... )
{
	CScriptMgr::Instance()->SetRunningScriptID(m_dwScriptID);
	CScriptMgr::Instance()->m_oLuaSystem.SetScriptPath(m_strScriptPath);

	bool bStatus = false;
	try
	{
		va_list stArgList;
		va_start(stArgList, pszArgFormat);
		bStatus = CScriptMgr::Instance()->m_oLuaSystem.CallFunction(false, pszFuncName, pszArgFormat, stArgList);
		va_end(stArgList);
	}
	catch(CLuaStateException& e)
	{
		WMASSERT(false);
	}

	return bStatus;
}

bool CScript::GetReturn( const char* pszReturnFormat, ... )
{
	bool bStatus = false;

	va_list stArgList;
	va_start(stArgList, pszReturnFormat);
	bStatus = CScriptMgr::Instance()->m_oLuaSystem.GetReturn(pszReturnFormat, stArgList);
	va_end(stArgList);

	return bStatus;
}

bool CScript::CallGlobalFunction( const char* pszFuncName, bool bHasReturn, const char* pszArgFormat, ... )
{
	CScriptMgr::Instance()->SetRunningScriptID(m_dwScriptID);
	CScriptMgr::Instance()->m_oLuaSystem.SetScriptPath(m_strScriptPath);

	bool bStatus = false;
	try
	{
		va_list stArgList;
		va_start(stArgList, pszArgFormat);
		bStatus = CScriptMgr::Instance()->m_oLuaSystem.CallFunction(true, pszFuncName, pszArgFormat, stArgList);
		va_end(stArgList);
	}
	catch(CLuaStateException& e)
	{
		WMASSERT(false);
	}

	return bStatus;
}

bool CScript::GetGlobalReturn( const char* pszReturnFormat, ... )
{
	bool bStatus = false;

	va_list stArgList;
	va_start(stArgList, pszReturnFormat);
	bStatus = CScriptMgr::Instance()->m_oLuaSystem.GetReturn(pszReturnFormat, stArgList);
	va_end(stArgList);

	return bStatus;
}
