include_guard()

option(BUILD_TESTS "Build test targets" OFF)
option(BUILD_EXAMPLES "Build example targets" ON)
option(BUILD_BOOK "Build book/chapter targets" OFF)
option(BUILD_DOCUMENTATION "Build documentation targets" OFF)

option(FVG_ENABLE_IPO "Enable IPO/LTO for supported release builds" ON)
option(FVG_ENABLE_NATIVE_OPTIMIZATION "Enable native CPU optimisation flags" ON)
option(FVG_ENABLE_NDEBUG "Define NDEBUG explicitly for release-like builds" OFF)
option(FVG_BUILD_SHARED "Build FVGridMaker as a shared library" OFF)

set(FVG_ALLOWED_BUILD_TYPES
    Debug
    Release
    RelWithDebInfo
    MinSizeRel
)

if(NOT CMAKE_CONFIGURATION_TYPES)
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    endif()

    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        ${FVG_ALLOWED_BUILD_TYPES}
    )

    if(NOT CMAKE_BUILD_TYPE IN_LIST FVG_ALLOWED_BUILD_TYPES)
        message(FATAL_ERROR
            "Invalid CMAKE_BUILD_TYPE='${CMAKE_BUILD_TYPE}'. "
            "Allowed values are: ${FVG_ALLOWED_BUILD_TYPES}"
        )
    endif()
endif()

if(BUILD_TESTS AND BUILD_EXAMPLES)
    message(FATAL_ERROR
        "BUILD_TESTS and BUILD_EXAMPLES cannot be enabled simultaneously."
    )
endif()

if(BUILD_TESTS AND BUILD_BOOK)
    message(FATAL_ERROR
        "BUILD_TESTS and BUILD_BOOK cannot be enabled simultaneously."
    )
endif()

if(FVG_BUILD_SHARED)
    set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)
else()
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
endif()