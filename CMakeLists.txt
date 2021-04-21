cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
project(liblog)
set( EXE_NAME ${PROJECT_NAME} )

include(cmake/CPM.cmake)
CPMAddPackage("gh:averrin/libprint#master")
CPMAddPackage("gh:fmtlib/fmt#7.1.3")

if(libprint_ADDED)
    include_directories("${libprint_SOURCE_DIR}/include")
endif()

add_executable(${EXE_NAME} liblog_test.cpp)
target_link_libraries(liblog PRIVATE fmt)
# target_link_libraries(liblog PRIVATE fmt::fmt-header-only)
