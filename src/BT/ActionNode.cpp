
#include "ActionNode.hpp"

BT::ActionNode::ActionNode() :
BehaviorNode(),
actionFunction()
{}

BT::ActionNode::ActionNode(ActionFunctionT actionFunction) :
BehaviorNode(),
actionFunction(actionFunction)
{}

BT::ActionNode::~ActionNode()
{}

void BT::ActionNode::setActionFunction(ActionFunctionT actionFunction)
{
    this->actionFunction = actionFunction;
}

BT::BehaviorNode::State BT::ActionNode::performAction()
{
    if(actionFunction)
    {
        state.stateType = actionFunction(false);
        return state;
    }
    else
    {
        state.stateType = State::ERROR;
        return state;
    }
}

BT::BehaviorNode::State BT::ActionNode::continueAction()
{
    if(actionFunction)
    {
        state.stateType = actionFunction(true);
        return state;
    }
    else
    {
        state.stateType = State::ERROR;
        return state;
    }
}

