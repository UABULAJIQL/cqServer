#定义cmake最低版本号要求
cmake_minimum_required(VERSION 3.24.0)

#定义项目名称
project(ProjectName)

#执行生成protobuf文件
execute_process(
    COMMAND protoc --cpp_out=../lib/packet/protobuf/ msg.proto protoId.proto
    WORKING_DIRECTORY ./protobuf
    )

#编译参数
add_compile_options(-Wall -std=c++11 -O3)

#添加子目录
add_subdirectory(./lib)
# add_subdirectory(./src/server)
# add_subdirectory(./src/client)
add_subdirectory(./src/Login)


