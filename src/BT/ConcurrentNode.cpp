
#include "ConcurrentNode.hpp"

BT::ConcurrentNode::ConcurrentNode() :
LogicNode()
{}

BT::ConcurrentNode::~ConcurrentNode()
{}

BT::BehaviorNode::Ptr BT::ConcurrentNode::getCopy()
{
    std::unique_ptr<ConcurrentNode> copy(new ConcurrentNode());

    for(std::size_t i = 0; i < children.size(); ++i)
    {
        copy->insert(children[i]->getCopy());
    }

    return Ptr(copy.release());
}

BT::BehaviorNode::State::StateType BT::ConcurrentNode::performAction()
{
    for(std::size_t i = 0; i < children.size(); ++i)
    {
        state.stateType = children[i]->activate();
        switch(state.stateType)
        {
        case State::READY_SUCCESS:
            break;
        case State::RUNNING:
            state.lastRunningIndex = i;
            return state.stateType;
        case State::FAILED:
        case State::ERROR:
            return state.stateType;
        default:
            state.stateType = State::ERROR;
            return state.stateType;
        }
    }

    // should be READY_SUCCESS
    return state.stateType;
}

BT::BehaviorNode::State::StateType BT::ConcurrentNode::continueAction()
{
    for(std::size_t i = state.lastRunningIndex; i < children.size(); ++i)
    {
        state.stateType = children[i]->activate();
        switch(state.stateType)
        {
        case State::READY_SUCCESS:
            break;
        case State::RUNNING:
            state.lastRunningIndex = i;
            return state.stateType;
        case State::FAILED:
        case State::ERROR:
            return state.stateType;
        default:
            state.stateType = State::ERROR;
            return state.stateType;
        }
    }

    // should be READY_SUCCESS
    return state.stateType;
}

