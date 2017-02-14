
#include "BehaviorLuaFactory.hpp"

#include <memory>
#include <iostream>
#include <cstring>

#include "PriorityNode.hpp"
#include "SequenceNode.hpp"
#include "RandomNode.hpp"
#include "LoopNode.hpp"
#include "ConcurrentNode.hpp"
#include "ActionNode.hpp"

BT::BehaviorLuaFactory::BehaviorLuaFactory(bool isSilentNotVerbose) :
LWrapper(),
isSilent(isSilentNotVerbose)
{
}

BT::BehaviorLuaFactory::~BehaviorLuaFactory()
{
}

BT::BehaviorLuaFactory::BehaviorLuaFactory(const BehaviorLuaFactory& other) :
LWrapper(),
isSilent(other.isSilent),
functions(other.functions)
{
    // do not copy LWrapper
}

BT::BehaviorLuaFactory& BT::BehaviorLuaFactory::operator = (const BehaviorLuaFactory& other)
{
    // do not copy LWrapper
    isSilent = other.isSilent;
    functions = other.functions;

    return *this;
}

void BT::BehaviorLuaFactory::exposeFunction(int (*function)(lua_State*), const char* name)
{
    functions.insert(std::make_pair(std::string(name), function));

    if(LWrapper)
    {
        lua_pushcfunction(LWrapper->L, function);
        lua_setglobal(LWrapper->L, name);
    }
}

void BT::BehaviorLuaFactory::exposeFunction(int (*function)(lua_State*), std::string name)
{
    functions.insert(std::make_pair(name, function));

    if(LWrapper)
    {
        lua_pushcfunction(LWrapper->L, function);
        lua_setglobal(LWrapper->L, name.c_str());
    }
}

BT::BehaviorNode::Ptr BT::BehaviorLuaFactory::createTreeFromFile(std::string luaFilename)
{
    if(!LWrapper)
    {
        initializeLuaState();
    }

    int type = luaL_loadfile(LWrapper->L, luaFilename.c_str());
    if(type != LUA_OK)
    {
        if(!isSilent)
        {
            std::cerr << "ERROR: Failed to load lua file!\n";
            if(type == LUA_ERRSYNTAX)
            {
                std::cerr << "HINT: Appears to be syntax error\n";
            }
        }
        return BT::BehaviorNode::Ptr();
    }
    else if(lua_pcall(LWrapper->L, 0, 0, 0) != LUA_OK)
    {
        if(!isSilent)
        {
            std::cerr << "ERROR: Failed to execute lua file!\n";
        }
        lua_pop(LWrapper->L, 1);
        return BT::BehaviorNode::Ptr();
    }

    return createTree();
}

BT::BehaviorNode::Ptr BT::BehaviorLuaFactory::createTreeFromScript(std::string luaScript)
{
    if(!LWrapper)
    {
        initializeLuaState();
    }

    int type = luaL_loadbuffer(
        LWrapper->L,
        luaScript.c_str(),
        luaScript.size(),
        "BT::BehaviorLuaFactory::createTreeFromScript"
    );
    if(type != LUA_OK)
    {
        if(!isSilent)
        {
            std::cerr << "ERROR: Failed to load lua script!\n";
            if(type == LUA_ERRSYNTAX)
            {
                std::cerr << "HINT: Appears to be syntax error\n";
            }
        }
        return BT::BehaviorNode::Ptr();
    }
    else if(lua_pcall(LWrapper->L, 0, 0, 0) != LUA_OK)
    {
        if(!isSilent)
        {
            std::cerr << "ERROR: Failed to execute lua script!\n";
        }
        lua_pop(LWrapper->L, 1);
        return BT::BehaviorNode::Ptr();
    }

    return createTree();
}

lua_State* BT::BehaviorLuaFactory::getLuaState()
{
    if(!LWrapper)
    {
        initializeLuaState();
    }

    return LWrapper->L;
}

void BT::BehaviorLuaFactory::resetLuaState()
{
    LWrapper.reset();
}

void BT::BehaviorLuaFactory::resetLuaCFunctions()
{
    functions.clear();
}

void BT::BehaviorLuaFactory::initializeLuaState()
{
    LWrapper = std::make_shared<LuaStateWrapper>();

    for(auto iter = functions.begin(); iter != functions.end(); ++iter)
    {
        lua_pushcfunction(LWrapper->L, iter->second);
        lua_setglobal(LWrapper->L, iter->first.c_str());
    }
}

