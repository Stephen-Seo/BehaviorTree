
#ifndef BT_PRIORITY_NODE
#define BT_PRIORITY_NODE

#include "LogicNode.hpp"

namespace BT
{

class PriorityNode : public LogicNode
{
public:
    PriorityNode();
    virtual ~PriorityNode();

    virtual Ptr getCopy() override;

protected:
    virtual State::StateType performAction() override;
    virtual State::StateType continueAction() override;

};

} // namespace BT

#endif

