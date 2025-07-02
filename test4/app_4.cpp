#include "behaviortree_cpp_v3/bt_factory.h"
#include "behaviortree_cpp_v3/convertFromString.h"
#include <string>
#include <sstream>
#include <thread>

// 定义 Pose2D 结构体
struct Pose2D {
    double x, y, theta;
    Pose2D(double x_ = 0, double y_ = 0, double theta_ = 0)
        : x(x_), y(y_), theta(theta_) {}
};

// 实现 Pose2D 的类型转换特化
namespace BT {
    template <>
    inline Pose2D convertFromString<Pose2D>(const std::string& text) {
        std::vector<double> values;
        std::stringstream ss(text);
        std::string token;
        
        while (std::getline(ss, token, ';')) {
            values.push_back(std::stod(token));
        }
        
        if (values.size() != 3) {
            throw BT::LogicError("Invalid Pose2D format: " + text);
        }
        
        return Pose2D(values[0], values[1], values[2]);
    }
}

// MoveBase 动作节点
class MoveBaseAction : public BT::SyncActionNode {
public:
    MoveBaseAction(const std::string& name, const BT::NodeConfig& config)
        : BT::SyncActionNode(name, config) {}
    
    static BT::PortsList providedPorts() {
        return { BT::InputPort<Pose2D>("goal") };
    }
    
    BT::NodeStatus tick() override {
        auto goal = getInput<Pose2D>("goal");
        if (!goal) {
            throw BT::RuntimeError("Missing goal port", goal.error());
        }
        
        std::cout << "执行移动到坐标: (" 
                  << goal.value().x << ", " 
                  << goal.value().y << ", " 
                  << goal.value().theta << ")\n";
        return BT::NodeStatus::SUCCESS;
    }
};

// SaySomething 动作节点
class SaySomething : public BT::SyncActionNode {
public:
    SaySomething(const std::string& name, const BT::NodeConfig& config)
        : BT::SyncActionNode(name, config) {}
    
    static BT::PortsList providedPorts() {
        return { BT::InputPort<std::string>("message") };
    }
    
    BT::NodeStatus tick() override {
        auto msg = getInput<std::string>("message");
        if (!msg) {
            throw BT::RuntimeError("Missing message port", msg.error());
        }
        
        std::cout << "机器人说: " << msg.value() << std::endl;
        return BT::NodeStatus::SUCCESS;
    }
};

// 检查电池状态的条件节点
bool CheckBattery() {
    // 模拟电池状态检查
    static bool battery_ok = true;
    return battery_ok;
}

// 4. XML树定义
static const char* xml_text = R"( 
<root BTCPP_format="4">
     <BehaviorTree>
        <ReactiveSequence>
            <Sequence>
                <SaySomething  message="任务开始..." />
                <MoveBase           goal="1;2;3"/>
                <SaySomething  message="任务完成!" />
            </Sequence>
        </ReactiveSequence>
     </BehaviorTree>
</root>
)";

int main() {
    // 5. 创建工厂并注册节点
    BT::BehaviorTreeFactory factory;
    factory.registerSimpleCondition("BatteryOK", [&](BT::TreeNode&) { return CheckBattery(); });
    factory.registerNodeType<MoveBaseAction>("MoveBase");
    factory.registerNodeType<SaySomething>("SaySomething");
    
    // 6. 创建和执行树
    auto tree = factory.createTreeFromText(xml_text);
    
    // 7. 自定义执行循环
    std::cout << "--- 行为树开始执行 ---\n";
    while(tree.tickOnce() == BT::NodeStatus::RUNNING) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::cout << "--- 行为树执行完成 ---\n";
    return 0;
}
