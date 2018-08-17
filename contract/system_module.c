#include <string.h>
#include <stdlib.h>
#include "vm.h"

static int systemPrint(lua_State *L)
{
	printf ("systemPrinted");
	return 1;
}

static int setItem(lua_State *L)
{
	printf ("setItem");
	return 0;
}

static int getItem(lua_State *L)
{
	printf ("getItem");
	return 1;
}

static int getSender(lua_State *L)
{
	const bc_ctx_t *exec = getLuaExecContext(L);
	if (exec == NULL) {
	    luaL_error(L, "cannot find execution context");
	}
	lua_pushstring(L, exec->sender);
	return 1;
}

static int getBlockhash(lua_State *L)
{
	const bc_ctx_t *exec = getLuaExecContext(L);
	if (exec == NULL) {
	    luaL_error(L, "cannot find execution context");
	}
	lua_pushstring(L, exec->blockHash);
	return 1;
}

static int getTxhash(lua_State *L)
{
	const bc_ctx_t *exec = getLuaExecContext(L);
	if (exec == NULL) {
	    luaL_error(L, "cannot find execution context");
	}
	lua_pushstring(L, exec->txHash);
	return 1;
}

static int getBlockHeight(lua_State *L)
{
	const bc_ctx_t *exec = getLuaExecContext(L);
	if (exec == NULL) {
	    luaL_error(L, "cannot find execution context");
	}
	lua_pushinteger(L, exec->blockHeight);
	return 1;
}

static int getTimestamp(lua_State *L)
{
	const bc_ctx_t *exec = getLuaExecContext(L);
	if (exec == NULL) {
	    luaL_error(L, "cannot find execution context");
	}
	lua_pushinteger(L, exec->timestamp);
	return 1;
}

static int getContractID(lua_State *L)
{
	const bc_ctx_t *exec = getLuaExecContext(L);
	if (exec == NULL) {
	    luaL_error(L, "cannot find execution context");
	}
	lua_pushstring(L, exec->contractId);
	return 1;
}

static const luaL_Reg sys_lib[] = {
	{"print", systemPrint},
	{"setItem", setItem},
	{"getItem", getItem},
	{"getSender", getSender},
	{"getCreator", getContractID},
	{"getBlockhash", getBlockhash},
	{"getTxhash", getTxhash},
	{"getBlockheight", getBlockHeight},
	{"getTimestamp", getTimestamp},
	{"getContractID", getContractID},
	{NULL, NULL}
};

int luaopen_system(lua_State *L)
{
    luaL_register(L, "system", sys_lib);
    return 1;
}
