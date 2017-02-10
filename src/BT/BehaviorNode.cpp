
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

BT::BehaviorNode::BehaviorNode(const BehaviorNode& other)
{
    parent = other.parent;
    state = other.state;
    children.clear();

    for(std::size_t i = 0; i < other.children.size(); ++i)
    {
        insert(other.children[i]->getCopy());
    }
}

BT::BehaviorNode& BT::BehaviorNode::operator = (const BehaviorNode& other)
{
    parent = other.parent;
    state = other.state;
    children.clear();

    for(std::size_t i = 0; i < other.children.size(); ++i)
    {
        insert(other.children[i]->getCopy());
    }

    return *this;
}

void BT::BehaviorNode::insert(Ptr& node)
{
    node->parent = this;
    children.push_back(std::move(node));
}

void BT::BehaviorNode::insert(Ptr&& node)
{
    node->parent = this;
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

