
#include "ActionNode.hpp"

#include <memory>
#include <iostream>

std::atomic_size_t BT::ActionNode::luaScriptIDCounter(0);

BT::ActionNode::ActionNode() :
BehaviorNode(),
actionFunction(),
LWrapper(),
lua(),
flags(),
luaActionFunctionName()
{
    std::size_t luaScriptID = luaScriptIDCounter++;
    luaActionFunctionName = std::string(BT_ACTION_NODE_ACTION_FUNCTION_PREFIX)
        + std::to_string(luaScriptID);
}

BT::ActionNode::ActionNode(ActionFunctionT actionFunction) :
BehaviorNode(),
actionFunction(actionFunction),
LWrapper(),
lua(),
flags(),
luaActionFunctionName()
{
    std::size_t luaScriptID = luaScriptIDCounter++;
    luaActionFunctionName = std::string(BT_ACTION_NODE_ACTION_FUNCTION_PREFIX)
        + std::to_string(luaScriptID);
}

BT::ActionNode::ActionNode(std::string lua, const LuaStateWrapper::Ptr& LWrapper, bool isFilename) :
BehaviorNode(),
actionFunction(),
LWrapper(LWrapper),
lua(lua),
flags(),
luaActionFunctionName()
{
    flags.set(0, isFilename);
    std::size_t luaScriptID = luaScriptIDCounter++;
    luaActionFunctionName = std::string(BT_ACTION_NODE_ACTION_FUNCTION_PREFIX)
        + std::to_string(luaScriptID);
}

BT::ActionNode::~ActionNode()
{}

BT::BehaviorNode::Ptr BT::ActionNode::getCopy()
{
    std::unique_ptr<ActionNode> copy(new ActionNode());

    copy->actionFunction = actionFunction;
    copy->LWrapper = LWrapper;
    copy->lua = lua;
    copy->flags = flags;
    copy->luaActionFunctionName = luaActionFunctionName;

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

void BT::ActionNode::setLuaActionFunction(std::string lua, const LuaStateWrapper::Ptr& LWrapper, bool isFilename)
{
    this->lua= lua;
    flags.set(0, isFilename);
    this->LWrapper = LWrapper;
    flags.reset(1);
}

const std::string& BT::ActionNode::getLuaActionFunctionName() const
{
    return luaActionFunctionName;
}

lua_State* BT::ActionNode::getLuaState()
{
    if(LWrapper)
    {
        return LWrapper->L;
    }
    else
    {
        return nullptr;
    }
}

BT::BehaviorNode::State::StateType BT::ActionNode::performAction()
{
    if(actionFunction)
    {
        state.stateType = actionFunction(false);
        return state.stateType;
    }
    else if(!lua.empty())
    {
        return performLuaScript(false);
    }
    else
    {
        state.stateType = State::ERROR;
        return state.stateType;
    }
}

BT::BehaviorNode::State::StateType BT::ActionNode::continueAction()
{
    if(actionFunction)
    {
        state.stateType = actionFunction(true);
        return state.stateType;
    }
    else if(!lua.empty())
    {
        return performLuaScript(true);
    }
    else
    {
        state.stateType = State::ERROR;
        return state.stateType;
    }
}

BT::BehaviorNode::State::StateType BT::ActionNode::performLuaScript(bool isContinuing)
{
    if(!LWrapper)
    {
        state.stateType = State::ERROR;
        return state.stateType;
    }

    int type = 0;
    if(!flags[1])
    {
        if(flags[0])
        {
            type = luaL_loadfile(LWrapper->L, lua.c_str());
            if(type != LUA_OK)
            {
                std::cerr << "ERROR: Failed to load lua file in ActionNode!\n";
                if(type == LUA_ERRSYNTAX)
                {
                    std::cerr << "HINT: Appears to be syntax error!\n";
                }
                state.stateType = State::ERROR;
                return state.stateType;
            }
            else if(lua_pcall(LWrapper->L, 0, 0, 0) != LUA_OK)
            {
                std::cerr << "ERROR: Lua file failed to execute in ActionNode!\n";
                lua_pop(LWrapper->L, 1);
                state.stateType = State::ERROR;
                return state.stateType;
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
                std::cerr << "ERROR: Failed to load lua script in ActionNode!\n";
                if(type == LUA_ERRSYNTAX)
                {
                    std::cerr << "HINT: Appears to be syntax error!\n";
                }
                state.stateType = State::ERROR;
                return state.stateType;
            }
            else if(lua_pcall(LWrapper->L, 0, 0, 0) != LUA_OK)
            {
                std::cerr << "ERROR: Lua script failed to execute in ActionNode!\n";
                lua_pop(LWrapper->L, 1);
                state.stateType = State::ERROR;
                return state.stateType;
            }
        }

        type = lua_getglobal(LWrapper->L, "actionFunction");
        if(type != LUA_TFUNCTION)
        {
            std::cerr << "ERROR: Global \"actionFunction\" is not a function!\n"
                "HINT: Error ocurred during renaming to internal\n";
        }

        lua_setglobal(LWrapper->L, luaActionFunctionName.c_str());

        flags.set(1);
    }

    // +1 stack: function
    type = lua_getglobal(LWrapper->L, luaActionFunctionName.c_str());
    if(type != LUA_TFUNCTION)
    {
        std::cerr << "ERROR: Global \"actionFunction\" is not a function!\n";
        state.stateType = State::ERROR;
        return state.stateType;
    }

    // +1 stack: argument
    lua_pushboolean(LWrapper->L, isContinuing ? 1 : 0);
    
    // -2, +1 stack: function return value (int)
    if(lua_pcall(LWrapper->L, 1, 1, 0) != LUA_OK)
    {
        std::cerr << "ERROR: Failed to execute actionFunction in ActionNode's lua!\n";
        lua_pop(LWrapper->L, 1);
        state.stateType = State::ERROR;
        return state.stateType;
    }
    else if(lua_isinteger(LWrapper->L, -1) != 1)
    {
        lua_pop(LWrapper->L, 1);
        state.stateType = State::ERROR;
        return state.stateType;
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

    return state.stateType;
}

