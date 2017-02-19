
#ifndef BT_PRIORITY_NODE
#define BT_PRIORITY_NODE

#include "LogicNode.hpp"

namespace BT
{

/*!
    \brief Iterates through child nodes, stopping on success or running or error

    Will return success if no nodes stop execution regardless of nodes that
    return fail.
*/
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

