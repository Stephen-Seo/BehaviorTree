cmake_minimum_required(VERSION 3.0)
project(BehaviorTree)

set(BehaviorTree_SOURCES
    src/BT/BehaviorNode.cpp
    src/BT/PriorityNode.cpp
    src/BT/SequenceNode.cpp
    src/BT/ActionNode.cpp
    src/BT/LoopNode.cpp
    src/BT/ConcurrentNode.cpp
)

set(UnitTest_SOURCES
    src/UnitTest/main.cpp
    src/UnitTest/TestRandomNode.cpp
    src/UnitTest/TestActionNode.cpp
    src/UnitTest/TestPriorityNode.cpp
)

add_library(BehaviorTree ${BehaviorTree_SOURCES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -D NDEBUG")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug', none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
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
endif()

