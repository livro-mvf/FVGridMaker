# ==============================================================================
# ConfigTargets.cmake - Configuração dos targets da biblioteca FVGridMaker
# ==============================================================================

include_guard()

# ------------------------------------------------------------------------------
# 1. MÓDULOS NECESSÁRIOS
# ------------------------------------------------------------------------------
include(GNUInstallDirs)

# ------------------------------------------------------------------------------
# 2. CONFIGURAÇÃO DO TARGET PRINCIPAL (BIBLIOTECA SHARED)
# ------------------------------------------------------------------------------

# Busca todos os arquivos fonte da biblioteca
file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS
    "${FVG_SRC_DIR}/*.cpp"
    "${FVG_SRC_DIR}/*.cxx"
    "${FVG_SRC_DIR}/*.cc"
)

# Busca todos os headers públicos da biblioteca (para verificação)
file(GLOB_RECURSE LIB_HEADERS CONFIGURE_DEPENDS
    "${FVG_INCLUDE_DIR}/FVGridMaker/*.hpp"
    "${FVG_INCLUDE_DIR}/FVGridMaker/*.h"
    "${FVG_INCLUDE_DIR}/FVGridMaker/*.tpp"
)

# Verifica se há fontes disponíveis
list(LENGTH LIB_SOURCES NUM_SOURCES)
if(NUM_SOURCES EQUAL 0)
    message(WARNING "Nenhum arquivo fonte encontrado em ${FVG_SRC_DIR}")
endif()

message(STATUS "Biblioteca FVGridMaker:")
message(STATUS "  Fontes: ${NUM_SOURCES} arquivos")
list(LENGTH LIB_HEADERS NUM_HEADERS)
message(STATUS "  Headers: ${NUM_HEADERS} arquivos")

# Cria a biblioteca dinâmica (shared)
add_library(FVGridMaker SHARED ${LIB_SOURCES})

# Define propriedades da biblioteca
set_target_properties(FVGridMaker PROPERTIES
    # Informações de versão da biblioteca (SOVERSION)
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    
    # Informações descritivas
    OUTPUT_NAME "FVGridMaker"
    DEBUG_POSTFIX "_d"
    
    # Metadados do projeto
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    
    # Configurações específicas para Windows
    WINDOWS_EXPORT_ALL_SYMBOLS OFF
)

# ------------------------------------------------------------------------------
# 3. DIRETÓRIOS DE INCLUDE
# ------------------------------------------------------------------------------

# Verifica se a variável FVG_GENERATED_INCLUDE_DIR foi definida
if(NOT DEFINED FVG_GENERATED_INCLUDE_DIR)
    set(FVG_GENERATED_INCLUDE_DIR "${CMAKE_BINARY_DIR}/generated_include")
endif()

if(NOT DEFINED FVG_VERSION_HEADER)
    set(FVG_VERSION_HEADER "${FVG_GENERATED_INCLUDE_DIR}/FVGridMaker/version.hpp")
endif()

# Cria diretório para headers gerados
file(MAKE_DIRECTORY "${FVG_GENERATED_INCLUDE_DIR}/FVGridMaker")

target_include_directories(FVGridMaker
    PUBLIC
        # Headers públicos da biblioteca
        $<BUILD_INTERFACE:${FVG_INCLUDE_DIR}>
        
        # Headers gerados automaticamente (version.hpp)
        $<BUILD_INTERFACE:${FVG_GENERATED_INCLUDE_DIR}>
        
        # Para quando a biblioteca for instalada
        $<INSTALL_INTERFACE:include>
    PRIVATE
        # Fontes podem incluir de outros diretórios privados
        ${FVG_SRC_DIR}
)

# ------------------------------------------------------------------------------
# 4. CONFIGURAÇÕES DE COMPILAÇÃO E LINK
# ------------------------------------------------------------------------------

# Aplica otimizações específicas para o target
if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    # Otimizações agressivas para Release
    target_compile_options(FVGridMaker PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-O3>
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-march=native>
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-ffast-math>
    )
    
    # Link Time Optimization
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(FVGridMaker PRIVATE -flto)
        target_link_options(FVGridMaker PRIVATE -flto)
    endif()
    
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Configurações para Debug
    target_compile_definitions(FVGridMaker PRIVATE
        FVG_DEBUG=1
        FVG_ENABLE_ASSERTIONS=1
    )
    
    target_compile_options(FVGridMaker PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-O0>
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-g3>
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-fno-omit-frame-pointer>
    )
