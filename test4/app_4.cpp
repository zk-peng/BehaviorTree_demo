#include "behaviortree_cpp/bt_factory.h"
#include <chrono>
#include <thread>
#include "behaviortree_cpp/action_node.h"
#include "bt_move_base.h"
// // 自定义位置类型
// struct Pose2D {
//     double x, y, theta;
// };
// // 使用别名简化时间类型
// namespace chr = std::chrono;
// // 正确的模板特化实现
// namespace BT {
// template <>
// inline Pose2D convertFromString(StringView str) {
//     // 预期格式: "x;y;theta"
//     auto parts = BT::splitString(str, ';');
//     if (parts.size() != 3) {
//         throw RuntimeError("错误的Pose2D格式: '", str, "'");
//     } else {
//         Pose2D result;
//         result.x = convertFromString<double>(parts[0]);
//         result.y = convertFromString<double>(parts[1]);
//         result.theta = convertFromString<double>(parts[2]);
//         return result;
//     }
// }
// } // namespace BT

// // 其余代码保持不变...
// // (MoveBaseAction类、CheckBattery函数、SaySomething类、xml_text常量和main函数)


// class MoveBaseAction : public BT::StatefulActionNode {
// public:
//     // 必须使用此构造函数签名
//     MoveBaseAction(const std::string& name, const BT::NodeConfig& config)
//         : StatefulActionNode(name, config) {}
    
//     // 声明端口
//     static BT::PortsList providedPorts() {
//         return { BT::InputPort<Pose2D>("goal") };
//     }
    
//     // 启动时执行（首次进入）
//     BT::NodeStatus onStart() override {
//         if (!getInput<Pose2D>("goal", _goal)) {
//             throw BT::RuntimeError("缺少必要输入[goal]");
//         }
//         printf("[移动基座: 发送请求]. 目标: x=%f y=%f θ=%f\n", 
//                _goal.x, _goal.y, _goal.theta);
        
//         // 设置220ms后完成（模拟耗时操作）
//         _completion_time = chr::system_clock::now() + chr::milliseconds(220);
//         return BT::NodeStatus::RUNNING;
//     }
    
//     // 运行中执行（重复调用直到非RUNNING状态）
//     BT::NodeStatus onRunning() override {
//         // 模拟非阻塞检查（实际应用中替换为实际状态检查）
//         std::this_thread::sleep_for(chr::milliseconds(10));
        
//         // 检查是否完成
//         if (chr::system_clock::now() >= _completion_time) {
//             std::cout << "[移动基座: 完成]" << std::endl;
//             return BT::NodeStatus::SUCCESS;
//         }
//         return BT::NodeStatus::RUNNING;
//     }
    
//     // 中止时执行
//     void onHalted() override {
//         printf("[移动基座: 已中止]");
//     }
    
// private:
//     Pose2D _goal;                   // 目标位置
//     chr::system_clock::time_point _completion_time; // 完成时间点
// };

// // 简化电池检查节点
// BT::NodeStatus CheckBattery() {
//     std::cout << "[电池状态: 正常]" << std::endl;
//     return BT::NodeStatus::SUCCESS;
// }

// // 说话节点（简化版）
// class SaySomething : public BT::SyncActionNode {
// public:
//     SaySomething(const std::string& name, const BT::NodeConfig& config)
//         : SyncActionNode(name, config) {}
    
//     static BT::PortsList providedPorts() {
//         return { BT::InputPort<std::string>("message") };
//     }
    
//     BT::NodeStatus tick() override {
//         auto msg = getInput<std::string>("message");
//         std::cout << msg.value() << std::endl;
//         return BT::NodeStatus::SUCCESS;
//     }
// };

// 行为树XML文本（简化版）
// static const char* xml_text=R"(
// <root BTCPP_format="4">
//   <BehaviorTree>
//     <Sequence>
//       <BatteryOK/>                         
//       <SaySomething message="任务开始..."/>
//       <MoveBase goal="1;2;3"/>
//       <SaySomething message="任务完成!"/>
//     </Sequence>
//   </BehaviorTree>
// </root>
// )";
/*
--- 开始执行 ---
[电池状态: 正常]
任务开始...
[移动基座: 发送请求]. 目标: x=1.000000 y=2.000000 θ=3.000000
[移动基座: 完成]
任务完成!
*/
static const char* xml_text=R"(
<root BTCPP_format="4">
     <BehaviorTree>
        <ReactiveSequence>
            <BatteryOK/>
            <Sequence>
                <SaySomething   message="mission started..." />
                <MoveBase           goal="1;2;3"/>
                <SaySomething   message="mission completed!" />
            </Sequence>
        </ReactiveSequence>
     </BehaviorTree>
 </root>
)";
int main() {
    BT::BehaviorTreeFactory factory;
    
    // 注册节点类型
    factory.registerSimpleCondition("BatteryOK", std::bind(CheckBattery));
    factory.registerNodeType<MoveBaseAction>("MoveBase");
    factory.registerNodeType<SaySomething>("SaySomething");
    
    // 创建行为树（实际应用中从文件加载）
    auto tree = factory.createTreeFromText(xml_text);
    
    // 手动执行循环（替代tree.tickWhileRunning()）
    std::cout << "--- 开始执行 ---\n";
    while (true) {
        auto status = tree.tickOnce();  // 执行单次tick
        if (status != BT::NodeStatus::RUNNING) {
            break;  // 树执行完成时退出
        }
        // 实际应用中可添加延迟或事件驱动逻辑
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}
