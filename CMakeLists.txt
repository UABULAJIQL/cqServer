#定义cmake最低版本号要求
cmake_minimum_required(VERSION 3.24.0)

#定义项目名称
project(ProjectName)

#编译参数
add_compile_options(-Wall -std=c++11 -O2)

#添加子目录
# add_subdirectory(./lib)
add_subdirectory(./src)


