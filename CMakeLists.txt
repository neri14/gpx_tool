cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

find_package(LibXml2)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE SRC ${SRC_DIR}/*)

include_directories(${SRC_DIR})
include_directories(${LIBXML2_INCLUDE_DIR})

add_executable(gpx_tool ${SRC})

target_link_libraries(gpx_tool ${LIBXML2_LIBRARIES})