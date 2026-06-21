// ----------------------------------------------------------------------------
// FVGridMaker: convenience include for the complete public API.
// ----------------------------------------------------------------------------
#pragma once

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/Core/Version.h>
#include <FVGridMaker/ErrorHandling/ErrorCatalog.h>
#include <FVGridMaker/ErrorHandling/ErrorCodes.h>
#include <FVGridMaker/ErrorHandling/ErrorRecord.h>
#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Custom1D.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Random1D.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Roberts1D.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>
#include <FVGridMaker/OneDimensional/Operations1D/Operations1D.h>
#include <FVGridMaker/Output/CSV/Axis1DCSVWriter.h>
#include <FVGridMaker/Output/VTK/LegacyVTKRectilinearGrid2DWriter.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateMappingFactory2D.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateMetrics2D.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateSystem2D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace fvgrid {
[[nodiscard]] const char* version() noexcept;
}  // namespace fvgrid
