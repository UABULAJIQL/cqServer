#添加头文件路径
# include_directories(${PROJECT_SOURCE_DIR}/mylib)

#添加生成可执行文件的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#多个源代码文件名放入到一个变量中
aux_source_directory(./ files)

#生成可执行文件
add_executable(main ${files})

#添加库文件
# target_link_libraries(main boost_system pthread protobuf mysqlclient)
