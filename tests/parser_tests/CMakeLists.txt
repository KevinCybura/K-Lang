include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(parser_test test.cpp)

target_link_libraries(parser_test gtest gtest_main)
target_link_libraries(parser_test parser_lib )
