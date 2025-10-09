# DS2025 数据结构库

# 设置源文件
set(MYSTL_SOURCES
    stack.cpp
    listnode.cpp
    vector.cpp
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