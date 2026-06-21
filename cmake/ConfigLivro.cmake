include_guard()

if(NOT BUILD_BOOK)
    return()
endif()

set(FVG_BOOK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/capitulos")

if(NOT EXISTS "${FVG_BOOK_DIR}")
    message(WARNING "Directory not found: ${FVG_BOOK_DIR}")
    return()
endif()

file(GLOB_RECURSE FVG_BOOK_SOURCES CONFIGURE_DEPENDS
    "${FVG_BOOK_DIR}/ex_*.cpp"
    "${FVG_BOOK_DIR}/ex_*.cc"
)

set(FVG_BOOK_RUN_TARGETS)

foreach(FVG_BOOK_SOURCE IN LISTS FVG_BOOK_SOURCES)
    file(RELATIVE_PATH
        FVG_BOOK_RELATIVE_PATH
        "${FVG_BOOK_DIR}"
        "${FVG_BOOK_SOURCE}"
    )

    get_filename_component(FVG_BOOK_NAME
        "${FVG_BOOK_RELATIVE_PATH}"
        NAME_WE
    )

    string(REGEX REPLACE "^ex_" "" FVG_BOOK_NAME_TOKEN
        "${FVG_BOOK_NAME}"
    )

    string(REGEX REPLACE "[^A-Za-z0-9_]" "_" FVG_BOOK_NAME_TOKEN
        "${FVG_BOOK_NAME_TOKEN}"
    )

    set(FVG_BOOK_TARGET
        "cap_${FVG_BOOK_NAME_TOKEN}"
    )

    if(TARGET "${FVG_BOOK_TARGET}")
        message(FATAL_ERROR
            "Duplicate book/chapter target detected: ${FVG_BOOK_TARGET}. "
            "Book/chapter executable names must be unique."
        )
    endif()

    add_executable("${FVG_BOOK_TARGET}"
        "${FVG_BOOK_SOURCE}"
    )

    target_link_libraries("${FVG_BOOK_TARGET}"
        PRIVATE
            FVGridMaker::FVGridMaker
    )

    target_compile_features("${FVG_BOOK_TARGET}"
        PRIVATE
            cxx_std_20
    )

    set_target_properties("${FVG_BOOK_TARGET}"
        PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/capitulos"
    )

    add_custom_target("run_${FVG_BOOK_TARGET}"
        COMMAND "$<TARGET_FILE:${FVG_BOOK_TARGET}>"
        DEPENDS "${FVG_BOOK_TARGET}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        VERBATIM
    )

    list(APPEND FVG_BOOK_RUN_TARGETS
        "run_${FVG_BOOK_TARGET}"
    )
endforeach()

if(FVG_BOOK_RUN_TARGETS)
    add_custom_target(run_all_capitulos
        DEPENDS ${FVG_BOOK_RUN_TARGETS}
    )
endif()
