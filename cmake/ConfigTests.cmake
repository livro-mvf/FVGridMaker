# ----------------------------------------------------------------------------
# File: ConfigTests.cmake
# Project: FVGridMaker
# Version: 0.1.0
# Description: Configures the FVGridMaker test infrastructure.
# Author: FVGridMaker Team
# License: MIT
# ----------------------------------------------------------------------------

include_guard()

if(NOT BUILD_TESTS)
    return()
endif()

message(STATUS "[tests] Building tests...")

include(CTest)
enable_testing()

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_subdirectory(tests)