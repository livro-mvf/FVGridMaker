// ============================================================================
// Arquivo: ThrowError.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Define funções auxiliares para lançar e validar erros tipados.
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
#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <source_location>
#include <string>
#include <string_view>
#include <utility>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/ErrorHandling/ErrorContext.h>
#include <FVGridMaker/ErrorHandling/ErrorDescriptor.h>
#include <FVGridMaker/ErrorHandling/ErrorTraits.h>
#include <FVGridMaker/ErrorHandling/FVGridException.h>

namespace fvgrid {

// ----------------------------------------------------------------------------
// Runtime overloads.
//
// These overloads keep the error system open to user-defined or externally
// generated descriptors without requiring a built-in error tag.
// ----------------------------------------------------------------------------

[[noreturn]] inline void throw_error(
    std::string_view code,
    std::string message,
    std::string_view category,
    ID source,
    ErrorContextList context,
    std::source_location location = std::source_location::current()
) {
    throw FVGridException{
        ErrorRecord{
            .code = code,
            .message = std::move(message),
            .category = category,
            .context = std::move(context),
            .source = source,
            .location = location,
        }
    };
}

[[noreturn]] inline void throw_error(
    ErrorDescriptor descriptor,
    ID source,
    ErrorContextList context,
    std::source_location location = std::source_location::current()
) {
    throw_error(
        descriptor.code,
        std::string{descriptor.message},
        descriptor.category,
        source,
        std::move(context),
        location
    );
}

[[noreturn]] inline void throw_error(
    std::string_view code,
    std::string message,
    std::string_view category,
    ID source,
    std::source_location location = std::source_location::current()
) {
    throw_error(
        code,
        std::move(message),
        category,
        source,
        ErrorContextList{},
        location
    );
}

[[noreturn]] inline void throw_error(
    ErrorDescriptor descriptor,
    ID source,
    std::source_location location = std::source_location::current()
) {
    throw_error(
        descriptor,
        source,
        ErrorContextList{},
        location
    );
}

inline void require(
    bool condition,
    std::string_view code,
    std::string message,
    std::string_view category,
    ID source,
    ErrorContextList context,
    std::source_location location = std::source_location::current()
) {
    if (!condition) {
        throw_error(
            code,
            std::move(message),
            category,
            source,
            std::move(context),
            location
        );
    }
}

inline void require(
    bool condition,
    ErrorDescriptor descriptor,
    ID source,
    ErrorContextList context,
    std::source_location location = std::source_location::current()
) {
    if (!condition) {
        throw_error(
            descriptor,
            source,
            std::move(context),
            location
        );
    }
}

inline void require(
    bool condition,
    std::string_view code,
    std::string message,
    std::string_view category,
    ID source,
    std::source_location location = std::source_location::current()
) {
    if (!condition) {
        throw_error(
            code,
            std::move(message),
            category,
            source,
            location
        );
    }
}

inline void require(
    bool condition,
    ErrorDescriptor descriptor,
    ID source,
    std::source_location location = std::source_location::current()
) {
    if (!condition) {
        throw_error(
            descriptor,
            source,
            location
        );
    }
}

// ----------------------------------------------------------------------------
// Typed overloads.
//
// These overloads are the preferred FVGridMaker API. They accept any type that
// satisfies ErrorDescriptorProvider, including built-in FVGridMaker error tags
// and user-defined error tags.
// ----------------------------------------------------------------------------

template <ErrorDescriptorProvider Error>
[[noreturn]] void throw_error(
    ID source,
    ErrorContextList context,
    std::source_location location = std::source_location::current()
) {
    throw_error(
        descriptor_of<Error>(),
        source,
        std::move(context),
        location
    );
}


template <ErrorDescriptorProvider Error>
[[noreturn]] void throw_error(
    ID source,
    std::source_location location = std::source_location::current()
) {
    throw_error<Error>(
        source,
        ErrorContextList{},
        location
    );
}

template <ErrorDescriptorProvider Error>
void require(
    bool condition,
    ID source,
    ErrorContextList context,
    std::source_location location = std::source_location::current()
) {
    if (!condition) {
        throw_error<Error>(
            source,
            std::move(context),
            location
        );
    }
}

template <ErrorDescriptorProvider Error>
void require(
    bool condition,
    ID source,
    std::source_location location = std::source_location::current()
) {
    if (!condition) {
        throw_error<Error>(
            source,
            location
        );
    }
}

}  // namespace fvgrid