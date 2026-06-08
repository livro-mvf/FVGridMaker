// ----------------------------------------------------------------------------
// File: Axis1D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Implements a one-dimensional structured finite-volume axis.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <algorithm>
#include <utility>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorCodes.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>

namespace fvgrid {

Axis1D::Axis1D(std::vector<Real> faces) : faces_(std::move(faces)) {
    validate_faces();
    rebuild_derived_data();
}

Size Axis1D::num_cells() const noexcept {
    return centers_.size();
}

Size Axis1D::num_faces() const noexcept {
    return faces_.size();
}

std::span<const Real> Axis1D::faces() const noexcept {
    return faces_;
}

std::span<const Real> Axis1D::centers() const noexcept {
    return centers_;
}

std::span<const Real> Axis1D::cell_lengths() const noexcept {
    return cell_lengths_;
}

Real Axis1D::xmin() const noexcept {
    return faces_.front();
}

Real Axis1D::xmax() const noexcept {
    return faces_.back();
}

Real Axis1D::length() const noexcept {
    return xmax() - xmin();
}

void Axis1D::validate_faces() const {
    require(
        faces_.size() >= 2,
        error_code::kInvalidFaceCount,
        "An Axis1D requires at least two faces.",
        "Axis1D"
    );

    const bool strictly_increasing =
        std::ranges::adjacent_find(
            faces_,
            [](Real left, Real right) {
                return !(right > left);
            }
        ) == faces_.end();

    require(
        strictly_increasing,
        error_code::kNonIncreasingFaces,
        "Axis1D faces must be strictly increasing.",
        "Axis1D"
    );
}

void Axis1D::rebuild_derived_data() {
    const Size cell_count = faces_.size() - 1;

    centers_.resize(cell_count);
    cell_lengths_.resize(cell_count);

    for (Size i = 0; i < cell_count; ++i) {
        const Real left = faces_[i];
        const Real right = faces_[i + 1];

        centers_[i] = static_cast<Real>(0.5) * (left + right);
        cell_lengths_[i] = right - left;
    }
}

}  // namespace fvgrid