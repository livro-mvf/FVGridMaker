// ----------------------------------------------------------------------------
// File: Operations2D.h
// Project: FVGridMaker
// Description: Logical operations for StructuredGrid2D.
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

#include <string_view>
#include <utility>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>
#include <FVGridMaker/OneDimensional/Operations1D/Operations1D.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateSystem2D.h>
#include <FVGridMaker/TwoDimensional/Operations2D/LogicalBox2D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace fvgrid {

class Operations2D final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "TwoDimensional",
            "Operations2D",
            "fvgrid.2d.operations.Operations2D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    static void validate_tolerance(Real tolerance);

    [[nodiscard]] static LogicalBox2D domain_box(
        const StructuredGrid2D& grid
    ) noexcept;

    [[nodiscard]] static LogicalBox2D intersection(
        const StructuredGrid2D& left,
        const StructuredGrid2D& right,
        Real tolerance = Real{}
    );

    [[nodiscard]] static LogicalBox2D require_box_intersection(
        const StructuredGrid2D& left,
        const StructuredGrid2D& right,
        Real tolerance = Real{}
    );

    [[nodiscard]] static StructuredGrid2D clip_to_logical_box(
        const StructuredGrid2D& grid,
        LogicalBox2D box,
        Real tolerance = Real{}
    );

    template <CoordinateMapping2D Mapping>
    [[nodiscard]] static StructuredGrid2D clip_to_logical_box(
        const StructuredGrid2D& grid,
        LogicalBox2D box,
        const Mapping& mapping,
        Real tolerance = Real{}
    ) {
        validate_tolerance(tolerance);
        require_area_box(box);
        require_volume_centered_axes(grid);

        Axis1D first_axis = Operations1D::clip_faces_to_interval(
            grid.first_axis(),
            box.first_interval(),
            tolerance
        );
        Axis1D second_axis = Operations1D::clip_faces_to_interval(
            grid.second_axis(),
            box.second_interval(),
            tolerance
        );

        return StructuredGrid2D{
            std::move(first_axis),
            std::move(second_axis),
            mapping
        };
    }

private:
    static void require_same_patterns(
        const StructuredGrid2D& left,
        const StructuredGrid2D& right
    );

    static void require_area_box(LogicalBox2D box);
    static void require_volume_centered_axes(const StructuredGrid2D& grid);
};

}  // namespace fvgrid
