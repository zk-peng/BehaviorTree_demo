#include "behaviortree_cpp/bt_factory.h"
static const char* xml_text = R"(
 <root BTCPP_format="4" >
     <BehaviorTree ID="MainTree">
        <Sequence name="root">
            <SaySomething     message="hello" />
            <ThinkWhatToSay   text="{the_answer}"/>
            <SaySomething     message="{the_answer}" />
        </Sequence>
     </BehaviorTree>
 </root>
 )";


class ThinkWhatToSay : public BT::SyncActionNode
{
public:
  ThinkWhatToSay(const std::string& name, const BT::NodeConfig& config) :
    BT::SyncActionNode(name, config)
  {}

  // This Action simply write a value in the port "text"
  BT::NodeStatus tick() override
  {
    setOutput("text", "The answer is 42");
    return BT::NodeStatus::SUCCESS;
  }

  // A node having ports MUST implement this STATIC method
  static BT::PortsList providedPorts()
  {
    return {BT::OutputPort<std::string>("text")};
  }
};


class SaySomething : public BT::SyncActionNode
{
  public:
    SaySomething(const std::string& name, const BT::NodeConfig& config)
      : BT::SyncActionNode(name, config)
    {
    }

    // You must override the virtual function tick()
    BT::NodeStatus tick() override
    {
        auto msg = getInput<std::string>("message");
        if (!msg)
        {
            throw BT::RuntimeError( "missing required input [message]: ", msg.error() );
        }

        std::cout << "Robot says: " << msg.value() << std::endl;
        return BT::NodeStatus::SUCCESS;
    }

    // It is mandatory to define this static method.
    static BT::PortsList providedPorts()
    {
        return{ BT::InputPort<std::string>("message") };
    }
};


int main()
{

  BT::BehaviorTreeFactory factory;
  factory.registerNodeType<ThinkWhatToSay>("ThinkWhatToSay");
  factory.registerNodeType<SaySomething>("SaySomething");
  auto tree = factory.createTreeFromText(xml_text);
  tree.tickWhileRunning();

  return 0;
}


