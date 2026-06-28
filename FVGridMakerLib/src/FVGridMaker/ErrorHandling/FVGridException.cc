// ============================================================================
// Arquivo: FVGridException.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Implementa a exceção principal e a formatação de mensagens de erro.
// Autor: João Flávio Vieira de Vasconcellos
//
// SPDX-FileCopyrightText: 2026 João Flávio Vieira de Vasconcellos
// SPDX-License-Identifier: BSD-3-Clause
//
// Este arquivo faz parte do FVGridMaker.
//
// Licença: BSD 3-Clause.
// É permitido usar, copiar, modificar e redistribuir este arquivo, em código-fonte
// ou forma binária, com ou sem modificações, desde que sejam preservados os avisos
// de copyright, esta identificação de licença e as condições descritas no arquivo
// LICENSE.
//
// O nome do autor, de colaboradores ou de instituições associadas ao projeto não
// pode ser usado para endossar ou promover produtos derivados sem autorização
// prévia por escrito.
//
// Este software é fornecido sem garantias de qualquer natureza. Consulte o arquivo
// LICENSE, na raiz do repositório, para o texto completo da licença.
// ============================================================================
// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <sstream>
#include <utility>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/FVGridException.h>


namespace fvgrid {

namespace {

void append_error_context(
    std::ostringstream& stream,
    const ErrorContextList& context
) {
    if (context.empty()) {
        return;
    }

    stream << " (context: ";

    for (std::size_t index = 0; index < context.size(); ++index) {
        if (index > 0) {
            stream << ", ";
        }

        stream << context[index].key << "=" << context[index].value;
    }

    stream << ")";
}

[[nodiscard]] std::string format_error_message(const ErrorRecord& record) {
    std::ostringstream stream;

    stream << "[" << record.code << "] " << record.message;

    if (!record.category.empty()) {
        stream << " (category: " << record.category << ")";
    }

    if (!record.source.module().empty()) {
        stream << " (module: " << record.source.module() << ")";
    }

    if (!record.source.class_name().empty()) {
        stream << " (class: " << record.source.class_name() << ")";
    }

    if (!record.source.class_id().empty()) {
        stream << " (class id: " << record.source.class_id() << ")";
    }

    append_error_context(stream, record.context);

    stream << " at " << record.location.file_name() << ":"
           << record.location.line() << " in "
           << record.location.function_name();

    return stream.str();
}

}  // namespace

FVGridException::FVGridException(ErrorRecord record)
    : record_(std::move(record)),
      formatted_message_(format_error_message(record_)) {}

const char* FVGridException::what() const noexcept {
    return formatted_message_.c_str();
}

const ErrorRecord& FVGridException::record() const noexcept {
    return record_;
}

}  // namespace fvgrid
