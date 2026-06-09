// ----------------------------------------------------------------------------
// File: ex_SymmetricRandomWeightedCenters1D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Demonstrates how to build a symmetric random 1D grid using
//              user-generated centre coordinates and an exercise-local
//              face-reconstruction pattern.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------
//
// Exercise goal
// -------------
// This example demonstrates how FVGridMaker can be used as a flexible
// structured-grid construction toolkit. The library is not extended here.
// Instead, the example builds a non-standard one-dimensional finite-volume
// grid using only the public API.
//
// The exercise performs four steps:
//
//   1. Generate random centre coordinates on the left half of the domain.
//   2. Mirror those centres around the domain midpoint to enforce symmetry.
//   3. Define a local pattern that reconstructs internal faces from centres
//      using the rule
//
//          face[i] = 0.25 * center[i - 1] + 0.75 * center[i].
//
//   4. Use Custom1D to build a validated Axis1D from the generated centres
//      and the local reconstruction pattern.
//
// For an odd number of volumes, one centre is placed exactly at the domain
// midpoint. For an even number of volumes, centres occur in mirrored pairs.
//
// The example then checks:
//
//   - centre symmetry around the domain midpoint;
//   - the weighted 25/75 internal-face reconstruction rule;
//   - whether any face spacing dx_faces[i] is smaller than a prescribed
//     warning tolerance.
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include <string_view>
#include <utility>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Custom1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/AxisGeometry1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateKind1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Coordinates1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>

// ----------------------------------------------------------------------------
// Exercise-local grid pattern
// ----------------------------------------------------------------------------
//
// This pattern is intentionally defined inside the example instead of inside
// FVGridMakerLib. It demonstrates that Custom1D can accept a user-side pattern
// as long as the type satisfies the minimal pattern contract:
//
//   - input_kind()
//   - complete_geometry(...)
//
// The pattern receives centre coordinates as primary coordinates and
// reconstructs face coordinates. The boundary faces are imposed by Domain1D.
// The internal faces are biased toward the right centre using the 25/75 rule.
//
struct WeightedFaceFromCenters1D final {
    [[nodiscard]] static constexpr std::string_view name() noexcept {
        return "WeightedFaceFromCenters1D";
    }

    [[nodiscard]] static constexpr fvgrid::CoordinateKind1D input_kind()
        noexcept {
        return fvgrid::CoordinateKind1D::Centers;
    }

    [[nodiscard]] static fvgrid::AxisGeometry1D complete_geometry(
        std::vector<fvgrid::Real> centers,
        fvgrid::Domain1D domain
    ) {
        const fvgrid::Size nvol = centers.size();

        // Build one face more than the number of control volumes.
        std::vector<fvgrid::Real> faces(nvol + 1);

        // The first and last faces are the physical domain boundaries.
        faces.front() = domain.xmin();
        faces.back() = domain.xmax();

        // Reconstruct internal faces from neighbouring centres.
        //
        // The face between center[i - 1] and center[i] is placed at 25% of
        // the left centre and 75% of the right centre.
        for (fvgrid::Size i = 1; i < nvol; ++i) {
            faces[i] =
                0.25 * centers[i - 1] +
                0.75 * centers[i];
        }

        return fvgrid::AxisGeometry1D{
            .faces = std::move(faces),
            .centers = std::move(centers),
            .pattern_name = name(),
        };
    }
};

// ----------------------------------------------------------------------------
// Generate symmetric random centre coordinates
// ----------------------------------------------------------------------------
//
// This function generates only the primary coordinates used by the exercise.
// The faces are reconstructed later by the local pattern above.
//
// The algorithm is:
//
//   1. Compute the domain midpoint.
//   2. Generate random centres in the left half of the domain.
//   3. Sort those centres to enforce monotonicity.
//   4. If nvol is odd, place one centre exactly at the midpoint.
//   5. Mirror the left centres into the right half.
//
// The resulting centres satisfy:
//
//   center[i] + center[nvol - 1 - i] = 2 * xmid.
//
[[nodiscard]] std::vector<fvgrid::Real> make_symmetric_random_centers(
    fvgrid::NVol nvol,
    fvgrid::Length length,
    fvgrid::XInit xinit,
    fvgrid::Seed seed
) {
    const fvgrid::Size n = nvol.value();
    const fvgrid::Real xmin = xinit.value();
    const fvgrid::Real xmax = xmin + length.value();
    const fvgrid::Real xmid = 0.5 * (xmin + xmax);

    const fvgrid::Size half = n / 2;

    std::mt19937_64 generator{seed.value()};
    std::uniform_real_distribution<fvgrid::Real> distribution{xmin, xmid};

    // Step 1: generate the centres in the left half of the domain.
    //
    // A separate vector is used here to avoid unsigned-to-signed iterator
    // arithmetic warnings in std::sort.
    std::vector<fvgrid::Real> left_centers(half);

    for (fvgrid::Size i = 0; i < half; ++i) {
        left_centers[i] = distribution(generator);
    }

    // Step 2: sort the left side to obtain increasing coordinates.
    std::sort(left_centers.begin(), left_centers.end());

    std::vector<fvgrid::Real> centers(n);

    // Step 3: copy the left-half centres.
    for (fvgrid::Size i = 0; i < half; ++i) {
        centers[i] = left_centers[i];
    }

    // Step 4: insert the midpoint centre when the number of volumes is odd.
    if (n % 2 != 0) {
        centers[half] = xmid;
    }

    // Step 5: mirror the left-half centres into the right half.
    for (fvgrid::Size i = 0; i < half; ++i) {
        centers[n - 1 - i] = 2.0 * xmid - left_centers[i];
    }

    return centers;
}

