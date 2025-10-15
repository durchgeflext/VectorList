cmake_minimum_required(VERSION 3.20)
project(VectorList)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(VectorList INTERFACE)
target_include_directories(VectorList INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

#TODO: Enable installation rules

#TODO: Enable compile choice for tests
add_subdirectory(test)
