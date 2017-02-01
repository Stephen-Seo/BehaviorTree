
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

protected:
    virtual State performAction() override;
    virtual State continueAction() override;

};

} // namespace BT

#endif

