# 目标名称
set(tgt_name unittest)

# 收集头文件和源文件
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.c src/*.cpp src/*.cc src/*.cxx)

add_executable(${tgt_name} ${HEADERS} ${SOURCES})

# 添加头文件路径
target_include_directories(${tgt_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

# # 链接依赖库
# if(UNIX)
#   target_link_libraries(${tgt_name} PRIVATE ${THREAD_LIB})
# endif()

# link fmt library
target_link_libraries(${tgt_name} PUBLIC fmt::fmt)
target_link_libraries(${tgt_name} PRIVATE Catch2::Catch2)
target_link_libraries(${tgt_name} PRIVATE mutils)

add_test(NAME ${tgt_name} COMMAND ${tgt_name})

