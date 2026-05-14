//
// SPDX-License-Identifier: Apache-2.0
//
#include "arnold_light_adapter.h"

#include <pxr/base/tf/staticTokens.h>
#include <pxr/base/tf/type.h>
#include <pxr/usd/usd/relationship.h>
#include <pxr/usdImaging/usdImaging/cylinderLightAdapter.h>
#include <pxr/usdImaging/usdImaging/diskLightAdapter.h>
#include <pxr/usdImaging/usdImaging/distantLightAdapter.h>
#include <pxr/usdImaging/usdImaging/domeLightAdapter.h>
#if PXR_VERSION >= 2405
#include <pxr/usdImaging/usdImaging/domeLight_1Adapter.h>
#endif
#if PXR_VERSION >= 2208
#include <pxr/usdImaging/usdImaging/geometryLightAdapter.h>
#endif
#include <pxr/usdImaging/usdImaging/primAdapter.h>
#include <pxr/usdImaging/usdImaging/rectLightAdapter.h>
#include <pxr/usdImaging/usdImaging/sphereLightAdapter.h>

PXR_NAMESPACE_OPEN_SCOPE

// clang-format off
TF_DEFINE_PRIVATE_TOKENS(_tokens,
    ((primvarsArnoldShaders, "primvars:arnold:shaders"))
);
// clang-format on

VtValue ArnoldUsdImagingGetLightShadersValue(const UsdPrim& prim, const TfToken& paramName)
{
    if (paramName != _tokens->primvarsArnoldShaders) {
        return VtValue();
    }
    UsdRelationship rel = prim.GetRelationship(paramName);
    if (!rel) {
        return VtValue();
    }
    SdfPathVector targets;
    if (!rel.GetForwardedTargets(&targets) || targets.empty()) {
        return VtValue();
    }
    return VtValue(targets);
}

// One adapter subclass per USD light type. Each one only overrides
// GetLightParamValue to forward the primvars:arnold:shaders relationship; for
// all other parameters it falls through to the per-type Pixar base adapter.
//
// The macro generates: class UsdImagingArnoldXxxAdapter : public UsdImagingXxxAdapter.
// The class name suffix matches the USD prim type name so plug registry
// resolution by primTypeName is unambiguous.
#define ARNOLD_LIGHT_ADAPTER(Suffix)                                                                                  \
    class UsdImagingArnold##Suffix##Adapter : public UsdImaging##Suffix##Adapter {                                    \
    public:                                                                                                            \
        using BaseAdapter = UsdImaging##Suffix##Adapter;                                                              \
        UsdImagingArnold##Suffix##Adapter() : UsdImaging##Suffix##Adapter() {}                                        \
        VtValue GetLightParamValue(                                                                                    \
            const UsdPrim& prim, const SdfPath& cachePath, const TfToken& paramName, UsdTimeCode time) const override \
        {                                                                                                              \
            VtValue v = ArnoldUsdImagingGetLightShadersValue(prim, paramName);                                        \
            if (!v.IsEmpty())                                                                                          \
                return v;                                                                                              \
            return BaseAdapter::GetLightParamValue(prim, cachePath, paramName, time);                                 \
        }                                                                                                              \
    }

ARNOLD_LIGHT_ADAPTER(CylinderLight);
ARNOLD_LIGHT_ADAPTER(DiskLight);
ARNOLD_LIGHT_ADAPTER(DistantLight);
ARNOLD_LIGHT_ADAPTER(DomeLight);
#if PXR_VERSION >= 2405
ARNOLD_LIGHT_ADAPTER(DomeLight_1);
#endif
#if PXR_VERSION >= 2208
ARNOLD_LIGHT_ADAPTER(GeometryLight);
#endif
ARNOLD_LIGHT_ADAPTER(RectLight);
ARNOLD_LIGHT_ADAPTER(SphereLight);

#define REGISTER_ARNOLD_LIGHT_ADAPTER(Suffix)                                  \
    {                                                                          \
        using Adapter = UsdImagingArnold##Suffix##Adapter;                     \
        TfType t = TfType::Define<Adapter, TfType::Bases<Adapter::BaseAdapter>>(); \
        t.SetFactory<UsdImagingPrimAdapterFactory<Adapter>>();                 \
    }

TF_REGISTRY_FUNCTION(TfType)
{
    REGISTER_ARNOLD_LIGHT_ADAPTER(CylinderLight);
    REGISTER_ARNOLD_LIGHT_ADAPTER(DiskLight);
    REGISTER_ARNOLD_LIGHT_ADAPTER(DistantLight);
    REGISTER_ARNOLD_LIGHT_ADAPTER(DomeLight);
#if PXR_VERSION >= 2405
    REGISTER_ARNOLD_LIGHT_ADAPTER(DomeLight_1);
#endif
#if PXR_VERSION >= 2208
    REGISTER_ARNOLD_LIGHT_ADAPTER(GeometryLight);
#endif
    REGISTER_ARNOLD_LIGHT_ADAPTER(RectLight);
    REGISTER_ARNOLD_LIGHT_ADAPTER(SphereLight);
}

PXR_NAMESPACE_CLOSE_SCOPE
