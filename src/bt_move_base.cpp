#include "bt_move_base.h"
#include <sstream>

// 注册Pose2D类型转换函数
namespace BT {
template <>
inline Pose2D convertFromString(StringView str) {
    // 预期格式: "x;y;theta"
    auto parts = BT::splitString(str, ';');
    if (parts.size() != 3) {
        throw RuntimeError("错误的Pose2D格式: '", str, "'");
    } else {
        Pose2D result;
        result.x = convertFromString<double>(parts[0]);
        result.y = convertFromString<double>(parts[1]);
        result.theta = convertFromString<double>(parts[2]);
        return result;
    }
}
} // namespace BT

// MoveBaseAction实现
MoveBaseAction::MoveBaseAction(const std::string& name, const BT::NodeConfig& config)
    : StatefulActionNode(name, config) {}

BT::PortsList MoveBaseAction::providedPorts() {
    return { BT::InputPort<Pose2D>("goal") };
}

BT::NodeStatus MoveBaseAction::onStart() {
    if (!getInput<Pose2D>("goal", _goal)) {
        throw BT::RuntimeError("缺少必要输入[goal]");
    }
    printf("[移动基座: 发送请求]. 目标: x=%f y=%f θ=%f\n", 
           _goal.x, _goal.y, _goal.theta);
    
    // 设置220ms后完成（模拟耗时操作）
    _completion_time = chr::system_clock::now() + chr::milliseconds(220);
    return BT::NodeStatus::RUNNING;
}

BT::NodeStatus MoveBaseAction::onRunning() {
    // 模拟非阻塞检查（实际应用中替换为实际状态检查）
    std::this_thread::sleep_for(chr::milliseconds(10));
    
    // 检查是否完成
    if (chr::system_clock::now() >= _completion_time) {
        std::cout << "[移动基座: 完成]" << std::endl;
        return BT::NodeStatus::SUCCESS;
    }
    return BT::NodeStatus::RUNNING;
}

void MoveBaseAction::onHalted() {
    printf("[移动基座: 已中止]");
}

// CheckBattery实现
BT::NodeStatus CheckBattery() {
    std::cout << "[电池状态: 正常]" << std::endl;
    return BT::NodeStatus::SUCCESS;
}

// SaySomething实现
SaySomething::SaySomething(const std::string& name, const BT::NodeConfig& config)
    : SyncActionNode(name, config) {}

BT::PortsList SaySomething::providedPorts() {
    return { BT::InputPort<std::string>("message") };
}

BT::NodeStatus SaySomething::tick() {
    auto msg = getInput<std::string>("message");
    std::cout << msg.value() << std::endl;
    return BT::NodeStatus::SUCCESS;
}

// 注册节点函数实现
void registerMoveBaseNodes(BT::BehaviorTreeFactory& factory) {
    factory.registerSimpleCondition("BatteryOK", std::bind(CheckBattery));
    factory.registerNodeType<MoveBaseAction>("MoveBase");
    factory.registerNodeType<SaySomething>("SaySomething");
}
