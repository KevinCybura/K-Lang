cmake_minimum_required(VERSION 3.13)

project(K_Lang)
set(CMAKE_CXX_STANDARD 17)

# find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# file(GLOB SOURCES "src/*.cpp"))
set(SOURCES src/parser.cpp src/lexer.cpp)

add_library(parser_lib STATIC ${SOURCES})
target_include_directories(parser_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(K_Lang src/main.cpp)

target_include_directories(K_Lang PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(K_Lang ${llvm_libs} PRIVATE parser_lib)

add_subdirectory(tests)
