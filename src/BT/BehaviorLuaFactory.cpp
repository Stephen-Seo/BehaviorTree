
#include "BehaviorLuaFactory.hpp"

#include <memory>
#include <iostream>
#include <cstring>
#include <vector>

#include "PriorityNode.hpp"
#include "SequenceNode.hpp"
#include "RandomNode.hpp"
#include "LoopNode.hpp"
#include "ConcurrentNode.hpp"
#include "ActionNode.hpp"
#include "CustomLuaNode.hpp"
#include "CustomLuaNodeState.hpp"

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

void BT::BehaviorLuaFactory::exposeLuaLibrary(int (*luaopen_)(lua_State *),
    const char* moduleName)
{
    libs.insert(std::make_pair(std::string(moduleName), luaopen_));

    if(LWrapper)
    {
        luaL_requiref(LWrapper->L, moduleName, luaopen_, 1); // +1
        lua_pop(LWrapper->L, 1); // -1
    }
}

void BT::BehaviorLuaFactory::exposeLuaLibrary(int (*luaopen_)(lua_State *),
    std::string moduleName)
{
    libs.insert(std::make_pair(moduleName, luaopen_));

    if(LWrapper)
    {
        luaL_requiref(LWrapper->L, moduleName.c_str(), luaopen_, 1); // +1
        lua_pop(LWrapper->L, 1); // -1
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

BT::BehaviorNode::Ptr BT::BehaviorLuaFactory::createTreeFromScript(const char* luaScript, std::size_t size)
{
    if(!LWrapper)
    {
        initializeLuaState();
    }

    int type = luaL_loadbuffer(
        LWrapper->L,
        luaScript,
        size,
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

BT::LuaStateWrapper::Ptr BT::BehaviorLuaFactory::getLuaStateWrapper()
{
    if(!LWrapper)
    {
        initializeLuaState();
    }

    return LWrapper;
}

void BT::BehaviorLuaFactory::resetLuaState()
{
    LWrapper.reset();
}

void BT::BehaviorLuaFactory::clearLuaCFunctions()
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
    for(const auto& pair : libs)
    {
        luaL_requiref(LWrapper->L, pair.first.c_str(),
            pair.second, 1); // +1
        lua_pop(LWrapper->L, 1); // -1
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
        bool gotSeed = false;
        std::size_t seed = 0;
        // +1 stack: field "seed"
        type = lua_getfield(LWrapper->L, -2, "seed");
        if(type != LUA_TNIL)
        {
            if(lua_isinteger(LWrapper->L, -1) == 1)
            {
                gotSeed = true;
                seed = lua_tointeger(LWrapper->L, -1);
            }
            else
            {
                if(!isSilent)
                {
                    std::cerr << "ERROR: Field \"seed\" specified for random "
                        "node but is invalid type! Using defaults (randomized "
                        "seed)\n";
                }
            }
        }
        else
        {
            if(!isSilent)
            {
                std::cerr << "WARNING: Field \"seed\" not specified, using "
                    "defaults (randomized seed)\n";
            }
        }
        lua_pop(LWrapper->L, 1); // -1 stack

        // +1 stack: field "randomType"
        type = lua_getfield(LWrapper->L, -2, "randomType");
        if(type == LUA_TSTRING)
        {
            const char* rtype = lua_tostring(LWrapper->L, -1);
            if(std::strcmp(rtype, "minstd_rand0") == 0)
            {
                if(gotSeed)
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<std::minstd_rand0>(seed));
                }
                else
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<std::minstd_rand0>());
                }
            }
            else if(std::strcmp(rtype, "minstd_rand") == 0)
            {
                if(gotSeed)
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<std::minstd_rand>(seed));
                }
                else
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<std::minstd_rand>());
                }
            }
            else if(std::strcmp(rtype, "mt19937") == 0)
            {
                if(gotSeed)
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<std::mt19937>(seed));
                }
                else
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<std::mt19937>());
                }
            }
            else if(std::strcmp(rtype, "mt19937_64") == 0)
            {
                if(gotSeed)
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<std::mt19937_64>(seed));
                }
                else
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<std::mt19937_64>());
                }
            }
            else if(std::strcmp(rtype, "ranlux24_base") == 0)
            {
                if(gotSeed)
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<std::ranlux24_base>(seed));
                }
                else
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<std::ranlux24_base>());
                }
            }
            else if(std::strcmp(rtype, "ranlux48_base") == 0)
            {
                if(gotSeed)
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<std::ranlux48_base>(seed));
                }
                else
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<std::ranlux48_base>());
                }
            }
            else if(std::strcmp(rtype, "ranlux24") == 0)
            {
                if(gotSeed)
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<std::ranlux24>(seed));
                }
                else
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<std::ranlux24>());
                }
            }
            else if(std::strcmp(rtype, "ranlux48") == 0)
            {
                if(gotSeed)
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<std::ranlux48>(seed));
                }
                else
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<std::ranlux48>());
                }
            }
            else if(std::strcmp(rtype, "knuth_b") == 0)
            {
                if(gotSeed)
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<std::knuth_b>(seed));
                }
                else
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<std::knuth_b>());
                }
            }
            else if(std::strcmp(rtype, "default") == 0)
            {
                if(gotSeed)
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<>(seed));
                }
                else
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<>());
                }
            }
            else
            {
                if(!isSilent)
                {
                    std::cerr << "ERROR: Field \"randomType\" has invalid "
                        "string! Using defaults\n";
                }

                if(gotSeed)
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<>(seed));
                }
                else
                {
                    ptr = BehaviorNode::Ptr(new BT::RandomNode<>());
                }
            }
        }
        else if(type != LUA_TNIL)
        {
            if(!isSilent)
            {
                std::cerr << "ERROR: Field \"randomType\" specified for random "
                    "node but is invalid type! Using defaults\n";
            }

            if(gotSeed)
            {
                ptr = BehaviorNode::Ptr(new BT::RandomNode<>(seed));
            }
            else
            {
                ptr = BehaviorNode::Ptr(new BT::RandomNode<>());
            }
        }
        else
        {
            if(!isSilent)
            {
                std::cerr << "WARNING: Field \"randomType\" not specified, "
                    "using defaults\n";
            }

            if(gotSeed)
            {
                ptr = BehaviorNode::Ptr(new BT::RandomNode<>(seed));
            }
            else
            {
                ptr = BehaviorNode::Ptr(new BT::RandomNode<>());
            }
        }
        lua_pop(LWrapper->L, 1); // -1 stack
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
        // -1 stack: field "type"
        lua_pop(LWrapper->L, 1);

        std::unique_ptr<BT::ActionNode> an{};
        LuaStateWrapper::Ptr newLWrapper{};
        // +1 stack: field "sharedState"
        type = lua_getfield(LWrapper->L, -1, "sharedState");
        if(type == LUA_TBOOLEAN && lua_toboolean(LWrapper->L, -1) == 0)
        {
            newLWrapper = LuaStateWrapper::Ptr(new LuaStateWrapper());
            an = std::unique_ptr<BT::ActionNode>(new BT::ActionNode(newLWrapper));

            for(auto iter = functions.begin(); iter != functions.end(); ++iter)
            {
                lua_register(newLWrapper->L, iter->first.c_str(), iter->second);
            }
            for(const auto& pair : libs)
            {
                luaL_requiref(newLWrapper->L, pair.first.c_str(),
                    pair.second, 1); // +1
                lua_pop(newLWrapper->L, 1); // -1
            }
        }
        else
        {
            an = std::unique_ptr<BT::ActionNode>(new BT::ActionNode(LWrapper));
        }
        lua_pop(LWrapper->L, 1); // -1 stack

        // +1 stack
        type = lua_getfield(LWrapper->L, -1, "id");
        if(type == LUA_TSTRING)
        {
            an->setID(std::string(lua_tostring(LWrapper->L, -1)));
        }
        lua_pop(LWrapper->L, 1); // -1 stack

        // +1 stack: field "actionFunction"
        type = lua_getfield(LWrapper->L, -1, "actionFunction");
        if(type != LUA_TFUNCTION)
        {
            if(!isSilent)
            {
                std::cerr << "ERROR: Field \"actionFunction\" is not a function!\n";
            }
            lua_pop(LWrapper->L, 1);
            return ptr;
        }
        else // is type LUA_TFUNCTION
        {
            if(newLWrapper)
            {
                std::vector<char> buffer;
                buffer.reserve(1024);
                lua_dump(LWrapper->L, BT::writerForTransferringFunction, &buffer, 1);
                lua_pop(LWrapper->L, 1); // -1 stack
                luaL_loadbufferx(newLWrapper->L, &buffer[0], buffer.size(),
                    "transferFunction", "b");
                lua_setglobal(newLWrapper->L, an->getLuaActionFunctionName().c_str());
            }
            else
            {
                // -1 stack
                lua_setglobal(LWrapper->L, an->getLuaActionFunctionName().c_str());
            }
        }

        ptr = BehaviorNode::Ptr(an.release());
        return ptr;
    }
    else if(std::strcmp(nodeType, "custom") == 0)
    {
        LuaStateWrapper::Ptr newLWrapper = std::make_shared<LuaStateWrapper>();
        std::unique_ptr<CustomLuaNode> clnPtr(new CustomLuaNode(newLWrapper));
        lua_pushstring(newLWrapper->L, BT_CUSTOM_LUA_NODE_STATE_REGISTRY_INDEX);
        lua_pushinteger(newLWrapper->L, (lua_Integer)clnPtr->getState());
        lua_settable(newLWrapper->L, LUA_REGISTRYINDEX);

        for(auto iter = functions.begin(); iter != functions.end(); ++iter)
        {
            lua_register(newLWrapper->L, iter->first.c_str(), iter->second);
        }
        for(const auto& pair : libs)
        {
            luaL_requiref(newLWrapper->L, pair.first.c_str(),
                pair.second, 1); // +1
            lua_pop(newLWrapper->L, 1); // -1
        }

        lua_register(newLWrapper->L, "getChildrenSize", BT::getChildrenSize);
        lua_register(newLWrapper->L, "activateChild", BT::activateChild);

        type = lua_getfield(LWrapper->L, -2, "activate");
        if(type != LUA_TFUNCTION)
        {
            if(!isSilent)
            {
                std::cerr << "ERROR: custom node field \"activate\" is not a function!\n";
                lua_pop(LWrapper->L, 2);
                return ptr;
            }
        }
        std::vector<char> buffer;
        buffer.reserve(1024);
        lua_dump(LWrapper->L, BT::writerForTransferringFunction, &buffer, 1);
        lua_pop(LWrapper->L, 1);
        luaL_loadbufferx(newLWrapper->L, &buffer[0], buffer.size(),
            "transferFunction", "b");
        lua_setglobal(newLWrapper->L, "activate");

        ptr = BehaviorNode::Ptr(clnPtr.release());
    }
    else
    {
        return ptr;
    }
    lua_pop(LWrapper->L, 1); // -1 stack

    // +1 stack
    type = lua_getfield(LWrapper->L, -1, "id");
    if(type == LUA_TSTRING)
    {
        ptr->setID(std::string(lua_tostring(LWrapper->L, -1)));
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
        return BT::BehaviorNode::Ptr();
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

