
#ifndef BT_LOOP_NODE
#define BT_LOOP_NODE

#include "LogicNode.hpp"

namespace BT
{

class LoopNode : public LogicNode
{
public:
    LoopNode();
    virtual ~LoopNode();

    virtual Ptr getCopy() override;

private:
    virtual State::StateType performAction() override;
    virtual State::StateType continueAction() override;

    void performLoop(std::size_t index = 0);

};

} // namespace BT

#endif

