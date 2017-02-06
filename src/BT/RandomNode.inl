
template <typename RandomEngine>
BT::RandomNode<RandomEngine>::RandomNode() :
LogicNode(),
rengine(std::random_device()())
{}

template <typename RandomEngine>
BT::RandomNode<RandomEngine>::RandomNode(RandomEngine::result_type seed) :
LogicNode(),
rengine(seed)
{}

template <typename RandomEngine>
BT::RandomNode<RandomEngine>::~RandomNode()
{}

template <typename RandomEngine>
BT::BehaviorNode::State BT::RandomNode<RandomEngine>::performAction()
{
    if(children.empty())
    {
        state.stateType = State::ERROR;
        return state;
    }
    std::uniform_int_distribution<std::size_t> dist(0, children.size() - 1);
    std::size_t index = dist(rengine);
    state = children[index]->activate();
    if(state.stateType == State::RUNNING)
    {
        state.lastRunningIndex = index;
    }
    return state;
}

template <typename RandomEngine>
BT::BehaviorNode::State BT::RandomNode<RandomEngine>::continueAction()
{
    State cState = children[state.lastRunningIndex]->activate();
    state.stateType = cState.stateType;

    return state;
}

