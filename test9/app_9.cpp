#include <behaviortree_cpp/bt_factory.h>
#include <behaviortree_cpp/utils/shared_library.h>
#include <iostream>
#include <string>

namespace DummyNodes {
    // 自定义动作节点：打印消息（支持黑板变量插值）
    class SaySomething : public BT::SyncActionNode {
    public:
        SaySomething(const std::string& name, const BT::NodeConfig& config)
            : BT::SyncActionNode(name, config) {}

        // 节点执行逻辑
        BT::NodeStatus tick() override {
            std::string message;
            // 从输入端口获取消息（支持黑板变量插值，如"{A}"）
            if (!getInput<std::string>("message", message)) {
                throw BT::RuntimeError("missing 'message' input port");
            }
            
            // 替换占位符（简化实现，实际BT库会自动处理）
            std::cout << "Robot says: " << message << std::endl;
            return BT::NodeStatus::SUCCESS;
        }

        // 声明节点支持的输入端口
        static BT::PortsList providedPorts() {
            return {BT::InputPort<std::string>("message", "要打印的消息，支持黑板变量如{msg}")};
        }
    };
} // namespace DummyNodes

// 定义颜色枚举（用于脚本语言）
enum class Color {
    RED = 5,
    BLUE = 2,
    GREEN = 3
};

int main() {
    // 确保编译时链接BehaviorTree库：g++ -std=c++17 script_node.cpp -lbehaviortree_cpp_v3 -o script_node

    BT::BehaviorTreeFactory factory;
    
    // 注册自定义节点
    factory.registerNodeType<DummyNodes::SaySomething>("SaySomething");
    
    // 注册枚举类型到脚本语言
    // 1. 注册Color枚举（使用magic_enum，注意范围限制）
    factory.registerScriptingEnums<Color>();
    // 2. 注册自定义别名（不受枚举范围限制）
    factory.registerScriptingEnum("THE_ANSWER", 4.5);//向下取整数。
    
    // 行为树XML定义（包含脚本和条件节点）
    const std::string xml_text = R"(
        <root BTCPP_format="4">
            <BehaviorTree>
                <Sequence>
                    <!-- 脚本节点：设置字符串变量 -->
                    <Script code=" msg:='hello world' " />
                    <!-- 脚本节点：设置数值和枚举变量 -->
                    <Script code=" A:=THE_ANSWER; B:=3.14; color:=RED " />
                    <!-- 条件节点：当A>B且color≠BLUE时执行子树 -->
                    <Precondition if="A>B && color != BLUE" else="FAILURE">
                        <Sequence>
                            <SaySomething message="{A}"/>
                            <SaySomething message="{B}"/>
                            <SaySomething message="{msg}"/>
                            <SaySomething message="{color}"/>
                        </Sequence>
                    </Precondition>
                </Sequence>
            </BehaviorTree>
        </root>
    )";
    
    try {
        // 创建行为树
        auto tree = factory.createTreeFromText(xml_text);
        std::cout << "----- 执行行为树 ----\n";
        tree.tickWhileRunning();
    } catch (const BT::RuntimeError& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}