#添加头文件路径
include_directories(${PROJECT_SOURCE_DIR}/lib)

#多个源代码文件名放入到一个变量中
aux_source_directory(./ _libs)#根目录一定是要有的
aux_source_directory(./tools _libs)
aux_source_directory(./network _libs)
aux_source_directory(./network/epoll _libs)
aux_source_directory(./buffer _libs)
aux_source_directory(./packet _libs)
aux_source_directory(./thread _libs)
aux_source_directory(./packet/protobuf _libs)
aux_source_directory(./app _libs)

#查看输出信息
#message(${lib_name})

#设置生成库的路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

#创建一个静态库Mylib
add_library(mylib STATIC ${_libs})

