
#ifndef BT_CUSTOM_LUA_NODE
#define BT_CUSTOM_LUA_NODE

#include "LogicNode.hpp"

#include <string>

#include "LuaStateWrapper.hpp"
#include "CustomLuaNodeState.hpp"

namespace BT
{

class CustomLuaNode : public LogicNode
{
public:
    CustomLuaNode(LuaStateWrapper::Ptr LWrapper);
    virtual ~CustomLuaNode();

    virtual Ptr getCopy() override;
    std::size_t getChildrenSize() const;
    State::StateType activateChild(std::size_t index);
    CustomLuaNodeState* getState();
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

