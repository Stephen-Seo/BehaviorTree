
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

BT::BehaviorNode::StateType BT::PriorityNode::performAction()
{
    resetState();

    StateType cStateType;
    for(std::size_t i = 0; i < children.size(); ++i)
    {
        cStateType = children[i]->activate();
        switch(cStateType)
        {
        case StateType::BT_READY_SUCCESS:
            // state should be default (READY_SUCCESS)
            return state.stateType;
        case StateType::BT_RUNNING:
            state.stateType = StateType::BT_RUNNING;
            state.lastRunningIndex = i;
            return state.stateType;
        case StateType::BT_FAILED:
            break;
        case StateType::BT_ERROR:
        default:
            state.stateType = StateType::BT_ERROR;
            return state.stateType;
        }
    }

    // here state should also be default (READY_SUCCESS)
    return state.stateType;
}

BT::BehaviorNode::StateType BT::PriorityNode::continueAction()
{
    StateType cStateType;
    for(std::size_t i = state.lastRunningIndex; i < children.size(); ++i)
    {
        cStateType = children[i]->activate();
        switch(cStateType)
        {
        case StateType::BT_READY_SUCCESS:
            state.stateType = StateType::BT_READY_SUCCESS;
            return state.stateType;
        case StateType::BT_RUNNING:
            state.stateType = StateType::BT_RUNNING;
            state.lastRunningIndex = i;
            return state.stateType;
        case StateType::BT_FAILED:
            break;
        case StateType::BT_ERROR:
        default:
            state.stateType = StateType::BT_ERROR;
            return state.stateType;
        }
    }
    state.stateType = StateType::BT_READY_SUCCESS;
    return state.stateType;
}

