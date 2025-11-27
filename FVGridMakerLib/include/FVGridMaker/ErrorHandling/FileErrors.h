// ----------------------------------------------------------------------------
// File: FileErrors.h
// Project: FVGridMaker
// Version: 1.6
// Description: Define a enumeração `FileErr` para erros de Entrada/Saída (I/O)
//              e manipulação de arquivos. Especializa o template `ErrorTraits`
//              para integrar ao sistema de tratamento de erros com suporte
//              a mensagens localizadas (i18n).
// Last modified: 2025-11-25
// Author: FVGridMaker Team
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once


// ----------------------------------------------------------------------------
// includes FVGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorTraits.h>

/**
 * @file FileErrors.h
 * @brief Definições de erros de E/S (Arquivo) e seus traits.
 *
 * Abrange falhas comuns de acesso ao sistema de arquivos, permissões
 * e integridade de leitura/escrita.
 *
 * @ingroup ErrorHandling
 */

FVGRIDMAKER_NAMESPACE_OPEN
ERROR_NAMESPACE_OPEN

/**
 * @enum FileErr
 * @brief Enumeração fortemente tipada para erros de Arquivo (I/O).
 * @note O Domain ID associado a estes erros é **0x0002**.
 */
enum class FileErr : std::uint16_t {
    /**
     * @brief O arquivo especificado não pôde ser encontrado.
     * @details Verifique se o caminho está correto e se o arquivo existe.
     */
    FileNotFound = 1,

    /**
     * @brief Permissão negada para acessar o arquivo.
     * @details Ocorre em tentativas de leitura/escrita sem privilégios suficientes.
     */
    AccessDenied = 2,

    /**
     * @brief Erro genérico de leitura (I/O Read Error).
     * @details Pode indicar disco corrompido, fim de arquivo inesperado ou falha de hardware.
     */
    ReadError = 3,

    /**
     * @brief Erro genérico de escrita (I/O Write Error).
     * @details Pode indicar disco cheio ou falha de hardware.
     */
    WriteError = 4,

    /**
     * @brief O caminho fornecido é sintaticamente inválido ou nulo.
     */
    InvalidPath = 5,

    /// @brief Limite inferior para iteração (metadado).
    _Min = FileNotFound,
    /// @brief Limite superior para iteração (metadado).
    _Max = InvalidPath
};

DETAIL_NAMESPACE_OPEN

    using sv = std::string_view;

    /**
     * @struct FileErrorInfo
     * @brief Estrutura auxiliar contendo metadados estáticos de um erro de arquivo.
     * @internal Detalhe de implementação.
     */
    struct FileErrorInfo {
        sv key;             ///< Chave única textual (ex: "FILE_NOT_FOUND").
        Severity severity;  ///< Severidade padrão.
        sv enUS;            ///< Mensagem em Inglês.
        sv ptBR;            ///< Mensagem em Português.
    };

    /**
     * @brief Tabela de lookup para erros de arquivo.
     */
    constexpr FileErrorInfo get_file_error_info_data(FileErr e) {
        switch (e) {
            case FileErr::FileNotFound:
                return {sv{"FILE_NOT_FOUND"}, Severity::Error,
                        sv{"File not found: {path}."},
                        sv{"Arquivo não encontrado: {path}."}};
            case FileErr::AccessDenied:
                return {sv{"FILE_ACCESS_DENIED"}, Severity::Error,
                        sv{"Access denied to file: {path}."},
                        sv{"Acesso negado ao arquivo: {path}."}};
            case FileErr::ReadError:
                return {sv{"FILE_READ_ERROR"}, Severity::Error,
                        sv{"An error occurred while reading the file: {path}."},
                        sv{"Ocorreu um erro ao ler o arquivo: {path}."}};
            case FileErr::WriteError:
                return {sv{"FILE_WRITE_ERROR"}, Severity::Error,
                        sv{"An error occurred while writing to the file: {path}."},
                        sv{"Ocorreu um erro ao escrever no arquivo: {path}."}};
            case FileErr::InvalidPath:
                return {sv{"FILE_INVALID_PATH"}, Severity::Error,
                        sv{"The provided path is invalid: {path}."},
                        sv{"O caminho fornecido é inválido: {path}."}};
            default:
                 return {sv{}, Severity::Trace, sv{}, sv{}};
        }
    }

    /**
     * @brief Wrapper seguro para obter informações do erro.
     */
    constexpr FileErrorInfo get_file_info(FileErr e) noexcept {
         if (static_cast<uint16_t>(e) >= static_cast<uint16_t>(FileErr::_Min) &&
             static_cast<uint16_t>(e) <= static_cast<uint16_t>(FileErr::_Max))
         {
              return get_file_error_info_data(e);
         }
         return {sv{}, Severity::Trace, sv{}, sv{}};
    }

DETAIL_NAMESPACE_CLOSE

/**
 * @brief Especialização de ErrorTraits para o domínio `FileErr`.
 */
template <> struct ErrorTraits<FileErr> {
    /** @brief ID do domínio File (0x0002). */
    static constexpr std::uint16_t domain_id() noexcept { return 0x0002; }

    /** @brief Nome do domínio ("File"). */
    static constexpr std::string_view domain_name() noexcept { return detail::sv{"File"}; }

    static constexpr Severity default_severity(FileErr e) noexcept { return detail::get_file_info(e).severity; }
    static constexpr std::string_view key(FileErr e) noexcept { return detail::get_file_info(e).key; }
    static constexpr std::string_view enUS(FileErr e) noexcept { return detail::get_file_info(e).enUS; }
    static constexpr std::string_view ptBR(FileErr e) noexcept { return detail::get_file_info(e).ptBR; }
};

ERROR_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE