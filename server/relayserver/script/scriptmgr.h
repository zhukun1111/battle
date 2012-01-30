#ifndef _H_SCRIPTMGR_H_
#define _H_SCRIPTMGR_H_

#include "kernel/singleton.h"
#include "luasystem.h"
#include "script.h"

void Include(const char* pszScrptPath);

class CScriptMgr
{
	DECLARE_SINGLETON(CScriptMgr);

private:
	CScriptMgr();
	virtual ~CScriptMgr();

public:
	bool Init(const char* pszPreloadFilePath);
	void UnInit(void);

	CScript* LoadFile(const char* pszScriptPath);
	bool ExecuteString(const char* pszScriptString);
	bool ReloadAllScripts(void);
	lua_State* GetLuaState(void)
	{
		return m_oLuaSystem.GetLuaState();
	}

	CScript* GetScript(const char* pszScriptPath);

	UINT32 GetLastScriptID() const
	{
		return m_dwLastScriptID;
	}

	void SetLastScriptID(UINT32 dwLastScriptID)
	{
		m_dwLastScriptID = dwLastScriptID;
	}

	UINT32 GetRunningScriptID() const
	{
		return m_dwRunningScriptID;
	}

	void SetRunningScriptID(UINT32 dwRunningScriptID)
	{
		m_dwRunningScriptID = dwRunningScriptID;
	}

	friend class CScript;

private:
	bool _LoadFile(const char* pszScriptpath, UINT32 dwScriptID);

	typedef std::map<UINT32, CScript*> CMapScriptID;
	CMapScriptID m_oMapScriptID;

	typedef std::map<std::string, CScript*> CMapScriptPath;
	CMapScriptPath m_oMapScriptPath;

	CLuaSystem m_oLuaSystem;
	UINT32 m_dwRunningScriptID;
	UINT32 m_dwLastScriptID;

	std::string m_strPreloadFilePath;
};

#endif