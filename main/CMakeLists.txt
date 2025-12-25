add_executable(main src/main.cpp)
target_link_libraries(main PUBLIC fmt::fmt)

add_executable(exam_base64 src/exam_base64.cpp)
target_link_libraries(exam_base64 PUBLIC fmt::fmt)
target_link_libraries(exam_base64 PUBLIC mutils)