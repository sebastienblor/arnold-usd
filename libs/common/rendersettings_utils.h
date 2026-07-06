//
// SPDX-License-Identifier: Apache-2.0
//

#pragma once
#include <vector>
#include <pxr/pxr.h>
#include <pxr/usd/usd/stage.h>
#include <ai.h>
#include "timesettings.h"
#include "api_adapter.h"
#include "procedural_reader.h"

// TODO: get rid of that and mode the code in the render_option.h

PXR_NAMESPACE_OPEN_SCOPE

struct ArnoldAOVTypes {
    const char *outputString;
    const AtString aovWrite;
    const AtString userData;
    bool isHalf;

    ArnoldAOVTypes(const char *_outputString, const AtString &_aovWrite, const AtString &_userData, bool _isHalf)
        : outputString(_outputString), aovWrite(_aovWrite), userData(_userData), isHalf(_isHalf)
    {
    }
};

ArnoldAOVTypes GetArnoldTypesFromFormatToken(const TfToken& type);

/// Hook for "raw" Hydra AOVs (sourceType="raw") whose source name needs
/// special handling on the Arnold side.
///
/// In the simplest case the name is just remapped to an Arnold builtin AOV
/// — e.g. \c "normal" is rewritten to \c "N", which Arnold writes out
/// automatically. Other names need an aov_shader chain to compute the value;
/// today this covers:
///   - \c "Neye" : state_vector(N) → space_transform(world→camera, normal)
///                 → aov_write_vector
///   - \c "Peye" : state_vector(P) → space_transform(world→camera, point)
///                 → aov_write_vector
///
/// On return \p sourceName may have been rewritten. When a shader chain is
/// created the head aov_write node is appended to \p aovShaders so the caller
/// can attach it to options.aov_shaders. \p nodeNamePrefix is used to derive
/// stable names for the generated nodes. The function silently no-ops when
/// the required Arnold shader nodes are not registered on the running build.
void SetupHdRawAovShaderChain(
    std::string& sourceName,
    const std::string& aovName,
    const std::string& nodeNamePrefix,
    ArnoldAPIAdapter& context,
    std::vector<AtNode*>& aovShaders);

void ChooseRenderSettings(UsdStageRefPtr stage, std::string &renderSettingsPath, TimeSettings &_time, UsdPrim *rootPrimPtr=nullptr);
AtNode* ReadRenderSettings(const UsdPrim &renderSettingsPrim, ArnoldAPIAdapter &context, ProceduralReader *reader, const TimeSettings &time, AtUniverse *universe, SdfPath& camera);
void ComputeMotionRange(UsdStageRefPtr _stage, const UsdPrim &options,  TimeSettings &_time);
void ComputeUsdLuxVersion(UsdStageRefPtr _stage, const UsdPrim &options,  TimeSettings &_time, const AtUniverse *universe);
void SetArnoldDefaultOptions(AtUniverse *universe);
void SetRegion(AtNode* options, const GfVec4f& windowNDC, const GfVec2i& resolution);

// Color manager helper functions
AtNode* GetOrCreateColorManager(const UsdPrim &renderSettingsPrim, ArnoldAPIAdapter &context, 
                                 const TimeSettings &time, AtNode *options);
void SetupColorManagerColorSpaces(AtNode *colorManager, const UsdPrim &renderSettingsPrim, 
                                   ArnoldAPIAdapter &context, const TimeSettings &time);

PXR_NAMESPACE_CLOSE_SCOPE