
BehaviorTree = {
    type = "sequence",
    children = {
        {
            type = "priority",
            children = {
                {
                    type = "random",
                    children = {
                        {
                            type = "concurrent",
                            children = {
                                {
                                    type = "action",
                                    actionFunction = [[
actionFunction = function (isContinuing)
    printOut("Action function ran")
    activate0()
    return 0
end
                                    ]]
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
