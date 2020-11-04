
#ifndef BT_LUA_STATE_WRAPPER
#define BT_LUA_STATE_WRAPPER

#include <memory>

#include <lua.hpp>

namespace BT
{

struct LuaStateWrapper
{
    typedef std::shared_ptr<LuaStateWrapper> Ptr;

    LuaStateWrapper();
    ~LuaStateWrapper();

    // deny copy
    LuaStateWrapper(const LuaStateWrapper&) = delete;
    LuaStateWrapper& operator =(const LuaStateWrapper&) = delete;

    // allow move
    LuaStateWrapper(LuaStateWrapper&&) = default;
    LuaStateWrapper& operator =(LuaStateWrapper&&) = default;

    lua_State* L;
};

} // namespace BT

#endif

