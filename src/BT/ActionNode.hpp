
#ifndef BT_ACTION_NODE
#define BT_ACTION_NODE

#include "BehaviorNode.hpp"

#include <functional>

namespace BT
{

class ActionNode : public BehaviorNode
{
public:
    typedef std::function<State::StateType(bool)> ActionFunctionT;

    ActionNode();
    ActionNode(ActionFunctionT actionFunction);
    virtual ~ActionNode();

    virtual Ptr getCopy() override;

    void setActionFunction(ActionFunctionT actionFunction);

protected:
    // bool - isContinuing
    ActionFunctionT actionFunction;

    virtual State performAction() override;
    virtual State continueAction() override;

};

} // namespace BT

#endif

