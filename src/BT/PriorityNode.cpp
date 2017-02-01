
#include "PriorityNode.hpp"

BT::PriorityNode::PriorityNode() :
LogicNode()
{}

BT::PriorityNode::~PriorityNode()
{}

BT::BehaviorNode::State BT::PriorityNode::performAction()
{
    resetState();

    for(std::size_t i = 0; i < children.size(); ++i)
    {
        State cState = children[i]->activate();
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

    return state;
}

BT::BehaviorNode::State BT::PriorityNode::continueAction()
{
    State cState{};
    for(std::size_t i = state.lastRunningIndex; i < children.size(); ++i)
    {
        if(i == state.lastRunningIndex)
        {
            cState = children[i]->activate();
            switch(cState.stateType)
            {
            case State::READY_SUCCESS:
                state.stateType = State::READY_SUCCESS;
                return state;
            case State::RUNNING:
                // state should already be set to this child's info
                return state;
            case State::FAILED:
            case State::ERROR:
            default:
                break;
            }
        }
        else
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
    }
    state.stateType = State::READY_SUCCESS;
    return state;
}

