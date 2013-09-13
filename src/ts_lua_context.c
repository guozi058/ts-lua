
#include "ts_lua_util.h"

static int ts_lua_context_get(lua_State *L);
static int ts_lua_context_set(lua_State *L);


void
ts_lua_inject_context_api(lua_State *L)
{
    lua_newtable(L);         /* .ctx */

    lua_createtable(L, 0, 2);       /* metatable for context */

    lua_pushcfunction(L, ts_lua_context_get);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, ts_lua_context_set);
    lua_setfield(L, -2, "__newindex");

    lua_setmetatable(L, -2); 

    lua_setfield(L, -2, "ctx");
}


static int
ts_lua_context_get(lua_State *L)
{
    const char  *key;
    size_t      key_len;

    key = luaL_checklstring(L, 2, &key_len);

    if (key && key_len) {
        lua_pushlstring(L, key, key_len);
        lua_rawget(L, LUA_GLOBALSINDEX);
    } else {
        lua_pushnil(L);
    }

    return 1;
}


static int
ts_lua_context_set(lua_State *L)
{
    const char  *key;
    const char  *val;
    size_t      val_len;
    size_t      key_len;

    key = luaL_checklstring(L, 2, &key_len);
    val = luaL_checklstring(L, 3, &val_len);

    lua_pushlstring(L, key, key_len);
    lua_pushlstring(L, val, val_len);

    lua_rawset(L, LUA_GLOBALSINDEX);

    return 0;
}