// ----------------------------------------------------------------------------
// Check the generated axis
// ----------------------------------------------------------------------------
//
// This function is not part of FVGridMaker. It is an exercise-side diagnostic
// routine. It demonstrates that the generated Axis1D satisfies the intended
// construction rules.
//
// It checks:
//
//   1. Centre symmetry around the midpoint.
//   2. The 25/75 internal-face reconstruction rule.
//   3. Whether any dx_faces value is smaller than the chosen warning tolerance.
//
// Failures in the imposed rules make the function return false. Small
// dx_faces values only produce warnings because very small cells may be
// acceptable in some mesh-generation contexts.
//
[[nodiscard]] bool check_generated_axis(
    const fvgrid::Axis1D& axis,
    fvgrid::Real check_tolerance,
    fvgrid::Real small_face_tolerance
) {
    bool passed = true;

    const auto faces = axis.faces();
    const auto centers = axis.centers();
    const auto dx_faces = axis.dx_faces();

    const fvgrid::Real xmid = 0.5 * (axis.xmin() + axis.xmax());

    std::cout << "\nChecks\n";

    // Check 1: centre symmetry.
    for (fvgrid::Size i = 0; i < centers.size(); ++i) {
        const fvgrid::Size j = centers.size() - 1 - i;

        if (i > j) {
            break;
        }

        const fvgrid::Real residual =
            std::abs((centers[i] + centers[j]) - 2.0 * xmid);

        if (residual > check_tolerance) {
            std::cout
                << "  [fail] symmetry: center[" << i << "] + center["
                << j << "] differs from 2*xmid by "
                << residual << '\n';

            passed = false;
        }
    }

    // Check 2: weighted face reconstruction.
    for (fvgrid::Size i = 1; i + 1 < faces.size(); ++i) {
        const fvgrid::Real expected_face =
            0.25 * centers[i - 1] +
            0.75 * centers[i];

        const fvgrid::Real residual = std::abs(faces[i] - expected_face);

        if (residual > check_tolerance) {
            std::cout
                << "  [fail] weighted face rule: face[" << i
                << "] differs from 0.25*center[" << i - 1
                << "] + 0.75*center[" << i << "] by "
                << residual << '\n';

            passed = false;
        }
    }

    // Check 3: warn about very small control volumes.
    for (fvgrid::Size i = 0; i < dx_faces.size(); ++i) {
        if (dx_faces[i] < small_face_tolerance) {
            std::cout
                << "  [warning] dx_faces[" << i << "] = "
                << dx_faces[i]
                << " is smaller than small_face_tolerance = "
                << small_face_tolerance << '\n';
        }
    }

    if (passed) {
        std::cout << "  Symmetry check passed.\n";
        std::cout << "  Weighted-face rule check passed.\n";
    }

    return passed;
}

int main() {
    // ------------------------------------------------------------------------
    // Step 1: define the exercise input data.
    // ------------------------------------------------------------------------
    //
    // These values could later come from a file or from command-line arguments.
    // For the exercise, constants keep the example short and reproducible.
    const fvgrid::XInit xinit{0.0};
    const fvgrid::Length length{1.0};
    const fvgrid::NVol nvol{9};
    const fvgrid::Seed seed{1234};

    constexpr fvgrid::Real check_tolerance = 1.0e-10;
    constexpr fvgrid::Real small_face_tolerance = 1.0e-2;

    // ------------------------------------------------------------------------
    // Step 2: generate user-side primary coordinates.
    // ------------------------------------------------------------------------
    //
    // The library is not responsible for this special construction. We generate
    // the centres externally and then pass them to Custom1D.
    std::vector<fvgrid::Real> centers =
        make_symmetric_random_centers(
            nvol,
            length,
            xinit,
            seed
        );

    // ------------------------------------------------------------------------
    // Step 3: build the Axis1D using Custom1D.
    // ------------------------------------------------------------------------
    //
    // Custom1D receives:
    //
    //   - the generated centre coordinates;
    //   - the exercise-local reconstruction pattern;
    //   - the physical domain.
    //
    // Axis1D then stores the completed geometry and computes the metrics.
    const fvgrid::Axis1D axis = fvgrid::Custom1D::make(
        fvgrid::Coordinates1D::centers(std::move(centers)),
        WeightedFaceFromCenters1D{},
        fvgrid::Domain1D::from_length(xinit, length)
    );

    // ------------------------------------------------------------------------
    // Step 4: print the generated grid.
    // ------------------------------------------------------------------------
    //
    // Axis1D uses the current stream precision. A moderate fixed precision keeps
    // the table readable.
    std::cout << std::fixed << std::setprecision(6);
    std::cout << axis << '\n';

    // ------------------------------------------------------------------------
    // Step 5: verify the exercise-specific construction rules.
    // ------------------------------------------------------------------------
    const bool passed =
        check_generated_axis(axis, check_tolerance, small_face_tolerance);

    return passed ? 0 : 1;
}