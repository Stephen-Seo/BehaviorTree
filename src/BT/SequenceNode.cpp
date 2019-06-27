
#include "SequenceNode.hpp"

BT::SequenceNode::SequenceNode() :
LogicNode()
{}

BT::SequenceNode::~SequenceNode()
{}

BT::BehaviorNode::Ptr BT::SequenceNode::getCopy()
{
    std::unique_ptr<SequenceNode> copy(new SequenceNode());

    for(std::size_t i = 0; i < children.size(); ++i)
    {
        copy->insert(children[i]->getCopy());
    }

    return Ptr(copy.release());
}

BT::BehaviorNode::StateType BT::SequenceNode::performAction()
{
    bool failed = false;
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
            failed = true;
            break;
        case StateType::BT_ERROR:
        default:
            state.stateType = StateType::BT_ERROR;
            return state.stateType;
        }
    }

    if(failed)
    {
        state.stateType = StateType::BT_FAILED;
    }
    else
    {
        state.stateType = StateType::BT_READY_SUCCESS;
    }
    return state.stateType;
}

BT::BehaviorNode::StateType BT::SequenceNode::continueAction()
{
    bool failed = false;
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
            failed = true;
            break;
        case StateType::BT_ERROR:
        default:
            state.stateType = StateType::BT_ERROR;
            return state.stateType;
        }
    }

    if(failed)
    {
        state.stateType = StateType::BT_FAILED;
    }
    else
    {
        state.stateType = StateType::BT_READY_SUCCESS;
    }
    return state.stateType;
}

