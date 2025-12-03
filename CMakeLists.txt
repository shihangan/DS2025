# DS2025 数据结构库
cmake_minimum_required(VERSION 3.16)
project(DS2025 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
  add_compile_options(/utf-8)
endif()

# 设置源文件
set(MYSTL_SOURCES
    mystl.cpp
)

# 设置头文件（如果有的话）
set(MYSTL_HEADERS
    # 目前所有定义都在.cpp文件中
)

# 创建静态库
add_library(mystl STATIC ${MYSTL_SOURCES} ${MYSTL_HEADERS})

# 设置库的属性
set_target_properties(mystl PROPERTIES
    OUTPUT_NAME "mystl"
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# 设置包含目录
target_include_directories(mystl PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# 创建测试可执行文件
add_executable(test_mystl test.cpp)

# 链接库到测试程序
target_link_libraries(test_mystl PRIVATE mystl)

# 设置测试程序属性
set_target_properties(test_mystl PROPERTIES
    OUTPUT_NAME "test_mystl"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# 添加编译定义（如果需要）
target_compile_definitions(mystl PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

# 为测试程序添加编译定义
target_compile_definitions(test_mystl PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

# 安装规则（可选）
install(TARGETS mystl test_mystl
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# 如果有头文件，安装头文件
if(MYSTL_HEADERS)
    install(FILES ${MYSTL_HEADERS} DESTINATION include/mystl)
endif()

# 打印库信息
message(STATUS "MySTL Library sources: ${MYSTL_SOURCES}")
message(STATUS "Building MySTL static library and test executable")
# 实验2：Huffman 编码演示程序
add_executable(exp2_huffman Huffman.cpp)
target_link_libraries(exp2_huffman PRIVATE mystl)

add_executable(exp2_app exp2/exp2.cpp)
target_link_libraries(exp2_app PRIVATE mystl)

add_executable(exp3_app exp3/exp3.cpp)
target_link_libraries(exp3_app PRIVATE mystl)
