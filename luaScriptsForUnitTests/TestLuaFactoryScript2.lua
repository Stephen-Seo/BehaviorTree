
BehaviorTree = {
    type = "action",
    actionFunction = function (isContinuing)
        setState(1)
        printOut("Set state to 1")
        return 0
    end
}
