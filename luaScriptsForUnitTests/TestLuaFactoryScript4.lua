
BehaviorTree = {
    id = "root",
    type = "sequence",
    children = {
        {
            id = "action0",
            type = "action",
            actionFunction = function (isContinuing)
                printOut("ActionFunction 0 called")
                activate0()
                return 0
            end
        },
        {
            id = "action1",
            type = "action",
            actionFunction = function (isContinuing)
                printOut("ActionFunction 1 called")
                activate1()
                return 0
            end
        },
        {
            id = "action2",
            type = "action",
            actionFunction = function (isContinuing)
                printOut("ActionFunction 2 called")
                activate2()
                return 0
            end
        }
    }
}
