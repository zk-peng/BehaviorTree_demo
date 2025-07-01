#include<iostream>
#include "behaviortree_cpp/bt_factory.h"
// #include "dummy_nodes.h"
/*
+ 简化版本：<saysomething name="xx" message="xx">(需要registerSimpleAction的时候进行测试)
+ 显化版本:<action ID="saysomething" name="xx" message="xx">
*/
// static const char* xml_text = R"(
// <root BTCPP_format="4" >
//      <BehaviorTree ID="MainTree">
//         <Sequence name="root_sequence">
//             <CheckBattery   name="check_battery"/>
//             <OpenGripper    name="open_gripper"/>
//             <ApproachObject name="approach_object"/>
//             <CloseGripper   name="close_gripper"/>
//         </Sequence>
//      </BehaviorTree>
// </root>
//  )";
// using namespace BT;
// We want to wrap into an ActionNode the methods open() and close()
class GripperInterface
{
public:
  GripperInterface(): _open(true) {}
    
  BT::NodeStatus open() 
  {
    _open = true;
    std::cout << "GripperInterface::open" << std::endl;
    return BT::NodeStatus::SUCCESS;
  }

  BT::NodeStatus close() 
  {
    std::cout << "GripperInterface::close" << std::endl;
    _open = false;
    return BT::NodeStatus::SUCCESS;
  }

private:
  bool _open; // shared information
};

// Example of custom SyncActionNode (synchronous action)
// without ports.
class ApproachObject : public BT::SyncActionNode
{
public:
  ApproachObject(const std::string& name) :
      BT::SyncActionNode(name, {})
  {}

  // You must override the virtual function tick()
  BT::NodeStatus tick() override
  {
    std::cout << "ApproachObject: " << this->name() << std::endl;
    return BT::NodeStatus::SUCCESS;
  }
};

BT::NodeStatus CheckBattery()
{
    std::cout << "[ Battery: OK ]" << std::endl;
    return BT::NodeStatus::SUCCESS;
}
int main()
{
    // We use the BehaviorTreeFactory to register our custom nodes
  BT::BehaviorTreeFactory factory;

  // The recommended way to create a Node is through inheritance.
  factory.registerNodeType<ApproachObject>("ApproachObject");

  // Registering a SimpleActionNode using a function pointer.
  // You can use C++11 lambdas or std::bind
  factory.registerSimpleCondition("CheckBattery", [&](BT::TreeNode&) { return CheckBattery(); });

  //You can also create SimpleActionNodes using methods of a class
  GripperInterface gripper;
  factory.registerSimpleAction("OpenGripper", [&](BT::TreeNode&){ return gripper.open(); });
  factory.registerSimpleAction("CloseGripper", [&](BT::TreeNode&){ return gripper.close(); } );

  // Trees are created at deployment-time (i.e. at run-time, but only 
  // once at the beginning).
  // IMPORTANT: when the object "tree" goes out of scope, all the 
  // TreeNodes are destroyed
  //auto tree = factory.createTreeFromText(xml_text);
  auto tree = factory.createTreeFromFile("/home/zd4090/BT/test001/demo.xml");
  // To "execute" a Tree you need to "tick" it.
  // The tick is propagated to the children based on the logic of the tree.
  // In this case, the entire sequence is executed, because all the children
  // of the Sequence return SUCCESS.
  tree.tickWhileRunning();
  return 0;
}