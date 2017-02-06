
#include "SequenceNode.hpp"

BT::SequenceNode::SequenceNode() :
LogicNode()
{}

BT::SequenceNode::~SequenceNode()
{}

BT::BehaviorNode::State BT::SequenceNode::performAction()
{
    state = State{};

    State cState;
    for(std::size_t i = 0; i < children.size(); ++i)
    {
        cState = children[i]->activate();
        switch(cState.stateType)
        {
        case State::READY_SUCCESS:
            break;
        case State::RUNNING:
            state.lastRunningIndex = i;
            state.stateType = State::RUNNING;
            return state;
        case State::FAILED:
            state.stateType = State::FAILED;
            return state;
        case State::ERROR:
        default:
            state.stateType = State::ERROR;
            return state;
        }
    }

    // at this point, state should be default
    return state;
}

BT::BehaviorNode::State BT::SequenceNode::continueAction()
{
    State cState;
    for(std::size_t i = state.lastRunningIndex; i < children.size(); ++i)
    {
        cState = children[i]->activate();
        switch(cState.stateType)
        {
        case State::READY_SUCCESS:
            break;
        case State::RUNNING:
            state.lastRunningIndex = i;
            state.stateType = State::RUNNING;
            return state;
        case State::FAILED:
            state.stateType = State::FAILED;
            return state;
        case State::ERROR:
        default:
            state.stateType = State::ERROR;
            return state;
        }
    }

    state.stateType = State::READY_SUCCESS;
    return state;
}

