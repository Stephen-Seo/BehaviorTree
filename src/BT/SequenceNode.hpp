
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

protected:
    virtual State performAction() override;
    virtual State continueAction() override;

};

} // namespace BT

#endif

