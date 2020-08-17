cmake_minimum_required(VERSION 3.0)
project(jpge)

option(BUILD_X64 "build 64-bit" TRUE)
option(STATIC "static linking" FALSE)

message("Initial BUILD_X64=${BUILD_X64}")
message("Initial CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

message(${PROJECT_NAME} " build type: " ${CMAKE_BUILD_TYPE})

if (BUILD_X64)
	message("Building 64-bit")
else()
	message("Building 32-bit")
endif(BUILD_X64)

set(BIN_DIRECTORY "bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${BIN_DIRECTORY})

if (NOT MSVC)
   if (NOT BUILD_X64)
	  set(EXTRA_COMPILE_FLAGS "${EXTRA_COMPILE_FLAGS} -m32")
   endif()

   if (STATIC)
	  set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
   else()
	  set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath .")
   endif(STATIC)

   set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${EXTRA_COMPILE_FLAGS} -fno-strict-aliasing -D_LARGEFILE64_SOURCE=1 -D_FILE_OFFSET_BITS=64 -Wall -Wextra")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC AND BUILD_SHARED_LIBS)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

# jgpd lib

set(JPGD_SRC_LIST jpgd.cpp)
set(JPGD_HDR_LIST jpgd.h jpgd_idct.h)
add_library(jpgd ${JPGD_SRC_LIST} ${JPGD_HDR_LIST})
target_include_directories(jpgd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS jpgd
  RUNTIME DESTINATION bin 
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib)
install(FILES ${JPGD_HDR_LIST} DESTINATION include)

# jgpe lib

set(JPGE_SRC_LIST jpge.cpp)
set(JPGE_HDR_LIST jpge.h jpge.h)
add_library(jpge ${JPGE_SRC_LIST} ${JPGE_HDR_LIST})
target_include_directories(jpge PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS jpge
  RUNTIME DESTINATION bin 
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib)
install(FILES ${JPGE_HDR_LIST} DESTINATION include)

# jgpe executable

set(STB_SRC_LIST stb_image.h stb_image_write.h)
set(JPGEEXE_SRC_LIST tga2jpg.cpp timer.h timer.cpp)

add_executable(jpge_exe ${JPGEEXE_SRC_LIST} ${STB_SRC_LIST})
set_target_properties(jpge_exe PROPERTIES OUTPUT_NAME "jpge")
target_include_directories(jpge_exe PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(jpge_exe jpgd jpge)

install(TARGETS jpge_exe DESTINATION bin)
