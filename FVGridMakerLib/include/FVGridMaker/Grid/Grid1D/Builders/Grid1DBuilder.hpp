// ----------------------------------------------------------------------------
// File: Grid1DBuilder.hpp
// Project: FVGridMaker
// Version: 3.0
// Description: Builder para construção de malhas 1D (Grid1D).
//              - Encapsula parâmetros de configuração (N, domínio, ghosts,
//                centering, distribuição).
//              - Despacha para backend em detail::build_grid1d().
//              - Suporta seleção de distribuição por tipo (functor) ou enum tag.
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// includes FVMGridMaker (ordem alfabética por caminho)
// ----------------------------------------------------------------------------
#include <FVGridMaker/Grid/Common/Tags1D.hpp>
#include <FVGridMaker/Grid/Grid1D/Grid1D.h>
#include <FVGridMaker/Grid/Grid1D/Patterns/Distribution/ConceptsDistribution.hpp>
#include <FVGridMaker/Grid/Grid1D/Patterns/Distribution/Random1D.hpp>
#include <FVGridMaker/Grid/Grid1D/Patterns/Distribution/Uniform1D.hpp>

// ----------------------------------------------------------------------------
// includes C++
// ----------------------------------------------------------------------------
#include <cstdint>
#include <typeindex>

#ifdef FVMGRIDMAKER_NAMESPACE_OPEN
FVMGRIDMAKER_NAMESPACE_OPEN
#else
FVGRIDMAKER_NAMESPACE_OPEN
#endif
GRID_NAMESPACE_OPEN
GRID1D_NAMESPACE_OPEN
BUILDERS_NAMESPACE_OPEN

// Aliases de namespaces utilizados neste header
namespace core   = ::FVGridMaker::core;
namespace dist1d = ::FVGridMaker::grid::grid1d::patterns::distribution;

// Importa tipos relevantes
using ::FVGridMaker::grid::CenteringTag;
using ::FVGridMaker::grid::DistributionTag;
using ::FVGridMaker::grid::grid1d::Grid1D;

enum class RandomSeedMode : std::uint8_t {
    Fixed,
    Varied
};

DETAIL_NAMESPACE_OPEN

/**
 * @brief Estrutura agregando todos os parâmetros usados pelo backend
 *        de construção de malhas 1D.
 *
 * Esta struct é preenchida por Grid1DBuilder e consumida por
 * detail::build_grid1d().
 */
struct Grid1DBuilderConfig {
    core::Index    n{};         ///< Número de células físicas.
    core::Real     a{};         ///< Limite esquerdo do domínio físico.
    core::Real     b{};         ///< Limite direito do domínio físico.
    core::Index    nGhost{};    ///< Número de células ghost em cada lado.
    CenteringTag   centering{CenteringTag::FaceCentered};
    core::Real     randomPercentual{core::Real(0.20)};
    std::uint64_t  randomSeed{12345u};
    RandomSeedMode randomSeedMode{RandomSeedMode::Fixed};
};

/**
 * @brief Backend que constrói um Grid1D a partir de uma configuração
 *        agregada e dos tipos de distribuição e centering.
 *
 * A seleção da distribuição é feita em runtime via std::type_index,
 * mas os geradores concretos (Uniform1D, Random1D, etc.) continuam
 * sendo functors estáticos, verificados em compile-time pelo concept
 * dist1d::Distribution1D.
 */
Grid1D build_grid1d(const Grid1DBuilderConfig& cfg,
                    const std::type_index& distribution_type,
                    const std::type_index& centering_type);

DETAIL_NAMESPACE_CLOSE

/**
 * @class Grid1DBuilder
 * @brief Builder fluente para construção de malhas 1D (Grid1D).
 *
 * Exemplo de uso:
 * @code
 * using dist1d::Uniform1D;
 *
 * Grid1D grid = Grid1DBuilder{}
 *     .setN(100)
 *     .setDomain(0.0, 1.0)
 *     .setNGhost(1)
 *     .setDistribution<Uniform1D>()
 *     .build();
 * @endcode
 */
class Grid1DBuilder {
public:
    using Index = core::Index;
    using Real  = core::Real;

    /// Constrói o builder em estado consistente, com distribuição Uniform1D
    /// e malha face-centrada por padrão.
    Grid1DBuilder() noexcept
        : n_{0}
        , a_{0.0}
        , b_{1.0}
        , nGhost_{0}
        , centering_{CenteringTag::FaceCentered}
        , randomPercentual_{Real(0.20)}
        , randomSeed_{12345u}
        , randomSeedMode_{RandomSeedMode::Fixed}
        , distribution_type_{typeid(dist1d::Uniform1D)}
        , centering_type_{typeid(void)} {}

    /// Define o número de células físicas.
    Grid1DBuilder& setN(Index n) noexcept {
        n_ = n;
        return *this;
    }

