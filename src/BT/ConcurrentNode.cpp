
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

BT::BehaviorNode::State BT::ConcurrentNode::performAction()
{
    for(std::size_t i = 0; i < children.size(); ++i)
    {
        state = children[i]->activate();
        switch(state.stateType)
        {
        case State::READY_SUCCESS:
            break;
        case State::RUNNING:
            state.lastRunningIndex = i;
            return state;
        case State::FAILED:
        case State::ERROR:
            return state;
        default:
            state.stateType = State::ERROR;
            return state;
        }
    }

    // should be READY_SUCCESS
    return state;
}

BT::BehaviorNode::State BT::ConcurrentNode::continueAction()
{
    for(std::size_t i = state.lastRunningIndex; i < children.size(); ++i)
    {
        state = children[i]->activate();
        switch(state.stateType)
        {
        case State::READY_SUCCESS:
            break;
        case State::RUNNING:
            state.lastRunningIndex = i;
            return state;
        case State::FAILED:
        case State::ERROR:
            return state;
        default:
            state.stateType = State::ERROR;
            return state;
        }
    }

    // should be READY_SUCCESS
    return state;
}
