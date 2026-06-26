// ----------------------------------------------------------------------------
// FVGridMaker: convenience include for the public API.
//
// Public API policy
// -----------------
// Stable API:
//   - Core
//   - ErrorHandling
//   - OneDimensional
//   - Axis1D CSV output
//
// Experimental API:
//   - TwoDimensional
//   - VTK 2D output
//
// Experimental headers are intentionally still included here because they are
// part of the current public development surface. Their APIs may change before
// the next stable release.
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// Stable public API: Core
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/Core/Version.h>

// ----------------------------------------------------------------------------
// Stable public API: Error handling
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ErrorContext.h>
#include <FVGridMaker/ErrorHandling/ErrorDescriptor.h>
#include <FVGridMaker/ErrorHandling/ErrorRecord.h>
#include <FVGridMaker/ErrorHandling/ErrorTraits.h>
#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>

// ----------------------------------------------------------------------------
// Stable public API: One-dimensional grids
// ----------------------------------------------------------------------------
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>

#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

#include <FVGridMaker/OneDimensional/Distribution1D/Custom1D.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Random1D.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Roberts1D.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVGridMaker/OneDimensional/Operations1D/Operations1D.h>

// ----------------------------------------------------------------------------
// Stable public API: One-dimensional output
// ----------------------------------------------------------------------------
#include <FVGridMaker/Output/CSV/Axis1DCSVWriter.h>

// ----------------------------------------------------------------------------
// Experimental public API: Two-dimensional grids and coordinate systems
// ----------------------------------------------------------------------------
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateMappingFactory2D.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateMetrics2D.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateSystem2D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

// ----------------------------------------------------------------------------
// Experimental public API: Two-dimensional output
// ----------------------------------------------------------------------------
#include <FVGridMaker/Output/VTK/LegacyVTKRectilinearGrid2DWriter.h>

namespace fvgrid {

[[nodiscard]] const char* version() noexcept;

}  // namespace fvgrid