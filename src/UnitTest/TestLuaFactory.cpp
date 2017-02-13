
#include "gtest/gtest.h"

#include <iostream>

#include <lua.hpp>

#include <BT/BehaviorLuaFactory.hpp>

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

int activate0(lua_State* L)
{
    ++a0;
    return 0;
}

int activate1(lua_State* L)
{
    ++a1;
    return 0;
}

int activate2(lua_State* L)
{
    ++a2;
    return 0;
}

TEST(BehaviorLuaFactory, Factory)
{
    BehaviorLuaFactory blf(false);

    blf.exposeFunction(printOut, "printOut");
    blf.exposeFunction(activate0, "activate0");
    blf.exposeFunction(activate1, "activate1");
    blf.exposeFunction(activate2, "activate2");

    BehaviorNode::Ptr tree = blf.createTreeFromFile("TestLuaFactoryScript.lua");

    ASSERT_TRUE(tree);

    tree->activate();

    EXPECT_EQ(1, a0);
    EXPECT_EQ(1, a1);
    EXPECT_EQ(1, a2);

    tree->activate();

    EXPECT_EQ(2, a0);
    EXPECT_EQ(2, a1);
    EXPECT_EQ(2, a2);
}

