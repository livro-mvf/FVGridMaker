include_guard()

if(NOT BUILD_EXAMPLES)
    return()
endif()

set(FVG_EXAMPLES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/examples")

if(NOT EXISTS "${FVG_EXAMPLES_DIR}")
    set(FVG_EXAMPLES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Examples")
endif()

if(NOT EXISTS "${FVG_EXAMPLES_DIR}")
    message(WARNING "Examples directory was not found.")
    return()
endif()

file(GLOB_RECURSE FVG_EXAMPLE_SOURCES CONFIGURE_DEPENDS
    "${FVG_EXAMPLES_DIR}/ex_*.cpp"
    "${FVG_EXAMPLES_DIR}/ex_*.cxx"
    "${FVG_EXAMPLES_DIR}/ex_*.cc"
)

if(NOT FVG_EXAMPLE_SOURCES)
    message(WARNING
        "No example entry-point files matching ex_*.cpp, ex_*.cxx or "
        "ex_*.cc were found in '${FVG_EXAMPLES_DIR}'."
    )
    return()
endif()

set(FVG_EXAMPLE_RUN_TARGETS)

foreach(FVG_EXAMPLE_SOURCE IN LISTS FVG_EXAMPLE_SOURCES)
    file(RELATIVE_PATH
        FVG_EXAMPLE_RELATIVE_PATH
        "${FVG_EXAMPLES_DIR}"
        "${FVG_EXAMPLE_SOURCE}"
    )

    get_filename_component(FVG_EXAMPLE_NAME
        "${FVG_EXAMPLE_RELATIVE_PATH}"
        NAME_WE
    )

    set(FVG_EXAMPLE_NAME_TOKEN "${FVG_EXAMPLE_NAME}")

    if(FVG_EXAMPLE_NAME_TOKEN MATCHES "^ex_(.+)$")
        set(FVG_EXAMPLE_NAME_TOKEN "${CMAKE_MATCH_1}")
    endif()

    string(REGEX REPLACE "[^A-Za-z0-9_]" "_" FVG_EXAMPLE_NAME_TOKEN
        "${FVG_EXAMPLE_NAME_TOKEN}"
    )

    set(FVG_EXAMPLE_TARGET "ex_${FVG_EXAMPLE_NAME_TOKEN}")

    if(TARGET "${FVG_EXAMPLE_TARGET}")
        message(FATAL_ERROR
            "Duplicate example target detected: ${FVG_EXAMPLE_TARGET}. "
            "Example executable names must be unique."
        )
    endif()

    add_executable("${FVG_EXAMPLE_TARGET}"
        "${FVG_EXAMPLE_SOURCE}"
    )

    target_link_libraries("${FVG_EXAMPLE_TARGET}"
        PRIVATE
            FVGridMaker::FVGridMaker
    )

    target_compile_features("${FVG_EXAMPLE_TARGET}"
        PRIVATE
            cxx_std_20
    )

    set_target_properties("${FVG_EXAMPLE_TARGET}"
        PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/examples"
    )

    add_custom_target("run_${FVG_EXAMPLE_TARGET}"
        COMMAND "$<TARGET_FILE:${FVG_EXAMPLE_TARGET}>"
        DEPENDS "${FVG_EXAMPLE_TARGET}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        VERBATIM
    )

    list(APPEND FVG_EXAMPLE_RUN_TARGETS
        "run_${FVG_EXAMPLE_TARGET}"
    )
endforeach()

if(FVG_EXAMPLE_RUN_TARGETS)
    add_custom_target(run_all_examples
        DEPENDS ${FVG_EXAMPLE_RUN_TARGETS}
    )
endif()
