
#ifndef BT_SEQUENCE_NODE
#define BT_SEQUENCE_NODE

#include "LogicNode.hpp"

namespace BT
{

/*!
    \brief Iterates through nodes, stop on running or error

    Returns success if all child nodes return success, otherwise
    returns fail.
*/
class SequenceNode : public LogicNode
{
public:
    SequenceNode();
    virtual ~SequenceNode();

    virtual Ptr getCopy() override;

protected:
    virtual State::StateType performAction() override;
    virtual State::StateType continueAction() override;

};

} // namespace BT

#endif

