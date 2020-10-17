cmake_minimum_required(VERSION 3.4.1)
if (${FLAVOR} STREQUAL "FLAVOR1")
    message("adding the source code for Flavor1...")
    add_library( # Sets the name of the library.
            hello-lib

            SHARED

            ${CMAKE_CURRENT_SOURCE_DIR}/flavor1/hello-lib.cpp)
elseif (${FLAVOR} STREQUAL "FLAVOR2")
    message("adding the source code for Flavor2...")
    add_library( # Sets the name of the library.
            hello-lib

            SHARED

            ${CMAKE_CURRENT_SOURCE_DIR}/flavor2/hello-lib.cpp)
endif ()

message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
