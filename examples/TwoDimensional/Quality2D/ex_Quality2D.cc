#include <iomanip>
#include <iostream>
#include <vector>

#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/TwoDimensional/Quality2D/Quality2D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

int main() {
    using namespace fvgrid;

    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{0.0, 1.0, 3.0}},
        Axis1D{std::vector<Real>{0.0, 1.0, 4.0}}
    };

    const QualityReport2D quality = Quality2D::evaluate(grid);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Quality2D report\n";
    std::cout << "cell measure min/max/ratio: "
              << quality.min_cell_measure << " / "
              << quality.max_cell_measure << " / "
              << quality.cell_measure_ratio << '\n';
    std::cout << "computational area min/max/ratio: "
              << quality.min_computational_area << " / "
              << quality.max_computational_area << " / "
              << quality.computational_area_ratio << '\n';
    std::cout << "max adjacent ratio first direction: "
              << quality.max_adjacent_cell_measure_ratio_first_direction
              << '\n';
    std::cout << "max adjacent ratio second direction: "
              << quality.max_adjacent_cell_measure_ratio_second_direction
              << '\n';
}
