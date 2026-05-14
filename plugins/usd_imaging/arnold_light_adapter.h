//
// SPDX-License-Identifier: Apache-2.0
//
#pragma once

#include <pxr/pxr.h>
#include "api.h"

#include <pxr/base/tf/token.h>
#include <pxr/base/vt/value.h>
#include <pxr/usd/sdf/path.h>
#include <pxr/usd/usd/prim.h>

PXR_NAMESPACE_OPEN_SCOPE

// If paramName is the primvars:arnold:shaders token and the prim authors it as
// a relationship, return a VtValue holding the forwarded SdfPathVector. Returns
// an empty VtValue otherwise so callers can fall through to the base adapter
// (which will handle the legacy string-attribute form).
USDIMAGINGARNOLD_API
VtValue ArnoldUsdImagingGetLightShadersValue(const UsdPrim& prim, const TfToken& paramName);

PXR_NAMESPACE_CLOSE_SCOPE
