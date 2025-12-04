#!/bin/bash
# scripts/create_release.sh
# Cria uma nova release com versionamento automático

set -e

# Diretório do projeto
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build"

# Verifica se há mudanças não commitadas
if [[ -n $(git status --porcelain) ]]; then
    echo "❌ Há mudanças não commitadas. Commit ou stash primeiro."
    exit 1
fi

# Obtém a última tag
LAST_TAG=$(git describe --tags --abbrev=0 2>/dev/null || echo "v0.0.0")

echo "Última versão: ${LAST_TAG}"
echo "Escolha o tipo de release:"
echo "1) Major (quebra compatibilidade)"
echo "2) Minor (novas funcionalidades)"
echo "3) Patch (correções)"

read -p "Opção (1-3): " OPTION

case $OPTION in
    1) RELEASE_TYPE="MAJOR" ;;
    2) RELEASE_TYPE="MINOR" ;;
    3) RELEASE_TYPE="PATCH" ;;
    *) echo "Opção inválida"; exit 1 ;;
esac

# Build com CMake para gerar nova versão
cd "${PROJECT_ROOT}"
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

# Configura com tipo de release
cmake .. -DRELEASE_TYPE=${RELEASE_TYPE}
make

# Obtém nova versão do header gerado
NEW_VERSION=$(grep -oP 'SEMANTIC_VERSION = "\K[^"]+' generated_include/FVGridMaker/version.hpp)

echo "✅ Nova versão gerada: ${NEW_VERSION}"
echo ""
echo "Próximos passos:"
echo "1. Teste a build: make test"
echo "2. Commit das mudanças: git commit -am 'Release ${NEW_VERSION}'"
echo "3. Crie a tag: git tag -a v${NEW_VERSION} -m 'Release ${NEW_VERSION}'"
echo "4. Push: git push origin main --tags"