
#include "PriorityNode.hpp"

BT::PriorityNode::PriorityNode() :
LogicNode()
{}

BT::PriorityNode::~PriorityNode()
{}

BT::BehaviorNode::State BT::PriorityNode::performAction()
{
    resetState();

    for(Ptr& child : children)
    {
        State cState = child->activate();
        switch(cState.stateType)
        {
        case State::READY_SUCCESS:
            // state should be default (READY_SUCCESS)
            return state;
        case State::RUNNING:
            state.stateType = State::RUNNING;
            state.lastRunning = child.get();
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
    BehaviorNode* lastRunning = state.lastRunning;
    if(lastRunning)
    {
        bool continued = false;
        State cState{};
        for(Ptr& child : children)
        {
            if(!continued && child.get() == lastRunning)
            {
                continued = true;
                cState = child->activate();
                switch(cState.stateType)
                {
                case State::READY_SUCCESS:
                    state.stateType = State::READY_SUCCESS;
                    state.lastRunning = nullptr;
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
            else if(continued)
            {
                cState = child->activate();
                switch(cState.stateType)
                {
                case State::READY_SUCCESS:
                    state.stateType = State::READY_SUCCESS;
                    state.lastRunning = nullptr;
                    return state;
                case State::RUNNING:
                    state.stateType = State::RUNNING;
                    state.lastRunning = child.get();
                    return state;
                case State::FAILED:
                case State::ERROR:
                default:
                    break;
                }
            }
        }
        if(!continued)
        {
            state.stateType = State::ERROR;
            state.lastRunning = nullptr;
            return state;
        }
        else
        {
            state.stateType = State::READY_SUCCESS;
            state.lastRunning = nullptr;
            return state;
        }
    }
    else
    {
        state.stateType = State::ERROR;
        state.lastRunning = nullptr;
        return state;
    }
}

