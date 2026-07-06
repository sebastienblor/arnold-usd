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

/// @file renderSettingsFilteringSIP.h
///
/// Hydra 2 scene-index plugin wrapping `HdsiRenderSettingsFilteringSceneIndex`
/// for the Arnold render delegate. Filters the namespaced settings on the
/// authored render-settings prim down to the `arnold:` / `outputs:arnold:`
/// namespaces, and synthesises a fallback (inactive, empty) render-settings
/// prim when the stage doesn't author one — so the render delegate always
/// sees a valid `HdRenderSettings` prim under Hydra 2.

#pragma once

#include "pxr/pxr.h"

#ifdef ENABLE_SCENE_INDEX

#if PXR_VERSION >= 2308

#include "pxr/imaging/hd/sceneIndexPlugin.h"

PXR_NAMESPACE_OPEN_SCOPE

/// Arnold equivalent of hdPrman's `HdPrman_RenderSettingsFilteringSceneIndexPlugin`.
///
/// Thin wrapper around `HdsiRenderSettingsFilteringSceneIndex` configured for
/// Arnold:
///  - Namespace prefixes: `arnold`, `outputs:arnold`.
///  - Fallback render-settings prim with empty namespaced settings and
///    `active = false`, so `HdArnoldRenderSettings::_Sync` always has a prim
///    to observe even when the stage didn't author one.
class HdArnoldRenderSettingsFilteringSceneIndexPlugin : public HdSceneIndexPlugin
{
public:
    HdArnoldRenderSettingsFilteringSceneIndexPlugin();

protected:
    HdSceneIndexBaseRefPtr _AppendSceneIndex(
        const HdSceneIndexBaseRefPtr &inputScene,
        const HdContainerDataSourceHandle &inputArgs) override;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_VERSION >= 2308

#endif // ENABLE_SCENE_INDEX
