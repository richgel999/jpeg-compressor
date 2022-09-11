cmake_minimum_required(VERSION 3.15)

project(jpge LANGUAGES CXX)

if(NOT MSVC)
    set_property(GLOBAL PROPERTY CXX_STANDARD 11)
    
    add_compile_options(
        -Wall
        -Wextra
        -fno-strict-aliasing
    )
    
    add_compile_definitions(
        _LARGEFILE64_SOURCE=1
        _FILE_OFFSET_BITS=64
    )

    if(MINGW)
        add_compile_definitions(
            WIN32
        )
    endif()
endif()

# JPEG decoder
add_library(jpgd jpgd.cpp)
add_library(jpge::jpgd ALIAS jpgd)
target_include_directories(jpgd
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# JPEG encoder (compressor)
add_library(jpge jpge.cpp)
add_library(jpge::jpge ALIAS jpge)
target_include_directories(jpge
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(tga2jpg tga2jpg.cpp timer.cpp)
target_link_libraries(tga2jpg PRIVATE jpgd jpge)

string(COMPARE EQUAL
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}"
    is_top_level)
option(jpge_INCLUDE_PACKAGING "Package jpge" ${is_top_level})
if(jpge_INCLUDE_PACKAGING)
    add_subdirectory(cmake/packaging)
endif()
