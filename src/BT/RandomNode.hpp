
#ifndef BT_RANDOM_NODE
#define BT_RANDOM_NODE

#include "LogicNode.hpp"

#include <random>

namespace BT
{

template <typename RandomEngine = std::mt19937>
class RandomNode : public LogicNode
{
public:
    RandomNode();
    RandomNode(RandomEngine::result_type seed);
    virtual ~RandomNode();

protected:
    virtual State performAction() override;
    virtual State continueAction() override;

private:
    RandomEngine rengine;

};

} // namespace BT

#include "RandomNode.inl"

#endif

