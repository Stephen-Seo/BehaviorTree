cmake_minimum_required(VERSION 3.0)
project(BehaviorTree)

set(BehaviorTree_VERSION_MAJOR 1)
set(BehaviorTree_VERSION_MINOR 0)

set(BehaviorTree_SOURCES
    src/BT/BehaviorNode.cpp
    src/BT/PriorityNode.cpp
    src/BT/SequenceNode.cpp
    src/BT/ActionNode.cpp
    src/BT/LoopNode.cpp
    src/BT/ConcurrentNode.cpp
    src/BT/LuaStateWrapper.cpp
    src/BT/BehaviorLuaFactory.cpp
)

set(UnitTest_SOURCES
    src/UnitTest/main.cpp
    src/UnitTest/TestRandomNode.cpp
    src/UnitTest/TestActionNode.cpp
    src/UnitTest/TestPriorityNode.cpp
    src/UnitTest/TestGetCopy.cpp
    src/UnitTest/TestLuaFactory.cpp
)

add_library(BehaviorTree ${BehaviorTree_SOURCES})

set_target_properties(BehaviorTree PROPERTIES
    VERSION "${BehaviorTree_VERSION_MAJOR}.${BehaviorTree_VERSION_MINOR}"
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -D NDEBUG")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug', none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

find_package(Lua)

if(LUA_FOUND)
target_include_directories(BehaviorTree
    PUBLIC ${LUA_INCLUDE_DIR}
)

target_link_libraries(BehaviorTree
    PUBLIC lua
)
endif()

find_package(GTest QUIET)

if(GTEST_FOUND)
    message(STATUS "Found GTest, building UnitTest...")

    add_executable(UnitTest ${UnitTest_SOURCES})

    target_include_directories(UnitTest
        PUBLIC src
    )

    target_link_libraries(UnitTest
        PUBLIC ${GTEST_BOTH_LIBRARIES}
        PUBLIC BehaviorTree
    )

    if(WIN32)
        add_custom_command(
            TARGET UnitTest POST_BUILD
            COMMAND copy "${CMAKE_CURRENT_SOURCE_DIR}/luaScriptsForUnitTests/TestLuaFactoryScript.lua" "${PROJECT_BINARY_DIR}"
            BYPRODUCTS "${PROJECT_BINARY_DIR}/TestLuaFactoryScript.lua"
        )
        add_custom_command(
            TARGET UnitTest POST_BUILD
            COMMAND copy "${CMAKE_CURRENT_SOURCE_DIR}/luaScriptsForUnitTests/TestLuaFactoryAction2.lua" "${PROJECT_BINARY_DIR}"
            BYPRODUCTS "${PROJECT_BINARY_DIR}/TestLuaFactoryAction2.lua"
        )
        add_custom_command(
            TARGET UnitTest POST_BUILD
            COMMAND copy "${CMAKE_CURRENT_SOURCE_DIR}/luaScriptsForUnitTests/TestLuaFactoryScript2.lua" "${PROJECT_BINARY_DIR}"
            BYPRODUCTS "${PROJECT_BINARY_DIR}/TestLuaFactoryScript2.lua"
        )
        add_custom_command(
            TARGET UnitTest POST_BUILD
            COMMAND copy "${CMAKE_CURRENT_SOURCE_DIR}/luaScriptsForUnitTests/TestLuaFactoryScript3.lua" "${PROJECT_BINARY_DIR}"
            BYPRODUCTS "${PROJECT_BINARY_DIR}/TestLuaFactoryScript3.lua"
        )
    else()
        add_custom_command(
            TARGET UnitTest POST_BUILD
            COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/luaScriptsForUnitTests/TestLuaFactoryScript.lua" "${PROJECT_BINARY_DIR}"
            BYPRODUCTS "${PROJECT_BINARY_DIR}/TestLuaFactoryScript.lua"
        )
        add_custom_command(
            TARGET UnitTest POST_BUILD
            COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/luaScriptsForUnitTests/TestLuaFactoryAction2.lua" "${PROJECT_BINARY_DIR}"
            BYPRODUCTS "${PROJECT_BINARY_DIR}/TestLuaFactoryAction2.lua"
        )
        add_custom_command(
            TARGET UnitTest POST_BUILD
            COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/luaScriptsForUnitTests/TestLuaFactoryScript2.lua" "${PROJECT_BINARY_DIR}"
            BYPRODUCTS "${PROJECT_BINARY_DIR}/TestLuaFactoryScript2.lua"
        )
        add_custom_command(
            TARGET UnitTest POST_BUILD
            COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/luaScriptsForUnitTests/TestLuaFactoryScript3.lua" "${PROJECT_BINARY_DIR}"
            BYPRODUCTS "${PROJECT_BINARY_DIR}/TestLuaFactoryScript3.lua"
        )
    endif()
endif()

install(TARGETS BehaviorTree
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
install(DIRECTORY src/BT DESTINATION include
    FILES_MATCHING
    PATTERN "*.hpp"
    PATTERN "*.inl"
)

