
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

BT::BehaviorNode::State::StateType BT::PriorityNode::performAction()
{
    resetState();

    State::StateType cStateType;
    for(std::size_t i = 0; i < children.size(); ++i)
    {
        cStateType = children[i]->activate();
        switch(cStateType)
        {
        case State::READY_SUCCESS:
            // state should be default (READY_SUCCESS)
            return state.stateType;
        case State::RUNNING:
            state.stateType = State::RUNNING;
            state.lastRunningIndex = i;
            return state.stateType;
        case State::FAILED:
            break;
        case State::ERROR:
        default:
            state.stateType = State::ERROR;
            return state.stateType;
        }
    }

    // here state should also be default (READY_SUCCESS)
    return state.stateType;
}

BT::BehaviorNode::State::StateType BT::PriorityNode::continueAction()
{
    State::StateType cStateType;
    for(std::size_t i = state.lastRunningIndex; i < children.size(); ++i)
    {
        cStateType = children[i]->activate();
        switch(cStateType)
        {
        case State::READY_SUCCESS:
            state.stateType = State::READY_SUCCESS;
            return state.stateType;
        case State::RUNNING:
            state.stateType = State::RUNNING;
            state.lastRunningIndex = i;
            return state.stateType;
        case State::FAILED:
            break;
        case State::ERROR:
        default:
            state.stateType = State::ERROR;
            return state.stateType;
        }
    }
    state.stateType = State::READY_SUCCESS;
    return state.stateType;
}

