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
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
#设/home/zd4090/BT/src/bt_move_base.cpp为一个特定的名字${BT_MOVE_BASE_SRC}
set(BT_MOVE_BASE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/bt_move_base.cpp)

#test1
add_executable(app1 test1/app_1.cpp ${BT_MOVE_BASE_SRC})
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
add_executable(app4 test4/app_4.cpp ${BT_MOVE_BASE_SRC})
target_link_libraries(app4 behaviortree_cpp)
target_link_libraries(app4 stdc++fs)
#test5
add_executable(app5 test5/app_5.cpp)
target_link_libraries(app5 behaviortree_cpp)
target_link_libraries(app5 stdc++fs)
#test6
add_executable(app6 test6/app_6.cpp ${BT_MOVE_BASE_SRC})
target_link_libraries(app6 behaviortree_cpp)
target_link_libraries(app6 stdc++fs)
#test7
add_executable(app7 test7/app_7.cpp ${BT_MOVE_BASE_SRC})
target_link_libraries(app7 behaviortree_cpp)
target_link_libraries(app7 stdc++fs)
#test8
add_executable(app8 test8/app_8.cpp ${BT_MOVE_BASE_SRC})
target_link_libraries(app8 behaviortree_cpp)
target_link_libraries(app8 stdc++fs)
# #test8_1
# add_executable(app8_1 test8/app_8_1.cpp)
# target_link_libraries(app8_1 behaviortree_cpp)
# target_link_libraries(app8_1 stdc++fs)
#test9
add_executable(app9 test9/app_9.cpp)
target_link_libraries(app9 behaviortree_cpp)
target_link_libraries(app9 stdc++fs)
#test10
add_executable(app10 test10/app_10.cpp)
target_link_libraries(app10 behaviortree_cpp)
target_link_libraries(app10 stdc++fs)
#test11
add_executable(app11 test11/app_11.cpp)
target_link_libraries(app11 behaviortree_cpp)
target_link_libraries(app11 stdc++fs)