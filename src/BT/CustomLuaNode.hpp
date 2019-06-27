
#ifndef BT_CUSTOM_LUA_NODE
#define BT_CUSTOM_LUA_NODE

#include "LogicNode.hpp"

#include <string>

#include "LuaStateWrapper.hpp"
#include "CustomLuaNodeState.hpp"

namespace BT
{

/*!
    \brief A custom node with behavior defined by a lua script

    Note that this node is created with a lua state different from
    the factory that created it. The only thing similar between the two lua
    states is that the factory exposes functions to this node's lua state
    that was already exposed to the factory's lua state.
*/
class CustomLuaNode : public LogicNode
{
public:
    CustomLuaNode(LuaStateWrapper::Ptr LWrapper);
    virtual ~CustomLuaNode();

    virtual Ptr getCopy() override;

    /*!
        \brief Returns the number of child node this node has

        Used internally by a different function exposed to this node's lua
        state. (see BT::CustomLuaNodeState.hpp)
    */
    std::size_t getChildrenSize() const;

    /*!
        \brief Activates a child at the given index and returns the result

        Used internally by a different function exposed to this node's lua
        state. (see BT::CustomLuaNodeState.hpp)
    */
    StateType activateChild(std::size_t index);

    /*!
        \brief Returns this node's CustomLuaNodeState as a pointer

        This node's lua state's registry table has an entry that is the ptr
        to this CustomLuaNodeState stored as a lua_Integer in the table.
        The index is BT_CUSTOM_LUA_NODE_STATE_REGISTRY_INDEX, which is defined
        in CustomLuaNodeState.hpp .
    */
    CustomLuaNodeState* getState();

    /*!
        \brief Returns this node's lua state
    */
    lua_State* getLuaState();
protected:
    LuaStateWrapper::Ptr LWrapper;
    CustomLuaNodeState state;

    virtual StateType performAction() override;
    virtual StateType continueAction() override;

    virtual void getLuaStatesHelper(std::unordered_set<lua_State*>& s) override;

private:
    StateType action(bool isContinuing);
};

} // namespace BT

#endif

