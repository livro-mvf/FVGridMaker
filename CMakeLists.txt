cmake_minimum_required(VERSION 3.15)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

project(FVGridMaker
    VERSION 1.0.0
    LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

include(ConfigUtils)
include(ConfigOptions)
include(ConfigVersion)
include(ConfigCompiler)
include(ConfigTargets)
include(ConfigInstall)
include(ConfigExamples)
include(ConfigLivro)
include(ConfigTests)
include(ConfigDocs)

set(FVG_RUN_ALL_TARGETS)

if(TARGET run_all_examples)
    list(APPEND FVG_RUN_ALL_TARGETS run_all_examples)
endif()

if(TARGET run_all_capitulos)
    list(APPEND FVG_RUN_ALL_TARGETS run_all_capitulos)
endif()

if(TARGET run_all_tests)
    list(APPEND FVG_RUN_ALL_TARGETS run_all_tests)
endif()

if(FVG_RUN_ALL_TARGETS)
    add_custom_target(run_all
        DEPENDS ${FVG_RUN_ALL_TARGETS}
    )
endif()

message(STATUS "FVGridMaker configuration complete")
message(STATUS "Version: ${FVG_VERSION_FULL}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")