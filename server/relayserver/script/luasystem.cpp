#include "utility/sharepch.h"
#include "luasystem.h"
#include "kernel/pch.h"

CLuaSystem::CLuaSystem()
{
	m_poLuaState = luaL_newstate();
	luaL_openlibs(m_poLuaState);
}

CLuaSystem::~CLuaSystem()
{
	lua_close(m_poLuaState);
}

bool CLuaSystem::LoadFile( const char* pszScriptPath )
{
	m_strScriptPath = std::string(pszScriptPath);

	INT32 nErrCode = luaL_loadfile(m_poLuaState, pszScriptPath);

	if (0 == nErrCode)
	{
		nErrCode = lua_pcall(m_poLuaState, 0, LUA_MULTRET, 0);
	}

	_HandleLuaError(nErrCode);

	return 0 == nErrCode;
}

bool CLuaSystem::CallFunction( bool bIsGlobalFunc, const char* pszFuncName, bool bHasReturn, const char* pszArgFormat, va_list stArgList )
{
	if (true == bIsGlobalFunc)
	{
		lua_pushstring(m_poLuaState, pszFuncName);
		lua_rawget(m_poLuaState, LUA_GLOBALSINDEX);

		if (lua_isnil(m_poLuaState, -1))
		{
			lua_pop(m_poLuaState, 1);
			LOG("global script function %s doesnt't exist", pszFuncName);

			WMASSERT(false);
			return false;
		}
	}
	else
	{
		lua_getglobal(m_poLuaState, pszFuncName);
		if (lua_isnil(m_poLuaState, -1))
		{
			lua_pop(m_poLuaState, 1);
			return false;
		}
	}

	INT64 llPara;
	double fPara;
	const char* pszPara;
	void* pObjPtr;
	const char* pszClassName;

	m_strFuncName = std::string(pszFuncName);
	UINT32 nArgCount = strlen(pszArgFormat);
	if (nArgCount != 0)
	{
		for (UINT32 i = 0; i < nArgCount; ++i)
		{
			switch(pszArgFormat[i])
			{
			case 'd':
				llPara = va_arg(stArgList, INT64);
				lua_pushnumber(m_poLuaState, llPara);
				break;
			
			case 'f':
				fPara = va_arg(stArgList, double);
				lua_pushnumber(m_poLuaState, fPara);
				break;

			case 's':
				pszPara = va_arg(stArgList, const char*);
				lua_pushstring(m_poLuaState, pszPara);
				break;

			case 'u':
				pObjPtr = va_arg(stArgList, void*);
				pszClassName = va_arg(stArgList, const char*);
				tolua_pushusertype(m_poLuaState, pObjPtr, pszClassName);
				break;

			default:
				LOG("invalid prameter type");
				WMASSERT(false);
				break;
			}

			
		}
	}

	INT32 nErrCode;
	if (bHasReturn)
	{
		nErrCode = lua_pcall(m_poLuaState, nArgCount, 1, 0);
	} 
	else
	{
		nErrCode = lua_pcall(m_poLuaState, nArgCount, 0, 0);
	}

	_HandleLuaError(nErrCode);
	return 0 == nErrCode;
}

bool CLuaSystem::GetReturn( const char* pszReturnFormat, va_list stArgList )
{
	INT32 nReturnCount = strlen(pszReturnFormat);
	if (nReturnCount != 1)
	{
		LOG("just support only one return value from lua");
		WMASSERT(false);
		return false;
	}

	bool* pbPara;
	INT64* pllPara;
	double* pfPara;

	bool bStatus = true;
	switch(pszReturnFormat[0])
	{
	case 'b':
		pbPara = va_arg(stArgList, bool*);
		*pbPara = lua_toboolean(m_poLuaState, -1);
		lua_pop(m_poLuaState, 1);
		break;

	case 'd':
		pllPara = va_arg(stArgList, INT64*);
		*pllPara = lua_tointeger(m_poLuaState, -1);
		lua_pop(m_poLuaState, 1);
		break;

	case 'f':
		pfPara = va_arg(stArgList, double*);
		*pfPara = lua_tonumber(m_poLuaState, -1);
		lua_pop(m_poLuaState, 1);
		break;

	default:
		LOG("invalid prameter type");
		WMASSERT(false);
		break;
	}

	return bStatus;
	
}

void CLuaSystem::ReNewLuaState()
{
	lua_close(m_poLuaState);

	m_poLuaState = luaL_newstate();
	luaL_openlibs(m_poLuaState);
}

void CLuaSystem::_HandleLuaError( INT32 nErrCode )
{
	if (0 == nErrCode)
	{
		m_strFuncName.clear();
		return;
	}

	const char* pszErrMsg = lua_tostring(m_poLuaState, -1);
	lua_pop(m_poLuaState, 1);

	throw CLuaStateException(this, nErrCode, pszErrMsg);
}

bool CLuaSystem::ExecuteString( const char* pszStriptString )
{
	INT32 nErrCode = luaL_loadstring(m_poLuaState, pszStriptString);

	if (0 == nErrCode)
	{
		nErrCode = lua_pcall(m_poLuaState,  0, LUA_MULTRET, 0);
	}

	_HandleLuaError(nErrCode);
	return  0 == nErrCode;
}
