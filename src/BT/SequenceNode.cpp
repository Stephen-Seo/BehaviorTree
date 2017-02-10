
#include "SequenceNode.hpp"

BT::SequenceNode::SequenceNode() :
LogicNode()
{}

BT::SequenceNode::~SequenceNode()
{}

BT::BehaviorNode::Ptr BT::SequenceNode::getCopy()
{
    std::unique_ptr<SequenceNode> copy(new SequenceNode());

    copy->parent = parent;
    copy->state = State{};

    for(std::size_t i = 0; i < children.size(); ++i)
    {
        copy->insert(children[i]->getCopy());
    }

    return Ptr(copy.release());
}

BT::BehaviorNode::State BT::SequenceNode::performAction()
{
    bool failed = false;
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
            failed = true;
            break;
        case State::ERROR:
        default:
            state.stateType = State::ERROR;
            return state;
        }
    }

    if(failed)
    {
        state.stateType = State::FAILED;
    }
    else
    {
        state.stateType = State::READY_SUCCESS;
    }
    return state;
}

BT::BehaviorNode::State BT::SequenceNode::continueAction()
{
    bool failed = false;
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
            failed = true;
            break;
        case State::ERROR:
        default:
            state.stateType = State::ERROR;
            return state;
        }
    }

    if(failed)
    {
        state.stateType = State::FAILED;
    }
    else
    {
        state.stateType = State::READY_SUCCESS;
    }
    return state;
}

