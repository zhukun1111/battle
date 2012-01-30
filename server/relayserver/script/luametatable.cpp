#include "utility/sharepch.h"
#include "tolua++/tolua++.h"
#include "luametatable.h"
#include "script.h"
#include "scriptmgr.h"

static int CustomIndex(lua_State* L)
{
	UINT32 dwRunningScriptID = CScriptMgr::Instance()->GetRunningScriptID();
	WMASSERT(dwRunningScriptID != -1);

	INT32 nFuncNameIndex = lua_gettop(L);

	lua_pushliteral(L, "ScriptsTable");
	lua_rawget(L, LUA_GLOBALSINDEX);

	lua_pushnumber(L, dwRunningScriptID);
	lua_rawget(L, -2);

	lua_pushvalue(L, nFuncNameIndex);
	lua_rawget(L, -2);

	lua_remove(L, -2);
	lua_remove(L, -3);

	return 1;
}

static int CustomNewIndex(lua_State* L)
{
	UINT32 dwLastScriptID = CScriptMgr::Instance()->GetLastScriptID();
	WMASSERT(-1 != dwLastScriptID);

	INT32 nFuncIndex = lua_gettop(L);
	INT32 nFuncNameIndex = nFuncIndex - 1;

	lua_pushliteral(L, "ScriptsTable");
	lua_rawget(L, LUA_GLOBALSINDEX);

	INT32 nScriptsTableIndex = lua_gettop(L);

	lua_pushnumber(L, dwLastScriptID);
	lua_rawget(L, -2);

	if (lua_isnil(L, -1))
	{
		lua_pop(L, -1);
		lua_pushnumber(L, dwLastScriptID);
		lua_newtable(L);
		lua_pushvalue(L, nFuncNameIndex);
		lua_pushvalue(L, nFuncIndex);
		lua_rawset(L, -3);

		lua_rawset(L, nScriptsTableIndex);
	}
	else
	{
		lua_pushvalue(L, nFuncNameIndex);
		lua_pushvalue(L, nFuncIndex);
		lua_rawset(L, -3);

		lua_pop(L, 1);
	}

	lua_pop(L, 1);

	return 0;
}

void SetMetaTable( lua_State* L )
{
	lua_pushliteral(L, "ScriptsTable");
	lua_newtable();
	lua_rawset(L, LUA_GLOBALSINDEX);


	luaL_newmetatable(L, "ScriptsMetaTable");

	lua_pushliteral(L, "__index");
	lua_pushcfunction(L, CustomIndex);
	lua_rawset(L, -3);

	lua_pushliteral(L,  "__newindex");
	lua_pushcfunction(L, CustomNewIndex);
	lua_rawset(L, -3);

	lua_setmetatable(L, LUA_GLOBALSINDEX);
}

void Include( const char* pszScrptPath )
{
	UINT32 dwCurrentScriptID = CScriptMgr::Instance()->GetLastScriptID();
	CScript* poScript = CScriptMgr::Instance()->LoadFile(pszScrptPath);
	WMASSERT(NULL != poScript);

	UINT32 dwIncludeScriptID = poScript->GetScriptID();
	CScriptMgr::Instance()->SetLastScriptID(dwCurrentScriptID);

	CopyScriptTable(CScriptMgr::Instance()->GetLuaState(), dwIncludeScriptID, dwCurrentScriptID);
}

static void CopyScriptTable(lua_State* L, UINT32 dwSrcScriptID, UINT32 dwDstScriptID)
{
	lua_pushliteral(L, "ScriptsTable");
	lua_rawget(L, LUA_GLOBALSINDEX);

	INT32 nScriptsTableIndex = lua_gettop(L);

	lua_pushnumber(L, dwDstScriptID);
	lua_rawget(L, nScriptsTableIndex);

	if (lua_isnil(L, -1))
	{
		lua_pop(L, 1);

		lua_pushnumber(L, dwDstScriptID);
		lua_newtable(L);
		lua_rawset(L, nScriptsTableIndex);

		lua_pushnumber(L, dwDstScriptID);
		lua_rawget(L, nScriptsTableIndex);
	}

	INT32 nCopyToTableIndex = lua_gettop(L);

	lua_pushnumber(L, dwSrcScriptID);
	lua_rawget(L, nScriptsTableIndex);

	INT32 nCopyFromTableIndex = lua_gettop(L);

	lua_pushnil(L);
	while (lua_next(L, nCopyFromTableIndex) != 0)
	{
		lua_pushvalue(L, -2);
		lua_insert(L, -2);
		lua_rawset(L, nCopyToTableIndex);
	}

	lua_pop(L, 3);
}
