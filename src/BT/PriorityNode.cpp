
#include "PriorityNode.hpp"

BT::PriorityNode::PriorityNode() :
LogicNode()
{}

BT::PriorityNode::~PriorityNode()
{}

BT::BehaviorNode::Ptr BT::PriorityNode::getCopy()
{
    std::unique_ptr<PriorityNode> copy(new PriorityNode());

    for(std::size_t i = 0; i < children.size(); ++i)
    {
        copy->insert(children[i]->getCopy());
    }

    return Ptr(copy.release());
}

BT::BehaviorNode::State BT::PriorityNode::performAction()
{
    resetState();

    State cState{};
    for(std::size_t i = 0; i < children.size(); ++i)
    {
        cState = children[i]->activate();
        switch(cState.stateType)
        {
        case State::READY_SUCCESS:
            // state should be default (READY_SUCCESS)
            return state;
        case State::RUNNING:
            state.stateType = State::RUNNING;
            state.lastRunningIndex = i;
            return state;
        case State::FAILED:
        case State::ERROR:
        default:
            break;
        }
    }

    // here state should also be default (READY_SUCCESS)
    return state;
}

BT::BehaviorNode::State BT::PriorityNode::continueAction()
{
    State cState{};
    for(std::size_t i = state.lastRunningIndex; i < children.size(); ++i)
    {
        cState = children[i]->activate();
        switch(cState.stateType)
        {
        case State::READY_SUCCESS:
            state.stateType = State::READY_SUCCESS;
            return state;
        case State::RUNNING:
            state.stateType = State::RUNNING;
            state.lastRunningIndex = i;
            return state;
        case State::FAILED:
        case State::ERROR:
        default:
            break;
        }
    }
    state.stateType = State::READY_SUCCESS;
    return state;
}

