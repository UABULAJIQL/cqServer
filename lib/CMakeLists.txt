#多个源代码文件名放入到一个变量中
# aux_source_directory(./ lib_name0)#没有这个本目录下好像编译会卡住不通过
# aux_source_directory(./controller/ lib_name1)
# aux_source_directory(./entidy lib_name2)
# aux_source_directory(./tool/ lib_name3)
# aux_source_directory(./dao/ lib_name4)

#将多个文件文件字符串拼接
#set(lib_name ${lib_name0} ${lib_name1} ${lib_name2} ${lib_name3} ${lib_name4})

#查看输出信息
#message(${lib_name})

#设置生成库的路径
#set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

#创建一个静态库Mylib
# add_library(mylib STATIC ${lib_name})