endif()

# Aplica otimizações padrão da função definida em ConfigCompiler.cmake
if(COMMAND set_target_optimizations)
    set_target_optimizations(FVGridMaker)
endif()

# ------------------------------------------------------------------------------
# 5. DIRETÓRIOS DE SAÍDA
# ------------------------------------------------------------------------------

# Configura diretórios de saída organizados
set(OUTPUT_RUNTIME_DIR "${FVG_OUTPUT_BIN_DIR}")
set(OUTPUT_LIBRARY_DIR "${FVG_OUTPUT_BIN_DIR}")
set(OUTPUT_ARCHIVE_DIR "${FVG_OUTPUT_BIN_DIR}")

set_target_properties(FVGridMaker PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_ARCHIVE_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_LIBRARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_RUNTIME_DIR}"
)

# Cria os diretórios se não existirem
file(MAKE_DIRECTORY "${OUTPUT_RUNTIME_DIR}")
file(MAKE_DIRECTORY "${OUTPUT_LIBRARY_DIR}")
file(MAKE_DIRECTORY "${OUTPUT_ARCHIVE_DIR}")

# ------------------------------------------------------------------------------
# 6. REGRAS DE INSTALAÇÃO
# ------------------------------------------------------------------------------

# Exporta o target para uso por outros projetos
install(TARGETS FVGridMaker
    EXPORT FVGridMakerTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Instala headers públicos
install(DIRECTORY ${FVG_INCLUDE_DIR}/FVGridMaker
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING
        PATTERN "*.hpp"
        PATTERN "*.h"
        PATTERN "*.tpp"
    PATTERN "*.in" EXCLUDE
    PATTERN ".gitkeep" EXCLUDE
)

# Instala headers gerados automaticamente (version.hpp)
if(EXISTS "${FVG_VERSION_HEADER}")
    install(FILES ${FVG_VERSION_HEADER}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/FVGridMaker
    )
endif()

# Instala implementações de templates (.tpp) do diretório src/
if(EXISTS "${FVG_SRC_DIR}")
    file(GLOB_RECURSE TPP_FILES "${FVG_SRC_DIR}/*.tpp")
    if(TPP_FILES)
        install(FILES ${TPP_FILES}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/FVGridMaker/internal
        )
        message(STATUS "Instalando templates de implementação")
    endif()
endif()

# Cria arquivo de configuração para o pacote
install(EXPORT FVGridMakerTargets
    FILE FVGridMakerTargets.cmake
    NAMESPACE FVGridMaker::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/FVGridMaker
)

# ------------------------------------------------------------------------------
# 7. TARGETS ADICIONAIS ÚTEIS
# ------------------------------------------------------------------------------

# Target para limpeza de builds antigos
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "Removendo diretório de build completo"
)

# Target para mostrar informações da biblioteca
add_custom_target(info
    COMMAND ${CMAKE_COMMAND} -E echo "=== FVGridMaker Library Info ==="
    COMMAND ${CMAKE_COMMAND} -E echo "Version: @FVG_VERSION_FULL@"
    COMMAND ${CMAKE_COMMAND} -E echo "Sources: ${NUM_SOURCES} files"
    COMMAND ${CMAKE_COMMAND} -E echo "Headers: ${NUM_HEADERS} files"
    COMMAND ${CMAKE_COMMAND} -E echo "Build type: ${CMAKE_BUILD_TYPE}"
    COMMAND ${CMAKE_COMMAND} -E echo "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}"
    COMMAND ${CMAKE_COMMAND} -E echo "Output dir: ${FVG_OUTPUT_BIN_DIR}"
    COMMENT "Exibindo informações da biblioteca"
)

# ------------------------------------------------------------------------------
# 8. MENSAGENS FINAIS
# ------------------------------------------------------------------------------

message(STATUS "✅ Target FVGridMaker configurado com sucesso!")
message(STATUS "   Tipo: Biblioteca Dinâmica (SHARED)")
message(STATUS "   Versão: @FVG_VERSION_SEMANTIC@")
message(STATUS "   SOVERSION: @PROJECT_VERSION_MAJOR@")
message(STATUS "   Saída: ${FVG_OUTPUT_BIN_DIR}")