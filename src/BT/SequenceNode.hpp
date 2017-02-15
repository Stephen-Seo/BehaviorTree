
#ifndef BT_SEQUENCE_NODE
#define BT_SEQUENCE_NODE

#include "LogicNode.hpp"

namespace BT
{

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

