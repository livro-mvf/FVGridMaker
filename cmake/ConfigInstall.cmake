include_guard()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

if(NOT TARGET FVGridMaker)
    message(WARNING
        "FVGridMaker target does not exist. "
        "Installation rules will not be generated."
    )
    return()
endif()

set(FVG_INSTALL_CMAKEDIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/FVGridMaker"
)

install(
    TARGETS FVGridMaker
    EXPORT FVGridMakerTargets
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(
    DIRECTORY "${FVG_LIBRARY_INCLUDE_DIR}/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(
    DIRECTORY "${FVG_GENERATED_INCLUDE_DIR}/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(
    EXPORT FVGridMakerTargets
    FILE FVGridMakerTargets.cmake
    NAMESPACE FVGridMaker::
    DESTINATION "${FVG_INSTALL_CMAKEDIR}"
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/FVGridMakerConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/FVGridMakerConfig.cmake"
    INSTALL_DESTINATION "${FVG_INSTALL_CMAKEDIR}"
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/FVGridMakerConfigVersion.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/FVGridMakerConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/FVGridMakerConfigVersion.cmake"
    DESTINATION "${FVG_INSTALL_CMAKEDIR}"
)