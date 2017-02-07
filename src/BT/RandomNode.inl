
template <typename RandomEngine>
BT::RandomNode<RandomEngine>::RandomNode() :
LogicNode(),
seed(std::random_device()()),
rengine()
{
    rengine.seed(seed);
}

template <typename RandomEngine>
BT::RandomNode<RandomEngine>::RandomNode(typename RandomEngine::result_type seed) :
LogicNode(),
seed(seed),
rengine(seed)
{}

template <typename RandomEngine>
BT::RandomNode<RandomEngine>::~RandomNode()
{}

template <typename RandomEngine>
typename RandomEngine::result_type BT::RandomNode<RandomEngine>::getSeed() const
{
    return seed;
}

template <typename RandomEngine>
void BT::RandomNode<RandomEngine>::setSeed(typename RandomEngine::result_type seed)
{
    this->seed = seed;
    rengine.seed(seed);
}

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
    if(children.empty())
    {
        state.stateType = State::ERROR;
        return state;
    }
    State cState = children[state.lastRunningIndex]->activate();
    state.stateType = cState.stateType;

    return state;
}

