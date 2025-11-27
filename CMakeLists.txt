# ------------------------------------------------------------
# Root CMakeLists.txt for FVGridMaker
# ------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0135 NEW)

# Definição do projeto
project(FVGridMaker VERSION 1.0 LANGUAGES CXX)

# Incluir todos os módulos
include(cmake/ConfigUtils.cmake)          # Utilitários (bloqueio de in-source build)
include(cmake/ConfigOptions.cmake)        # Opções (C++20, BUILD_TESTS, etc)
include(cmake/ConfigCompiler.cmake)       # Configurações do compilador
include(cmake/ConfigTargets.cmake)        # Alvo principal da biblioteca
include(cmake/ConfigExamples.cmake)       # Exemplos (chama add_subdirectory)
include(cmake/ConfigTests.cmake)          # Testes (chama add_subdirectory)
include(cmake/ConfigDocs.cmake)           # Documentação (Sphinx)


# Mensagem final
message(STATUS "FVGridMaker configuration complete!")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")