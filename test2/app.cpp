#include "behaviortree_cpp/bt_factory.h"

using namespace BT;

// 自定义节点: 说话动作
class SaySomething : public SyncActionNode {
public:
//自定义的构造函数。
  SaySomething(const std::string& name, const NodeConfig& config)
    : SyncActionNode(name, config) {}
  
  static PortsList providedPorts() {
    return { InputPort<std::string>("message") };
  }
  
  NodeStatus tick() override {
    if (auto msg = getInput<std::string>("message"); msg) {
    // 1. 第一步：调用 getInput 获取 Expected 对象
    // 2. 第二步：检查 msg 是否有效（即是否获取到了合法数据）
    // 3. 若有效，执行以下操作：
      std::cout << "机器人说: " << msg.value() << std::endl;
      return NodeStatus::SUCCESS;
    }
    return NodeStatus::FAILURE;
  }
};

// 自定义节点: 生成消息
class ThinkWhatToSay : public SyncActionNode {
public:
  ThinkWhatToSay(const std::string& name, const NodeConfig& config)
    : SyncActionNode(name, config) {}
  
  static PortsList providedPorts() {
    return { OutputPort<std::string>("text") };
  }
  
  NodeStatus tick() override {
    setOutput("text", "答案是42");
    return NodeStatus::SUCCESS;
  }
};

int main() {
  BehaviorTreeFactory factory;
  
  // 注册自定义节点--端口
  factory.registerNodeType<SaySomething>("SaySomething");
  factory.registerNodeType<ThinkWhatToSay>("ThinkWhatToSay");
  
  // 从XML创建树
  auto tree = factory.createTreeFromFile("/home/zd4090/BT/test002/demo.xml");
  
  // 执行树
  tree.tickWhileRunning();
  
  return 0;
}