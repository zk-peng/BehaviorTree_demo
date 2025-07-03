
#include<iostream>
#include "bt_move_base.h"
#include "behaviortree_cpp/bt_factory.h"
/*
1. 手动添加多xml文件
*/
// int main()
// {
//   BT::BehaviorTreeFactory factory;
//   factory.registerNodeType<SaySomething>("SaySomething");
//   // You can create the MainTree and the subtrees will be added automatically.
//   std::string search_directory = "/home/zd4090/BT/test7/";

//   using std::filesystem::directory_iterator;
//   for (auto const& entry : directory_iterator(search_directory)) 
//   {
//     std::cout<<entry<<std::endl;
//     if( entry.path().extension() == ".xml")
//     {
//       factory.registerBehaviorTreeFromFile(entry.path().string());
//     }
//   }
//   std::cout << "----- MainTree tick ----" << std::endl;
//   auto main_tree = factory.createTree("MainTree");
//   main_tree.tickWhileRunning();
//   return 0;
// }

int main()
{
  BT::BehaviorTreeFactory factory;
  factory.registerNodeType<SaySomething>("SaySomething");
  // You can create the MainTree and the subtrees will be added automatically.
  // std::string search_directory = "/home/zd4090/BT/test7/";

  // using std::filesystem::directory_iterator;
  // for (auto const& entry : directory_iterator(search_directory)) 
  // {
  //   std::cout<<entry<<std::endl;
  //   if( entry.path().extension() == ".xml")
  //   {
  //     factory.registerBehaviorTreeFromFile(entry.path().string());
  //   }
  // }
  factory.registerBehaviorTreeFromFile("/home/zd4090/BT/test7/main_tree.xml");
  std::cout << "----- MainTree tick ----" << std::endl;
  auto main_tree = factory.createTree("MainTree");
  main_tree.tickWhileRunning();
  return 0;
}

