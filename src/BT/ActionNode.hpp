
#ifndef BT_ACTION_NODE
#define BT_ACTION_NODE

#define BT_ACTION_NODE_ACTION_FUNCTION_PREFIX "BT_ActionNodeActionFunction_"

#include "BehaviorNode.hpp"

#include <functional>
#include <string>
#include <unordered_map>
#include <bitset>
#include <atomic>

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
    ActionNode(std::string lua, const LuaStateWrapper::Ptr& LWrapper, bool isFilename = false);
    virtual ~ActionNode();

    virtual Ptr getCopy() override;

    void setActionFunction(ActionFunctionT actionFunction);
    void setLuaActionFunction(std::string lua, const LuaStateWrapper::Ptr& LWrapper, bool isFilename = false);

protected:
    ActionFunctionT actionFunction;
    LuaStateWrapper::Ptr LWrapper;
    std::string lua;
    /*
        0 - lua is file
        1 - lua script loaded
    */
    std::bitset<2> flags;
    std::string luaActionFunctionName;
    static std::atomic_size_t luaScriptIDCounter;

    virtual State performAction() override;
    virtual State continueAction() override;

private:
    State performLuaScript(bool isContinuing);

};

} // namespace BT

#endif

