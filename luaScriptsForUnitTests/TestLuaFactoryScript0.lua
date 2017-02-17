
-- Each node in the tree must have field "type"
-- type may be one of the following:
--   "priority"
--   "sequence"
--   "random"
--   "loop"
--   "concurrent"
--   "action"
-- Any node that isn't type "action" must have a field called
--   "children" that is a table of tables.
-- Any node that is type "action" must instead have a field
--   "actionFunction" that is a function.

-- "actionFunction" must be a lua function that takes a boolean parameter and
-- returns 0 to 3.
-- action function return values
-- 0 - success
-- 1 - isRunning
-- 2 - fail
-- 3 - error

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
