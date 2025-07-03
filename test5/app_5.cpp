#include "behaviortree_cpp/bt_factory.h"

using namespace BT;

static const char* xml_text = R"(
<root BTCPP_format="4">

    <BehaviorTree ID="MainTree">
        <Sequence>
            <Fallback>
                <Inverter>
                    <IsDoorClosed/>
                </Inverter>
                <SubTree ID="DoorClosed"/>
            </Fallback>
            <PassThroughDoor/>
        </Sequence>
    </BehaviorTree>

    <BehaviorTree ID="DoorClosed">
        <Fallback>
            <OpenDoor/>
            <RetryUntilSuccessful num_attempts="5">
                <PickLock/>
            </RetryUntilSuccessful>
            <SmashDoor/>
        </Fallback>
    </BehaviorTree>
    
</root>
)";

class CrossDoor
{
public:
    void registerNodes(BT::BehaviorTreeFactory& factory);

    // SUCCESS if _door_open != true
    BT::NodeStatus isDoorClosed();

    // SUCCESS if _door_open == true
    BT::NodeStatus passThroughDoor();

    // After 3 attempts, will open a locked door
    BT::NodeStatus pickLock();

    // FAILURE if door locked
    BT::NodeStatus openDoor();

    // WILL always open a door
    BT::NodeStatus smashDoor();

private:
    bool _door_open   = false;
    bool _door_locked = true;
    int _pick_attempts = 0;
};

// 实现CrossDoor类的成员函数
BT::NodeStatus CrossDoor::isDoorClosed() {
    std::cout << "检查门是否关闭: " << (_door_open ? "门是开着的" : "门是关着的") << std::endl;
    return !_door_open ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;
}

BT::NodeStatus CrossDoor::passThroughDoor() {
    if (_door_open) {
        std::cout << "成功穿过门" << std::endl;
        return BT::NodeStatus::SUCCESS;
    } else {
        std::cout << "门是关着的，无法穿过" << std::endl;
        return BT::NodeStatus::FAILURE;
    }
}

BT::NodeStatus CrossDoor::pickLock() {
    _pick_attempts++;
    std::cout << "尝试撬锁，第 " << _pick_attempts << " 次尝试" << std::endl;
    
    if (_pick_attempts >= 3) {
        _door_locked = false;
        _door_open = true;
        std::cout << "撬锁成功，门已打开" << std::endl;
        return BT::NodeStatus::SUCCESS;
    } else {
        std::cout << "撬锁失败，继续尝试" << std::endl;
        return BT::NodeStatus::FAILURE;
    }
}

BT::NodeStatus CrossDoor::openDoor() {
    std::cout << "尝试开门";
    if (_door_locked) {
        std::cout << "，但门是锁着的" << std::endl;
        return BT::NodeStatus::FAILURE;
    } else {
        _door_open = true;
        std::cout << "，门已打开" << std::endl;
        return BT::NodeStatus::SUCCESS;
    }
}

BT::NodeStatus CrossDoor::smashDoor() {
    std::cout << "砸开门" << std::endl;
    _door_locked = false;
    _door_open = true;
    return BT::NodeStatus::SUCCESS;
}

// Helper method to make registering less painful for the user
void CrossDoor::registerNodes(BT::BehaviorTreeFactory &factory)
{
  factory.registerSimpleCondition(
      "IsDoorClosed", std::bind(&CrossDoor::isDoorClosed, this));

  factory.registerSimpleAction(
      "PassThroughDoor", std::bind(&CrossDoor::passThroughDoor, this));

  factory.registerSimpleAction(
      "OpenDoor", std::bind(&CrossDoor::openDoor, this));

  factory.registerSimpleAction(
      "PickLock", std::bind(&CrossDoor::pickLock, this));

  factory.registerSimpleAction(
      "SmashDoor", std::bind(&CrossDoor::smashDoor, this));
}

int main()
{
  BehaviorTreeFactory factory;

  CrossDoor cross_door;
  cross_door.registerNodes(factory);

  // In this example a single XML contains multiple <BehaviorTree>
  // To determine which one is the "main one", we should first register
  // the XML and then allocate a specific tree, using its ID

  factory.registerBehaviorTreeFromText(xml_text);
  auto tree = factory.createTree("MainTree");

  // helper function to print the tree
  printTreeRecursively(tree.rootNode());

  tree.tickWhileRunning();

  return 0;
}
