#include<iostream>
#include "behaviortree_cpp/bt_factory.h"

/**
 * 夹爪控制器接口
 * 提供打开和关闭夹爪的方法，维护夹爪状态
 */
class GripperInterface
{
public:
  // 构造函数：初始化夹爪为打开状态
  GripperInterface(): _open(true) {}
    
  // 打开夹爪动作：设置状态并返回成功
  BT::NodeStatus open() 
  {
    _open = true;
    std::cout << "GripperInterface::open" << std::endl;
    return BT::NodeStatus::SUCCESS;
  }

  // 关闭夹爪动作：设置状态并返回成功
  BT::NodeStatus close() 
  {
    std::cout << "GripperInterface::close" << std::endl;
    _open = false;
    return BT::NodeStatus::SUCCESS;
  }

private:
  bool _open; // 夹爪状态：true表示打开，false表示关闭
};

/**
 * 接近目标对象动作节点
 * 继承自SyncActionNode，表示同步执行的动作
 */
class ApproachObject : public BT::SyncActionNode
{
public:
  // 构造函数：初始化节点名称
  ApproachObject(const std::string& name) :
      BT::SyncActionNode(name, {})
  {}

  // 节点执行函数：每次tick时调用
  BT::NodeStatus tick() override
  {
    std::cout << "ApproachObject: " << this->name() << std::endl;
    return BT::NodeStatus::SUCCESS;
  }
};

/**
 * 检查电池状态条件函数
 * @param node 行为树节点引用(当前未使用)
 * @return 节点状态(SUCCESS表示电池正常)
 */
BT::NodeStatus CheckBattery(BT::TreeNode& node)
{
    std::cout << "[ Battery: OK ]" << std::endl;
    return BT::NodeStatus::SUCCESS;
}

/**
 * 打开夹爪函数对象
 * 用于封装对GripperInterface::open()的调用
 */
struct OpenGripperFunctor {
    GripperInterface& gripper; // 夹爪控制器引用
    //&是引用的意思。-必须初始化，不能重新绑定到其他对象，使用时无需解引用（如 ref 而非 *ref）。
    
    // 构造函数：传入夹爪控制器实例
    OpenGripperFunctor(GripperInterface& gripper) : gripper(gripper) {}
    
    // 函数调用运算符重载：调用夹爪打开方法
    BT::NodeStatus operator()(BT::TreeNode& node) {
        return gripper.open();
    }
};

/**
 * 关闭夹爪函数对象
 * 用于封装对GripperInterface::close()的调用
 */
struct CloseGripperFunctor {
    GripperInterface& gripper; // 夹爪控制器引用
    
    // 构造函数：传入夹爪控制器实例
    CloseGripperFunctor(GripperInterface& gripper) : gripper(gripper) {}
    
    // 函数调用运算符重载：调用夹爪关闭方法
    BT::NodeStatus operator()(BT::TreeNode& node) {
        return gripper.close();
    }
};

int main()
{
    // 创建行为树工厂，用于注册和创建节点
    BT::BehaviorTreeFactory factory;

    // 注册ApproachObject节点类型
    // 使用继承方式实现的节点需要通过registerNodeType注册
    factory.registerNodeType<ApproachObject>("ApproachObject");

    // 注册CheckBattery条件节点
    // 使用普通函数方式注册，直接传入函数名
    factory.registerSimpleCondition("CheckBattery", CheckBattery);

    // 创建夹爪控制器实例
    GripperInterface gripper;
    
    // 注册OpenGripper动作节点
    // 使用函数对象方式注册，传入包含夹爪引用的函数对象
    OpenGripperFunctor open_gripper_functor(gripper);
    factory.registerSimpleAction("OpenGripper", open_gripper_functor);

    // 注册CloseGripper动作节点
    // 同样使用函数对象方式注册
    CloseGripperFunctor close_gripper_functor(gripper);
    factory.registerSimpleAction("CloseGripper", close_gripper_functor);

    // 从XML文件加载行为树结构
    // 注意：需要确保XML文件路径正确且格式符合要求
    auto tree = factory.createTreeFromFile("/home/zd4090/BT/test001/demo.xml");
    
    // 执行行为树，直到所有节点完成或失败
    tree.tickWhileRunning();
    
    return 0;
}
