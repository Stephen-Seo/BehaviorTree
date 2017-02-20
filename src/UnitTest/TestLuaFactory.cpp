
#include "gtest/gtest.h"

#include <iostream>

#include <lua.hpp>

#include <BT/BehaviorLuaFactory.hpp>
#include <BT/ActionNode.hpp>

using namespace BT;

int printOut(lua_State* L)
{
    const char* str = lua_tostring(L, 1);

    std::cout << str << std::endl;

    return 0;
}

int a0 = 0;
int a1 = 0;
int a2 = 0;

int activate0(lua_State* /*L*/)
{
    ++a0;
    return 0;
}

int activate1(lua_State* /*L*/)
{
    ++a1;
    return 0;
}

int activate2(lua_State* /*L*/)
{
    ++a2;
    return 0;
}

struct A
{
    A() :
    state(0)
    {}

    int state;
};

int setState(lua_State* L)
{
    A* a = *((A**)lua_getextraspace(L));
    a->state = lua_tointeger(L, -1);

    return 0;
}

TEST(BehaviorLuaFactory, Factory)
{
    a0 = 0;
    a1 = 0;
    a2 = 0;
    BehaviorLuaFactory blf(false);

    blf.exposeFunction(printOut, "printOut");
    blf.exposeFunction(activate0, "activate0");
    blf.exposeFunction(activate1, "activate1");
    blf.exposeFunction(activate2, "activate2");

    {
        BehaviorNode::Ptr tree = blf.createTreeFromFile("TestLuaFactoryScript0.lua");
        ASSERT_TRUE(tree);

        EXPECT_EQ(tree->activate(), BehaviorNode::State::READY_SUCCESS);

        EXPECT_EQ(1, a0);
        EXPECT_EQ(1, a1);
        EXPECT_EQ(1, a2);

        EXPECT_EQ(tree->activate(), BehaviorNode::State::READY_SUCCESS);

        EXPECT_EQ(2, a0);
        EXPECT_EQ(2, a1);
        EXPECT_EQ(2, a2);
    }

    a0 = 0;
    {
        BehaviorNode::Ptr tree = blf.createTreeFromFile("TestLuaFactoryScript1.lua");
        ASSERT_TRUE(tree);

        EXPECT_EQ(tree->activate(), BehaviorNode::State::READY_SUCCESS);

        EXPECT_EQ(1, a0);
    }

    {
        A a{};
        *((A**)lua_getextraspace(blf.getLuaState())) = &a;

        EXPECT_EQ(0, a.state);

        blf.exposeFunction(setState, "setState");

        BehaviorNode::Ptr tree = blf.createTreeFromFile("TestLuaFactoryScript2.lua");
        ASSERT_TRUE(tree);

        EXPECT_EQ(tree->activate(), BehaviorNode::State::READY_SUCCESS);

        EXPECT_EQ(1, a.state);
    }
}

TEST(BehaviorLuaFactory, FactoryCustom)
{
    a0 = 0;
    a1 = 0;
    a2 = 0;

    BehaviorLuaFactory blf(false);

    blf.exposeFunction(printOut, "printOut");
    blf.exposeFunction(activate0, "activate0");
    blf.exposeFunction(activate1, "activate1");
    blf.exposeFunction(activate2, "activate2");

    {
        BehaviorNode::Ptr tree = blf.createTreeFromFile("TestLuaFactoryScript3.lua");
        ASSERT_TRUE(tree);

        EXPECT_EQ(tree->activate(), BehaviorNode::State::READY_SUCCESS);

        EXPECT_EQ(1, a0);
        EXPECT_EQ(1, a1);
        EXPECT_EQ(1, a2);
    }
}

TEST(BehaviorLuaFactory, ID)
{
    a0 = 0;
    a1 = 0;
    a2 = 0;

    BehaviorLuaFactory blf(false);

    blf.exposeFunction(printOut, "printOut");
    blf.exposeFunction(activate0, "activate0");
    blf.exposeFunction(activate1, "activate1");
    blf.exposeFunction(activate2, "activate2");

    {
        BehaviorNode::Ptr tree = blf.createTreeFromFile("TestLuaFactoryScript4.lua");
        ASSERT_TRUE(tree);

        BehaviorNode* ptr = tree->findByID("action0");
        ASSERT_TRUE(ptr);
        ptr->activate();
        EXPECT_EQ(1, a0);
        EXPECT_EQ(0, a1);
        EXPECT_EQ(0, a2);

        ptr = tree->findByID("action1");
        ASSERT_TRUE(ptr);
        ptr->activate();
        EXPECT_EQ(1, a0);
        EXPECT_EQ(1, a1);
        EXPECT_EQ(0, a2);

        ptr = tree->findByID("action2");
        ASSERT_TRUE(ptr);
        ptr->activate();
        EXPECT_EQ(1, a0);
        EXPECT_EQ(1, a1);
        EXPECT_EQ(1, a2);

        ptr = tree->findByID("root");
        ASSERT_TRUE(ptr);
        ptr->activate();
        EXPECT_EQ(2, a0);
        EXPECT_EQ(2, a1);
        EXPECT_EQ(2, a2);
    }
}

TEST(BehaviorLuaFactory, NonSharedState)
{
    a0 = 0;
    a1 = 0;
    a2 = 0;

    BehaviorLuaFactory blf(false);

    blf.exposeFunction(printOut, "printOut");
    blf.exposeFunction(activate0, "activate0");

    {
        BehaviorNode::Ptr tree = blf.createTreeFromFile("TestLuaFactoryScript5.lua");
        ASSERT_TRUE(tree);

        // overwrite activate0 with activate1
        blf.exposeFunction(activate1, "activate0");

        // anode0 should actually call activate1 because of shared state
        // anode1 should still call activate0 because of different state

        BehaviorNode* anode0 = tree->findByID("action0");
        anode0->activate();

        EXPECT_EQ(a0, 0);
        EXPECT_EQ(a1, 1);

        BehaviorNode* anode1 = tree->findByID("action1");
        anode1->activate();

        EXPECT_EQ(a0, 1);
        EXPECT_EQ(a1, 1);

        lua_register(
            ((ActionNode*)tree->findByID("action2"))->getLuaState(),
            "mightBeUndefined",
            printOut
        );

        tree->activate();

        EXPECT_EQ(a0, 2);
        EXPECT_EQ(a1, 2);
    }
}

