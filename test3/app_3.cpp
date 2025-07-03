/*

*/
#include "behaviortree_cpp/bt_factory.h"
#include <iostream>
using namespace BT;
static const char* xml_text = R"(
<root BTCPP_format="4">
    <BehaviorTree ID="MainTree">
        <Sequence name="root">
            <!-- 计算并存储位置到GoalPosition -->
            <CalculateGoal goal="{GoalPosition}" />
            
            <!-- 从GoalPosition读取位置 -->
            <PrintTarget target="{GoalPosition}" />
            
            <!-- 使用Script节点直接设置位置 -->
            <Script code=" OtherGoal:='-1;3' " />
            
            <!-- 从OtherGoal读取位置 -->
            <PrintTarget target="{OtherGoal}" />
        </Sequence>
    </BehaviorTree>
</root>
)";
// 自定义位置结构
struct Position2D {
    double x;
    double y;
};
// 字符串转换特化
namespace BT {
template <>
inline Position2D convertFromString(StringView str) {
    auto parts = splitString(str, ';');
    if(parts.size() != 2) {
        throw RuntimeError("输入格式错误，应为: X;Y");
    } else {
        return {
            convertFromString<double>(parts[0]),
            convertFromString<double>(parts[1])
        };
    }
}
}
// 节点实现
// 计算目标位置（输出端口）
class CalculateGoal : public SyncActionNode {
public:
    CalculateGoal(const std::string& name, const NodeConfig& config)
        : SyncActionNode(name, config) {}
    
    // 声明输出端口
    static PortsList providedPorts() {
        return { OutputPort<Position2D>("goal") };
    }
    
    NodeStatus tick() override {
        Position2D mygoal = {1.1, 2.3}; // 创建位置对象
        setOutput<Position2D>("goal", mygoal); // 写入输出端口
        return NodeStatus::SUCCESS;
    }
};

// 打印目标位置（输入端口）
class PrintTarget : public SyncActionNode {
public:
    PrintTarget(const std::string& name, const NodeConfig& config)
        : SyncActionNode(name, config) {}
    
    // 声明输入端口（可添加描述文本）
    static PortsList providedPorts() {
        const char* description = "在控制台打印目标位置...";
        return { InputPort<Position2D>("target", description) };
    }
    
    NodeStatus tick() override {
        // 获取输入值
        auto res = getInput<Position2D>("target");
        if(!res) {
            throw RuntimeError("读取端口[target]错误:", res.error());
        }
        
        Position2D target = res.value();
        printf("目标位置: [ %.1f, %.1f ]\n", target.x, target.y);
        return NodeStatus::SUCCESS;
    }
};

int main() {
    BehaviorTreeFactory factory;
    
    // 注册节点类型
    factory.registerNodeType<CalculateGoal>("CalculateGoal");
    factory.registerNodeType<PrintTarget>("PrintTarget");
    
    // 从XML文本创建树
    auto tree = factory.createTreeFromText(xml_text);
    
    // 执行树
    tree.tickWhileRunning();
    
    return 0;
}