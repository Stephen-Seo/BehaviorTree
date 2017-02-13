-- action function return values
-- 0 - success
-- 1 - isRunning
-- 2 - fail
-- 3 - error

BehaviorTree = {
    type = "sequence",
    children = {
        {
            type = "action",
            actionFunction = [[
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
            ]]
        },
        {
            type = "action",
            actionFunction = [[
                actionFunction = function (isContinuing)
                    printOut("Action function 1 ran")
                    activate1()
                    return 0
                end
            ]]
        },
        {
            type = "action",
            actionFunction = [[
                actionFunction = function (isContinuing)
                    printOut("Action function 2 ran")
                    activate2()
                    return 0
                end
            ]]
        }
    }
}
