cmake_minimum_required(VERSION 3.13)

project(K_Lang)
set (CMAKE_CXX_STANDARD 14)


find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})


file(GLOB SOURCES "src/*.cpp")

add_executable(K_Lang ${SOURCES} )

target_include_directories(K_Lang
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        )
target_link_libraries(K_Lang ${llvm_libs})


option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
