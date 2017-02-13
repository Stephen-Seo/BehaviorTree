
#include "LuaStateWrapper.hpp"

BT::LuaStateWrapper::LuaStateWrapper() :
L(nullptr)
{
    L = luaL_newstate();
}

BT::LuaStateWrapper::~LuaStateWrapper()
{
    lua_close(L);
}
