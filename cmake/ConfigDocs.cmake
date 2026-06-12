include_guard()

if(NOT BUILD_DOCUMENTATION)
    return()
endif()

find_package(Doxygen QUIET)
find_package(Python3 COMPONENTS Interpreter QUIET)

set(FVG_DOCS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/docs_sphinx")
set(FVG_DOCS_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/docs_sphinx")
set(FVG_DOCS_HTML_DIR "${FVG_DOCS_BUILD_DIR}/html")
set(FVG_DOXYGEN_INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in")
set(FVG_DOXYGEN_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
set(FVG_DOXYGEN_XML_DIR "${CMAKE_CURRENT_BINARY_DIR}/doxygen/xml")
set(FVG_DOCS_DEPENDENCIES)

if(DOXYGEN_FOUND AND EXISTS "${FVG_DOXYGEN_INPUT_FILE}")
    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doxygen")
    set(DOXYGEN_XML_OUTPUT "xml")
    set(DOXYGEN_GENERATE_XML "YES")
    set(DOXYGEN_GENERATE_HTML "NO")
    set(DOXYGEN_PROJECT_NAME "${PROJECT_NAME}")
    set(DOXYGEN_PROJECT_NUMBER "${PROJECT_VERSION}")

    configure_file(
        "${FVG_DOXYGEN_INPUT_FILE}"
        "${FVG_DOXYGEN_OUTPUT_FILE}"
        @ONLY
    )

    add_custom_target(doxygen_xml
        COMMAND "${DOXYGEN_EXECUTABLE}" "${FVG_DOXYGEN_OUTPUT_FILE}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Generating Doxygen XML documentation"
        VERBATIM
    )

    list(APPEND FVG_DOCS_DEPENDENCIES doxygen_xml)
elseif(NOT DOXYGEN_FOUND)
    message(WARNING
        "Doxygen was not found. Doxygen XML documentation will not be built."
    )
elseif(NOT EXISTS "${FVG_DOXYGEN_INPUT_FILE}")
    message(WARNING
        "Doxyfile.in was not found. Doxygen XML documentation will not be built."
    )
endif()

if(NOT Python3_FOUND)
    message(WARNING
        "Python3 was not found. Sphinx documentation will not be built."
    )

    if(TARGET doxygen_xml)
        add_custom_target(docs
            DEPENDS doxygen_xml
        )
    endif()

    return()
endif()

if(NOT EXISTS "${FVG_DOCS_SOURCE_DIR}/conf.py")
    message(WARNING
        "Sphinx source directory or conf.py was not found. "
        "Sphinx documentation will not be built."
    )

    if(TARGET doxygen_xml)
        add_custom_target(docs
            DEPENDS doxygen_xml
        )
    endif()

    return()
endif()

set(FVG_DOCS_VENV_DIR "${CMAKE_CURRENT_BINARY_DIR}/.venv-docs")

if(WIN32)
    set(FVG_DOCS_VENV_PYTHON
        "${FVG_DOCS_VENV_DIR}/Scripts/python.exe"
    )
    set(FVG_DOCS_SPHINX_BUILD
        "${FVG_DOCS_VENV_DIR}/Scripts/sphinx-build.exe"
    )
else()
    set(FVG_DOCS_VENV_PYTHON
        "${FVG_DOCS_VENV_DIR}/bin/python3"
    )
    set(FVG_DOCS_SPHINX_BUILD
        "${FVG_DOCS_VENV_DIR}/bin/sphinx-build"
    )
endif()

set(FVG_DOCS_REQUIREMENTS_FILE
    "${FVG_DOCS_SOURCE_DIR}/requirements.txt"
)

add_custom_command(
    OUTPUT "${FVG_DOCS_SPHINX_BUILD}"
    COMMAND "${Python3_EXECUTABLE}" -m venv "${FVG_DOCS_VENV_DIR}"
    COMMAND "${FVG_DOCS_VENV_PYTHON}" -m pip install --upgrade pip
    COMMAND "${FVG_DOCS_VENV_PYTHON}" -m pip install sphinx breathe
    COMMAND
        "${CMAKE_COMMAND}" -E env
        "PIP_DISABLE_PIP_VERSION_CHECK=1"
        "${FVG_DOCS_VENV_PYTHON}" -m pip install
        $<$<BOOL:$<EXISTS:${FVG_DOCS_REQUIREMENTS_FILE}>>:-r>
        $<$<BOOL:$<EXISTS:${FVG_DOCS_REQUIREMENTS_FILE}>>:${FVG_DOCS_REQUIREMENTS_FILE}>
    COMMENT "Preparing Python virtual environment for Sphinx"
    VERBATIM
)

add_custom_target(sphinx_html
    COMMAND
        "${CMAKE_COMMAND}" -E env
        "FVG_DOXYGEN_XML_DIR=${FVG_DOXYGEN_XML_DIR}"
        "${FVG_DOCS_SPHINX_BUILD}"
        -b html
        "${FVG_DOCS_SOURCE_DIR}"
        "${FVG_DOCS_HTML_DIR}"
    DEPENDS
        "${FVG_DOCS_SPHINX_BUILD}"
        ${FVG_DOCS_DEPENDENCIES}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Generating Sphinx HTML documentation"
    VERBATIM
)

add_custom_target(docs
    DEPENDS sphinx_html
)