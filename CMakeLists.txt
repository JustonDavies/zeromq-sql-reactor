cmake_minimum_required(VERSION 3.3)

#Definitions
project(nexus_adapter)
add_definitions(-std=c11)

#Set output directory because CLion
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out)

#Linking dependencies
find_package(Lua REQUIRED)

#Set variables
set(HANDLE_SOURCE nexus_handle.c)

include_directories(include include/${CMAKE_CURRENT_BINARY_DIR})

add_library(nexus_handle SHARED src/${HANDLE_SOURCE})
set_target_properties(nexus_handle PROPERTIES
        PUBLIC_HEADERS "${PUBLIC_HEADERS}"
        OUTPUT_NAME "nexus_handle"
        PREFIX "")
target_link_libraries (nexus_handle sqlite3 sodium czmq json lua5.2)