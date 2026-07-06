// Copyright 2024 Autodesk, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#pragma once

#ifdef ENABLE_SCENE_INDEX

#include <pxr/imaging/hd/filteringSceneIndex.h>
#include <pxr/imaging/hd/sceneIndexPlugin.h>
#include <pxr/pxr.h>

#include <unordered_set>

PXR_NAMESPACE_OPEN_SCOPE

TF_DECLARE_REF_PTRS(HdArnoldPortalLightResolvingSceneIndex);

/// Scene index filter for UsdLuxPortalLight / UsdLuxDomeLight portal support.
///
/// When a portal light is added, removed, or dirtied this filter propagates
/// a light-dirty notification to all known DomeLights so that each dome's
/// render-delegate Sync can refresh its Arnold skydome_light.portals[] array
/// with the updated light_portal nodes.
///
/// The implementation uses a simple broadcast strategy (portal change →
/// dirty all domes) which is correct for any scene topology and efficient
/// for the common case of a single dome light.
///
/// For Hydra 1 the dome-light Sync already reads the portals relationship
/// directly via HdSceneDelegate::GetLightParamValue.  This plugin is only
/// loaded when ENABLE_SCENE_INDEX is defined (Hydra-2 code path).
class HdArnoldPortalLightResolvingSceneIndex : public HdSingleInputFilteringSceneIndexBase {
public:
    static HdArnoldPortalLightResolvingSceneIndexRefPtr New(
        const HdSceneIndexBaseRefPtr& inputSceneIndex);

    HdSceneIndexPrim GetPrim(const SdfPath& primPath) const override;
    SdfPathVector GetChildPrimPaths(const SdfPath& primPath) const override;

protected:
    explicit HdArnoldPortalLightResolvingSceneIndex(
        const HdSceneIndexBaseRefPtr& inputSceneIndex);
    ~HdArnoldPortalLightResolvingSceneIndex() override = default;

    void _PrimsAdded(
        const HdSceneIndexBase& sender,
        const HdSceneIndexObserver::AddedPrimEntries& entries) override;

    void _PrimsRemoved(
        const HdSceneIndexBase& sender,
        const HdSceneIndexObserver::RemovedPrimEntries& entries) override;

    void _PrimsDirtied(
        const HdSceneIndexBase& sender,
        const HdSceneIndexObserver::DirtiedPrimEntries& entries) override;

private:
    /// Emit an HdLightSchema dirty notification for every known dome light.
    void _DirtyAllDomes(HdSceneIndexObserver::DirtiedPrimEntries& out) const;

    std::unordered_set<SdfPath, SdfPath::Hash> _domeLights;
    std::unordered_set<SdfPath, SdfPath::Hash> _portalLights;
};

class HdArnoldPortalLightResolvingSceneIndexPlugin : public HdSceneIndexPlugin {
public:
    HdArnoldPortalLightResolvingSceneIndexPlugin() = default;

protected:
    HdSceneIndexBaseRefPtr _AppendSceneIndex(
        const HdSceneIndexBaseRefPtr& inputScene,
        const HdContainerDataSourceHandle& inputArgs) override;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // ENABLE_SCENE_INDEX
