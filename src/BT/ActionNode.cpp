
#include "ActionNode.hpp"

#include <memory>

BT::ActionNode::ActionNode() :
BehaviorNode(),
actionFunction(),
LWrapper(std::make_shared<LuaStateWrapper>()),
luaScript()
{}

BT::ActionNode::ActionNode(ActionFunctionT actionFunction) :
BehaviorNode(),
actionFunction(actionFunction),
LWrapper(std::make_shared<LuaStateWrapper>()),
luaScript()
{}

BT::ActionNode::ActionNode(std::string luaScript) :
BehaviorNode(),
actionFunction(),
LWrapper(std::make_shared<LuaStateWrapper>()),
luaScript(luaScript)
{}

BT::ActionNode::~ActionNode()
{}

BT::BehaviorNode::Ptr BT::ActionNode::getCopy()
{
    std::unique_ptr<ActionNode> copy(new ActionNode());

    copy->actionFunction = actionFunction;

    for(std::size_t i = 0; i < children.size(); ++i)
    {
        copy->insert(children[i]->getCopy());
    }

    return Ptr(copy.release());

}

void BT::ActionNode::setActionFunction(ActionFunctionT actionFunction)
{
    this->actionFunction = actionFunction;
    state = State();
}

void BT::ActionNode::setLuaActionFunction(std::string luaScript)
{
    this->luaScript = luaScript;
}

void BT::ActionNode::exposeFunctionToLuaScript(
    int (*function)(lua_State* L),
    const char* name
)
{
    lua_pushcfunction(LWrapper->L, function);
    lua_setglobal(LWrapper->L, name);
}

void BT::ActionNode::exposeFunctionToLuaScript(
    int (*function)(lua_State* L),
    std::string name
)
{
    lua_pushcfunction(LWrapper->L, function);
    lua_setglobal(LWrapper->L, name.c_str());
}

BT::BehaviorNode::State BT::ActionNode::performAction()
{
    if(actionFunction)
    {
        state.stateType = actionFunction(false);
        return state;
    }
    else if(!luaScript.empty())
    {
        return performLuaScript(false);
    }
    else
    {
        state.stateType = State::ERROR;
        return state;
    }
}

BT::BehaviorNode::State BT::ActionNode::continueAction()
{
    if(actionFunction)
    {
        state.stateType = actionFunction(true);
        return state;
    }
    else if(!luaScript.empty())
    {
        return performLuaScript(true);
    }
    else
    {
        state.stateType = State::ERROR;
        return state;
    }
}

BT::BehaviorNode::State BT::ActionNode::performLuaScript(bool isContinuing)
{
    int type = luaL_loadbuffer(
        LWrapper->L,
        luaScript.c_str(),
        luaScript.size(),
        "BT::ActionNode"
    );
    if(type != LUA_OK)
    {
        state.stateType = State::ERROR;
        return state;
    }
    else if(lua_pcall(LWrapper->L, 0, 0, 0) != LUA_OK)
    {
        lua_pop(LWrapper->L, 1);
        state.stateType = State::ERROR;
        return state;
    }

    // +1 stack: function
    type = lua_getglobal(LWrapper->L, "actionFunction");
    if(type != LUA_TFUNCTION)
    {
        state.stateType = State::ERROR;
        return state;
    }

    // +1 stack: argument
    lua_pushboolean(LWrapper->L, isContinuing ? 1 : 0);
    
    // -2, +1 stack: function return value (int)
    if(lua_pcall(LWrapper->L, 1, 1, 0) != LUA_OK)
    {
        lua_pop(LWrapper->L, 1);
        state.stateType = State::ERROR;
        return state;
    }
    else if(lua_isinteger(LWrapper->L, -1) != 1)
    {
        lua_pop(LWrapper->L, 1);
        state.stateType = State::ERROR;
        return state;
    }

    type = lua_tointeger(LWrapper->L, -1);
    lua_pop(LWrapper->L, 1); // -1 stack

    switch(type)
    {
    case 0:
        state.stateType = State::READY_SUCCESS;
        break;
    case 1:
        state.stateType = State::RUNNING;
        break;
    case 2:
        state.stateType = State::FAILED;
        break;
    case 3:
    default:
        state.stateType = State::ERROR;
        break;
    }

    return state;
}

