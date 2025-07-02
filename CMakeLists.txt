cmake_minimum_required(VERSION 3.5.0)
project(BT)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-Wno-unused-but-set-variable)

set(BTCPP_SHARED_LIBS OFF CACHE BOOL "xxx" FORCE)
set(BTCPP_ENABLE_COROUTINES OFF CACHE BOOL "xxx" FORCE)
set(BTCPP_MANUAL_SELECTOR OFF CACHE BOOL "xxx" FORCE)

#使用的那个版本的数据：
#外部数据-文件夹
# add_subdirectory(../BTSourceCode_4_0/ ${CMAKE_CURRENT_SOURCE_DIR}/BTSourceCode_4_0-build)
# include_directories(../BTSourceCode_4_0/include)
#内部数据-文件夹
find_package(behaviortree_cpp)

#test1
add_executable(app1 test1/app_1.cpp)
target_link_libraries(app1 behaviortree_cpp)
target_link_libraries(app1 stdc++fs)
#test2
add_executable(app2 test2/app_2.cpp)
target_link_libraries(app2 behaviortree_cpp)
target_link_libraries(app2 stdc++fs)
#test3
add_executable(app3 test3/app_3.cpp)
target_link_libraries(app3 behaviortree_cpp)
target_link_libraries(app3 stdc++fs)
#test4
add_executable(app4 test4/app_4.cpp)
target_link_libraries(app4 behaviortree_cpp)
target_link_libraries(app4 stdc++fs)
