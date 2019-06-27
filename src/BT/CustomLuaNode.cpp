
#include "CustomLuaNode.hpp"

#include <iostream>

BT::CustomLuaNode::CustomLuaNode(LuaStateWrapper::Ptr LWrapper) :
LogicNode(),
LWrapper(LWrapper),
state()
{
    state.cln = this;
}

BT::CustomLuaNode::~CustomLuaNode()
{}

BT::BehaviorNode::Ptr BT::CustomLuaNode::getCopy()
{
    std::unique_ptr<CustomLuaNode> clnPtr(new CustomLuaNode(LWrapper));

    return Ptr(clnPtr.release());
}

std::size_t BT::CustomLuaNode::getChildrenSize() const
{
    return children.size();
}

BT::BehaviorNode::StateType BT::CustomLuaNode::activateChild(std::size_t index)
{
    return children.at(index)->activate();
}

BT::CustomLuaNodeState* BT::CustomLuaNode::getState()
{
    return &state;
}

lua_State* BT::CustomLuaNode::getLuaState()
{
    return LWrapper->L;
}

BT::BehaviorNode::StateType BT::CustomLuaNode::performAction()
{
    return action(false);
}

BT::BehaviorNode::StateType BT::CustomLuaNode::continueAction()
{
    return action(true);
}

void BT::CustomLuaNode::getLuaStatesHelper(std::unordered_set<lua_State*>& s)
{
    if(LWrapper)
    {
        s.insert(LWrapper->L);
    }
    BT::BehaviorNode::getLuaStatesHelper(s);
}

BT::BehaviorNode::StateType BT::CustomLuaNode::action(bool isContinuing)
{
    // +1 stack: activate function
    int type = lua_getglobal(LWrapper->L, "activate");
    if(type != LUA_TFUNCTION)
    {
        std::cerr << "ERROR: global \"activate\" is not a function!\n";
        lua_pop(LWrapper->L, 1);
        return StateType::BT_ERROR;
    }

    // +1 stack: boolean parameter
    lua_pushboolean(LWrapper->L, isContinuing ? 1 : 0);

    // -2, +1 stack: function and parameter, resulting state
    if((type = lua_pcall(LWrapper->L, 1, 1, 0)) != LUA_OK)
    {
        // +1 stack: error object
        std::cerr << "ERROR: Failed to call \"activate\" function!\n";
        if(type == LUA_ERRRUN)
        {
            std::cerr << "HINT: runtime error\n";
        }
        else if(type == LUA_ERRMEM)
        {
            std::cerr << "HINT: memory allocation error\n";
        }
        lua_pop(LWrapper->L, 1); // -1 stack
    }
    
    int resultType = lua_tointeger(LWrapper->L, -1);
    lua_pop(LWrapper->L, 1); // -1 stack

    switch(resultType)
    {
    case 0:
        return StateType::BT_READY_SUCCESS;
    case 1:
        return StateType::BT_RUNNING;
    case 2:
        return StateType::BT_FAILED;
    default:
        return StateType::BT_ERROR;
    }
}

