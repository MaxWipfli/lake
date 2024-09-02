cmake_minimum_required(VERSION 3.24)
cmake_policy(VERSION 3.24)
project(lake VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

add_library(lake SHARED)
set_target_properties(lake PROPERTIES VERSION ${PROJECT_VERSION})

# Add sources and includes
add_subdirectory(src)

# Do not include C++ standard library
target_compile_options(lake PRIVATE -nostdinc++)
target_link_options(lake PRIVATE -nodefaultlibs)
# Warnings
target_compile_options(lake PRIVATE -Werror -Wall -Wextra)

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
