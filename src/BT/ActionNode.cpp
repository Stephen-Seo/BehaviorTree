
#include "ActionNode.hpp"

#include <memory>

BT::ActionNode::ActionNode() :
BehaviorNode(),
actionFunction(),
LWrapper(),
lua(),
luaIsFilename(false)
{}

BT::ActionNode::ActionNode(ActionFunctionT actionFunction) :
BehaviorNode(),
actionFunction(actionFunction),
LWrapper(),
lua(),
luaIsFilename(false)
{}

BT::ActionNode::ActionNode(std::string lua, bool isFilename) :
BehaviorNode(),
actionFunction(),
LWrapper(),
lua(lua),
luaIsFilename(isFilename)
{}

BT::ActionNode::~ActionNode()
{}

BT::BehaviorNode::Ptr BT::ActionNode::getCopy()
{
    std::unique_ptr<ActionNode> copy(new ActionNode());

    copy->actionFunction = actionFunction;
    copy->lua = lua;
    copy->luaIsFilename = luaIsFilename;
    copy->exposedFunctions = exposedFunctions;

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

void BT::ActionNode::setLuaActionFunction(std::string lua, bool isFilename)
{
    this->lua= lua;
    luaIsFilename = isFilename;
}

void BT::ActionNode::exposeFunctionToLuaScript(
    int (*function)(lua_State*),
    const char* name
)
{
    LWrapper.reset();
    exposedFunctions.insert(std::make_pair(std::string(name), function));
}

void BT::ActionNode::exposeFunctionToLuaScript(
    int (*function)(lua_State*),
    std::string name
)
{
    LWrapper.reset();
    exposedFunctions.insert(std::make_pair(name, function));
}

BT::BehaviorNode::State BT::ActionNode::performAction()
{
    if(actionFunction)
    {
        state.stateType = actionFunction(false);
        return state;
    }
    else if(!lua.empty())
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
    else if(!lua.empty())
    {
        return performLuaScript(true);
    }
    else
    {
        state.stateType = State::ERROR;
        return state;
    }
}

void BT::ActionNode::initializeLuaState()
{
    LWrapper = std::make_shared<LuaStateWrapper>();

    for(auto iter = exposedFunctions.begin(); iter != exposedFunctions.end(); ++iter)
    {
        lua_pushcfunction(LWrapper->L, iter->second);
        lua_setglobal(LWrapper->L, iter->first.c_str());
    }
}

BT::BehaviorNode::State BT::ActionNode::performLuaScript(bool isContinuing)
{
    if(!LWrapper)
    {
        initializeLuaState();
    }

    int type = 0;
    if(luaIsFilename)
    {
        type = luaL_loadfile(LWrapper->L, lua.c_str());
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
    }
    else
    {
        type = luaL_loadbuffer(
            LWrapper->L,
            lua.c_str(),
            lua.size(),
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

