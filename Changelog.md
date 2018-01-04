# Version 1.8

Fix forward declare of lua\_State in src/BT/BehaviorNode.hpp.

# Version 1.7

Changed "getLuaStatesInTree" to return an unordered\_set of lua states
(pointers) instead of a vector to avoid duplicates in the returned data
structure.

Minor change to .gitignore.

# Version 1.6

Added "getLuaStatesInTree" to BehaviorNode.

Generating a behavior tree from lua script no longer allows ActionNodes to have
children.

# Version 1.5

Added "exposeLuaLibrary" functions to BehaviorLuaFactory that can be called to
load modules (such as the lua standard library modules).

# Version 1.4

Custom Lua Nodes now use the lua registry to store their required state instead
of using "lua\_getextraspace"

The UnitTest no longer uses "lua\_getextraspace" but also uses the lua registry.

# Version 1.3

Added a function to BT::BehaviorLuaFactory "getLuaStateWrapper" that returns
the std::shared\_ptr managing the Factory's lua state.

# Version 1.2

Changed the void pointer in CustomLuaNodeState to an actual void pointer.

# Version 1.1

Some nodes wouldn't return error immediately if error was returned by a child
node. This has been fixed.

# Version 1.0

Initial release

Can manually create a BehaviorTree by creating BehaviorNodes and putting them
together, or can use BT::BehaviorLuaFactory to generate a tree from a lua
script.

