
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

BT::BehaviorNode::StateType BT::ConcurrentNode::performAction()
{
    for(std::size_t i = 0; i < children.size(); ++i)
    {
        state.stateType = children[i]->activate();
        switch(state.stateType)
        {
        case StateType::BT_READY_SUCCESS:
            break;
        case StateType::BT_RUNNING:
            state.lastRunningIndex = i;
            return state.stateType;
        case StateType::BT_FAILED:
        case StateType::BT_ERROR:
            return state.stateType;
        default:
            state.stateType = StateType::BT_ERROR;
            return state.stateType;
        }
    }

    // should be READY_SUCCESS
    return state.stateType;
}

BT::BehaviorNode::StateType BT::ConcurrentNode::continueAction()
{
    for(std::size_t i = state.lastRunningIndex; i < children.size(); ++i)
    {
        state.stateType = children[i]->activate();
        switch(state.stateType)
        {
        case StateType::BT_READY_SUCCESS:
            break;
        case StateType::BT_RUNNING:
            state.lastRunningIndex = i;
            return state.stateType;
        case StateType::BT_FAILED:
        case StateType::BT_ERROR:
            return state.stateType;
        default:
            state.stateType = StateType::BT_ERROR;
            return state.stateType;
        }
    }

    // should be READY_SUCCESS
    return state.stateType;
}

