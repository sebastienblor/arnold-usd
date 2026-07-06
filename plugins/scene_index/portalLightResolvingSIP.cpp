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
#include "portalLightResolvingSIP.h"

#ifdef ENABLE_SCENE_INDEX

#include <pxr/pxr.h>

#include <pxr/imaging/hd/filteringSceneIndex.h>
#include <pxr/imaging/hd/lightSchema.h>
#include <pxr/imaging/hd/sceneIndexPluginRegistry.h>
#include <pxr/imaging/hd/tokens.h>

PXR_NAMESPACE_OPEN_SCOPE

TF_DEFINE_PRIVATE_TOKENS(
    _tokens,
    ((sceneIndexPluginName, "HdArnoldPortalLightResolvingSceneIndexPlugin"))
    (portalLight));

// ---------------------------------------------------------------------------

HdArnoldPortalLightResolvingSceneIndex::HdArnoldPortalLightResolvingSceneIndex(
    const HdSceneIndexBaseRefPtr& inputSceneIndex)
    : HdSingleInputFilteringSceneIndexBase(inputSceneIndex)
{
    SetDisplayName("Arnold: portal lights");
}

HdArnoldPortalLightResolvingSceneIndexRefPtr
HdArnoldPortalLightResolvingSceneIndex::New(
    const HdSceneIndexBaseRefPtr& inputSceneIndex)
{
    return TfCreateRefPtr(
        new HdArnoldPortalLightResolvingSceneIndex(inputSceneIndex));
}

// Pass-through: we do not modify prim data, only propagate dirty bits.
HdSceneIndexPrim
HdArnoldPortalLightResolvingSceneIndex::GetPrim(const SdfPath& primPath) const
{
    return _GetInputSceneIndex()->GetPrim(primPath);
}

SdfPathVector
HdArnoldPortalLightResolvingSceneIndex::GetChildPrimPaths(
    const SdfPath& primPath) const
{
    return _GetInputSceneIndex()->GetChildPrimPaths(primPath);
}

// ---------------------------------------------------------------------------

void HdArnoldPortalLightResolvingSceneIndex::_DirtyAllDomes(
    HdSceneIndexObserver::DirtiedPrimEntries& out) const
{
    for (const SdfPath& domePath : _domeLights) {
        out.push_back({domePath, HdLightSchema::GetDefaultLocator()});
    }
}

// ---------------------------------------------------------------------------

void HdArnoldPortalLightResolvingSceneIndex::_PrimsAdded(
    const HdSceneIndexBase& /*sender*/,
    const HdSceneIndexObserver::AddedPrimEntries& entries)
{
    if (!_IsObserved()) {
        return;
    }

    bool portalAdded = false;
    for (const auto& entry : entries) {
        if (entry.primType == HdPrimTypeTokens->domeLight) {
            _domeLights.insert(entry.primPath);
        } else if (entry.primType == _tokens->portalLight) {
            _portalLights.insert(entry.primPath);
            portalAdded = true;
        }
    }

    _SendPrimsAdded(entries);

    // When a new portal appears, all domes should refresh their portals[].
    if (portalAdded && !_domeLights.empty()) {
        HdSceneIndexObserver::DirtiedPrimEntries dirtied;
        _DirtyAllDomes(dirtied);
        _SendPrimsDirtied(dirtied);
    }
}

void HdArnoldPortalLightResolvingSceneIndex::_PrimsRemoved(
    const HdSceneIndexBase& /*sender*/,
    const HdSceneIndexObserver::RemovedPrimEntries& entries)
{
    bool portalRemoved = false;
    for (const auto& entry : entries) {
        if (_domeLights.erase(entry.primPath)) {
            // dome removed — nothing extra to dirty
        } else if (_portalLights.erase(entry.primPath)) {
            portalRemoved = true;
        }
    }

    _SendPrimsRemoved(entries);

    // When a portal disappears, domes must drop it from their portals[].
    if (portalRemoved && !_domeLights.empty()) {
        HdSceneIndexObserver::DirtiedPrimEntries dirtied;
        _DirtyAllDomes(dirtied);
        _SendPrimsDirtied(dirtied);
    }
}

void HdArnoldPortalLightResolvingSceneIndex::_PrimsDirtied(
    const HdSceneIndexBase& /*sender*/,
    const HdSceneIndexObserver::DirtiedPrimEntries& entries)
{
    HdSceneIndexObserver::DirtiedPrimEntries extra;

    for (const auto& entry : entries) {
        // When a portal light param / transform is dirtied, domes need to
        // re-read the portal's Arnold node (the light_portal node may have
        // moved or changed shape).
        if (_portalLights.count(entry.primPath)) {
            _DirtyAllDomes(extra);
            // One portal dirtied is enough to dirty all domes once.
            break;
        }
    }

    _SendPrimsDirtied(entries);
    if (!extra.empty()) {
        _SendPrimsDirtied(extra);
    }
}

// ---------------------------------------------------------------------------

TF_REGISTRY_FUNCTION(TfType)
{
    HdSceneIndexPluginRegistry::Define<
        HdArnoldPortalLightResolvingSceneIndexPlugin>();
}

TF_REGISTRY_FUNCTION(HdSceneIndexPlugin)
{
    // Insert right after meshLightResolving (phase 115), before light-linking
    // (which requires the final light node set to be complete).
    const HdSceneIndexPluginRegistry::InsertionPhase insertionPhase = 116;

    HdSceneIndexPluginRegistry::GetInstance().RegisterSceneIndexForRenderer(
        "Arnold",
        _tokens->sceneIndexPluginName,
        /*inputArgs=*/nullptr,
        insertionPhase,
        HdSceneIndexPluginRegistry::InsertionOrderAtStart);
}

HdSceneIndexBaseRefPtr
HdArnoldPortalLightResolvingSceneIndexPlugin::_AppendSceneIndex(
    const HdSceneIndexBaseRefPtr& inputScene,
    const HdContainerDataSourceHandle& /*inputArgs*/)
{
    return HdArnoldPortalLightResolvingSceneIndex::New(inputScene);
}

PXR_NAMESPACE_CLOSE_SCOPE

#endif // ENABLE_SCENE_INDEX
