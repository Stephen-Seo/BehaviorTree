
#include "gtest/gtest.h"

#include <BT/PriorityNode.hpp>
#include <BT/ActionNode.hpp>

using namespace BT;

TEST(BehaviorTree, GetCopy)
{
    std::size_t sCount = 0;
    std::size_t fCount = 0;

    const auto alwaysFail = [&fCount] (bool /*isContinuing*/) {
        ++fCount;
        return BehaviorNode::State::FAILED;
    };
    const auto alwaysSuccess = [&sCount] (bool /*isContinuing*/) {
        ++sCount;
        return BehaviorNode::State::READY_SUCCESS;
    };

    PriorityNode pn;

    pn.insert(BehaviorNode::Ptr(new ActionNode(alwaysFail)));
    pn.insert(BehaviorNode::Ptr(new ActionNode(alwaysFail)));
    pn.insert(BehaviorNode::Ptr(new ActionNode(alwaysSuccess)));

    BehaviorNode::Ptr copy(pn.getCopy());
    copy->activate();

    EXPECT_EQ(sCount, 1);
    EXPECT_EQ(fCount, 2);
}