    /// Define o domínio físico [a, b].
    Grid1DBuilder& setDomain(Real a, Real b) noexcept {
        a_ = a;
        b_ = b;
        return *this;
    }

    /// Define o número de células ghost em cada lado (alias legado).
    Grid1DBuilder& setNGhost(Index nGhost) noexcept {
        nGhost_ = nGhost;
        return *this;
    }

    /// Define o número de volumes fictícios em cada lado.
    Grid1DBuilder& setNFicticios(Index nFicticios) noexcept {
        nGhost_ = nFicticios;
        return *this;
    }

    /// Define o número de volumes fictícios em cada lado.
    Grid1DBuilder& setNVolumesFicticios(Index nFicticios) noexcept {
        nGhost_ = nFicticios;
        return *this;
    }

    /// Define a política de centralização (face-centred, cell-centred, etc.).
    Grid1DBuilder& setCentering(CenteringTag centering) noexcept {
        centering_ = centering;
        return *this;
    }

    /// Define o percentual de perturbação usado por Random1D.
    /// Exemplo: 0.20 significa 20% da distância uniforme de referência.
    Grid1DBuilder& setRandomPercentual(Real percentual) noexcept {
        randomPercentual_ = percentual;
        return *this;
    }

    /// Define seed fixo para Random1D, tornando a malha reprodutível.
    Grid1DBuilder& setRandomSeed(std::uint64_t seed) noexcept {
        randomSeed_ = seed;
        randomSeedMode_ = RandomSeedMode::Fixed;
        return *this;
    }

    /// Usa seed variado para Random1D a cada construção de malha.
    Grid1DBuilder& setRandomSeedVariado() noexcept {
        randomSeedMode_ = RandomSeedMode::Varied;
        return *this;
    }

    /**
     * @brief Define a distribuição via functor que satisfaz o concept
     *        dist1d::Distribution1D.
     *
     * Exemplo:
     * @code
     * builder.setDistribution<dist1d::Random1D>();
     * @endcode
     */
    template <dist1d::Distribution1D Dist>
    Grid1DBuilder& setDistribution() noexcept {
        distribution_type_ = std::type_index(typeid(Dist));
        return *this;
    }

    /**
     * @brief Define a distribuição via enum DistributionTag.
     *
     * Fornece compatibilidade com código anterior baseado em Tags1D.hpp.
     * Valores não suportados são ignorados silenciosamente para manter
     * a função noexcept; o usuário pode verificar posteriormente se o
     * resultado faz sentido.
     */
    Grid1DBuilder& setDistribution(DistributionTag tag) noexcept {
        switch (tag) {
        case DistributionTag::Uniform1D:
            distribution_type_ =
                std::type_index(typeid(dist1d::Uniform1D));
            break;
        case DistributionTag::Random1D:
            distribution_type_ =
                std::type_index(typeid(dist1d::Random1D));
            break;
        default:
            // Mantém a configuração anterior para não violar noexcept.
            break;
        }
        return *this;
    }

    /**
     * @brief Constrói o Grid1D a partir da configuração acumulada.
     *
     * A validação de consistência (n > 0, b > a, etc.) é realizada
     * no backend detail::build_grid1d(), que lança exceções em caso
     * de erro de configuração.
     */
    [[nodiscard]] Grid1D build() const {
        detail::Grid1DBuilderConfig cfg{};
        cfg.n         = n_;
        cfg.a         = a_;
        cfg.b         = b_;
        cfg.nGhost    = nGhost_;
        cfg.centering = centering_;
        cfg.randomPercentual = randomPercentual_;
        cfg.randomSeed = randomSeed_;
        cfg.randomSeedMode = randomSeedMode_;
        return detail::build_grid1d(cfg, distribution_type_, centering_type_);
    }

private:
    Index          n_;                ///< Número de células físicas.
    Real           a_;                ///< Limite esquerdo do domínio.
    Real           b_;                ///< Limite direito do domínio.
    Index          nGhost_;           ///< Número de células ghost por lado.
    CenteringTag   centering_;        ///< Política de centralização.
    Real           randomPercentual_; ///< Perturbação relativa para Random1D.
    std::uint64_t  randomSeed_;       ///< Seed fixo para Random1D.
    RandomSeedMode randomSeedMode_;   ///< Modo de seed para Random1D.
    std::type_index distribution_type_; ///< Tipo da distribuição 1D.
    std::type_index centering_type_;    ///< Tipo de centering (reservado).
};

BUILDERS_NAMESPACE_CLOSE
GRID1D_NAMESPACE_CLOSE
GRID_NAMESPACE_CLOSE
#ifdef FVMGRIDMAKER_NAMESPACE_CLOSE
FVMGRIDMAKER_NAMESPACE_CLOSE
#else
FVGRIDMAKER_NAMESPACE_CLOSE
#endif
