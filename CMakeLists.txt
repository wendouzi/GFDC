cmake_minimum_required(VERSION 3.0)
PROJECT(DISEASECONTROL)
set(CMAKE_BUILD_TYPE Debug)

if(DEBUG)
    add_definitions(-Dprintf_ON)
endif(DEBUG)

set(WORKSPACE ${PROJECT_SOURCE_DIR}/..)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake
                      ${WORKSPACE}/thirdparty/cmake/modules
                      ${CMAKE_MODULE_PATH})
include(cmake/thirdparty.cmake)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_subdirectory(module)

