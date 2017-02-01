
#include "BehaviorNode.hpp"

#include <utility>

BT::BehaviorNode::State::State() :
stateType(READY_SUCCESS),
lastRunningIndex(0)
{}

BT::BehaviorNode::State::State(StateType stateType, std::size_t lastRunningIndex) :
stateType(stateType),
lastRunningIndex(lastRunningIndex)
{}

BT::BehaviorNode::BehaviorNode() :
parent(nullptr),
children(),
state()
{}

BT::BehaviorNode::~BehaviorNode()
{}

void BT::BehaviorNode::insert(Ptr& node)
{
    children.push_back(std::move(node));
}

void BT::BehaviorNode::insert(Ptr&& node)
{
    children.push_back(std::forward<Ptr>(node));
}

void BT::BehaviorNode::resetState()
{
    state = State();
}

BT::BehaviorNode::State BT::BehaviorNode::activate()
{
    if(state.stateType == State::RUNNING)
    {
        return continueAction();
    }
    else
    {
        return performAction();
    }
}

