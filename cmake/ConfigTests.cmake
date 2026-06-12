include_guard()

if(NOT BUILD_TESTS)
    return()
endif()

include(CTest)

if(NOT BUILD_TESTING)
    return()
endif()

enable_testing()

option(FVG_TESTS_FETCH_GOOGLETEST
    "Download GoogleTest with FetchContent if it is not found"
    ON
)

find_package(GTest CONFIG QUIET)

if(NOT GTest_FOUND)
    if(NOT FVG_TESTS_FETCH_GOOGLETEST)
        message(FATAL_ERROR
            "GoogleTest was not found and FVG_TESTS_FETCH_GOOGLETEST is OFF."
        )
    endif()

    include(FetchContent)

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    FetchContent_MakeAvailable(googletest)
endif()

include(GoogleTest)

set(FVG_TESTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")

if(NOT EXISTS "${FVG_TESTS_DIR}")
    set(FVG_TESTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Tests")
endif()

if(NOT EXISTS "${FVG_TESTS_DIR}")
    message(WARNING "Tests directory was not found.")
    return()
endif()

file(GLOB_RECURSE FVG_TEST_SOURCES CONFIGURE_DEPENDS
    "${FVG_TESTS_DIR}/tst_*.cpp"
    "${FVG_TESTS_DIR}/tst_*.cxx"
    "${FVG_TESTS_DIR}/tst_*.cc"
)

if(NOT FVG_TEST_SOURCES)
    message(WARNING
        "No test entry-point files matching tst_*.cpp, tst_*.cxx or "
        "tst_*.cc were found in '${FVG_TESTS_DIR}'."
    )
    return()
endif()

foreach(FVG_TEST_SOURCE IN LISTS FVG_TEST_SOURCES)
    file(RELATIVE_PATH
        FVG_TEST_RELATIVE_PATH
        "${FVG_TESTS_DIR}"
        "${FVG_TEST_SOURCE}"
    )

    get_filename_component(FVG_TEST_NAME
        "${FVG_TEST_RELATIVE_PATH}"
        NAME_WE
    )

    get_filename_component(FVG_TEST_RELATIVE_DIR
        "${FVG_TEST_RELATIVE_PATH}"
        DIRECTORY
    )

    set(FVG_TEST_NAME_TOKEN "${FVG_TEST_NAME}")

    if(FVG_TEST_NAME_TOKEN MATCHES "^tst_(.+)$")
        set(FVG_TEST_NAME_TOKEN "${CMAKE_MATCH_1}")
    endif()

    if(FVG_TEST_RELATIVE_DIR)
        string(REPLACE "/" "_" FVG_TEST_DIR_TOKEN
            "${FVG_TEST_RELATIVE_DIR}"
        )
        string(REPLACE "\\" "_" FVG_TEST_DIR_TOKEN
            "${FVG_TEST_DIR_TOKEN}"
        )

        set(FVG_TEST_TARGET
            "tst_${FVG_TEST_DIR_TOKEN}_${FVG_TEST_NAME_TOKEN}"
        )
    else()
        set(FVG_TEST_TARGET "tst_${FVG_TEST_NAME_TOKEN}")
    endif()

    if(TARGET "${FVG_TEST_TARGET}")
        message(FATAL_ERROR
            "Duplicate test target detected: ${FVG_TEST_TARGET}"
        )
    endif()

    add_executable("${FVG_TEST_TARGET}"
        "${FVG_TEST_SOURCE}"
    )

    target_link_libraries("${FVG_TEST_TARGET}"
        PRIVATE
            FVGridMaker::FVGridMaker
            GTest::gtest_main
    )

    target_compile_features("${FVG_TEST_TARGET}"
        PRIVATE
            cxx_std_20
    )

    set_target_properties("${FVG_TEST_TARGET}"
        PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            RUNTIME_OUTPUT_DIRECTORY
                "${CMAKE_CURRENT_BINARY_DIR}/bin/tests"
    )

    gtest_discover_tests("${FVG_TEST_TARGET}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )

    add_custom_target("run_${FVG_TEST_TARGET}"
        COMMAND "$<TARGET_FILE:${FVG_TEST_TARGET}>"
        DEPENDS "${FVG_TEST_TARGET}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        VERBATIM
    )
endforeach()