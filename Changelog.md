# Version 1.12

Disabled copy constructor/operator for BT::LuaStateWrapper. This has no effect
on existing programs that use the BT::LuaStateWrapper::Ptr, which is a
std::shared\_ptr. Copy has been disabled because if the wrapper instance is
copied, it will attempt to cleanup the lua state more than once, which is an
exception waiting to happen.

# Version 1.11

Changed "BT::BehaviorNode::State::StateType" enum due to possible conflict with
the names of the enum values. The enum has been redifined as "enum class" and is
now "BT::BehaviorNode::StateType"

# Version 1.10

Previous implementation did not properly handle an error while generating a
BehaviorTree from a lua script in one specific case. This has been fixed in this
version.

# Version 1.9

Previous implementation placed a pointer into the lua state as an integer. This
new version now places it as "lightuserdata" which is more appropriate for a
pointer. Note this is an internal change of how the library works.

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

