
#ifndef BT_LOGIC_NODE
#define BT_LOGIC_NODE

#include "BehaviorNode.hpp"

namespace BT
{

class LogicNode : public BehaviorNode
{
public:
    LogicNode() :
    BehaviorNode()
    {}

    virtual ~LogicNode() {}

};

} // namespace BT

#endif

