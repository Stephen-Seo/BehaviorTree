
#include "gtest/gtest.h"

#include <random>
#include <bitset>
#include <memory>
#include <iostream>

#include <BT/RandomNode.hpp>
#include <BT/ActionNode.hpp>

TEST(BehaviorTree, RandomNode)
{
    std::default_random_engine rengine(0);
    std::uniform_int_distribution<unsigned int> dist(0, 3);
    std::bitset<4> flags;

    // Next time, just use "auto" instead of "std::function"
    const std::function<void()> resetFlags = [&flags] () {
        for(unsigned int i = 0; i < 4; ++i)
        {
            flags.reset(i);
        }
    };

    const std::function<void()> atLeastOneIsTrue = [&flags] () {
        bool allFalse = true;
        for(unsigned int i = 0; i < 4; ++i)
        {
            if(flags.test(i))
            {
                allFalse = false;
            }
        }
        EXPECT_FALSE(allFalse);
    };

    const std::function<void(unsigned int)> checkFlags = [&flags] (unsigned int e) {
        for(unsigned int i = 0; i < 4; ++i)
        {
            if(i == e)
            {
                std::cout << i << " is expected true" << std::endl;
                EXPECT_TRUE(flags.test(i));
            }
            else
            {
                EXPECT_FALSE(flags.test(i));
            }
        }
    };

    {
        std::cout << "Begin block 0" << std::endl;
        BT::RandomNode<> rn(0);
        
        for(unsigned int i = 0; i < 4; ++i)
        {
            BT::BehaviorNode::Ptr an(
                std::unique_ptr<BT::ActionNode>(
                    new BT::ActionNode(
                        [&flags, i] (bool /*isContinuing*/)
                            -> BT::BehaviorNode::StateType {
                flags.set(i);
                return BT::BehaviorNode::StateType::BT_READY_SUCCESS;
            })));
            rn.insert(std::move(an));
        }

        EXPECT_FALSE(flags.test(0));
        EXPECT_FALSE(flags.test(1));
        EXPECT_FALSE(flags.test(2));
        EXPECT_FALSE(flags.test(3));

        unsigned int nextIndex = 0;
        
        for(unsigned int i = 0; i < 10; ++i)
        {
            nextIndex = dist(rengine);
            EXPECT_EQ(rn.activate(), BT::BehaviorNode::StateType::BT_READY_SUCCESS);
            checkFlags(nextIndex);
            atLeastOneIsTrue();
            resetFlags();
        }
        std::cout << "End block 0" << std::endl;
    }

    {
        std::cout << "Begin block 1" << std::endl;
        BT::RandomNode<> rn(1);
        std::bitset<4> hasRun;

        for(unsigned int i = 0; i < 4; ++i)
        {
            BT::BehaviorNode::Ptr an(
                std::unique_ptr<BT::ActionNode>(
                    new BT::ActionNode(
                        [&flags, &hasRun, i] (bool /*isContinuing*/)
                            -> BT::BehaviorNode::StateType {
                if(hasRun.test(i))
                {
                    flags.set(i);
                    return BT::BehaviorNode::StateType::BT_READY_SUCCESS;
                }
                else
                {
                    hasRun.set(i);
                    return BT::BehaviorNode::StateType::BT_RUNNING;
                }
            })));
            rn.insert(std::move(an));
        }

        rengine.seed(1);
        dist = std::uniform_int_distribution<unsigned int>(0, 3);
        BT::BehaviorNode::StateType stateType;
        unsigned int nextIndex = 0;
        const unsigned int maxIterations = 1024;
        unsigned int iterations = 0;

        for(unsigned int i = 0; i < 10; ++i)
        {
            nextIndex = dist(rengine);
            iterations = 0;
            do {
                stateType = rn.activate();
                if(iterations++ > maxIterations)
                {
                    break;
                }
            }
            while(stateType == BT::BehaviorNode::StateType::BT_RUNNING);
            checkFlags(nextIndex);
            atLeastOneIsTrue();
            resetFlags();
        }
        std::cout << "End block 1" << std::endl;
    }
}

