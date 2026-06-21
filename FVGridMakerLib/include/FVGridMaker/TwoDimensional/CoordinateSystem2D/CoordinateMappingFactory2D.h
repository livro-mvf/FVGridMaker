#pragma once

#include <string>
#include <string_view>
#include <utility>

#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateMetrics2D.h>

namespace fvgrid {

template <class Map, class CellMeasure, class FirstFaceMeasure,
          class SecondFaceMeasure>
class FunctionalCoordinateMetrics2D final {
public:
    FunctionalCoordinateMetrics2D(
        std::string name, std::string first_name, std::string second_name,
        Map map, CellMeasure cell_measure,
        FirstFaceMeasure first_face_measure,
        SecondFaceMeasure second_face_measure,
        bool vtk_rectilinear=false
    ) : name_(std::move(name)), first_name_(std::move(first_name)),
        second_name_(std::move(second_name)), map_(std::move(map)),
        cell_measure_(std::move(cell_measure)),
        first_face_measure_(std::move(first_face_measure)),
        second_face_measure_(std::move(second_face_measure)),
        vtk_rectilinear_(vtk_rectilinear) {}

    [[nodiscard]] std::string_view name() const noexcept { return name_; }
    [[nodiscard]] std::string_view first_coordinate_name() const noexcept {
        return first_name_;
    }
    [[nodiscard]] std::string_view second_coordinate_name() const noexcept {
        return second_name_;
    }
    [[nodiscard]] bool vtk_rectilinear() const noexcept {
        return vtk_rectilinear_;
    }
    [[nodiscard]] PhysicalPoint2D map(Real first, Real second) const {
        return map_(first,second);
    }
    [[nodiscard]] Real cell_measure(CoordinateCell2D cell) const {
        return cell_measure_(cell);
    }
    [[nodiscard]] Real first_face_measure(
        Real first, Real second_min, Real second_max
    ) const { return first_face_measure_(first,second_min,second_max); }
    [[nodiscard]] Real second_face_measure(
        Real second, Real first_min, Real first_max
    ) const { return second_face_measure_(second,first_min,first_max); }

private:
    std::string name_, first_name_, second_name_;
    Map map_;
    CellMeasure cell_measure_;
    FirstFaceMeasure first_face_measure_;
    SecondFaceMeasure second_face_measure_;
    bool vtk_rectilinear_{};
};

template <class Map, class CellMeasure, class FirstFaceMeasure,
          class SecondFaceMeasure>
[[nodiscard]] auto make_coordinate_mapping_2d(
    std::string name, std::string first_name, std::string second_name,
    Map map, CellMeasure cell_measure,
    FirstFaceMeasure first_face_measure,
    SecondFaceMeasure second_face_measure,
    bool vtk_rectilinear=false
) {
    return FunctionalCoordinateMetrics2D<
        Map,CellMeasure,FirstFaceMeasure,SecondFaceMeasure>{
        std::move(name),std::move(first_name),std::move(second_name),
        std::move(map),std::move(cell_measure),
        std::move(first_face_measure),std::move(second_face_measure),
        vtk_rectilinear};
}

}  // namespace fvgrid
