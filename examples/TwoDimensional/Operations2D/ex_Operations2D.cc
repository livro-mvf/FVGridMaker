#include <iostream>
#include <vector>

#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/TwoDimensional/Operations2D/Operations2D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

int main() {
    using namespace fvgrid;

    const StructuredGrid2D left{
        Axis1D{std::vector<Real>{0.0, 1.0, 2.0, 3.0}},
        Axis1D{std::vector<Real>{0.0, 1.0, 2.0}}
    };
    const StructuredGrid2D right{
        Axis1D{std::vector<Real>{0.5, 1.5, 2.5}},
        Axis1D{std::vector<Real>{0.5, 1.5}}
    };

    const LogicalBox2D overlap = Operations2D::require_box_intersection(
        left,
        right
    );

    std::cout << "Logical overlap\n";
    std::cout << "first : [" << overlap.first_interval().lower()
              << ", " << overlap.first_interval().upper() << "]\n";
    std::cout << "second: [" << overlap.second_interval().lower()
              << ", " << overlap.second_interval().upper() << "]\n";
    std::cout << "computational area: "
              << overlap.computational_area() << "\n\n";

    const StructuredGrid2D clipped = Operations2D::clip_to_logical_box(
        left,
        overlap
    );

    std::cout << "Clipped grid\n" << clipped << '\n';
}
