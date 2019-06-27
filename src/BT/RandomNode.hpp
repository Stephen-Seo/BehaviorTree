
#ifndef BT_RANDOM_NODE
#define BT_RANDOM_NODE

#include "LogicNode.hpp"

#include <random>

namespace BT
{

/*!
    \brief Randomly picks one child node and returns the result of activating it

    For default use, declare as type "RandomEngine<>"
*/
template <typename RandomEngine = std::default_random_engine>
class RandomNode : public LogicNode
{
public:
    RandomNode();
    RandomNode(typename RandomEngine::result_type seed);
    virtual ~RandomNode();

    virtual Ptr getCopy() override;

    typename RandomEngine::result_type getSeed() const;
    void setSeed(typename RandomEngine::result_type seed);

protected:
    virtual StateType performAction() override;
    virtual StateType continueAction() override;

private:
    typename RandomEngine::result_type seed;
    RandomEngine rengine;

};

} // namespace BT

#include "RandomNode.inl"

#endif

