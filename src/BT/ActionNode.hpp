
#ifndef BT_ACTION_NODE
#define BT_ACTION_NODE

#include "BehaviorNode.hpp"

#include <functional>
#include <string>
#include <unordered_map>

#include <lua.hpp>

#include "LuaStateWrapper.hpp"

namespace BT
{

class ActionNode : public BehaviorNode
{
public:
    // bool - isContinuing
    typedef std::function<State::StateType(bool)> ActionFunctionT;

    ActionNode();
    ActionNode(ActionFunctionT actionFunction);
    ActionNode(std::string lua, bool isFilename = false);
    virtual ~ActionNode();

    virtual Ptr getCopy() override;

    void setActionFunction(ActionFunctionT actionFunction);
    void setLuaActionFunction(std::string lua, bool isFilename = false);

    void exposeFunctionToLuaScript(int (*function)(lua_State* L), const char* name);
    void exposeFunctionToLuaScript(int (*function)(lua_State* L), std::string name);

protected:
    ActionFunctionT actionFunction;
    LuaStateWrapper::Ptr LWrapper;
    std::string lua;
    bool luaIsFilename;
    std::unordered_map<std::string, int (*)(lua_State*)> exposedFunctions;

    virtual State performAction() override;
    virtual State continueAction() override;

private:
    void initializeLuaState();
    State performLuaScript(bool isContinuing);

};

} // namespace BT

#endif

