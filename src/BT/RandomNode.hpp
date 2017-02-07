
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

    RandomEngine::result_type getSeed() const;
    void setSeed(RandomEngine::result_type seed);

protected:
    virtual State performAction() override;
    virtual State continueAction() override;

private:
    RandomEngine::result_type seed;
    RandomEngine rengine;

};

} // namespace BT

#include "RandomNode.inl"

#endif

