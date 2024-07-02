cmake_minimum_required(VERSION 3.29.3)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
project(liblog)
set( EXE_NAME ${PROJECT_NAME} )

set(FMT_VERSION 11.0.0)
include(cmake/CPM.cmake)
CPMAddPackage("gh:averrin/libprint#master")
CPMAddPackage("gh:fmtlib/fmt#${FMT_VERSION}")

if(libprint_ADDED)
    include_directories("${libprint_SOURCE_DIR}/include")
endif()

add_executable(${EXE_NAME} liblog_test.cpp)
target_link_libraries(liblog PRIVATE fmt)
target_compile_features(liblog PUBLIC cxx_std_20)
# set_target_properties(liblog PROPERTIES CXX_EXTENSIONS OFF)
# target_link_libraries(liblog PRIVATE fmt::fmt-header-only)
