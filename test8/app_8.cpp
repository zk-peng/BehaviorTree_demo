#include <behaviortree_cpp/bt_factory.h>
#include <string>

// 自定义同步动作节点
class Action_A : public BT::SyncActionNode {
public:
    // 构造函数添加额外参数（int, std::string）
    Action_A(const std::string& name, const BT::NodeConfig& config,
             int arg_int, std::string arg_str)
        : SyncActionNode(name, config), _arg1(arg_int), _arg2(arg_str) {}

    // 无输入输出端口
    static BT::PortsList providedPorts() { return {}; }

    // 节点执行逻辑（示例：打印参数并成功）
    BT::NodeStatus tick() override {
        printf("Action_A: arg1=%d, arg2=%s\n", _arg1, _arg2.c_str());
        return BT::NodeStatus::SUCCESS;
    }

private:
    int _arg1;
    std::string _arg2;
};
 static const char* xml_text=R"(
        <root BTCPP_format="4">
            <BehaviorTree ID="MainTree">
                <Action_A name="A"/>
            </BehaviorTree>
        </root>
    )";
int main() {
    BT::BehaviorTreeFactory factory;
    
    // 注册节点时指定额外参数（42, "hello world"）
    factory.registerNodeType<Action_A>("Action_A", 42, "hello world");
    
    // 构建行为树（XML中只需<Action_A name="A"/>）
   
    factory.registerBehaviorTreeFromText(xml_text);
    auto tree = factory.createTree("MainTree");

    // 执行节点
    tree.tickWhileRunning();  // 输出: Action_A: arg1=42, arg2=hello world
    return 0;
}