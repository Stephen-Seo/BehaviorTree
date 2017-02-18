
-- Custom nodes must have type = "custom", an "activate" function, and
-- a children field which is a table of tables (nodes).

-- The activate function must take a boolean parameter and return between 0-3.
-- 0 - Ready or Success
-- 1 - Running
-- 2 - Failed
-- 3 - Error

-- The parameter "isContinuing" is true when resuming the node after it has
-- previously returned 1 (Running).
-- If a node returns 1 for running, the parent node usually keeps track of
-- which node returned running, returns running itself, and on the next time
-- the parent node is activated, it runs the same node again with
-- isContinuing = true. A custom node doesn't have to obey this behavior, but
-- it is recommended.

-- The function "getChildrenSize" is available to get the number of children
-- the current node has.
-- The function "activateChild" is available to activate a node.
-- Note that child indices are from 0 to size - 1, not like lua indices.

BehaviorTree = {
    type = "custom",
    activate = function (isContinuing)
        printOut("Got " .. getChildrenSize() .. " children")
        printOut("0 returned " .. activateChild(0))
        printOut("1 returned " .. activateChild(1))
        printOut("2 returned " .. activateChild(2))
        return 0
    end,
    children = {
        {
            type = "action",
            actionFunction = function ()
                printOut("ActionFunction 0 activated")
                activate0()
                return 0
            end
        },
        {
            type = "action",
            actionFunction = function ()
                printOut("ActionFunction 1 activated")
                activate1()
                return 2
            end
        },
        {
            type = "action",
            actionFunction = function ()
                printOut("ActionFunction 2 activated")
                activate2()
                return 0
            end
        },
    }
}
