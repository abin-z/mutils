add_executable(main src/main.cpp)
target_link_libraries(main PUBLIC fmt::fmt)

add_executable(exam_base64 src/exam_base64.cpp)
target_link_libraries(exam_base64 PUBLIC fmt::fmt)
target_link_libraries(exam_base64 PUBLIC mutils)

add_executable(exam_make_unique src/exam_make_unique.cpp)
target_link_libraries(exam_make_unique PUBLIC fmt::fmt)
target_link_libraries(exam_make_unique PUBLIC mutils)

add_executable(exam_time src/exam_time.cpp)
target_link_libraries(exam_time PUBLIC fmt::fmt)
target_link_libraries(exam_time PUBLIC mutils)