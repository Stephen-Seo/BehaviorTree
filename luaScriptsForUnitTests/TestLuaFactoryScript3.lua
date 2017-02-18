
BehaviorTree = {
    type = "custom",
    activate = function (isContinuing)
        printOut("Got " .. getChildrenSize() .. " children")
        printOut("1 returned " .. activateChild(1))
        printOut("2 returned " .. activateChild(2))
        printOut("0 returned " .. activateChild(0))
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
