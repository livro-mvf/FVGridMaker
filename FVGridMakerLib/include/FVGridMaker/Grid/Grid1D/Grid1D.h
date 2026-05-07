// ----------------------------------------------------------------------------
// File: Grid1D.h
// Project: FVGridMaker
// Version: 3.0
// Description: Classe de dados para malha 1D (SoA).
//              Armazena geometria pré-calculada: faces, centros e métricas.
//              Este arquivo é o header canônico de Grid1D (sem API/ duplicado).
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// includes FVMGridMaker (ordem alfabética por caminho)
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Common/constants.hpp>
#include <FVGridMaker/Core/Common/ID.hpp>
#include <FVGridMaker/Core/Common/namespace.hpp>
#include <FVGridMaker/Core/Common/types.hpp>
#include <FVGridMaker/ErrorHandling/CoreErrors.h>
#include <FVGridMaker/ErrorHandling/Throw.h>
#include <FVGridMaker/Grid/Common/Tags1D.hpp>

// ----------------------------------------------------------------------------
// includes C++ (ordem alfabética)
// ----------------------------------------------------------------------------
#include <cstddef>
#include <iomanip>
#include <ostream>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

/**
 * @file Grid1D.h
 * @brief Container de dados (SoA) para geometria de malhas 1D.
 * @ingroup Grid1D
 *
 * Este header define a única “fonte da verdade” da classe Grid1D.
 * Caso você precise compatibilidade com código antigo, existe um alias
 * `FVGridMaker::grid::grid1d::api::Grid1D` ao final do arquivo.
 */

FVG_GRID1D_OPEN

using ::FVGridMaker::core::Index;
using ::FVGridMaker::core::Real;
using ::FVGridMaker::core::common::ID;

/**
 * @class Grid1D
 * @brief Representação imutável da geometria de uma malha 1D.
 *
 * Responsabilidade: armazenar dados geométricos já calculados (SoA),
 * sem lógica de geração. A geração pertence ao Grid1DBuilder.
 *
 * Estrutura interna (SoA):
 *  - faces_   (N_faces = N_cells + 1): coordenadas das faces.
 *  - centers_ (N_cells): coordenadas dos centros.
 *  - dF_      (N_cells): deltas face-a-face (faces[i+1] - faces[i]).
 *  - dC_      (N_faces): deltas relacionados a centros (convenção do builder):
 *        dC_[0]     = centers[0] - faces[0]
 *        dC_[i]     = centers[i] - centers[i-1]    (1..N_cells-1)
 *        dC_[N]     = faces[N] - centers[N-1]
 *
 * Metadados:
 *  - n_physical_: número de células físicas (interior).
 *  - n_ghost_   : número de células ghost por lado.
 */
class Grid1D final : public ID {

public:
    // ------------------------------------------------------------------------
    // Construtores
    // ------------------------------------------------------------------------

    /// @brief Construtor padrão: grid vazio.
    Grid1D()
        : ID("EmptyGrid1D")
        , faces_{}
        , centers_{}
        , dF_{}
        , dC_{}
        , n_physical_{0}
        , n_ghost_{0}
        , centering_{::FVGridMaker::grid::CenteringTag::FaceCentered} {}

    /**
     * @brief Construtor principal (usado pelo builder).
     *
     * Os vetores são recebidos por valor e movidos internamente.
     * Se n_physical == 0, ele assume centers.size().
     */
    explicit Grid1D(std::vector<Real> faces,
                    std::vector<Real> centers,
                    std::vector<Real> dF,
                    std::vector<Real> dC,
                    Index n_physical = 0,
                    Index n_ghost = 0,
                    ::FVGridMaker::grid::CenteringTag centering =
                        ::FVGridMaker::grid::CenteringTag::FaceCentered)
        : ID("Grid1D")
        , faces_(std::move(faces))
        , centers_(std::move(centers))
        , dF_(std::move(dF))
        , dC_(std::move(dC))
        , n_physical_{n_physical}
        , n_ghost_{n_ghost}
        , centering_{centering} {

        if (n_physical_ == 0) {
            n_physical_ = static_cast<Index>(centers_.size());
        }
    }

