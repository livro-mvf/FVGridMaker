// ----------------------------------------------------------------------------
// File: Axis1D.cpp
// Project: FVGridMaker
// Version: 0.1.0
// Description: Implements a one-dimensional structured finite-volume axis.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
// #include <algorithm>
#include <iomanip>
// #include <ostream>
// #include <string>
// #include <utility>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorCatalog.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>

namespace fvgrid {

Axis1D::Axis1D(std::vector<Real> faces)
    : Axis1D(
          faces,
          VolumeCentered1D::centers_from_faces(faces),
          VolumeCentered1D::name()
      ) {}

Axis1D::Axis1D(std::vector<Real> faces, std::string_view pattern_name)
    : Axis1D(
          faces,
          VolumeCentered1D::centers_from_faces(faces),
          pattern_name
      ) {}

Axis1D::Axis1D(
    std::vector<Real> faces,
    std::vector<Real> centers,
    std::string_view pattern_name
)
    : faces_(std::move(faces)),
      centers_(std::move(centers)),
      pattern_name_(pattern_name) {
    validate_geometry();
    rebuild_metrics();
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

std::span<const Real> Axis1D::dx_faces() const noexcept {
    return dx_faces_;
}

std::span<const Real> Axis1D::dx_centers() const noexcept {
    return dx_centers_;
}

std::span<const Real> Axis1D::cell_lengths() const noexcept {
    return dx_faces_;
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

std::string_view Axis1D::pattern_name() const noexcept {
    return pattern_name_;
}

void Axis1D::validate_geometry() const {
    require(
        faces_.size() >= 2,
        error_catalog::kInvalidFaceCount,
        Axis1D::id()
    );

    require(
        centers_.size() + 1 == faces_.size(),
        error_catalog::kInvalidCenterCount,
        Axis1D::id()
    );

    const bool faces_strictly_increasing =
        std::ranges::adjacent_find(
            faces_,
            [](Real left, Real right) {
                return !(right > left);
            }
        ) == faces_.end();

    require(
        faces_strictly_increasing,
        error_catalog::kNonIncreasingFaces,
        Axis1D::id()
    );

    const bool centers_strictly_increasing =
        std::ranges::adjacent_find(
            centers_,
            [](Real left, Real right) {
                return !(right > left);
            }
        ) == centers_.end();

    require(
        centers_strictly_increasing,
        error_catalog::kNonIncreasingCenters,
        Axis1D::id()
    );

    require(
        centers_.front() > faces_.front(),
        error_catalog::kOutOfRange,
        Axis1D::id()
    );

    require(
        centers_.back() < faces_.back(),
        error_catalog::kOutOfRange,
        Axis1D::id()
    );
}

void Axis1D::rebuild_metrics() {
    const Size cell_count = centers_.size();

    dx_faces_.resize(cell_count);
    dx_centers_.resize(cell_count + 1);

    for (Size i = 0; i < cell_count; ++i) {
        dx_faces_[i] = faces_[i + 1] - faces_[i];
    }

    dx_centers_[0] = centers_[0] - faces_[0];

    for (Size i = 1; i < cell_count; ++i) {
        dx_centers_[i] = centers_[i] - centers_[i - 1];
    }

    dx_centers_[cell_count] = faces_[cell_count] - centers_[cell_count - 1];
}

std::ostream& operator<<(std::ostream& os, const Axis1D& axis) {
    const auto faces = axis.faces();
    const auto centers = axis.centers();
    const auto dx_faces = axis.dx_faces();
    const auto dx_centers = axis.dx_centers();

    const Size nvol = axis.num_cells();

    os << "Axis1D\n";
    os << "pattern         : " << axis.pattern_name() << '\n';
    os << "number of faces : " << axis.num_faces() << '\n';
    os << "number of cells : " << axis.num_cells() << '\n';
    os << "xmin            : " << axis.xmin() << '\n';
    os << "xmax            : " << axis.xmax() << '\n';
    os << "length          : " << axis.length() << '\n';
    os << '\n';

    os << std::setw(6) << "i"
       << std::setw(16) << "xface[i]"
       << std::setw(16) << "xcenter[i]"
       << std::setw(16) << "dxface[i]"
       << std::setw(16) << "dxcenter[i]"
       << '\n';

    os << std::string(70, '-') << '\n';

    for (Size i = 0; i < nvol; ++i) {
        os << std::setw(6) << i
           << std::setw(16) << faces[i]
           << std::setw(16) << centers[i]
           << std::setw(16) << dx_faces[i]
           << std::setw(16) << dx_centers[i]
           << '\n';
    }

    os << std::setw(6) << nvol
       << std::setw(16) << faces[nvol]
       << std::setw(16) << ""
       << std::setw(16) << ""
       << std::setw(16) << dx_centers[nvol];

    return os;
}

}  // namespace fvgrid