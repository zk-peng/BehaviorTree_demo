cmake_minimum_required(VERSION 3.5.0)
project(demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_definitions(-Wno-unused-but-set-variable)

set(BTCPP_SHARED_LIBS OFF CACHE BOOL "xxx" FORCE)
set(BTCPP_ENABLE_COROUTINES OFF CACHE BOOL "xxx" FORCE)
set(BTCPP_MANUAL_SELECTOR OFF CACHE BOOL "xxx" FORCE)

#使用的那个版本的数据：
#外部数据-文件夹
add_subdirectory(../BTSourceCode_4_0/ ${CMAKE_CURRENT_SOURCE_DIR}/BTSourceCode_4_0-build)
include_directories(../BTSourceCode_4_0/include)
#内部数据-文件夹
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/BTSourceCode_4_6/)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/BTSourceCode_4_6/include)
#demo
# add_executable(demo main.cpp)
# target_link_libraries(demo behaviortree_cpp)
# target_link_libraries(demo stdc++fs)
#app
add_executable(app app1.cpp)
target_link_libraries(app behaviortree_cpp)
target_link_libraries(app stdc++fs)