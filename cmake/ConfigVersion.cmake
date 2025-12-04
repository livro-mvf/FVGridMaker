# ------------------------------------------------------------------------------
# ConfigVersion.cmake
# Versionamento automático para FVGridMaker
# ------------------------------------------------------------------------------

include_guard()

# ------------------------------------------------------------------------------
# 1. BUSCA POR GIT E OBTÉM VERSÃO
# ------------------------------------------------------------------------------

# Busca Git (opcional, mas recomendado)
find_package(Git QUIET)

# Variáveis de versão padrão
set(FVG_PROJECT_VERSION_MAJOR 1)
set(FVG_PROJECT_VERSION_MINOR 0)
set(FVG_PROJECT_VERSION_PATCH 0)
set(FVG_PROJECT_VERSION_TWEAK 0)
set(FVG_GIT_HASH "unknown")
set(FVG_GIT_BRANCH "unknown")
set(FVG_BUILD_TIMESTAMP "")

# Se Git estiver disponível, obtém informações de versão
if(GIT_FOUND)
    # Obtém hash do commit atual (curto)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE FVG_GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    
    # Obtém branch atual
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE FVG_GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    
    # Tenta obter versão a partir de tags (semantic versioning)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --always --match "v[0-9]*.[0-9]*.[0-9]*"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE FVG_GIT_DESCRIBE
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    
    # Se obteve uma tag, extrai versão semântica
    if(FVG_GIT_DESCRIBE AND FVG_GIT_DESCRIBE MATCHES "v?([0-9]+)\\.([0-9]+)\\.([0-9]+)")
        set(FVG_PROJECT_VERSION_MAJOR ${CMAKE_MATCH_1})
        set(FVG_PROJECT_VERSION_MINOR ${CMAKE_MATCH_2})
        set(FVG_PROJECT_VERSION_PATCH ${CMAKE_MATCH_3})
    endif()
    
    # Verifica se há modificações não commitadas
    execute_process(
        COMMAND ${GIT_EXECUTABLE} status --porcelain
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_STATUS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
    if(GIT_STATUS)
        set(FVG_GIT_DIRTY_SUFFIX "-dirty")
    else()
        set(FVG_GIT_DIRTY_SUFFIX "")
    endif()
endif()

# Define timestamp de build
string(TIMESTAMP FVG_BUILD_TIMESTAMP "%Y-%m-%d %H:%M:%S" UTC)
string(TIMESTAMP FVG_BUILD_DATE "%Y-%m-%d" UTC)

# ------------------------------------------------------------------------------
# 2. DEFINE VERSÃO COMPLETA
# ------------------------------------------------------------------------------

# Versão semântica completa
set(FVG_VERSION_SEMANTIC 
    "${FVG_PROJECT_VERSION_MAJOR}.${FVG_PROJECT_VERSION_MINOR}.${FVG_PROJECT_VERSION_PATCH}"
)

# Versão com informações do Git (se disponível)
if(GIT_FOUND AND FVG_GIT_HASH)
    set(FVG_VERSION_FULL 
        "${FVG_VERSION_SEMANTIC}+${FVG_GIT_HASH}${FVG_GIT_DIRTY_SUFFIX}"
    )
else()
    set(FVG_VERSION_FULL 
        "${FVG_VERSION_SEMANTIC}+nogit"
    )
endif()

# Define versão do projeto CMake (para uso interno)
set(PROJECT_VERSION ${FVG_VERSION_SEMANTIC})
set(PROJECT_VERSION_MAJOR ${FVG_PROJECT_VERSION_MAJOR})
set(PROJECT_VERSION_MINOR ${FVG_PROJECT_VERSION_MINOR})
set(PROJECT_VERSION_PATCH ${FVG_PROJECT_VERSION_PATCH})

# ------------------------------------------------------------------------------
# 3. GERA ARQUIVO DE HEADER COM INFORMAÇÕES DE VERSÃO
# ------------------------------------------------------------------------------

# Define diretório de saída para headers gerados
set(FVG_GENERATED_INCLUDE_DIR "${CMAKE_BINARY_DIR}/generated_include")
set(FVG_VERSION_HEADER "${FVG_GENERATED_INCLUDE_DIR}/FVGridMaker/version.hpp")

# Cria o diretório se não existir
file(MAKE_DIRECTORY "${FVG_GENERATED_INCLUDE_DIR}/FVGridMaker")

# Template do arquivo de versão
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/version.hpp.in"
    "${FVG_VERSION_HEADER}"
    @ONLY
)

# Mensagens informativas
message(STATUS "---------------------------------------------------")
message(STATUS "FVGridMaker Version Configuration:")
message(STATUS "  Semantic Version:  ${FVG_VERSION_SEMANTIC}")
message(STATUS "  Full Version:      ${FVG_VERSION_FULL}")
message(STATUS "  Git Hash:          ${FVG_GIT_HASH}")
message(STATUS "  Git Branch:        ${FVG_GIT_BRANCH}")
message(STATUS "  Build Timestamp:   ${FVG_BUILD_TIMESTAMP}")
if(GIT_FOUND)
    message(STATUS "  Git Available:     YES")
else()
    message(STATUS "  Git Available:     NO - using default version")
endif()
message(STATUS "---------------------------------------------------")

# ------------------------------------------------------------------------------
# 4. FUNÇÕES ÚTEIS PARA VERSIONAMENTO
# ------------------------------------------------------------------------------

# Função para incrementar versão via script
function(fvg_increment_version TYPE)
    if(TYPE STREQUAL "MAJOR")
        math(EXPR NEW_MAJOR "${FVG_PROJECT_VERSION_MAJOR} + 1")
        set(FVG_PROJECT_VERSION_MAJOR ${NEW_MAJOR} PARENT_SCOPE)
        set(FVG_PROJECT_VERSION_MINOR 0 PARENT_SCOPE)
        set(FVG_PROJECT_VERSION_PATCH 0 PARENT_SCOPE)
    elseif(TYPE STREQUAL "MINOR")
        math(EXPR NEW_MINOR "${FVG_PROJECT_VERSION_MINOR} + 1")
        set(FVG_PROJECT_VERSION_MINOR ${NEW_MINOR} PARENT_SCOPE)
        set(FVG_PROJECT_VERSION_PATCH 0 PARENT_SCOPE)
    elseif(TYPE STREQUAL "PATCH")
        math(EXPR NEW_PATCH "${FVG_PROJECT_VERSION_PATCH} + 1")
        set(FVG_PROJECT_VERSION_PATCH ${NEW_PATCH} PARENT_SCOPE)
    else()
        message(FATAL_ERROR "Tipo de incremento inválido. Use: MAJOR, MINOR ou PATCH")
    endif()
endfunction()

# Função para criar tag de release
function(fvg_create_release_tag)
    if(NOT GIT_FOUND)
        message(WARNING "Git não encontrado. Não é possível criar tag.")
        return()
    endif()
    
    set(TAG_NAME "v${FVG_VERSION_SEMANTIC}")
    
    message(STATUS "Criando tag de release: ${TAG_NAME}")
    
    # Verifica se a tag já existe
    execute_process(
        COMMAND ${GIT_EXECUTABLE} tag -l "${TAG_NAME}"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE EXISTING_TAG
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
    if(EXISTING_TAG)
        message(WARNING "Tag ${TAG_NAME} já existe!")
    else()
        # Cria a tag anotada
        execute_process(
            COMMAND ${GIT_EXECUTABLE} tag -a "${TAG_NAME}" -m "Release ${TAG_NAME}"
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )
        message(STATUS "Tag ${TAG_NAME} criada com sucesso!")
    endif()
endfunction()