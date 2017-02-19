
#include "gtest/gtest.h"

#include <BT/ActionNode.hpp>

using namespace BT;

TEST(BehaviorTree, ActionNode)
{
    ActionNode an;

    an.setActionFunction([] (bool /*isContinuing*/) {
        static unsigned int count = 0;

        if(count < 2)
        {
            ++count;
            return BehaviorNode::State::RUNNING;
        }
        else
        {
            return BehaviorNode::State::READY_SUCCESS;
        }
    });

    for(std::size_t i = 0; i < 3; ++i)
    {
        BehaviorNode::State::StateType stateType = an.activate();
        if(i != 2)
        {
            EXPECT_EQ(stateType, BehaviorNode::State::RUNNING);
        }
        else
        {
            EXPECT_EQ(stateType, BehaviorNode::State::READY_SUCCESS);
        }
    }

    an.setActionFunction([] (bool isContinuing) {
        static bool continuedOnce = false;

        if(!continuedOnce)
        {
            if(!isContinuing)
            {
                return BehaviorNode::State::RUNNING;
            }
            else
            {
                continuedOnce = true;
                return BehaviorNode::State::READY_SUCCESS;
            }
        }
        else
        {
            return BehaviorNode::State::READY_SUCCESS;
        }
    });

    EXPECT_EQ(an.activate(), BehaviorNode::State::RUNNING);
    EXPECT_EQ(an.activate(), BehaviorNode::State::READY_SUCCESS);
    EXPECT_EQ(an.activate(), BehaviorNode::State::READY_SUCCESS);
}

