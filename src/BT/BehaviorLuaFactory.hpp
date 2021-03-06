
#ifndef BT_BEHAVIOR_LUA_FACTORY_HPP
#define BT_BEHAVIOR_LUA_FACTORY_HPP

#include <lua.hpp>
#include <string>
#include <unordered_map>

#include "LuaStateWrapper.hpp"
#include "BehaviorNode.hpp"

namespace BT
{

/*!
    \brief A factory that produces a BehaviorTree from a lua script.

    Example of a lua script generating a BehaviorTree:
    ~~~{.lua}
    BehaviorTree = {
        type = "sequence",
        id = "root",
        children = {
            {
                type = "action",
                id = "action0",
                actionFunction = function (isContinuing)
                    -- Do stuff
                    return 0 -- return success
                end
            },
            {
                type = "action",
                id = "action1",
                sharedState = false,
                actionFunction = function (isContinuing)
                    -- Do stuff
                    return 2 -- return fail
                end
            },
            {
                type = "random",
                id = "random",
                randomType = "default",
                seed = "0", -- not advised to do this, as otherwise default
                -- is to randomize the seed.
                children = {
                    {
                        type = "action",
                        id = "action2",
                        actionFunction = function (isContinuing)
                            -- Do stuff
                            return 0 -- return success
                        end
                    },
                    {
                        type = "action",
                        id = "action3",
                        actionFunction = function (isContinuing)
                            -- Do stuff
                            return 3 -- return error
                        end
                    }
                }
            }
        }
    }
    ~~~
*/
class BehaviorLuaFactory
{
public:
    /*!
        If isSilentNotVerbose is false, errors will be output on standard error.
    */
    BehaviorLuaFactory(bool isSilentNotVerbose = true);
    ~BehaviorLuaFactory();

    // copy
    /*!
        \brief Copies the other BehaviorLuaFactory, except the lua state.

        This copy constructor will default initialize the lua state instead of
        copying it from the other BehaviorLuaFactory.
    */
    BehaviorLuaFactory(const BehaviorLuaFactory& other);

    /*!
        \brief Copies the other BehaviorLuaFactory, except the lua state.

        This copy operator will leave the current lua state untouched instead
        of copying it from the other BehaviorLuaFactory.
    */
    BehaviorLuaFactory& operator = (const BehaviorLuaFactory& other);

    /*!
        \brief Places a function into the lua state with global name "name".

        Note that when the BehaviorTree is generated, all ActionNodes (with
        field "sharedState" nil or true) will share the same lua state as this
        factory on creation, meaning that any function exposed with this
        function before generating the tree will also be available to the
        generated ActionNodes.

        An exposed function is saved in a map for when the lua state is
        initialized. Initialization is when the functions will be inserted into
        the lua state with the global name. Note that if the lua state was
        already initialized when this function is called, the exposed function
        will be put in the initialized lua state.
        Use function BehaviorLuaFactory::clearLuaCFunctions to remove all
        functions from this map.
    */
    void exposeFunction(int (*function)(lua_State*), const char* name);

    /*!
        \brief Places a function into the lua state with global name "name".

        Note that when the BehaviorTree is generated, all ActionNodes (with
        field "sharedState" nil or true) will share the same lua state as this
        factory on creation, meaning that any function exposed with this
        function before generating the tree will also be available to the
        generated ActionNodes.

        An exposed function is saved in a map for when the lua state is
        initialized. Initialization is when the functions will be inserted into
        the lua state with the global name. Note that if the lua state was
        already initialized when this function is called, the exposed function
        will be put in the initialized lua state.
        Use function BehaviorLuaFactory::clearLuaCFunctions to remove all
        functions from this map.
    */
    void exposeFunction(int (*function)(lua_State*), std::string name);

    /*!
        \brief Registers a module (luaopen_*) with global name "moduleName"

        Note that when registering a lua standard library like "base", the
        library's functions will be in a table of name "moduleName".

        For example, if exposeLuaLibrary was called with the "math" library:

            exposeLuaLibrary(luaopen_math, "math");

        Then the registered functions will be available with calls like

            math.random(5)

        where the table name is the "moduleName" passed to the function.

        Note that registering base with "moduleName" as "base" will place
        base functions in the "base" table.
    */
    void exposeLuaLibrary(
        int (*luaopen_)(lua_State*), const char* moduleName);

