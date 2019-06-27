
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
            return BehaviorNode::StateType::BT_RUNNING;
        }
        else
        {
            return BehaviorNode::StateType::BT_READY_SUCCESS;
        }
    });

    for(std::size_t i = 0; i < 3; ++i)
    {
        BehaviorNode::StateType stateType = an.activate();
        if(i != 2)
        {
            EXPECT_EQ(stateType, BehaviorNode::StateType::BT_RUNNING);
        }
        else
        {
            EXPECT_EQ(stateType, BehaviorNode::StateType::BT_READY_SUCCESS);
        }
    }

    an.setActionFunction([] (bool isContinuing) {
        static bool continuedOnce = false;

        if(!continuedOnce)
        {
            if(!isContinuing)
            {
                return BehaviorNode::StateType::BT_RUNNING;
            }
            else
            {
                continuedOnce = true;
                return BehaviorNode::StateType::BT_READY_SUCCESS;
            }
        }
        else
        {
            return BehaviorNode::StateType::BT_READY_SUCCESS;
        }
    });

    EXPECT_EQ(an.activate(), BehaviorNode::StateType::BT_RUNNING);
    EXPECT_EQ(an.activate(), BehaviorNode::StateType::BT_READY_SUCCESS);
    EXPECT_EQ(an.activate(), BehaviorNode::StateType::BT_READY_SUCCESS);
}

