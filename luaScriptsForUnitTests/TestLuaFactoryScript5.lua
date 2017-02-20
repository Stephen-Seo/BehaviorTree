
-- In an action node, if field "sharedState" is false,
-- then it will have a different lua state from the factory.
-- If the field is nil/undefined or set to true, the action
-- node will share the same lua state with the factory.

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
        },
        {
            type = "action",
            id = "action2",
            sharedState = false,
            actionFunction = function (isContinuing)
                mightBeUndefined("Calling might be undefined")
            end
        }
    }
}
