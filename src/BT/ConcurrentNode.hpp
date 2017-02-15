
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
    virtual State::StateType performAction() override;
    virtual State::StateType continueAction() override;

};

} // namespace BT

#endif

