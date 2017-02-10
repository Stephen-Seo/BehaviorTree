
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

    virtual Ptr getCopy() override;

private:
    virtual State performAction() override;
    virtual State continueAction() override;

};

} // namespace BT

#endif

