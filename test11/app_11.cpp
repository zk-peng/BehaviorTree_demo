#include <behaviortree_cpp/bt_factory.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <behaviortree_cpp/loggers/groot2_publisher.h>
#include <behaviortree_cpp/xml_parsing.h>
#include <boost/smart_ptr/shared_ptr.hpp>

// 假设CrossDoor类已定义，此处仅为示例声明
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

    void reset() {
        _door_open   = false;
        _door_locked = true;
        _pick_attempts = 0;
    }

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

int main() {
    BT::BehaviorTreeFactory factory;
    
    // 假设CrossDoor类的节点注册函数
    CrossDoor cross_door;
    cross_door.registerNodes(factory);
    
    // 生成节点模型的XML
    std::string xml_models = BT::writeTreeNodesModelXML(factory);
    
    // 行为树的XML定义
    const std::string xml_text = R"(
<root BTCPP_format="4">
  <BehaviorTree ID="MainTree">
    <Sequence>
      <Script code="door_open:=false" />
      <Fallback>
        <Inverter>
          <IsDoorClosed/>
        </Inverter>
        <SubTree ID="DoorClosed" _autoremap="true" door_open="{door_open}"/>
      </Fallback>
      <PassThroughDoor/>
    </Sequence>
  </BehaviorTree>
  <BehaviorTree ID="DoorClosed">
    <Fallback name="tryOpen" _onSuccess="door_open:=true">
      <OpenDoor/>
        <RetryUntilSuccessful num_attempts="5">
          <PickLock/>
        </RetryUntilSuccessful>
      <SmashDoor/>
    </Fallback>
  </BehaviorTree>
</root>
)";
    
    // 从文本加载行为树
    factory.registerBehaviorTreeFromText(xml_text);
    auto tree = factory.createTree("MainTree");
    
    // 连接Groot2Publisher用于实时可视化
    BT::Groot2Publisher publisher(tree);
    
    // 主循环
    while (1) {
        std::cout << "Start" << std::endl;
        cross_door.reset();
        tree.tickWhileRunning();
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
    
    return 0;
}
