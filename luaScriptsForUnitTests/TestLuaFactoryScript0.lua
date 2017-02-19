
-- Each node in the tree must have field "type"
-- type may be one of the following:
--   "priority"
--   "sequence"
--   "random"
--   "loop"
--   "concurrent"
--   "action"
--   "custom"
-- Any node that isn't type "action" must have a field called
--   "children" that is a table of tables.
-- Any node that is type "action" must instead have a field
--   "actionFunction" that is a function.
-- See luaScriptsForUnitTests/TestLuaFactoryScript3.lua for info on "custom"
--   nodes.
-- Note that if a node is invalid in some way (invalid type or is not an
-- ActionNode and has no children) then it will be not part of the resulting
-- behavior tree. Thus if the root node is invalid, the resulting
-- std::unique_ptr will not hold a node.

-- Every node type may have an optional field "id" that is a string
-- representing the id of that node.
-- This id is used for finding and getting a specific node in a tree.
-- BT::BehaviorNode has a function "findByID" that returns the node with
-- the given id, or null if there is no node with that id.
-- Thus, "findByID" can be called on the root node to find a specific node
-- in the tree. Note that the node is found via depth-first search.
-- Also note that if two nodes have the same id that is being searched for,
-- only one of them will be picked consistently on each search.
-- By default, all nodes have a default id that is usually
-- "BehaviorNodeDefaultID".
-- see luaScriptsForUnitTests/TestLuaFactoryScript4.lua as an example of
-- using "id".

-- "actionFunction" must be a lua function that takes a boolean parameter and
-- returns 0 to 3.
-- action function return values
-- 0 - success
-- 1 - running
-- 2 - fail
-- 3 - error
-- The boolean parameter "isContinuing" is true when resuming the node after it
-- has previously returned 1 (Running).
-- If a node returns 1 for running, the parent node usually keeps track of
-- which node returned running, returns running itself, and on the next time
-- the parent node is activated, it runs the same node again with
-- isContinuing = true.
-- "action" type nodes can have an optional boolean "sharedState".
-- if "sharedState" is false, then the action node will be created with a
-- different state than the BehaviorLuaFactory.
-- if "sharedState" is true or undefined (nil), then the action node will be
-- created with the same state as the BehaviorLuaFactory.
-- luaScriptsForUnitTests/TestLuaFactoryScript5.lua and the UnitTest tests this
-- functionality.

-- RandomNodes can have optional fields:
-- seed: can be an integer that sets the seed of the RandomNode's
--   random engine. If not specified, the seed is randomized.
-- randomType: can be a string specifying the random engine used
--   in the RandomNode.
-- valid random types:
--   default (same as not specifying a randomType)
--   minstd_rand0
--   minstd_rand
--   mt19937
--   mt19937_64
--   ranlux24_base
--   ranlux48_base
--   ranlux24
--   ranlux48
--   knuth_b
-- see http://en.cppreference.com/w/cpp/numeric/random for info on these random
-- engines


actionFunction2 = function (isContinuing)
    printOut("Action function 2 ran")
    activate2()
    return 0
end

-- global "BehaviorTree" will be parsed to create the tree
BehaviorTree = {
    type = "sequence",
    -- children must be a table of tables
    -- each subtable is expected to be a node in the tree
    children = {
        {
            type = "action",
            -- note "printOut" and "activate0" are exposed functions from
            -- src/UnitTest/TestLuaFactory.cpp
            actionFunction = function (isContinuing)
                printOut("Action function 0 ran")
                if isContinuing then
                    printOut("Action function 0 isContinuing true")
                else
                    printOut("Action function 0 isContinuing false")
                end
                activate0()
                return 0
            end
        },
        {
            type = "action",
            actionFunction = function (isContinuing)
                printOut("Action function 1 ran")
                activate1()
                return 0
            end
        },
        {
            type = "action",
            actionFunction = actionFunction2
        }
    }
}
