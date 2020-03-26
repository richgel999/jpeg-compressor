project(jpge)

cmake_minimum_required(VERSION 3.0)
option(BUILD_X64 "build 64-bit" TRUE)
option(STATIC "static linking" FALSE)

message("Initial BUILD_X64=${BUILD_X64}")
message("Initial CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release )
endif( NOT CMAKE_BUILD_TYPE )

message( ${PROJECT_NAME} " build type: " ${CMAKE_BUILD_TYPE} )

if (BUILD_X64)
	message("Building 64-bit")
else()
	message("Building 32-bit")
endif(BUILD_X64)

if (NOT MSVC)
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
   set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

   set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
   set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

   set(CMAKE_CXX_FLAGS -std=c++11)
   set(GCC_COMPILE_FLAGS "-fno-strict-aliasing -D_LARGEFILE64_SOURCE=1 -D_FILE_OFFSET_BITS=64 -Wall -Wextra")

   if (NOT BUILD_X64)
	  set(GCC_COMPILE_FLAGS "${GCC_COMPILE_FLAGS} -m32")
   endif()

   if (STATIC)
	  set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_LINK_FLAGS} -static-libgcc -static-libstdc++ -static")
   else()
	  set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_LINK_FLAGS} -Wl,-rpath .")
   endif(STATIC)

   set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${GCC_COMPILE_FLAGS}")
   set(CMAKE_C_FLAGS_RELEASE  "${CMAKE_C_FLAGS_RELEASE} ${GCC_COMPILE_FLAGS}")
   set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${GCC_COMPILE_FLAGS} -D_DEBUG")

   set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")
   set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} ${GCC_COMPILE_FLAGS}")
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${GCC_COMPILE_FLAGS} -D_DEBUG")
endif()

set(BASISU_SRC_LIST ${COMMON_SRC_LIST} 
	jpgd.cpp
	jpge.cpp
	tga2jpg.cpp
	timer.cpp
	)

set(BIN_DIRECTORY "bin")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${BIN_DIRECTORY})

add_executable(jpge ${BASISU_SRC_LIST})

install(TARGETS jpge DESTINATION bin)

