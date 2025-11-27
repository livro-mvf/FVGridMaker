# ------------------------------------------------------------
# Main library target
# ------------------------------------------------------------

# Find sources
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
     "${FVG_SRC_DIR}/*.cpp")

# Create library
add_library(FVGridMaker SHARED ${SOURCES})

# Adiciona diretórios de include (Modo Moderno)
target_include_directories(FVGridMaker
    PUBLIC
        ${FVG_INCLUDE_DIR}
)

## Link dependencies (RNF06: Nenhuma)
#target_link_libraries(FVGridMaker
#    PUBLIC
#)

# Set optimizations
set_target_optimizations(FVGridMaker)

# Output directories (usa variável adaptada)
set_target_properties(FVGridMaker PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${FVG_OUTPUT_BIN_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${FVG_OUTPUT_BIN_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${FVG_OUTPUT_BIN_DIR}
)

# Install rules
install(TARGETS FVGridMaker
    EXPORT FVGridMakerTargets
    LIBRARY DESTINATION ${FVG_OUTPUT_BIN_DIR}
    ARCHIVE DESTINATION ${FVG_OUTPUT_BIN_DIR}
    RUNTIME DESTINATION ${FVG_OUTPUT_BIN_DIR})

# Instala o header namespaced (FVGridMakerLib/include/FVGridMaker)
install(DIRECTORY ${FVG_INCLUDE_DIR}/FVGridMaker
    DESTINATION include
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.tpp")

# Instala arquivos .tpp (template implementations) do src/
install(DIRECTORY ${FVG_SRC_DIR}
    DESTINATION include
    FILES_MATCHING PATTERN "*.tpp")