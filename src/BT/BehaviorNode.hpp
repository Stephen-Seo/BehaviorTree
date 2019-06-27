
#ifndef BT_BEHAVIOR_NODE
#define BT_BEHAVIOR_NODE

#define BT_BEHAVIOR_NODE_DEFAULT_ID "BehaviorNodeDefaultID"

#include <cstdlib>

#include <vector>
#include <memory>
#include <string>
#include <unordered_set>

struct lua_State;

namespace BT
{

/*!
    \brief The base class for all BehaviorTree nodes.

    Note that this class is an abstract class; An instance of type BehaviorNode
    is not permitted.

    Either use BehaviorLuaFactory to create a BehaviorTree, or manually build
    one composed of these node types: PriorityNode, SequenceNode, RandomNode,
    LoopNode, ConcurrentNode, and ActionNode.

    Example of creating a BehaviorTree manually:
    ~~~{.cpp}
        PriorityNode pn;
        BehaviorNode::Ptr anPtr = BehaviorNode::Ptr(
            new ActionNode([] (bool isContinuing) {
                std::cout << "Hello World" << std::endl;
                return BehaviorNode::StateType::BT_READY_SUCCESS;
        }));
        pn.insert(anPtr);
        anPtr = BehaviorNode::Ptr(
            new ActionNode([] (bool isContinuing) {
                std::cout << "This shouldn't execute" << std::endl;
                return BehaviorNode::StateType::BT_READY_SUCCESS;
        }));
        pn.insert(anPtr);

        pn.activate();
    ~~~
*/
class BehaviorNode
{
public:
    typedef std::unique_ptr<BehaviorNode> Ptr;

    enum class StateType : unsigned int
    {
        BT_READY_SUCCESS = 0,
        BT_RUNNING,
        BT_FAILED,
        BT_ERROR
    };

    struct State
    {
        State();
        State(StateType stateType, std::size_t lastRunningIndex);

        StateType stateType;
        std::size_t lastRunningIndex;
    };

    BehaviorNode();
    virtual ~BehaviorNode();

    BehaviorNode(const BehaviorNode& other);
    BehaviorNode& operator = (const BehaviorNode& other);

    BehaviorNode(BehaviorNode&& other) = default;
    BehaviorNode& operator = (BehaviorNode&& other) = default;

    /*!
        \brief Adds the BehaviorNode to this node's list of children (sub nodes)

        Since this version of insert expects a reference to a unique_ptr, this
        function will std::move the given pointer into its list. The given node
        should therefore be empty after a call to this function.
    */
    void insert(Ptr& node);
    /*!
        \brief Adds the BehaviorNode to this node's list of children (sub nodes)

        Since this version of insert expects a r-value of a unique_ptr, this
        function will std::forward<BehaviorNode> the given pointer into its list.
    */
    void insert(Ptr&& node);

    /*!
        \brief Resets this node's State to defaults.
    */
    void resetState();

    /*!
        \brief Activates the tree described by this node and its children

        Based on what type of nodes you are using, this function will behave
        as described per node and return a StateType describing the
        results of the activation.

        PriorityNode -
            Iterates through child nodes, on success or running stop and return
        state.

        SequenceNode -
            Iterates through child nodes, on fail continue, on running stop.
        Return success if all child nodes return success.

        RandomNode -
            Randomly pick one child node and activate it, returning the
        resulting state.

        LoopNode -
            Loop through child nodes until a child node returns running or fail.

        ConcurrentNode -
            Iterates through child nodes, stop on running or fail. Returns
        success if all child nodes return success.

        ActionNode -
            Performs a custom action that returns any of the four possible
        state types.
    */
    StateType activate();

    /*!
        \brief Returns a copy of the tree described by this node and children

        Note that the copy created using this function will hold no references
        to any part of the original BehaviorTree, except for the lua_State in
        ActionNode. In other words, ActionNode copies will share the same
        lua_State.
    */
    virtual Ptr getCopy() = 0;

    /*!
        \brief Sets the string id of this node, to be found via findByID

        BT::BehaviorLuaFactory automatically sets a node's id if it has the
        field "id" set with a string.
    */
    void setID(std::string id);

    /*!
        \brief Returns the id of this node

        Used internally by findByID.
    */
    const std::string& getID() const;

    /*!
        \brief Finds a node in this tree that has the given id

        Returns null if there is no such node with the given id.
    */
    BehaviorNode* findByID(std::string id);

    /*!
        \brief Returns a vector of all the lua_State instances in the tree.

        Note that if there are 3 action nodes and 2 custom lua nodes in the
        tree, then 5 states will be returned in the vector regardless if some
        of the states are the same.

        Also note if the tree was not generated via a lua script, then this
        function will only return an empty vector.
    */
    std::unordered_set<lua_State*> getLuaStatesInTree();

protected:
    std::vector<Ptr> children;
    State state;

    virtual StateType performAction() = 0;
    virtual StateType continueAction() = 0;

    virtual void getLuaStatesHelper(std::unordered_set<lua_State*>& s);

private:
    std::string id;

};

} // namespace BT

#endif

