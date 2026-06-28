include_guard()

if(NOT BUILD_EXAMPLES)
    return()
endif()

set(FVG_MANUAL_EXAMPLES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/manual")

if(NOT EXISTS "${FVG_MANUAL_EXAMPLES_DIR}")
    set(FVG_MANUAL_EXAMPLES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Manual")
endif()

if(NOT EXISTS "${FVG_MANUAL_EXAMPLES_DIR}")
    return()
endif()

file(GLOB_RECURSE FVG_MANUAL_PROGRAM_SOURCES CONFIGURE_DEPENDS
    "${FVG_MANUAL_EXAMPLES_DIR}/man_*.cpp"
    "${FVG_MANUAL_EXAMPLES_DIR}/man_*.cxx"
    "${FVG_MANUAL_EXAMPLES_DIR}/man_*.cc"
)

if(NOT FVG_MANUAL_PROGRAM_SOURCES)
    message(STATUS
        "No manual entry-point files matching man_*.cpp, "
        "man_*.cxx or man_*.cc were found in "
        "'${FVG_MANUAL_EXAMPLES_DIR}'."
    )
    return()
endif()

set(FVG_MANUAL_PROGRAM_RUN_TARGETS)

foreach(FVG_MANUAL_PROGRAM_SOURCE IN LISTS FVG_MANUAL_PROGRAM_SOURCES)
    file(RELATIVE_PATH
        FVG_MANUAL_PROGRAM_RELATIVE_PATH
        "${FVG_MANUAL_EXAMPLES_DIR}"
        "${FVG_MANUAL_PROGRAM_SOURCE}"
    )

    get_filename_component(FVG_MANUAL_PROGRAM_NAME
        "${FVG_MANUAL_PROGRAM_RELATIVE_PATH}"
        NAME_WE
    )

    set(FVG_MANUAL_PROGRAM_NAME_TOKEN "${FVG_MANUAL_PROGRAM_NAME}")

    if(FVG_MANUAL_PROGRAM_NAME_TOKEN MATCHES "^man_(.+)$")
        set(FVG_MANUAL_PROGRAM_NAME_TOKEN "${CMAKE_MATCH_1}")
    endif()

    string(REGEX REPLACE "[^A-Za-z0-9_]" "_" FVG_MANUAL_PROGRAM_NAME_TOKEN
        "${FVG_MANUAL_PROGRAM_NAME_TOKEN}"
    )

    set(FVG_MANUAL_PROGRAM_TARGET "man_${FVG_MANUAL_PROGRAM_NAME_TOKEN}")

    if(TARGET "${FVG_MANUAL_PROGRAM_TARGET}")
        message(FATAL_ERROR
            "Duplicate manual program target detected: "
            "${FVG_MANUAL_PROGRAM_TARGET}. "
            "Manual program executable names must be unique."
        )
    endif()

    add_executable("${FVG_MANUAL_PROGRAM_TARGET}"
        "${FVG_MANUAL_PROGRAM_SOURCE}"
    )

    target_link_libraries("${FVG_MANUAL_PROGRAM_TARGET}"
        PRIVATE
            FVGridMaker::FVGridMaker
    )

    target_compile_features("${FVG_MANUAL_PROGRAM_TARGET}"
        PRIVATE
            cxx_std_20
    )

    set_target_properties("${FVG_MANUAL_PROGRAM_TARGET}"
        PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/manual"
    )

    add_custom_target("run_${FVG_MANUAL_PROGRAM_TARGET}"
        COMMAND "$<TARGET_FILE:${FVG_MANUAL_PROGRAM_TARGET}>"
        DEPENDS "${FVG_MANUAL_PROGRAM_TARGET}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        VERBATIM
    )

    list(APPEND FVG_MANUAL_PROGRAM_RUN_TARGETS
        "run_${FVG_MANUAL_PROGRAM_TARGET}"
    )
endforeach()

if(FVG_MANUAL_PROGRAM_RUN_TARGETS)
    add_custom_target(run_all_manual_programs
        DEPENDS ${FVG_MANUAL_PROGRAM_RUN_TARGETS}
    )
endif()