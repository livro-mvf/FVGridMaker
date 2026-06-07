add_executable(ex_Minimal
    Ex_Minimal/ex_Minimal.cc
)

target_link_libraries(ex_Minimal
    PRIVATE
        FVGridMaker
)

set_target_properties(ex_Minimal
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)

add_custom_target(run_ex_Minimal
    COMMAND $<TARGET_FILE:ex_Minimal>
    DEPENDS ex_Minimal
    WORKING_DIRECTORY "$<TARGET_FILE_DIR:ex_Minimal>"
    COMMENT "Executando exemplo ex_Minimal..."
    VERBATIM
)
