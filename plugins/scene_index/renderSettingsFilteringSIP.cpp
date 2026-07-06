//
// SPDX-License-Identifier: Apache-2.0
//

// Copyright 2026 Autodesk, Inc.
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

#include "renderSettingsFilteringSIP.h"

#ifdef ENABLE_SCENE_INDEX

#if PXR_VERSION >= 2308

#include <pxr/base/tf/staticTokens.h>
#include <pxr/base/vt/array.h>
#include <pxr/imaging/hd/renderSettingsSchema.h>
#include <pxr/imaging/hd/retainedDataSource.h>
#include <pxr/imaging/hd/sceneIndexPluginRegistry.h>
#include <pxr/imaging/hdsi/renderSettingsFilteringSceneIndex.h>

PXR_NAMESPACE_OPEN_SCOPE

TF_DEFINE_PRIVATE_TOKENS(
    _tokens,
    ((sceneIndexPluginName, "HdArnoldRenderSettingsFilteringSceneIndexPlugin"))
);

// Namespaces of settings that the filter passes through onto the render-
// settings prim. Mirrors hdPrman's `ri` / `outputs:ri`, swapped for the
// Arnold equivalents that `HdArnoldRenderSettings::_UpdateArnoldOptions` and
// `_UpdateRenderProducts` consume.
TF_DEFINE_PRIVATE_TOKENS(
    _namespaceTokens,
    (arnold)
    ((outputsArnold, "outputs:arnold"))
);

namespace {

/// Build an empty `HdRenderSettings` prim data source. The fallback prim is
/// inactive — its only purpose is to guarantee that a render-settings prim
/// exists in the scene so the rest of the Arnold delegate's render-settings
/// machinery has something to observe.
HdContainerDataSourceHandle
_BuildFallbackRenderSettingsSchemaDataSource()
{
    return HdRenderSettingsSchema::Builder()
        .SetNamespacedSettings(HdRetainedContainerDataSource::New())
        .SetActive(HdRetainedTypedSampledDataSource<bool>::New(false))
        .Build();
}

HdContainerDataSourceHandle
_BuildFallbackRenderSettingsPrimDataSource()
{
    return HdRetainedContainerDataSource::New(
        HdRenderSettingsSchemaTokens->renderSettings,
        _BuildFallbackRenderSettingsSchemaDataSource());
}

} // namespace

TF_REGISTRY_FUNCTION(TfType)
{
    HdSceneIndexPluginRegistry::Define<HdArnoldRenderSettingsFilteringSceneIndexPlugin>();
}

TF_REGISTRY_FUNCTION(HdSceneIndexPlugin)
{
    // Match hdPrman's insertion phase / order so chains stay comparable
    // across renderers.
    const HdSceneIndexPluginRegistry::InsertionPhase insertionPhase = 1;

    HdSceneIndexPluginRegistry::GetInstance().RegisterSceneIndexForRenderer(
        "Arnold",
        _tokens->sceneIndexPluginName,
        /*inputArgs=*/nullptr,
        insertionPhase,
        HdSceneIndexPluginRegistry::InsertionOrderAtStart);
}

HdArnoldRenderSettingsFilteringSceneIndexPlugin::
HdArnoldRenderSettingsFilteringSceneIndexPlugin() = default;

HdSceneIndexBaseRefPtr
HdArnoldRenderSettingsFilteringSceneIndexPlugin::_AppendSceneIndex(
    const HdSceneIndexBaseRefPtr &inputScene,
    const HdContainerDataSourceHandle &inputArgs)
{
    TF_UNUSED(inputArgs);

    const VtArray<TfToken> namespacePrefixes = {
        _namespaceTokens->arnold,
        _namespaceTokens->outputsArnold,
    };

    const HdContainerDataSourceHandle localInputArgs =
        HdRetainedContainerDataSource::New(
            HdsiRenderSettingsFilteringSceneIndexTokens->namespacePrefixes,
            HdRetainedTypedSampledDataSource<VtArray<TfToken>>::New(namespacePrefixes)
#if PXR_VERSION >= 2311
            ,
            HdsiRenderSettingsFilteringSceneIndexTokens->fallbackPrimDs,
            _BuildFallbackRenderSettingsPrimDataSource()
#endif
        );

    return HdsiRenderSettingsFilteringSceneIndex::New(inputScene, localInputArgs);
}

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_VERSION >= 2308

#endif // ENABLE_SCENE_INDEX
