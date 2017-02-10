
#ifndef BT_RANDOM_NODE
#define BT_RANDOM_NODE

#include "LogicNode.hpp"

#include <random>

namespace BT
{

/**
    For default use, declare as type "RandomEngine<>"
*/
template <typename RandomEngine = std::mt19937>
class RandomNode : public LogicNode
{
public:
    RandomNode();
    RandomNode(typename RandomEngine::result_type seed);
    virtual ~RandomNode();

    typename RandomEngine::result_type getSeed() const;
    void setSeed(typename RandomEngine::result_type seed);

protected:
    virtual State performAction() override;
    virtual State continueAction() override;

private:
    typename RandomEngine::result_type seed;
    RandomEngine rengine;

};

} // namespace BT

#include "RandomNode.inl"

#endif

