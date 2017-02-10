
#include "LoopNode.hpp"

BT::LoopNode::LoopNode() :
LogicNode()
{}

BT::LoopNode::~LoopNode()
{}

BT::BehaviorNode::State BT::LoopNode::performAction()
{
    performLoop();

    return state;
}

BT::BehaviorNode::State BT::LoopNode::continueAction()
{
    performLoop(state.lastRunningIndex);

    return state;
}

void BT::LoopNode::performLoop(std::size_t index)
{
    bool isRunningOrFail = false;
    do
    {
        state = children[index]->activate();
        if(state.stateType == State::RUNNING)
        {
            state.lastRunningIndex = index;
            isRunningOrFail = true;
        }
        else if(state.stateType == State::FAILED
            || state.stateType == State::ERROR)
        {
            isRunningOrFail = true;
        }
        index = (index + 1) % children.size();
    } while(!isRunningOrFail);
}

