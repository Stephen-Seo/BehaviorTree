
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

    void exposeFunction(int (*function)(lua_State* L), const char* name);
    void exposeFunction(int (*function)(lua_State* L), std::string name);
    BehaviorNode::Ptr createTreeFromFile(std::string luaFilename);
    BehaviorNode::Ptr createTreeFromScript(std::string luaScript);
private:
    LuaStateWrapper::Ptr LWrapper;
    bool isSilent;
    std::unordered_map<std::string, int(*)(lua_State* L)> functions;

    BehaviorNode::Ptr createTree();
    BehaviorNode::Ptr createTreeHelper();

};

} // namespace BT

#endif

