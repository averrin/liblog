cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 17)

project(liblog)
set( EXE_NAME ${PROJECT_NAME} )
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${EXE_NAME} liblog_test.cpp)
target_link_libraries(liblog PRIVATE fmt)
