# ----------------------------------------------------------------------------
# File: CMakeLists.txt
# Project: FVGridMaker
# Version: 0.1.0
# Description: Discovers and builds FVGridMaker example programs.
# Author: FVGridMaker Team
# License: MIT
# ----------------------------------------------------------------------------

file(GLOB_RECURSE FVGRIDMAKER_EXAMPLE_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*/ex_*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/*/ex_*.cpp"
)

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

    add_custom_target("run_${example_name}"
        COMMAND "${example_name}"
        DEPENDS "${example_name}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )
endforeach()