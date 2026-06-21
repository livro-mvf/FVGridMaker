include_guard()

if(NOT BUILD_DOCUMENTATION)
    return()
endif()

find_package(Doxygen QUIET)
find_package(Python3 COMPONENTS Interpreter REQUIRED)

set(FVG_DOCS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/docs")
set(FVG_DOCS_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/docs")
set(FVG_DOCS_HTML_DIR "${FVG_DOCS_BUILD_DIR}/html")
set(FVG_DOXYGEN_INPUT_FILE "${FVG_DOCS_SOURCE_DIR}/Doxyfile.in")
set(FVG_DOXYGEN_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
set(FVG_DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/doxygen")
set(FVG_DOXYGEN_XML_DIR "${FVG_DOXYGEN_OUTPUT_DIR}/xml")
set(FVG_DOCS_DEPENDENCIES)

if(DOXYGEN_FOUND)
    configure_file(
        "${FVG_DOXYGEN_INPUT_FILE}"
        "${FVG_DOXYGEN_OUTPUT_FILE}"
        @ONLY
    )
    add_custom_target(doxygen_xml
        COMMAND "${DOXYGEN_EXECUTABLE}" "${FVG_DOXYGEN_OUTPUT_FILE}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Generating FVGridMaker Doxygen XML"
        VERBATIM
    )
    list(APPEND FVG_DOCS_DEPENDENCIES doxygen_xml)
else()
    message(WARNING "Doxygen not found: API XML will be omitted.")
endif()

execute_process(
    COMMAND "${Python3_EXECUTABLE}" -m sphinx --version
    RESULT_VARIABLE FVG_SPHINX_RESULT
    OUTPUT_QUIET ERROR_QUIET
)
if(NOT FVG_SPHINX_RESULT EQUAL 0)
    message(FATAL_ERROR
        "Sphinx is required for BUILD_DOCUMENTATION. "
        "Install docs/requirements.txt in the selected Python environment."
    )
endif()

add_custom_target(sphinx_html
    COMMAND "${CMAKE_COMMAND}" -E env
        "FVG_DOXYGEN_XML_DIR=${FVG_DOXYGEN_XML_DIR}"
        "FVG_VERSION=${FVG_VERSION_FULL}"
        "${Python3_EXECUTABLE}" -m sphinx
        -W --keep-going -b html
        "${FVG_DOCS_SOURCE_DIR}"
        "${FVG_DOCS_HTML_DIR}"
    DEPENDS ${FVG_DOCS_DEPENDENCIES}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Generating FVGridMaker Sphinx HTML"
    VERBATIM
)

add_custom_target(docs DEPENDS sphinx_html)
