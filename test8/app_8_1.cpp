#include <behaviortree_cpp/bt_factory.h>
#include <string>

// 自定义同步动作节点
class Action_B : public BT::SyncActionNode {
public:
    // 标准构造函数
    Action_B(const std::string& name, const BT::NodeConfig& config)
        : SyncActionNode(name, config) {}

    // 初始化方法（部署时调用）
    void initialize(int arg_int, const std::string& arg_str) {
        _arg1 = arg_int;
        _arg2 = arg_str;
    }

    // 无输入输出端口
    static BT::PortsList providedPorts() { return {}; }

    // 节点执行逻辑（示例：打印参数并成功）
    BT::NodeStatus tick() override {
        printf("Action_B: arg1=%d, arg2=%s\n", _arg1, _arg2.c_str());
        return BT::NodeStatus::SUCCESS;
    }

private:
    int _arg1;
    std::string _arg2;
};

int main() {
    BT::BehaviorTreeFactory factory;
    
    // 仅注册类型，不绑定参数
    factory.registerNodeType<Action_B>("Action_B");
    
    // 从XML构建树（节点未初始化）
    auto tree = factory.createTreeFromText(R"(
        <root BTCPP_format="4">

                <Action_B name="B1"/>
                <Action_B name="B2"/>
            
        </root>
    )");

    // 批量初始化节点（支持不同实例不同参数）
    tree.applyVisitor([](BT::TreeNode* node) {
        if (auto actionB = dynamic_cast<Action_B*>(node)) {
            if (node->name() == "B1") {
                actionB->initialize(69, "instance1");
            } else if (node->name() == "B2") {
                actionB->initialize(100, "instance2");
            }
        }
    });

    // 执行节点
    tree.tickWhileRunning();  // 输出: 
                      // Action_B: arg1=69, arg2=instance1
                      // Action_B: arg1=100, arg2=instance2
    return 0;
}