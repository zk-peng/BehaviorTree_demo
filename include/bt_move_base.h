#ifndef BT_MOVE_BASE_H
#define BT_MOVE_BASE_H

#include "behaviortree_cpp/bt_factory.h"
#include "behaviortree_cpp/action_node.h"
#include <chrono>

namespace BT {
class BehaviorTreeFactory;
}

// 自定义位置类型
struct Pose2D {
    double x, y, theta;
};

// 使用别名简化时间类型
namespace chr = std::chrono;

/**
 * @brief 移动基座动作节点 - 控制机器人移动到指定位置
 */
class MoveBaseAction : public BT::StatefulActionNode {
public:
    MoveBaseAction(const std::string& name, const BT::NodeConfig& config);
    
    // 声明端口
    static BT::PortsList providedPorts();
    
    // 启动时执行（首次进入）
    BT::NodeStatus onStart() override;
    
    // 运行中执行（重复调用直到非RUNNING状态）
    BT::NodeStatus onRunning() override;
    
    // 中止时执行
    void onHalted() override;
    
private:
    Pose2D _goal;                    // 目标位置
    chr::system_clock::time_point _completion_time; // 完成时间点
};

/**
 * @brief 电池检查条件节点 - 检查机器人电池状态
 */
BT::NodeStatus CheckBattery();

/**
 * @brief 说话动作节点 - 让机器人说出指定消息
 */
class SaySomething : public BT::SyncActionNode {
public:
    SaySomething(const std::string& name, const BT::NodeConfig& config);
    
    static BT::PortsList providedPorts();
    
    BT::NodeStatus tick() override;
};

/**
 * @brief 注册所有自定义节点到行为树工厂
 * @param factory 行为树工厂对象
 */
void registerMoveBaseNodes(BT::BehaviorTreeFactory& factory);

#endif // BT_MOVE_BASE_H
