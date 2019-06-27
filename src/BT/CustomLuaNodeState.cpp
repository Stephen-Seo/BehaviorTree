
#include "CustomLuaNodeState.hpp"

#include <vector>
#include <cstring>

#include "CustomLuaNode.hpp"

BT::CustomLuaNodeState::CustomLuaNodeState() :
cln(nullptr)
{
}

int BT::getChildrenSize(lua_State* L)
{
    lua_pushstring(L, BT_CUSTOM_LUA_NODE_STATE_REGISTRY_INDEX);
    lua_gettable(L, LUA_REGISTRYINDEX);
    CustomLuaNodeState* state = (CustomLuaNodeState*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    if(state && state->cln)
    {
        lua_pushinteger(L, state->cln->getChildrenSize());
    }
    else
    {
        lua_pushinteger(L, 0);
    }

    return 1;
}

int BT::activateChild(lua_State* L)
{
    lua_pushstring(L, BT_CUSTOM_LUA_NODE_STATE_REGISTRY_INDEX);
    lua_gettable(L, LUA_REGISTRYINDEX);
    CustomLuaNodeState* state = (CustomLuaNodeState*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    if(lua_gettop(L) == 1)
    {
        std::size_t index = lua_tointeger(L, 1);
        if(state && state->cln)
        {
            BehaviorNode::StateType result = state->cln->activateChild(index);
            switch(result)
            {
            case BehaviorNode::StateType::BT_READY_SUCCESS:
                lua_pushinteger(L, 0);
                break;
            case BehaviorNode::StateType::BT_RUNNING:
                lua_pushinteger(L, 1);
                break;
            case BehaviorNode::StateType::BT_FAILED:
                lua_pushinteger(L, 2);
                break;
            default:
                lua_pushinteger(L, 3);
                break;
            }
        }
        else
        {
            lua_pushinteger(L, 3);
        }
    }
    else
    {
        lua_pushinteger(L, 3);
    }

    return 1;
}

int BT::writerForTransferringFunction(lua_State* /*L*/, const void* p, std::size_t sz, void* ud)
{
    std::vector<char>* v = (std::vector<char>*)ud;
    std::size_t psize = v->size();
    v->resize(psize + sz);
    std::memcpy(&(*v)[psize], p, sz);
    return 0;
}

