
#ifndef BT_CONCURRENT_NODE
#define BT_CONCURRENT_NODE

#include "LogicNode.hpp"

namespace BT
{

class ConcurrentNode : public LogicNode
{
public:
    ConcurrentNode();
    virtual ~ConcurrentNode();

private:
    virtual State performAction();
    virtual State continueAction();

};

} // namespace BT

#endif

