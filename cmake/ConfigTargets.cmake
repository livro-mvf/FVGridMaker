include_guard()

include(GNUInstallDirs)

set(FVG_LIBRARY_SOURCE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/FVGridMakerLib/src"
)

set(FVG_LIBRARY_INCLUDE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/FVGridMakerLib/include"
)

set(FVG_GENERATED_INCLUDE_DIR
    "${CMAKE_CURRENT_BINARY_DIR}/generated"
)

set(FVG_GENERATED_VERSION_HEADER
    "${FVG_GENERATED_INCLUDE_DIR}/FVGridMaker/version.hpp"
)

if(NOT EXISTS "${FVG_LIBRARY_INCLUDE_DIR}")
    message(FATAL_ERROR
        "FVGridMaker include directory was not found: "
        "${FVG_LIBRARY_INCLUDE_DIR}"
    )
endif()

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/version.hpp.in"
    "${FVG_GENERATED_VERSION_HEADER}"
    @ONLY
)

set(FVG_LIBRARY_SOURCES)

if(EXISTS "${FVG_LIBRARY_SOURCE_DIR}")
    file(GLOB_RECURSE FVG_LIBRARY_SOURCES CONFIGURE_DEPENDS
        "${FVG_LIBRARY_SOURCE_DIR}/*.cpp"
        "${FVG_LIBRARY_SOURCE_DIR}/*.cxx"
        "${FVG_LIBRARY_SOURCE_DIR}/*.cc"
    )
endif()

if(FVG_LIBRARY_SOURCES)
    add_library(FVGridMaker ${FVG_LIBRARY_SOURCES})

    target_compile_features(FVGridMaker
        PUBLIC
            cxx_std_20
    )

    target_include_directories(FVGridMaker
        PUBLIC
            $<BUILD_INTERFACE:${FVG_LIBRARY_INCLUDE_DIR}>
            $<BUILD_INTERFACE:${FVG_GENERATED_INCLUDE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    target_compile_definitions(FVGridMaker
        PRIVATE
            $<$<AND:$<BOOL:${FVG_ENABLE_NDEBUG}>,$<CONFIG:Release>>:NDEBUG>
            $<$<AND:$<BOOL:${FVG_ENABLE_NDEBUG}>,$<CONFIG:RelWithDebInfo>>:NDEBUG>
            $<$<AND:$<BOOL:${FVG_ENABLE_NDEBUG}>,$<CONFIG:MinSizeRel>>:NDEBUG>
    )

    set_target_properties(FVGridMaker
        PROPERTIES
            OUTPUT_NAME FVGridMaker
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            POSITION_INDEPENDENT_CODE ON
    )

    set_target_optimizations(FVGridMaker)
else()
    add_library(FVGridMaker INTERFACE)

    target_compile_features(FVGridMaker
        INTERFACE
            cxx_std_20
    )

    target_include_directories(FVGridMaker
        INTERFACE
            $<BUILD_INTERFACE:${FVG_LIBRARY_INCLUDE_DIR}>
            $<BUILD_INTERFACE:${FVG_GENERATED_INCLUDE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    message(WARNING
        "No FVGridMaker source files were found in "
        "'${FVG_LIBRARY_SOURCE_DIR}'. "
        "FVGridMaker will be configured as an INTERFACE header-only target."
    )
endif()

add_library(FVGridMaker::FVGridMaker ALIAS FVGridMaker)

add_custom_target(info
    COMMAND ${CMAKE_COMMAND} -E echo "Project: ${PROJECT_NAME}"
    COMMAND ${CMAKE_COMMAND} -E echo "Version: ${FVG_VERSION_FULL}"
    COMMAND ${CMAKE_COMMAND} -E echo "Semantic version: ${FVG_VERSION_SEMANTIC}"
    COMMAND ${CMAKE_COMMAND} -E echo "Git hash: ${FVG_GIT_HASH}"
    COMMAND ${CMAKE_COMMAND} -E echo "Git branch: ${FVG_GIT_BRANCH}"
    COMMAND ${CMAKE_COMMAND} -E echo "Git dirty suffix: ${FVG_GIT_DIRTY_SUFFIX}"
    COMMAND ${CMAKE_COMMAND} -E echo "Build type: ${CMAKE_BUILD_TYPE}"
    COMMAND ${CMAKE_COMMAND} -E echo "C++ compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}"
    COMMAND ${CMAKE_COMMAND} -E echo "C++ standard: C++20"
    COMMAND ${CMAKE_COMMAND} -E echo "Source directory: ${FVG_LIBRARY_SOURCE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E echo "Include directory: ${FVG_LIBRARY_INCLUDE_DIR}"
    VERBATIM
)

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E rm -rf
        "${CMAKE_CURRENT_BINARY_DIR}/CMakeCache.txt"
        "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/Makefile"
        "${CMAKE_CURRENT_BINARY_DIR}/build.ninja"
        "${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt"
        "${CMAKE_CURRENT_BINARY_DIR}/bin"
        "${CMAKE_CURRENT_BINARY_DIR}/lib"
        "${CMAKE_CURRENT_BINARY_DIR}/generated"
    COMMENT "Removing generated CMake and build artefacts"
    VERBATIM
)