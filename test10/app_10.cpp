#include <behaviortree_cpp/bt_factory.h>
#include <behaviortree_cpp/loggers/bt_observer.h>
#include <iostream>
#include <map>
#include <string>

// // 自定义节点：始终失败
// class AlwaysFailure : public BT::SyncActionNode {
// public:
//     AlwaysFailure(const std::string& name, const BT::NodeConfig& config)
//         : BT::SyncActionNode(name, config) {}

//     // 必须实现的静态方法：返回端口列表（此处为空）
//     static BT::PortsList providedPorts() {
//         return {};
//     }

//     BT::NodeStatus tick() override {
//         return BT::NodeStatus::FAILURE;
//     }
// };

// // 自定义节点：始终成功
// class AlwaysSuccess : public BT::SyncActionNode {
// public:
//     AlwaysSuccess(const std::string& name, const BT::NodeConfig& config)
//         : BT::SyncActionNode(name, config) {}

//     // 必须实现的静态方法：返回端口列表（此处为空）
//     static BT::PortsList providedPorts() {
//         return {};
//     }

//     BT::NodeStatus tick() override {
//         return BT::NodeStatus::SUCCESS;
//     }
// };

int main() {
    BT::BehaviorTreeFactory factory;
    // factory.registerNodeType<AlwaysFailure>("AlwaysFailure");
    // factory.registerNodeType<AlwaysSuccess>("AlwaysSuccess");
    
    // 行为树XML定义
    const std::string xml_text = R"(
        <root BTCPP_format="4">
            <BehaviorTree ID="MainTree">
                <Sequence>
                    <Fallback>
                        <AlwaysFailure name="failing_action"/>
                        <SubTree ID="SubTreeA" name="mysub"/>
                    </Fallback>
                    <AlwaysSuccess name="last_action"/>
                </Sequence>
            </BehaviorTree>
            <BehaviorTree ID="SubTreeA">
                <Sequence>
                    <AlwaysSuccess name="action_subA"/>
                    <SubTree ID="SubTreeB" name="sub_nested"/>
                    <SubTree ID="SubTreeB" />
                </Sequence>
            </BehaviorTree>
            <BehaviorTree ID="SubTreeB">
                <AlwaysSuccess name="action_subB"/>
            </BehaviorTree>
        </root>
    )";
    
    try {
        factory.registerBehaviorTreeFromText(xml_text);
        auto tree = factory.createTree("MainTree");
        
        std::cout << "----- 行为树结构 -----\n";
        BT::printTreeRecursively(tree.rootNode());
        
        BT::TreeObserver observer(tree);
        
        std::map<uint16_t, std::string> uid_to_path;
        for (const auto& [path, uid] : observer.pathToUID()) {
            uid_to_path[uid] = path;
        }
        
        std::cout << "\n----- UID 与路径映射 -----\n";
        for (const auto& [uid, path] : uid_to_path) {
            std::cout << "UID: " << uid << " | 路径: " << path << std::endl;
        }
        
        std::cout << "\n----- 执行行为树 -----\n";
        tree.tickWhileRunning();
        
        const auto& last_action_stats = observer.getStatistics("last_action");
        std::cout << "\n----- last_action 统计 -----\n";
        std::cout << "状态转换次数: " << last_action_stats.transitions_count << std::endl;
        std::cout << "成功次数: " << last_action_stats.success_count << std::endl;
        std::cout << "失败次数: " << last_action_stats.failure_count << std::endl;
        
        std::cout << "\n----- 所有节点统计 -----\n";
        for (const auto& [uid, path] : uid_to_path) {
            const auto& stats = observer.getStatistics(uid);
            std::cout << "路径: " << path 
                      << " | 转换/成功/失败: " 
                      << stats.transitions_count << "/" 
                      << stats.success_count << "/" 
                      << stats.failure_count << std::endl;
        }
        
    } catch (const BT::RuntimeError& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
