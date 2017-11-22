
#include "BehaviorNode.hpp"

#include <utility>

BT::BehaviorNode::State::State() :
stateType(READY_SUCCESS),
lastRunningIndex(0)
{}

BT::BehaviorNode::State::State(StateType stateType, std::size_t lastRunningIndex) :
stateType(stateType),
lastRunningIndex(lastRunningIndex)
{}

BT::BehaviorNode::BehaviorNode() :
children(),
state(),
id(BT_BEHAVIOR_NODE_DEFAULT_ID)
{}

BT::BehaviorNode::~BehaviorNode()
{}

BT::BehaviorNode::BehaviorNode(const BehaviorNode& other)
{
    state = other.state;
    id = other.id;
    children.clear();

    for(std::size_t i = 0; i < other.children.size(); ++i)
    {
        insert(other.children[i]->getCopy());
    }
}

BT::BehaviorNode& BT::BehaviorNode::operator = (const BehaviorNode& other)
{
    state = other.state;
    id = other.id;
    children.clear();

    for(std::size_t i = 0; i < other.children.size(); ++i)
    {
        insert(other.children[i]->getCopy());
    }

    return *this;
}

void BT::BehaviorNode::insert(Ptr& node)
{
    if(node)
    {
        children.push_back(std::move(node));
    }
}

void BT::BehaviorNode::insert(Ptr&& node)
{
    if(node)
    {
        children.push_back(std::forward<Ptr>(node));
    }
}

void BT::BehaviorNode::resetState()
{
    state = State();
}

BT::BehaviorNode::State::StateType BT::BehaviorNode::activate()
{
    if(state.stateType == State::RUNNING)
    {
        return continueAction();
    }
    else
    {
        return performAction();
    }
}

void BT::BehaviorNode::setID(std::string id)
{
    this->id = id;
}

const std::string& BT::BehaviorNode::getID() const
{
    return id;
}

BT::BehaviorNode* BT::BehaviorNode::findByID(std::string id)
{
    if(this->id == id)
    {
        return this;
    }
    else
    {
        BehaviorNode* ptr = nullptr;
        for(auto iter = children.begin(); iter != children.end(); ++iter)
        {
            ptr = (*iter)->findByID(id);
            if(ptr)
            {
                break;
            }
        }
        return ptr;
    }
}

std::vector<lua_State*> BT::BehaviorNode::getLuaStatesInTree()
{
    std::vector<lua_State*> states;

    getLuaStatesHelper(states);

    return states;
}

void BT::BehaviorNode::getLuaStatesHelper(std::vector<lua_State*>& v)
{
    for(auto& child : children)
    {
        child->getLuaStatesHelper(v);
    }
}

