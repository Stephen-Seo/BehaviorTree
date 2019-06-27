
#ifndef BT_CONCURRENT_NODE
#define BT_CONCURRENT_NODE

#include "LogicNode.hpp"

namespace BT
{

/*!
    \brief Iterates through child nodes, stop on running or fail or error
*/
class ConcurrentNode : public LogicNode
{
public:
    ConcurrentNode();
    virtual ~ConcurrentNode();

    virtual Ptr getCopy() override;

private:
    virtual StateType performAction() override;
    virtual StateType continueAction() override;

};

} // namespace BT

#endif

