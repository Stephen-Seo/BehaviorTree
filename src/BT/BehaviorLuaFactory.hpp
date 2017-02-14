
#ifndef BT_BEHAVIOR_LUA_FACTORY_HPP
#define BT_BEHAVIOR_LUA_FACTORY_HPP

#include <lua.hpp>
#include <string>
#include <unordered_map>

#include "LuaStateWrapper.hpp"
#include "BehaviorNode.hpp"

namespace BT
{

class BehaviorLuaFactory
{
public:
    BehaviorLuaFactory(bool isSilentNotVerbose = true);
    ~BehaviorLuaFactory();

    // copy
    BehaviorLuaFactory(const BehaviorLuaFactory& other);
    BehaviorLuaFactory& operator = (const BehaviorLuaFactory& other);

    void exposeFunction(int (*function)(lua_State*), const char* name);
    void exposeFunction(int (*function)(lua_State*), std::string name);
    BehaviorNode::Ptr createTreeFromFile(std::string luaFilename);
    BehaviorNode::Ptr createTreeFromScript(std::string luaScript);

    lua_State* getLuaState();
    void resetLuaState();
    void resetLuaCFunctions();
private:
    LuaStateWrapper::Ptr LWrapper;
    bool isSilent;
    std::unordered_map<std::string, int(*)(lua_State*)> functions;

    void initializeLuaState();
    BehaviorNode::Ptr createTree();
    BehaviorNode::Ptr createTreeHelper();

};

} // namespace BT

#endif