    // ------------------------------------------------------------------------
    // Semântica de cópia/movimento
    // ------------------------------------------------------------------------

    /**
     * @brief Cópia: cria um novo objeto com novo ID, copiando os dados SoA.
     *
     * Motivo: ID é não-copiável por design (identidade única).
     * Aqui, a cópia é útil como “cópia de valor” do grid, com identidade nova.
     */
    Grid1D(const Grid1D& other)
        : ID(other.name())
        , faces_(other.faces_)
        , centers_(other.centers_)
        , dF_(other.dF_)
        , dC_(other.dC_)
        , n_physical_(other.n_physical_)
        , n_ghost_(other.n_ghost_)
        , centering_(other.centering_) {}

    /// @brief Atribuição por cópia é desabilitada (evita ambiguidades de identidade).
    Grid1D& operator=(const Grid1D&) = delete;

    Grid1D(Grid1D&&) noexcept = default;
    Grid1D& operator=(Grid1D&&) noexcept = default;

    ~Grid1D() override = default;

    // ------------------------------------------------------------------------
    // Views (read-only) - SoA
    // ------------------------------------------------------------------------

    [[nodiscard]] std::span<const Real> faces() const noexcept {
        return std::span<const Real>{faces_.data(), faces_.size()};
    }

    [[nodiscard]] std::span<const Real> centers() const noexcept {
        return std::span<const Real>{centers_.data(), centers_.size()};
    }

    [[nodiscard]] std::span<const Real> deltasFaces() const noexcept {
        return std::span<const Real>{dF_.data(), dF_.size()};
    }

    [[nodiscard]] std::span<const Real> deltasCenters() const noexcept {
        return std::span<const Real>{dC_.data(), dC_.size()};
    }

    [[nodiscard]] std::span<const Real> centros() const noexcept {
        return centers();
    }

    [[nodiscard]] std::span<const Real> deltasCentros() const noexcept {
        return deltasCenters();
    }

    // ------------------------------------------------------------------------
    // Acesso escalar por índice total (inclui volumes fictícios)
    // ------------------------------------------------------------------------

    [[nodiscard]] Real face(Index i) const {
        validarIndiceSpan(i, nFaces(), "face");
        return faces_[static_cast<std::size_t>(i)];
    }

    [[nodiscard]] Real centro(Index i) const {
        validarIndiceSpan(i, nVolumes(), "centro");
        return centers_[static_cast<std::size_t>(i)];
    }

    [[nodiscard]] Real deltaFace(Index i) const {
        validarIndiceSpan(i, nVolumes(), "deltaFace");
        return dF_[static_cast<std::size_t>(i)];
    }

    [[nodiscard]] Real deltaCentro(Index i) const {
        validarIndiceSpan(i, nFaces(), "deltaCentro");
        return dC_[static_cast<std::size_t>(i)];
    }

    // ------------------------------------------------------------------------
    // Acesso escalar por índice físico (não inclui volumes fictícios)
    // ------------------------------------------------------------------------

    [[nodiscard]] Real centroFisico(Index k) const {
        validarIndiceFisico(k, "centroFisico");
        return centro(primeiroVolumeFisico() + k);
    }

    [[nodiscard]] Real faceFisica(Index k) const {
        validarIndiceSpan(k, n_physical_ + 1, "faceFisica");
        return face(primeiroVolumeFisico() + k);
    }

    [[nodiscard]] Real deltaFaceFisica(Index k) const {
        validarIndiceFisico(k, "deltaFaceFisica");
        return deltaFace(primeiroVolumeFisico() + k);
    }

    [[nodiscard]] Real deltaCentroFisico(Index k) const {
        validarIndiceSpan(k, n_physical_ + 1, "deltaCentroFisico");

        if (k == 0) {
            return centroFisico(0) - faceFisica(0);
        }

        if (k == n_physical_) {
            return faceFisica(n_physical_) - centroFisico(n_physical_ - 1);
        }

        return centroFisico(k) - centroFisico(k - 1);
    }