BT::BehaviorNode::Ptr BT::BehaviorLuaFactory::createTree()
{
    // +1 stack: global "BehaviorTree"
    int type = lua_getglobal(LWrapper->L, "BehaviorTree");
    if(type != LUA_TTABLE)
    {
        lua_pop(LWrapper->L, 1);
        if(!isSilent)
        {
            std::cerr << "ERROR: Failed to get global \"BehaviorTree\"!\n";
        }
        return BT::BehaviorNode::Ptr();
    }

    BT::BehaviorNode::Ptr ptr = createTreeHelper();
    lua_pop(LWrapper->L, 1); // -1 stack: global "BehaviorTree"

    return ptr;
}

BT::BehaviorNode::Ptr BT::BehaviorLuaFactory::createTreeHelper()
{
    BT::BehaviorNode::Ptr ptr;
    // +1 stack: field "type"
    int type = lua_getfield(LWrapper->L, -1, "type");
    if(type != LUA_TSTRING)
    {
        lua_pop(LWrapper->L, 1);
        if(!isSilent)
        {
            std::cerr << "ERROR: Field \"type\" is not a string!\n";
        }
        return ptr;
    }

    const char* nodeType = lua_tostring(LWrapper->L, -1);
    if(std::strcmp(nodeType, "priority") == 0)
    {
        ptr = BehaviorNode::Ptr(new BT::PriorityNode());
    }
    else if(std::strcmp(nodeType, "sequence") == 0)
    {
        ptr = BehaviorNode::Ptr(new BT::SequenceNode());
    }
    else if(std::strcmp(nodeType, "random") == 0)
    {
        ptr = BehaviorNode::Ptr(new BT::SequenceNode());
    }
    else if(std::strcmp(nodeType, "loop") == 0)
    {
        ptr = BehaviorNode::Ptr(new BT::LoopNode());
    }
    else if(std::strcmp(nodeType, "concurrent") == 0)
    {
        ptr = BehaviorNode::Ptr(new BT::ConcurrentNode());
    }
    else if(std::strcmp(nodeType, "action") == 0)
    {
        std::string actionFunction{};
        std::string actionFunctionName{};
        // -1 stack: field "type"
        lua_pop(LWrapper->L, 1);
        // +1 stack: field "actionFunction"
        type = lua_getfield(LWrapper->L, -1, "actionFunction");
        if(type == LUA_TNIL)
        {
            if(!isSilent)
            {
                std::cerr << "WARNING: Field \"actionFunction\" is nil, trying \"actionFunctionName\"...\n";
            }
            lua_pop(LWrapper->L, 1); // -1 stack: field "actionFunction"
            // +1 stack: field "actionFunctionName"
            type = lua_getfield(LWrapper->L, -1, "actionFunctionName");
            if(type != LUA_TSTRING)
            {
                if(!isSilent)
                {
                    std::cerr << "ERROR: Field \"actionFunctionName\" is not a string!\n";
                }
                lua_pop(LWrapper->L, 1);
                return ptr;
            }
            actionFunctionName = std::string(lua_tostring(LWrapper->L, -1));
            lua_pop(LWrapper->L, 1); // -1 stack: field "actionFunctionName"
        }
        else if(type != LUA_TSTRING)
        {
            if(!isSilent)
            {
                std::cerr << "ERROR: Field \"actionFunction\" is not a string!\n";
            }
            lua_pop(LWrapper->L, 1);
            return ptr;
        }
        else // is type LUA_TSTRING
        {
            actionFunction = std::string(lua_tostring(LWrapper->L, -1));
            // -1 stack
            lua_pop(LWrapper->L, 1);
        }

        std::unique_ptr<BT::ActionNode> an;
        if(!actionFunction.empty())
        {
            an = std::unique_ptr<BT::ActionNode>(new BT::ActionNode(actionFunction, LWrapper));
        }
        else if(!actionFunctionName.empty())
        {
            an = std::unique_ptr<BT::ActionNode>(new BT::ActionNode(actionFunctionName, LWrapper, true));
        }

        ptr = BehaviorNode::Ptr(an.release());

        return ptr;
    }
    else
    {
        return ptr;
    }
    lua_pop(LWrapper->L, 1); // -1 stack

    // +1 stack: field "children"
    type = lua_getfield(LWrapper->L, -1, "children");
    if(type != LUA_TTABLE)
    {
        if(!isSilent)
        {
            std::cerr << "ERROR: Field \"children\" is not a table!\n";
        }
        lua_pop(LWrapper->L, 1);
        return ptr;
    }

    int i = 1;
    while(lua_geti(LWrapper->L, -1, i++) == LUA_TTABLE)
    {
        // +1 stack: per iter of loop

        ptr->insert(createTreeHelper());
        lua_pop(LWrapper->L, 1); // -1 stack
    }
    lua_pop(LWrapper->L, 1); // -1 stack: field "children"

    return ptr;
}

