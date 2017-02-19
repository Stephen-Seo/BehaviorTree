
#include "LoopNode.hpp"

BT::LoopNode::LoopNode() :
LogicNode()
{}

BT::LoopNode::~LoopNode()
{}

BT::BehaviorNode::Ptr BT::LoopNode::getCopy()
{
    std::unique_ptr<LoopNode> copy(new LoopNode());

    for(std::size_t i = 0; i < children.size(); ++i)
    {
        copy->insert(children[i]->getCopy());
    }

    return Ptr(copy.release());
}

BT::BehaviorNode::State::StateType BT::LoopNode::performAction()
{
    performLoop();

    return state.stateType;
}

BT::BehaviorNode::State::StateType BT::LoopNode::continueAction()
{
    performLoop(state.lastRunningIndex);

    return state.stateType;
}

void BT::LoopNode::performLoop(std::size_t index)
{
    bool isRunningOrFail = false;
    do
    {
        state.stateType = children[index]->activate();
        if(state.stateType == State::RUNNING)
        {
            state.lastRunningIndex = index;
            state.stateType = State::RUNNING;
            isRunningOrFail = true;
        }
        else if(state.stateType == State::FAILED)
        {
            state.stateType = State::FAILED;
            isRunningOrFail = true;
        }
        else if(state.stateType == State::ERROR)
        {
            state.stateType = State::ERROR;
            isRunningOrFail = true;
        }
        index = (index + 1) % children.size();
    } while(!isRunningOrFail);
}

