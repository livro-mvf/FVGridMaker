# ----------------------------------------------------------------------------
# File: CMakeLists.txt
# Project: FVGridMaker
# Version: 0.1.0
# Description: Discovers and builds FVGridMaker books programs.
# Author: FVGridMaker Team
# License: MIT
# ----------------------------------------------------------------------------

file(GLOB_RECURSE FVGRIDMAKER_EXAMPLE_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Capitulos/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/Capitulos/*.cpp"
)

set(FVGRIDMAKER_EXAMPLE_RUN_TARGETS "")

foreach(example_source IN LISTS FVGRIDMAKER_EXAMPLE_SOURCES)
    get_filename_component(example_name "${example_source}" NAME_WE)

    add_executable("${example_name}" "${example_source}")

    target_link_libraries("${example_name}"
        PRIVATE
            FVGridMaker
    )

    target_include_directories("${example_name}"
        PRIVATE
            "${PROJECT_SOURCE_DIR}/FVGridMakerLib/include"
    )

    set(example_run_target "run_${example_name}")

    add_custom_target("${example_run_target}"
        COMMAND "$<TARGET_FILE:${example_name}>"
        DEPENDS "${example_name}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        COMMENT "Running ${example_name}"
        VERBATIM
    )

    list(APPEND FVGRIDMAKER_EXAMPLE_RUN_TARGETS "${example_run_target}")
endforeach()

add_custom_target(run_all_book
    COMMENT "Running all FVGridMaker book examples"
)

if(FVGRIDMAKER_EXAMPLE_RUN_TARGETS)
    add_dependencies(run_all_book ${FVGRIDMAKER_EXAMPLE_RUN_TARGETS})
endif()