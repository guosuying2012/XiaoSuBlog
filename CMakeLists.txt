#Cmake 最低版本号要求
cmake_minimum_required(VERSION 2.8)

#c++语言版本
add_definitions(-std=c++11)

#项目信息
project(XiaoSu)

#查找当前目录下的所有源文件
#并将名称保存到DIR_SRC变量
aux_source_directory(src DIR_SRCS)

#指定生成目标
add_executable(../bin/xiaosu ${DIR_SRCS})

#添加连接库
target_link_libraries(../bin/xiaosu booster cppcms cppdb curl ssl)
