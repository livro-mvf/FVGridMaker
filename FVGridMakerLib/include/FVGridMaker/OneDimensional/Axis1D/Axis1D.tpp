// ----------------------------------------------------------------------------
// File: Axis1D.tpp
// Project: FVGridMaker
// Description: Template implementation for BasicAxis1D.
// ----------------------------------------------------------------------------

#pragma once

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>
#include <ostream>
#include <string>
#include <utility>

#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

namespace fvgrid {
namespace detail {

template <std::floating_point T>
[[nodiscard]] T clean_axis_value_for_print(T value) noexcept {
    const T tolerance = static_cast<T>(100) * std::numeric_limits<T>::epsilon();

    if (std::abs(value) < tolerance) {
        return T{};
    }

    return value;
}

}  // namespace detail

template <std::floating_point T>
BasicAxis1D<T>::BasicAxis1D(std::vector<T> faces)
    : faces_(std::move(faces)),
      centers_(VolumeCentered1D::centers_from_faces<T>(faces_)),
      pattern_name_(VolumeCentered1D::name()) {
    validate_geometry();
    rebuild_metrics();
}

template <std::floating_point T>
BasicAxis1D<T>::BasicAxis1D(
    std::vector<T> faces,
    std::vector<T> centers,
    std::string pattern_name
)
    : faces_(std::move(faces)),
      centers_(std::move(centers)),
      pattern_name_(std::move(pattern_name)) {
    validate_geometry();
    rebuild_metrics();
}

template <std::floating_point T>
BasicAxis1D<T> BasicAxis1D<T>::from_geometry(
    BasicAxisGeometry1D<T> geometry
) {
    return BasicAxis1D{
        std::move(geometry.faces),
        std::move(geometry.centers),
        std::move(geometry.pattern_name)
    };
}

template <std::floating_point T>
Size BasicAxis1D<T>::num_cells() const noexcept {
    return centers_.size();
}

template <std::floating_point T>
Size BasicAxis1D<T>::num_faces() const noexcept {
    return faces_.size();
}

template <std::floating_point T>
std::span<const T> BasicAxis1D<T>::faces() const noexcept {
    return faces_;
}

template <std::floating_point T>
std::span<const T> BasicAxis1D<T>::centers() const noexcept {
    return centers_;
}

template <std::floating_point T>
std::span<const T> BasicAxis1D<T>::dx_faces() const noexcept {
    return dx_faces_;
}

template <std::floating_point T>
std::span<const T> BasicAxis1D<T>::dx_centers() const noexcept {
    return dx_centers_;
}

template <std::floating_point T>
std::span<const T> BasicAxis1D<T>::cell_lengths() const noexcept {
    return dx_faces_;
}

template <std::floating_point T>
T BasicAxis1D<T>::face(Size i) const noexcept {
    return faces_[i];
}

template <std::floating_point T>
T BasicAxis1D<T>::center(Size p) const noexcept {
    return centers_[p];
}

template <std::floating_point T>
T BasicAxis1D<T>::west_face(Size p) const noexcept {
    return faces_[p];
}

template <std::floating_point T>
T BasicAxis1D<T>::east_face(Size p) const noexcept {
    return faces_[p + static_cast<Size>(1)];
}

template <std::floating_point T>
T BasicAxis1D<T>::cell_length(Size p) const noexcept {
    return dx_faces_[p];
}

template <std::floating_point T>
T BasicAxis1D<T>::center_distance(Size i) const noexcept {
    return dx_centers_[i];
}

template <std::floating_point T>
T BasicAxis1D<T>::DxP(Size p) const noexcept {
    return dx_faces_[p];
}

template <std::floating_point T>
T BasicAxis1D<T>::DxW(Size p) const noexcept {
    return dx_faces_[p - static_cast<Size>(1)];
}

template <std::floating_point T>
T BasicAxis1D<T>::DxE(Size p) const noexcept {
    return dx_faces_[p + static_cast<Size>(1)];
}

template <std::floating_point T>
T BasicAxis1D<T>::deltaxw(Size p) const noexcept {
    return dx_centers_[p];
}

template <std::floating_point T>
T BasicAxis1D<T>::deltaxe(Size p) const noexcept {
    return dx_centers_[p + static_cast<Size>(1)];
}

template <std::floating_point T>
T BasicAxis1D<T>::xmin() const noexcept {
    return faces_.front();
}

template <std::floating_point T>
T BasicAxis1D<T>::xmax() const noexcept {
    return faces_.back();
}

template <std::floating_point T>
T BasicAxis1D<T>::length() const noexcept {
    return xmax() - xmin();
}

template <std::floating_point T>
std::string_view BasicAxis1D<T>::pattern_name() const noexcept {
    return pattern_name_;
}

template <std::floating_point T>
void BasicAxis1D<T>::validate_geometry() const {
    require<errors::grid::InvalidFaceCount>(
        faces_.size() >= static_cast<Size>(2),
        BasicAxis1D::id()
    );

    require<errors::grid::InvalidCenterCount>(
        centers_.size() + static_cast<Size>(1) == faces_.size(),
        BasicAxis1D::id()
    );

    require<errors::core::InvalidArgument>(
        !pattern_name_.empty(),
        BasicAxis1D::id()
    );

    const bool faces_are_finite = std::ranges::all_of(
        faces_,
        [](T value) {
            return std::isfinite(value);
        }
    );

    require<errors::core::InvalidArgument>(
        faces_are_finite,
        BasicAxis1D::id()
    );

    const bool centers_are_finite = std::ranges::all_of(
        centers_,
        [](T value) {
            return std::isfinite(value);
        }
    );

    require<errors::core::InvalidArgument>(
        centers_are_finite,
        BasicAxis1D::id()
    );

    const bool faces_strictly_increasing =
        std::ranges::adjacent_find(
            faces_,
            [](T left, T right) {
                return !(right > left);
            }
        ) == faces_.end();

    require<errors::grid::NonIncreasingFaces>(
        faces_strictly_increasing,
        BasicAxis1D::id()
    );

    const bool centers_strictly_increasing =
        std::ranges::adjacent_find(
            centers_,
            [](T left, T right) {
                return !(right > left);
            }
        ) == centers_.end();

    require<errors::grid::NonIncreasingCenters>(
        centers_strictly_increasing,
        BasicAxis1D::id()
    );

    require<errors::core::OutOfRange>(
        centers_.front() > faces_.front(),
        BasicAxis1D::id()
    );

    require<errors::core::OutOfRange>(
        centers_.back() < faces_.back(),
        BasicAxis1D::id()
    );

    for (Size p = 0; p < centers_.size(); ++p) {
        require<errors::core::OutOfRange>(
            faces_[p] < centers_[p] &&
                centers_[p] < faces_[p + static_cast<Size>(1)],
            BasicAxis1D::id()
        );
    }
}

template <std::floating_point T>
void BasicAxis1D<T>::rebuild_metrics() {
    const Size cell_count = centers_.size();

    dx_faces_.resize(cell_count);
    dx_centers_.resize(cell_count + static_cast<Size>(1));

    for (Size i = 0; i < cell_count; ++i) {
        dx_faces_[i] = faces_[i + static_cast<Size>(1)] - faces_[i];
    }

    dx_centers_[0] = centers_[0] - faces_[0];

    for (Size i = 1; i < cell_count; ++i) {
        dx_centers_[i] = centers_[i] - centers_[i - static_cast<Size>(1)];
    }

    dx_centers_[cell_count] =
        faces_[cell_count] - centers_[cell_count - static_cast<Size>(1)];
}

template <std::floating_point T>
std::ostream& operator<<(std::ostream& os, const BasicAxis1D<T>& axis) {
    os << "Axis1D\n";
    os << "pattern         : " << axis.pattern_name() << "\n";
    os << "number of faces : " << axis.num_faces() << "\n";
    os << "number of cells : " << axis.num_cells() << "\n";
    os << "xmin            : "
       << detail::clean_axis_value_for_print(axis.xmin()) << "\n";
    os << "xmax            : "
       << detail::clean_axis_value_for_print(axis.xmax()) << "\n";
    os << "length          : "
       << detail::clean_axis_value_for_print(axis.length()) << "\n";
    os << "\n";

    os << std::setw(6) << "i"
       << std::setw(16) << "xface[i]"
       << std::setw(16) << "xcenter[i]"
       << std::setw(16) << "dxface[i]"
       << std::setw(16) << "dxcenter[i]"
       << "\n";

    os << std::string(70, static_cast<char>(45)) << "\n";

    const auto faces = axis.faces();
    const auto centers = axis.centers();
    const auto dx_faces = axis.dx_faces();
    const auto dx_centers = axis.dx_centers();
    const Size cell_count = axis.num_cells();

    for (Size i = 0; i < cell_count; ++i) {
        os << std::setw(6) << i
           << std::setw(16) << detail::clean_axis_value_for_print(faces[i])
           << std::setw(16) << detail::clean_axis_value_for_print(centers[i])
           << std::setw(16) << detail::clean_axis_value_for_print(dx_faces[i])
           << std::setw(16) << detail::clean_axis_value_for_print(dx_centers[i])
           << "\n";
    }

    os << std::setw(6) << cell_count
       << std::setw(16) << detail::clean_axis_value_for_print(faces[cell_count])
       << std::setw(16) << "-"
       << std::setw(16) << "-"
       << std::setw(16)
       << detail::clean_axis_value_for_print(dx_centers[cell_count])
       << "\n";

    return os;
}

}  // namespace fvgrid
