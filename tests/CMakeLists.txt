# ----------------------------------------------------------------------------
# File: CMakeLists.txt
# Project: FVGridMaker
# Version: 0.1.0
# Description: Discovers, builds and runs FVGridMaker test programs.
# Author: FVGridMaker Team
# License: MIT
# ----------------------------------------------------------------------------

include(CTest)

# ----------------------------------------------------------------------------
# Global test targets.
# ----------------------------------------------------------------------------

add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    COMMENT "Running all FVGridMaker tests through CTest."
    VERBATIM
)

add_custom_target(run_tst_all
    DEPENDS run_all_tests
    COMMENT "Alias for run_all_tests."
)

# ----------------------------------------------------------------------------
# Test discovery.
# ----------------------------------------------------------------------------

file(GLOB_RECURSE FVGRIDMAKER_TEST_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/tst_*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tst_*.cpp"
)

message(STATUS "[tests] Discovering FVGridMaker tests...")

if(NOT googletest_SOURCE_DIR)
    message(FATAL_ERROR "[tests] googletest_SOURCE_DIR is not defined.")
endif()

set(FVGRIDMAKER_TEST_TARGETS "")

foreach(test_source IN LISTS FVGRIDMAKER_TEST_SOURCES)
    get_filename_component(test_name "${test_source}" NAME_WE)
    get_filename_component(test_dir "${test_source}" DIRECTORY)

    message(STATUS "  - Configuring test: ${test_name}")

    file(GLOB test_helper_sources
        CONFIGURE_DEPENDS
        "${test_dir}/*.cc"
        "${test_dir}/*.cpp"
    )

    list(REMOVE_ITEM test_helper_sources "${test_source}")

    foreach(source IN LISTS test_helper_sources)
        if(source MATCHES ".*/tst_.*\\.(cc|cpp)$")
            list(REMOVE_ITEM test_helper_sources "${source}")
        endif()
    endforeach()

    add_executable("${test_name}"
        "${test_source}"
        ${test_helper_sources}
    )

    set_target_properties("${test_name}" PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )

    target_link_libraries("${test_name}"
        PRIVATE
            FVGridMaker
            gtest_main
    )

    target_include_directories("${test_name}"
        PRIVATE
            SYSTEM "${googletest_SOURCE_DIR}/googletest/include"
            SYSTEM "${googletest_SOURCE_DIR}/googlemock/include"
            "${FVG_INCLUDE_DIR}"
    )

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
       AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10.0)
        target_compile_options("${test_name}" PRIVATE -fconcepts)
    endif()

    add_test(
        NAME "${test_name}"
        COMMAND "$<TARGET_FILE:${test_name}>"
    )

    set(test_run_target "run_${test_name}")

    add_custom_target("${test_run_target}"
        COMMAND "$<TARGET_FILE:${test_name}>"
        DEPENDS "${test_name}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        COMMENT "Running ${test_name} through GoogleTest."
        VERBATIM
    )

    list(APPEND FVGRIDMAKER_TEST_TARGETS "${test_name}")
endforeach()

if(FVGRIDMAKER_TEST_TARGETS)
    add_dependencies(run_all_tests ${FVGRIDMAKER_TEST_TARGETS})
else()
    message(WARNING "[tests] No tst_*.cc or tst_*.cpp file was found.")
endif()