    // ------------------------------------------------------------------------
    // Metadados e informações agregadas
    // ------------------------------------------------------------------------

    [[nodiscard]] bool empty() const noexcept {
        return centers_.empty();
    }

    /// @brief Número de células armazenadas (centros).
    [[nodiscard]] Index nVolumes() const noexcept {
        return static_cast<Index>(centers_.size());
    }

    /// @brief Número de faces armazenadas.
    [[nodiscard]] Index nFaces() const noexcept {
        return static_cast<Index>(faces_.size());
    }

    /// @brief Número de células físicas (interior).
    [[nodiscard]] Index nPhysicalCells() const noexcept {
        return n_physical_;
    }

    /// @brief Número de volumes físicos (nomenclatura usada no livro).
    [[nodiscard]] Index nVolumesFisicos() const noexcept {
        return n_physical_;
    }

    /// @brief Número de células ghost por lado.
    [[nodiscard]] Index nGhost() const noexcept {
        return n_ghost_;
    }

    /// @brief Número de volumes fictícios por lado.
    [[nodiscard]] Index nFicticios() const noexcept {
        return n_ghost_;
    }

    /// @brief Número de volumes fictícios por lado.
    [[nodiscard]] Index nVolumesFicticios() const noexcept {
        return n_ghost_;
    }

    /// @brief Informa se a malha possui volumes fictícios.
    [[nodiscard]] bool possuiVolumesFicticios() const noexcept {
        return n_ghost_ > 0;
    }

    /// @brief Total de células (físicas + ghosts).
    [[nodiscard]] Index nTotalCells() const noexcept {
        return n_physical_ + 2 * n_ghost_;
    }

    /// @brief Total de volumes armazenados (físicos + fictícios).
    [[nodiscard]] Index nVolumesTotais() const noexcept {
        return nTotalCells();
    }

    /// @brief Primeiro índice correspondente a um volume físico.
    [[nodiscard]] Index primeiroVolumeFisico() const noexcept {
        return n_ghost_;
    }

    /// @brief Índice após o último volume físico.
    [[nodiscard]] Index fimVolumesFisicos() const noexcept {
        return n_ghost_ + n_physical_;
    }

    /// @brief Último índice correspondente a um volume físico.
    [[nodiscard]] Index ultimoVolumeFisico() const noexcept {
        return n_physical_ > 0 ? fimVolumesFisicos() - 1 : Index{-1};
    }

    /// @brief Primeiro índice dos volumes fictícios à esquerda.
    [[nodiscard]] Index inicioVolumesFicticiosEsquerda() const noexcept {
        return Index{0};
    }

    /// @brief Índice após o último volume fictício à esquerda.
    [[nodiscard]] Index fimVolumesFicticiosEsquerda() const noexcept {
        return n_ghost_;
    }

    /// @brief Primeiro índice dos volumes fictícios à direita.
    [[nodiscard]] Index inicioVolumesFicticiosDireita() const noexcept {
        return n_ghost_ + n_physical_;
    }

    /// @brief Índice após o último volume fictício à direita.
    [[nodiscard]] Index fimVolumesFicticiosDireita() const noexcept {
        return nTotalCells();
    }

    /// @brief Informa se o índice está dentro dos volumes armazenados.
    [[nodiscard]] bool indiceValido(Index i) const noexcept {
        return i >= 0 && i < nTotalCells();
    }

    /// @brief Informa se o índice corresponde a um volume físico.
    [[nodiscard]] bool ehVolumeFisico(Index i) const noexcept {
        return i >= primeiroVolumeFisico() && i < fimVolumesFisicos();
    }

    /// @brief Informa se o índice corresponde a um volume fictício.
    [[nodiscard]] bool ehVolumeFicticio(Index i) const noexcept {
        return ehVolumeFicticioEsquerda(i) || ehVolumeFicticioDireita(i);
    }