    /*!
        \brief Registers a module (luaopen_*) with global name "moduleName"

        Note that when registering a lua standard library like "base", the
        library's functions will be in a table of name "moduleName".

        For example, if exposeLuaLibrary was called with the "math" library:

            exposeLuaLibrary(luaopen_math, "math");

        Then the registered functions will be available with calls like

            math.random(5)

        where the table name is the "moduleName" passed to the function.

        Note that registering base with "moduleName" as "base" will place
        base functions in the "base" table.
    */
    void exposeLuaLibrary(
        int (*luaopen_)(lua_State*), std::string moduleName);

    /*!
        \brief Creates a BehaviorTree from a lua script from a file.

        If an error occurs during tree generation, errors will be output
        on standard error (if isSilentNotVerbose is false when creating this
        factory) and a std::unique_ptr<BehaviorNode> holding nothing will be
        returned.
    */
    BehaviorNode::Ptr createTreeFromFile(std::string luaFilename);

    /*!
        \brief Creates a BehaviorTree from a lua script from a string.

        If an error occurs during tree generation, errors will be output
        on standard error (if isSilentNotVerbose is false when creating this
        factory) and a std::unique_ptr<BehaviorNode> holding nothing will be
        returned.
    */
    BehaviorNode::Ptr createTreeFromScript(const char* luaScript, std::size_t size);

    /*!
        \brief Creates a BehaviorTree from a lua script from a string.

        If an error occurs during tree generation, errors will be output
        on standard error (if isSilentNotVerbose is false when creating this
        factory) and a std::unique_ptr<BehaviorNode> holding nothing will be
        returned.
    */
    BehaviorNode::Ptr createTreeFromScript(std::string luaScript);

    /*!
        \brief Gets the current lua state held by this factory.

        Internally, the lua state isn't initialized until a tree is
        generated or this function is called. Only the function
        BehaviorLuaFactory::resetLuaState will force this factory to use a
        different new lua state. Use BehaviorLuaFactory::resetLuaState and
        BehaviorLuaFactory::clearLuaCFunctions when generating multiple trees
        that should have different lua states (and exposed functions).
    */
    lua_State* getLuaState();

    /*!
        \brief Gets the current lua state wrapper held by this factory.

        Internally, the lua state isn't initialized until a tree is
        generated or this function is called. Only the function
        BehaviorLuaFactory::resetLuaState will force this factory to use a
        different new lua state. Use BehaviorLuaFactory::resetLuaState and
        BehaviorLuaFactory::clearLuaCFunctions when generating multiple trees
        that should have different lua states (and exposed functions).

        Use this function instead of getLuaState() if you want the lua state
        to persist past the life of this factory and generated lua nodes by
        keeping the returned shared pointer.

        If all shared pointers (lua state wrappers) with the same lua state are
        destructed, the lua state will be cleaned up via "lua_close"
    */
    LuaStateWrapper::Ptr getLuaStateWrapper();

    /*!
        \brief Resets the internal lua state to a new different state.

        Internally, the lua state doesn't actually get reinitialized until
        BehaviorLuaFactory::getLuaState is called or a new tree is generated.
        This function ensures that this factory's lua state will be different
        than the previous one.
    */
    void resetLuaState();

    /*!
        \brief Clears the registered/exposed functions from this factory.

        After some functions have been exposed to the internal lua state, if
        this function was not called and BehaviorLuaFactory::resetLuaState was
        called and the lua state is reinitialized by calling the function
        BehaviorLuaFactory::getLuaState or by generating the tree, the exposed
        functions will be put in the new lua state.

        Calling this function will remove all stored functions.
    */
    void clearLuaCFunctions();
private:
    LuaStateWrapper::Ptr LWrapper;
    bool isSilent;
    std::unordered_map<std::string, int(*)(lua_State*)> functions;
    std::unordered_map<std::string, int(*)(lua_State*)> libs;

    void initializeLuaState();
    BehaviorNode::Ptr createTree();
    BehaviorNode::Ptr createTreeHelper();

};

} // namespace BT

#endif

