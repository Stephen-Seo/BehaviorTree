
BehaviorTree = {
    type = "sequence",
    id = "root",
    children = {
        {
            type = "action",
            id = "action0",
            actionFunction = function (isContinuing)
                printOut("Calling activate0")
                activate0()
                return 0
            end
        },
        {
            type = "action",
            id = "action1",
            sharedState = false,
            actionFunction = function (isContinuing)
                printOut("Calling activate0 with different state")
                activate0()
                return 0
            end
        }
    }
}
