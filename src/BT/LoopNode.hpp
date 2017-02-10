
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

private:
    virtual State performAction();
    virtual State continueAction();

    void performLoop(std::size_t index = 0);

};

} // namespace BT

#endif

