cmake_minimum_required(VERSION 3.24)
cmake_policy(VERSION 3.24)
project(lake VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

add_library(lake SHARED)
set_target_properties(lake PROPERTIES VERSION ${PROJECT_VERSION})

add_subdirectory(src)
target_include_directories(lake PRIVATE include/lake)
target_include_directories(lake PRIVATE src)

# Installation
include(GNUInstallDirs)
install(TARGETS lake
    LIBRARY DESTINATION ${CMAKE_INSTALL_DIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Testing (with GoogleTest)
enable_testing()
include(meta/cmake/googletest.cmake)
add_subdirectory(test)