    /// @brief Informa se o índice corresponde a um volume fictício à esquerda.
    [[nodiscard]] bool ehVolumeFicticioEsquerda(Index i) const noexcept {
        return i >= inicioVolumesFicticiosEsquerda() &&
               i < fimVolumesFicticiosEsquerda();
    }

    /// @brief Informa se o índice corresponde a um volume fictício à direita.
    [[nodiscard]] bool ehVolumeFicticioDireita(Index i) const noexcept {
        return i >= inicioVolumesFicticiosDireita() &&
               i < fimVolumesFicticiosDireita();
    }

    /// @brief Padrão de armazenamento/centragem da malha.
    [[nodiscard]] ::FVGridMaker::grid::CenteringTag padraoArmazenamento()
        const noexcept {
        return centering_;
    }

    /// @brief Nome textual do padrão de armazenamento/centragem.
    [[nodiscard]] std::string_view nomePadraoArmazenamento() const noexcept {
        return ::FVGridMaker::grid::to_string(centering_);
    }

    /// @brief Informa se a malha é face-centrada.
    [[nodiscard]] bool ehFaceCentrada() const noexcept {
        return centering_ == ::FVGridMaker::grid::CenteringTag::FaceCentered;
    }

    /// @brief Informa se a malha é volume-centrada.
    [[nodiscard]] bool ehVolumeCentrada() const noexcept {
        return centering_ == ::FVGridMaker::grid::CenteringTag::CellCentered;
    }

    [[nodiscard]] Real minCoord() const noexcept {
        return faces_.empty() ? Real(0) : faces_.front();
    }

    [[nodiscard]] Real maxCoord() const noexcept {
        return faces_.empty() ? Real(0) : faces_.back();
    }

    [[nodiscard]] Real length() const noexcept {
        return maxCoord() - minCoord();
    }

private:
    static void validarIndiceSpan(Index i, Index n, const char* nome) {
        if (i < 0 || i >= n) {
            ::FVGridMaker::error::lancarErro(
                ::FVGridMaker::error::CoreErr::OutOfRange,
                {{"index", std::to_string(i)},
                 {"name", std::string(nome)}});
        }
    }

    void validarIndiceFisico(Index k, const char* nome) const {
        validarIndiceSpan(k, n_physical_, nome);
    }

    std::vector<Real> faces_;
    std::vector<Real> centers_;
    std::vector<Real> dF_;
    std::vector<Real> dC_;

    Index n_physical_;
    Index n_ghost_;
    ::FVGridMaker::grid::CenteringTag centering_;
};

// ----------------------------------------------------------------------------
// Impressão formatada (debug): std::ostream << Grid1D
// ----------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& os, const Grid1D& grid) {
    if (grid.empty()) {
        return os;
    }

    const auto faces   = grid.faces();
    const auto centers = grid.centers();
    const auto dF      = grid.deltasFaces();
    const auto dC      = grid.deltasCenters();

    const auto flags = os.flags();
    const auto prec  = os.precision();

    auto col = [&os](int w, const auto& x) { os << std::setw(w) << x; };

    os << std::right << std::fixed << std::setprecision(6);

    constexpr std::size_t kLine = 80;
    os << std::string(kLine, '-') << "\n";
    os << "Grid1D\n";
    os << std::string(kLine, '-') << "\n\n";

    col(6,  "idx");
    col(15, "face");
    col(15, "center");
    col(15, "dF");
    col(15, "dC");
    os << "\n";

    const std::size_t n = static_cast<std::size_t>(grid.nVolumes());

    for (std::size_t i = 0; i < n; ++i) {
        col(6,  i);
        col(15, faces[i]);
        col(15, centers[i]);
        col(15, dF[i]);
        col(15, dC[i]);
        os << "\n";
    }

    // Última linha: face final + dC final (convenção N+1)
    col(6,  n);
    col(15, faces[n]);
    col(45, dC[n]);
    os << "\n";

    os.flags(flags);
    os.precision(prec);
    return os;
}


FVG_GRID1D_CLOSE
