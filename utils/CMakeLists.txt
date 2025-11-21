# 目标名称
set(tgt_name mutils)

# 收集头文件和源文件
file(GLOB_RECURSE HEADERS include/${tgt_name}/*.h include/${tgt_name}/*.hpp)
file(GLOB_RECURSE SOURCES src/*.c src/*.cpp src/*.cc src/*.cxx)

add_library(${tgt_name} ${HEADERS} ${SOURCES})

# 添加头文件路径
target_include_directories(${tgt_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

# # 链接依赖库
# if(UNIX)
#   target_link_libraries(${tgt_name} PRIVATE ${THREAD_LIB})
# endif()

# link fmt library
target_link_libraries(${tgt_name} PUBLIC fmt::fmt)
