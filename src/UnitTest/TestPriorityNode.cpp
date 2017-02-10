
#include "gtest/gtest.h"

#include <BT/PriorityNode.hpp>
#include <BT/ActionNode.hpp>

using namespace BT;

TEST(BehaviorTree, PriorityNode)
{
    std::size_t sCount = 0;
    std::size_t fCount = 0;
    bool runningOnceDidRunOnce = false;

    const auto reset = [&sCount, &fCount, &runningOnceDidRunOnce] () {
        sCount = 0;
        fCount = 0;
        runningOnceDidRunOnce = false;
    };
    const auto alwaysSuccess = [&sCount] (bool isContinuing) {
        ++sCount;
        return BehaviorNode::State::READY_SUCCESS;
    };
    const auto alwaysFail = [&fCount] (bool isContinuing) {
        ++fCount;
        return BehaviorNode::State::FAILED;
    };
    const auto runningOnce = [&runningOnceDidRunOnce] (bool isContinuing) {
        if(!runningOnceDidRunOnce)
        {
            runningOnceDidRunOnce = true;
            return BehaviorNode::State::RUNNING;
        }
        else
        {
            return BehaviorNode::State::READY_SUCCESS;
        }
    };
    const auto runningOnceThenFail = [&runningOnceDidRunOnce] (bool isContinuing) {
        if(!runningOnceDidRunOnce)
        {
            runningOnceDidRunOnce = true;
            return BehaviorNode::State::RUNNING;
        }
        else
        {
            return BehaviorNode::State::FAILED;
        }
    };

    {
        PriorityNode pn{};

        pn.insert(std::unique_ptr<BehaviorNode>(new ActionNode(alwaysFail)));
        pn.insert(std::unique_ptr<BehaviorNode>(new ActionNode(alwaysFail)));
        pn.insert(std::unique_ptr<BehaviorNode>(new ActionNode(alwaysSuccess)));
        pn.insert(std::unique_ptr<BehaviorNode>(new ActionNode(alwaysFail)));
        pn.insert(std::unique_ptr<BehaviorNode>(new ActionNode(alwaysSuccess)));

        pn.activate();

        EXPECT_EQ(sCount, 1);
        EXPECT_EQ(fCount, 2);
    }
    reset();

    {
        PriorityNode pn{};

        pn.insert(std::unique_ptr<BehaviorNode>(new ActionNode(alwaysFail)));
        pn.insert(std::unique_ptr<BehaviorNode>(new ActionNode(runningOnceThenFail)));
        pn.insert(std::unique_ptr<BehaviorNode>(new ActionNode(alwaysFail)));

        pn.activate();
        EXPECT_EQ(sCount, 0);
        EXPECT_EQ(fCount, 1);
        EXPECT_TRUE(runningOnceDidRunOnce);

        pn.activate();
        EXPECT_EQ(sCount, 0);
        EXPECT_EQ(fCount, 2);
        EXPECT_TRUE(runningOnceDidRunOnce);
    }
    reset();
}

