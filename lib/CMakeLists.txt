#添加头文件路径
include_directories(${PROJECT_SOURCE_DIR}/lib)

#多个源代码文件名放入到一个变量中
aux_source_directory(./ _lib)#没有这个本目录下好像编译会卡住不通过
aux_source_directory(./tools _tools)
aux_source_directory(./network _network)
aux_source_directory(./network/epoll _epoll)
aux_source_directory(./buffer _buffer)
aux_source_directory(./packet _packet)
aux_source_directory(./thread _thread)
aux_source_directory(./packet/protobuf _protobuf)
aux_source_directory(./app _app)


#将多个文件文件字符串拼接
set(_libs ${_lib} ${_tools} ${_network} ${_epoll} ${_buffer} ${_packet} ${_thread} ${_protobuf} ${_app})

#查看输出信息
#message(${lib_name})

#设置生成库的路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

#创建一个静态库Mylib
add_library(mylib STATIC ${_libs})

