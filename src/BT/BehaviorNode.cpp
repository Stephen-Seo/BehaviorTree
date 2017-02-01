
#include "BehaviorNode.hpp"

#include <utility>

BT::BehaviorNode::State::State() :
stateType(READY_SUCCESS),
lastRunning(nullptr)
{}

BT::BehaviorNode::State::State(StateType stateType, BehaviorNode* lastRunning) :
stateType(stateType),
lastRunning(lastRunning)
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

