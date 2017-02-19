
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

/*!
    \brief A node that performs an action when activated

    It is expected to have every leaf of a BehaviorTree to be an ActionNode.
    The action it performs can be defined manually by setting the action
    function with setActionFunction() or it can be defined by the lua script
    that generates the tree containing instance(s) of this node.
    See the documentation for BT::BehaviorLuaFactory for more info on
    generating a BehaviorTree.

    One could manually register C functions to the lua state provided an id
    was set for this node (to find it in the tree).
    ~~~{.cpp}
    // A C function registered with lua must take the lua state as a parameter
    // and return the number of results it has (pushed onto the lua state's
    // stack).
    // Note this function takes one parameter, and receives it from the lua
    // state's stack.
    int printStuff(lua_State* L)
    {
        const char* str = lua_tostring(L, -1);
        std::cout << str << std::endl;
        return 0;
    }

    int doOtherStuff(lua_State* L)
    {
        // do other stuff
        return 0;
    }

    BT::BehaviorLuaFactory factory;
    factory.exposeFunction(printStuff, "printStuff");
    BT::BehaviorNode::Ptr tree = factory.createTreeFromFile("script.lua");

    BT::BehaviorNode* anode = tree->findByID("actionNode0");
    lua_register(anode->getLuaState(), "doOtherStuff", doOtherStuff);

    tree->activate();
    ~~~

    Note that it often isn't enough to expose a function to the lua state
    because no state is saved. Consider using "lua_getextraspace" to
    assign a struct/class to a void pointer that is available to any
    function using the same lua state. The src/UnitTest/TestLuaFactory.cpp
    provides an example of doing so.
*/
class ActionNode : public BehaviorNode
{
public:
    // bool - isContinuing
    typedef std::function<State::StateType(bool)> ActionFunctionT;

    /*!
        \brief Default initializes ActionNode, not in valid state

        ActionNode is only valid if it has been assigned an action function
        or a lua state containing an action function at a global variable
        with the same name as getLuaActionFunctionName().
    */
    ActionNode();

    /*!
        \brief Initializes ActionNode with the given action function
    */
    ActionNode(ActionFunctionT actionFunction);

    /*!
        \brief Initializes ActionNode with a lua state

        ActionNode will expect a global function with the same name as
        getLuaActionFunctionName().
    */
    ActionNode(const LuaStateWrapper::Ptr& LWrapper);
    virtual ~ActionNode();

    virtual Ptr getCopy() override;

    /*!
        \brief Sets the action function for this node

        An action function set in C++ has precedence over an action function
        in a given lua state.
    */
    void setActionFunction(ActionFunctionT actionFunction);

    /*!
        \brief Sets the lua state that holds an action function

        ActionNode will expect a global function with the same name as
        getLuaActionFunctionName() in the lua state. If there isn't one
        or if it is invalid, this node will return error on activate().
    */
    void setLuaState(const LuaStateWrapper::Ptr& LWrapper);

    /*!
        \brief Gets the luaActionFunctionName of this node

        Each ActionNode has a unique luaActionFunctionName which is
        created during construction of the node.
    */
    const std::string& getLuaActionFunctionName() const;

    /*!
        \brief Gets the lua state of this node

        If there is no lua state associated with this node, then this function
        will return null.
    */
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

