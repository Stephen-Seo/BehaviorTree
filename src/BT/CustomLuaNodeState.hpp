
#ifndef BT_CUSTOM_LUA_NODE_STATE
#define BT_CUSTOM_LUA_NODE_STATE

#include <memory>
#include <lua.hpp>


namespace BT
{

class CustomLuaNode;

struct CustomLuaNodeState
{
    CustomLuaNodeState();

    CustomLuaNode* cln;
    std::unique_ptr<void*> uvp;
};

int getChildrenSize(lua_State* L);
int activateChild(lua_State* L);

int writerForTransferringFunction(lua_State* /*L*/, const void* p, std::size_t sz, void* ud);

} // namespace BT

#endif

