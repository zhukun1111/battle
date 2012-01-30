#ifndef _H_LUASYSTEM_H_
#define _H_LUASYSTEM_H_

#include "tolua++/tolua++.h"
extern "C"
{
#include "lua/lualib.h"
};

class CLuaSystem;

class CLuaStateException
{
public:
	CLuaStateException(CLuaSystem* poLuaSystem, INT32 nErrCode, const char* pszErrMsg) : 
	  m_poLuaSystem(poLuaSystem), m_nErrCode(nErrCode), m_strErrMsg(pszErrMsg)
	  {

	  }

	  virtual ~CLuaStateException()
	  {

	  }

	  CLuaSystem* GetLuaSystem(void)
	  {
		  return m_poLuaSystem;
	  }

	  INT32 GetErrCode(void)
	  {
		  return m_nErrCode;
	  }

	  const char* GetErrMsg(void)
	  {
		  return m_strErrMsg.c_str();
	  }

private:
	CLuaSystem* m_poLuaSystem;
	INT32 m_nErrCode;
	std::string m_strErrMsg;
};

class CLuaSystem
{
public:
	CLuaSystem();
	virtual ~CLuaSystem();

	lua_State* GetLuaState(void)
	{
		return m_poLuaState;
	}

	bool LoadFile(const char* pszScriptPath);
	bool ExecuteString(const char* pszStriptString);

	bool CallFunction(bool bIsGlobalFunc, const char* pszFuncName, bool bHasReturn, const char* pszArgFormat, va_list stArgList);

	bool GetReturn(const char* pszReturnFormat, va_list stArgList);

	void SetScriptPath(const std::string& rstrScriptPath)
	{
		m_strScriptPath = rstrScriptPath;
	}

	const char* GetScriptPath() const
	{
		return m_strScriptPath.c_str();
	}

	const char* GetFuncName() const
	{
		return m_strFuncName.c_str();
	}

	void ReNewLuaState();

private:
	void _HandleLuaError(INT32 nErrCode);

private:
	lua_State* m_poLuaState;

	std::string m_strScriptPath;
	std::string m_strFuncName;
};
#endif