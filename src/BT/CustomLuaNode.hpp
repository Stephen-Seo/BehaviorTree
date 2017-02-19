
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
    State::StateType activateChild(std::size_t index);

    /*!
        \brief Returns this node's CustomLuaNodeState as a pointer

        This node's lua state's "extraspace" (lua_getextraspace) is set to
        this node's CustomLuaNodeState. Thus CustomLuaNodeState's void pointer
        can be set to whatever is necessary.
    */
    CustomLuaNodeState* getState();

    /*!
        \brief Returns this node's lua state
    */
    lua_State* getLuaState();
protected:
    LuaStateWrapper::Ptr LWrapper;
    CustomLuaNodeState state;

    virtual State::StateType performAction() override;
    virtual State::StateType continueAction() override;
private:
    State::StateType action(bool isContinuing);
};

} // namespace BT

#endif

