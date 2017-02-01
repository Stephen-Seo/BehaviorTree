
#ifndef BT_BEHAVIOR_NODE
#define BT_BEHAVIOR_NODE

#include <vector>
#include <memory>

namespace BT
{

class BehaviorNode
{
public:
    typedef std::unique_ptr<BehaviorNode> Ptr;

    struct State
    {
        enum StateType
        {
            READY_SUCCESS,
            RUNNING,
            FAILED,
            ERROR
        };

        State();
        State(StateType stateType, BehaviorNode* lastRunning);

        StateType stateType;
        BehaviorNode* lastRunning;
    };

    BehaviorNode();
    virtual ~BehaviorNode();

    void insert(Ptr& node);
    void insert(Ptr&& node);

    void resetState();

    State activate();

protected:
    BehaviorNode* parent;
    std::vector<Ptr> children;
    State state;

    virtual State performAction() = 0;
    virtual State continueAction() = 0;

};

} // namespace BT

#endif

