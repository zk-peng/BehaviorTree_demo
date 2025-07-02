## README
这是行为树(BehaviorTree.cpp)的官方教程的代码演示。
+ test_1-test_n(n<=16)就是官方案例Tutorials (Basic)和Tutorials (Advanced)。
+ 代码的流程
```bash
git clone https://github.com/BehaviorTree/BehaviorTree.CPP.git
sudo apt-get update
sudo apt-get sqlite3
sudo apt-get install libzmq3-dev libboost-dev
sudo apt-get install libboost-coroutine-dev # 需要用到协程
cd BehaviorTree.cpp
mkdir build
cd build
cmake ..
make
sudo make install
```
这个代码的流程是：
1. 克隆代码仓库
2. 更新apt-get
3. 安装sqlite3
4. 安装libzmq3-dev和libboost-dev
5. 安装libboost-coroutine-dev # 需要用到协程
6. 进入代码目录
7. 创建build目录
8. 进入build目录
9. 执行cmake ..
10. 执行make
11. 执行sudo make install