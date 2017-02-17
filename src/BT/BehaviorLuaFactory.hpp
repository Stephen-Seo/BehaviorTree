
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

        Note that when the BehaviorTree is generated, all ActionNodes will
        share the same lua state as this factory on creation, meaning that
        any function exposed with this function before generating the tree
        will also be available to the generated ActionNodes.

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

        Note that when the BehaviorTree is generated, all ActionNodes will
        share the same lua state as this factory on creation, meaning that
        any function exposed with this function before generating the tree
        will also be available to the generated ActionNodes.

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

    void initializeLuaState();
    BehaviorNode::Ptr createTree();
    BehaviorNode::Ptr createTreeHelper();

};

} // namespace BT

#endif

