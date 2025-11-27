// ----------------------------------------------------------------------------
// File: GridErrors.h
// Project: FVGridMaker
// Version: 1.0
// Description: Define a enumeração `GridErr` para erros relacionados à
//              geração e validação de malhas (Grids). Especializa o template
//              `ErrorTraits` para integração com o sistema de erros.
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
 * @file GridErrors.h
 * @brief Definições de erros do domínio Grid (Malhas).
 *
 * Abrange erros de validação de parâmetros de construção de malha (Builder),
 * inconsistências geométricas (malhas degeneradas) e problemas de backend
 * de execução paralela.
 *
 * @ingroup ErrorHandling
 */

FVGRIDMAKER_NAMESPACE_OPEN
ERROR_NAMESPACE_OPEN

/**
 * @enum GridErr
 * @brief Enumeração fortemente tipada para códigos de erro de Grid.
 * @note O Domain ID associado a estes erros é **0x0003**.
 */
enum class GridErr : std::uint16_t {
    /** @brief Número de volumes inválido (N <= 0). */
    InvalidN = 1,

    /** @brief Domínio inválido (B <= A). */
    InvalidDomain = 2,

    /** @brief Centralização (Cell/Face/Vertex) desconhecida ou não suportada. */
    InvalidCentering = 3,

    /** @brief Tipo de distribuição (Uniform, Tanh, etc.) desconhecido. */
    InvalidDistribution = 4,

    /** @brief Opções obrigatórias para a distribuição escolhida estão ausentes. */
    MissingOptions = 5,

    /** @brief Opções da distribuição fora dos limites (ex: pesos negativos). */
    OptionsOutOfRange = 6,

    /** @brief Malha degenerada (tamanhos de célula nulos ou negativos). */
    DegenerateMesh = 7,

    /** @brief Faces da malha não estão estritamente crescentes. */
    NonIncreasingFaces = 8,

    /** @brief Centros da malha não estão estritamente crescentes. */
    NonIncreasingCenters = 9,

    /** @brief Coordenada contém valor NaN (Not a Number). */
    NaNCoordinate = 10,

    /** @brief Coordenada contém valor Infinito (+/- Inf). */
    InfCoordinate = 11,

    /**
     * @brief Política de execução solicitada (ex: ParUnseq) não suportada.
     * @details Geralmente reverte para execução serial (Warning).
     */
    ExecPolicyUnsupported = 12,

    /**
     * @brief Backend paralelo (ex: TBB, OpenMP) ausente na compilação.
     * @details Reverte para execução serial (Warning).
     */
    ParallelBackendMissing = 13,

    /** @brief O Builder foi invocado em um estado inválido ou incompleto. */
    BuilderStateInvalid = 14,

    /// @brief Limite inferior para iteração (metadado).
    _Min = InvalidN,
    /// @brief Limite superior para iteração (metadado).
    _Max = BuilderStateInvalid
};

