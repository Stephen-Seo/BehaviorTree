
#include "CustomLuaNodeState.hpp"

#include <vector>
#include <cstring>

#include "CustomLuaNode.hpp"

BT::CustomLuaNodeState::CustomLuaNodeState() :
cln(nullptr),
uvp(new void*)
{
}

int BT::getChildrenSize(lua_State* L)
{
    CustomLuaNodeState* state = *((CustomLuaNodeState**)lua_getextraspace(L));

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
    CustomLuaNodeState* state = *((CustomLuaNodeState**)lua_getextraspace(L));

    if(lua_gettop(L) == 1)
    {
        std::size_t index = lua_tointeger(L, 1);
        if(state && state->cln)
        {
            BehaviorNode::State::StateType result = state->cln->activateChild(index);
            switch(result)
            {
            case BehaviorNode::State::READY_SUCCESS:
                lua_pushinteger(L, 0);
                break;
            case BehaviorNode::State::RUNNING:
                lua_pushinteger(L, 1);
                break;
            case BehaviorNode::State::FAILED:
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

