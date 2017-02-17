
#ifndef BT_ACTION_NODE
#define BT_ACTION_NODE

#define BT_ACTION_NODE_ACTION_FUNCTION_PREFIX "BT_ActionNodeActionFunction_"

#include "BehaviorNode.hpp"

#include <functional>
#include <string>
#include <unordered_map>
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
    ActionNode(const LuaStateWrapper::Ptr& LWrapper);
    virtual ~ActionNode();

    virtual Ptr getCopy() override;

    void setActionFunction(ActionFunctionT actionFunction);
    void setLuaState(const LuaStateWrapper::Ptr& LWrapper);

    const std::string& getLuaActionFunctionName() const;
    lua_State* getLuaState();

protected:
    ActionFunctionT actionFunction;
    LuaStateWrapper::Ptr LWrapper;
    std::string luaActionFunctionName;
    static std::atomic_size_t luaScriptIDCounter;

    virtual State::StateType performAction() override;
    virtual State::StateType continueAction() override;

private:
    State::StateType performLuaScript(bool isContinuing);

};

} // namespace BT

#endif

