#include <iostream>
#include <numbers>
#include <string_view>
#include <vector>

#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateMetrics2D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace {

void print_geometry(std::string_view title, const fvgrid::StructuredGrid2D& grid) {
    std::cout << title << '\n'
              << "  distance between first-direction faces: "
              << grid.first_axis().dx_faces().front() << '\n'
              << "  distance between first-direction centres/boundary: "
              << grid.first_axis().dx_centers().front() << '\n'
              << "  first-direction face measure: "
              << grid.first_face_measure(1,0) << '\n'
              << "  second-direction face measure: "
              << grid.second_face_measure(0,1) << '\n'
              << "  cell measure: " << grid.cell_measure(0,0) << "\n\n";
}

}  // namespace

int main() {
    using namespace fvgrid;
    const Axis1D first{std::vector<Real>{0.0,1.0,2.0}};
    const Axis1D second{std::vector<Real>{0.0,1.0}};

    print_geometry("Cartesian",StructuredGrid2D{first,second});
    print_geometry("Polar",StructuredGrid2D{first,second,PolarCoordinates2D{}});
    print_geometry("Cylindrical axisymmetric",
        StructuredGrid2D{first,second,CylindricalAxisymmetricCoordinates2D{}});

    const Axis1D theta{std::vector<Real>{0.0,std::numbers::pi/2.0}};
    print_geometry("Spherical axisymmetric",
        StructuredGrid2D{first,theta,SphericalAxisymmetricCoordinates2D{}});
}
