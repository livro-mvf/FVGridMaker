# ------------------------------------------------------------------------------
# ConfigDocs.cmake
# Configuração da geração de documentação (Doxygen + Sphinx)
# ------------------------------------------------------------------------------

include_guard()

# Se a opção de build de docs estiver desligada, não faz nada
if(NOT BUILD_DOCS)
    message(STATUS "[docs] BUILD_DOCS=OFF - documentação desabilitada.")
    return()
endif()

# ------------------------------------------------------------------------------
# Diretórios base
# ------------------------------------------------------------------------------

# Pasta com os arquivos de configuração de documentação (conf.py, .rst, Doxyfile.in)
set(DOCS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/docs")

# Diretório onde o Doxygen vai escrever tudo (HTML desabilitado; só XML nos interessa)
set(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/docs_doxygen")  # CORRIGIDO: mantém docs_doxygen
set(DOXYGEN_XML_DIR    "${DOXYGEN_OUTPUT_DIR}/xml")

# Diretórios do Sphinx
set(DOCS_SPHINX_DOCTREES "${CMAKE_BINARY_DIR}/docs_sphinx_doctrees")
set(DOCS_HTML_DIR        "${CMAKE_BINARY_DIR}/docs_html")

# Diretório do ambiente virtual de documentação
set(DOCS_VENV_DIR "${CMAKE_SOURCE_DIR}/.venv-docs")
set(DOCS_VENV_PY  "${DOCS_VENV_DIR}/bin/python3")

message(STATUS "-- [docs] Docs source dir   : ${DOCS_SOURCE_DIR}")
message(STATUS "-- [docs] Doxygen XML dir  : ${DOXYGEN_XML_DIR}")
message(STATUS "-- [docs] Docs venv dir    : ${DOCS_VENV_DIR}")
message(STATUS "-- [docs] Sphinx cache dir : ${DOCS_SPHINX_DOCTREES}")
message(STATUS "-- [docs] Publish dir      : ${DOCS_HTML_DIR}")

# ------------------------------------------------------------------------------
# Doxygen: gera XML a partir dos headers da biblioteca
# ------------------------------------------------------------------------------

find_package(Doxygen QUIET)

if(DOXYGEN_FOUND)
    set(DOXYGEN_INPUT_DXY "${DOCS_SOURCE_DIR}/Doxyfile.in")
    set(DOXYGEN_OUTPUT_DXY "${CMAKE_BINARY_DIR}/Doxyfile")

    if(NOT EXISTS "${DOXYGEN_INPUT_DXY}")
        message(WARNING
            "[docs] Doxyfile.in não encontrado em ${DOXYGEN_INPUT_DXY}; "
            "alvo doxygen_xml não será criado."
        )
    else()
        # Variáveis usadas dentro do Doxyfile.in (@DOXYGEN_OUTPUT_DIR@ etc.)
        set(DOXYGEN_OUTPUT_DIR "${DOXYGEN_OUTPUT_DIR}")

        configure_file(
            "${DOXYGEN_INPUT_DXY}"
            "${DOXYGEN_OUTPUT_DXY}"
            @ONLY
        )

        message(STATUS "[docs] Doxygen input  : ${DOXYGEN_INPUT_DXY}")
        message(STATUS "[docs] Doxygen file   : ${DOXYGEN_OUTPUT_DXY}")

        add_custom_target(doxygen_xml
            COMMAND "${DOXYGEN_EXECUTABLE}" "${DOXYGEN_OUTPUT_DXY}"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
            COMMENT "[docs] Gerando XML do Doxygen"
            VERBATIM
        )
    endif()
else()
    message(WARNING
        "[docs] Doxygen não encontrado; XML para o Breathe não será gerado."
    )
endif()

# ------------------------------------------------------------------------------
# Ambiente virtual Python para documentação
# ------------------------------------------------------------------------------

find_package(Python3 COMPONENTS Interpreter REQUIRED)

# Script Python para verificar e criar o ambiente virtual de forma robusta
set(VENV_SETUP_SCRIPT "${CMAKE_BINARY_DIR}/setup_venv.py")
file(WRITE "${VENV_SETUP_SCRIPT}"
"import os
import subprocess
import sys
import shutil

venv_dir = '${DOCS_VENV_DIR}'
requirements = '${DOCS_SOURCE_DIR}/requirements.txt'

# Remove ambiente virtual corrompido se existir
if os.path.exists(venv_dir):
    try:
        # Tenta usar o Python do venv para verificar se está válido
        venv_python = os.path.join(venv_dir, 'bin', 'python3')
        if os.path.exists(venv_python):
            result = subprocess.run([venv_python, '--version'], 
                                  capture_output=True, text=True, timeout=10)
            if result.returncode != 0:
                raise Exception('Venv Python inválido')
        else:
            raise Exception('Venv Python não encontrado')
    except Exception as e:
        print(f'Removendo ambiente virtual corrompido: {e}')
        shutil.rmtree(venv_dir, ignore_errors=True)

# Cria novo ambiente virtual se não existir
if not os.path.exists(venv_dir):
    print('Criando novo ambiente virtual...')
    subprocess.run([sys.executable, '-m', 'venv', venv_dir], check=True)
    
    # Instala dependências
    venv_python = os.path.join(venv_dir, 'bin', 'python3')
    print('Instalando dependências...')
    subprocess.run([venv_python, '-m', 'pip', 'install', '-r', requirements], check=True)
    print('Ambiente virtual configurado com sucesso!')
else:
    print('Ambiente virtual já existe e é válido.')
")

add_custom_target(docs_venv_setup
    COMMAND "${Python3_EXECUTABLE}" "${VENV_SETUP_SCRIPT}"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    COMMENT "[docs] Verificando/criando ambiente virtual de documentação"
    VERBATIM
)

# ------------------------------------------------------------------------------
# Sphinx: gera HTML a partir dos .rst + Breathe (XML do Doxygen)
# ------------------------------------------------------------------------------

# OBS IMPORTANTE:
#   - conf.py lê a variável de ambiente FVGRIDMAKER_DOXYGEN_XML
#     para definir o caminho do XML do Doxygen:
#       doxygen_xml = os.environ.get("FVGRIDMAKER_DOXYGEN_XML", ...)
#   - Aqui garantimos que essa variável aponte para ${DOXYGEN_XML_DIR}.

add_custom_target(docs_sphinx
    COMMAND
        "${CMAKE_COMMAND}" -E env
            "FVGRIDMAKER_DOXYGEN_XML=${DOXYGEN_XML_DIR}"
            "VIRTUAL_ENV=${DOCS_VENV_DIR}"
            "PATH=${DOCS_VENV_DIR}/bin:$ENV{PATH}"
        "${DOCS_VENV_PY}" -m sphinx
            -b html
            -c "${DOCS_SOURCE_DIR}"
            -d "${DOCS_SPHINX_DOCTREES}"
            "${DOCS_SOURCE_DIR}"
            "${DOCS_HTML_DIR}"
    WORKING_DIRECTORY "${DOCS_SOURCE_DIR}"
    DEPENDS docs_venv_setup doxygen_xml
    COMMENT "[docs] Gerando documentação HTML com Sphinx"
    VERBATIM
)

# ------------------------------------------------------------------------------
# Alvo agregado 'docs' (não faz parte de 'all'; chame 'make docs')
# ------------------------------------------------------------------------------

add_custom_target(docs
    DEPENDS docs_sphinx
    COMMENT "[docs] Geração completa da documentação (Doxygen + Sphinx)"
)