DETAIL_NAMESPACE_OPEN

    using sv = std::string_view;

    /**
     * @struct GridErrorInfo
     * @brief Estrutura auxiliar contendo metadados estáticos de um erro de grid.
     * @internal Detalhe de implementação.
     */
    struct GridErrorInfo {
        sv key;             ///< Chave única textual.
        Severity severity;  ///< Severidade padrão.
        sv enUS;            ///< Mensagem em Inglês.
        sv ptBR;            ///< Mensagem em Português.
    };

    /**
     * @brief Tabela de lookup para erros de grid.
     */
    constexpr GridErrorInfo get_grid_error_info_data(GridErr e) {
        switch (e) {
            case GridErr::InvalidN:
                return {sv{"GRID_INVALID_N"}, Severity::Error,
                        sv{"Invalid number of volumes N: {N} (must be > 0)."},
                        sv{"Número de volumes N inválido: {N} (deve ser > 0)."}};

            case GridErr::InvalidDomain:
                return {sv{"GRID_INVALID_DOMAIN"}, Severity::Error,
                        sv{"Invalid domain: B <= A (A={A}, B={B})."},
                        sv{"Domínio inválido: B <= A (A={A}, B={B})."}};

            case GridErr::InvalidCentering:
                return {sv{"GRID_INVALID_CENTERING"}, Severity::Error,
                        sv{"Unsupported or unknown centering: {center}."},
                        sv{"Centering desconhecido ou não suportado: {center}."}};

            case GridErr::InvalidDistribution:
                return {sv{"GRID_INVALID_DISTRIBUTION"}, Severity::Error,
                        sv{"Unsupported or unknown distribution: {dist}."},
                        sv{"Distribuição desconhecida ou não suportada: {dist}."}};

            case GridErr::MissingOptions:
                return {sv{"GRID_MISSING_OPTIONS"}, Severity::Error,
                        sv{"Required distribution options are missing for {dist}."},
                        sv{"Opções obrigatórias da distribuição ausentes para {dist}."}};

            case GridErr::OptionsOutOfRange:
                return {sv{"GRID_OPTIONS_OUT_OF_RANGE"}, Severity::Error,
                        sv{"Distribution options out of valid range (e.g., w_lo={w_lo}, w_hi={w_hi})."},
                        sv{"Opções da distribuição fora da faixa válida (ex.: w_lo={w_lo}, w_hi={w_hi})."}};

            case GridErr::DegenerateMesh:
                return {sv{"GRID_DEGENERATE_MESH"}, Severity::Error,
                        sv{"Degenerate mesh: at least one cell size is non-positive."},
                        sv{"Malha degenerada: ao menos um tamanho de célula é não-positivo."}};

            case GridErr::NonIncreasingFaces:
                return {sv{"GRID_NON_INCREASING_FACES"}, Severity::Error,
                        sv{"Faces must be strictly increasing; violation at index {i}."},
                        sv{"Faces devem ser estritamente crescentes; violação no índice {i}."}};

            case GridErr::NonIncreasingCenters:
                return {sv{"GRID_NON_INCREASING_CENTERS"}, Severity::Error,
                        sv{"Centers must be strictly increasing; violation at index {i}."},
                        sv{"Centros devem ser estritamente crescentes; violação no índice {i}."}};

            case GridErr::NaNCoordinate:
                return {sv{"GRID_NAN_COORDINATE"}, Severity::Error,
                        sv{"Coordinate has NaN at index {i}."},
                        sv{"Coordenada com NaN no índice {i}."}};

            case GridErr::InfCoordinate:
                return {sv{"GRID_INF_COORDINATE"}, Severity::Error,
                        sv{"Coordinate has +/-inf at index {i}."},
                        sv{"Coordenada com +/-inf no índice {i}."}};

            case GridErr::ExecPolicyUnsupported:
                return {sv{"GRID_EXEC_POLICY_UNSUPPORTED"}, Severity::Warning,
                        sv{"Requested execution policy is unsupported; falling back to serial."},
                        sv{"Política de execução solicitada não suportada; retornando ao modo serial."}};

            case GridErr::ParallelBackendMissing:
                return {sv{"GRID_PAR_BACKEND_MISSING"}, Severity::Warning,
                        sv{"Parallel execution requested but backend is missing (e.g., TBB)."},
                        sv{"Execução paralela solicitada, mas o backend está ausente (ex.: TBB)."}};

            case GridErr::BuilderStateInvalid:
                return {sv{"GRID_BUILDER_STATE_INVALID"}, Severity::Error,
                        sv{"Grid1DBuilder used in an invalid or incomplete state."},
                        sv{"Grid1DBuilder usado em estado inválido ou incompleto."}};
            default:
                return {sv{}, Severity::Trace, sv{}, sv{}};
        }
    }

    /**
     * @brief Wrapper seguro para obter informações do erro.
     */
    constexpr GridErrorInfo get_grid_info(GridErr e) noexcept {
        if (static_cast<std::uint16_t>(e) >= static_cast<std::uint16_t>(GridErr::_Min) &&
            static_cast<std::uint16_t>(e) <= static_cast<std::uint16_t>(GridErr::_Max))
        {
            return get_grid_error_info_data(e);
        }
        return {sv{}, Severity::Trace, sv{}, sv{}};
    }

DETAIL_NAMESPACE_CLOSE

/**
 * @brief Especialização de ErrorTraits para o domínio `GridErr`.
 */
template <> struct ErrorTraits<GridErr> {
    static constexpr std::uint16_t domain_id() noexcept { return 0x0003; }
    static constexpr std::string_view domain_name() noexcept { return detail::sv{"Grid"}; }

    static constexpr Severity default_severity(GridErr e) noexcept { return detail::get_grid_info(e).severity; }
    static constexpr std::string_view key(GridErr e) noexcept { return detail::get_grid_info(e).key; }
    static constexpr std::string_view enUS(GridErr e) noexcept { return detail::get_grid_info(e).enUS; }
    static constexpr std::string_view ptBR(GridErr e) noexcept { return detail::get_grid_info(e).ptBR; }
};

ERROR_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE