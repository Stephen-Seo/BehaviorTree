
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
    const char* luaScript = ""
"-- action function return values\n"
"-- 0 - success\n"
"-- 1 - isRunning\n"
"-- 2 - fail\n"
"-- 3 - error\n"
"\n"
"BehaviorTree = {\n"
"    type = \"sequence\",\n"
"    children = {\n"
"        {\n"
"            type = \"action\",\n"
"            actionFunction = [[\n"
"                actionFunction = function (isContinuing)\n"
"                    printOut(\"Action function 0 ran\")\n"
"                    if isContinuing then\n"
"                        printOut(\"Action function 0 isContinuing true\")\n"
"                    else\n"
"                        printOut(\"Action function 0 isContinuing false\")\n"
"                    end\n"
"                    activate0()\n"
"                    return 0\n"
"                end\n"
"            ]]\n"
"        },\n"
"        {\n"
"            type = \"action\",\n"
"            actionFunction = [[\n"
"                actionFunction = function (isContinuing)\n"
"                    printOut(\"Action function 1 ran\")\n"
"                    activate1()\n"
"                    return 0\n"
"                end\n"
"            ]]\n"
"        },\n"
"        {\n"
"            type = \"action\",\n"
"            actionFunction = [[\n"
"                actionFunction = function (isContinuing)\n"
"                    printOut(\"Action function 2 ran\")\n"
"                    activate2()\n"
"                    return 0\n"
"                end\n"
"            ]]\n"
"        }\n"
"    }\n"
"}\n";

    BehaviorLuaFactory blf(false);

    blf.exposeFunction(printOut, "printOut");
    blf.exposeFunction(activate0, "activate0");
    blf.exposeFunction(activate1, "activate1");
    blf.exposeFunction(activate2, "activate2");

    BehaviorNode::Ptr tree = blf.createTreeFromScript(std::string(luaScript));

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

