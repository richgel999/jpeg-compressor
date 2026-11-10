include(GNUInstallDirs)

if(NOT DEFINED jpge_INSTALL_CMAKEDIR)
    set(jpge_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/jpge"
        CACHE STRING "Path to jpge CMake modules")
endif()

install(TARGETS jpgd jpge EXPORT jpge_Targets
    ARCHIVE COMPONENT jpge_Development
    LIBRARY COMPONENT jpge_Runtime
    RUNTIME COMPONENT jpge_Runtime
    INCLUDES DIRECTORY "${CMAKE_INSTALL_INCLUDEDIR}")
install(DIRECTORY "${jpge_SOURCE_DIR}/include/"
    TYPE INCLUDE
    COMPONENT jpge_Development)
install(TARGETS tga2jpg RUNTIME COMPONENT jpge_Runtime)

if(BUILD_SHARED_LIBS)
    set(type shared)
else()
    set(type static)
endif()

install(EXPORT jpge_Targets
    DESTINATION "${jpge_INSTALL_CMAKEDIR}"
    NAMESPACE jpge::
    FILE jpge-${type}-targets.cmake
    COMPONENT jpge_Development)

install(FILES
    jpgeConfig.cmake
    DESTINATION "${jpge_INSTALL_CMAKEDIR}"
    COMPONENT jpge_Development)

include(CPack)
