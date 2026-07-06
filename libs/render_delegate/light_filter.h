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

/// @file render_delegate/light_filter.h
///
/// Hydra Sprim adapter for `UsdLuxLightFilter`-derived prims.
///
/// Equivalent of hdPrman's `HdPrmanLightFilter`. The Sprim owns the Arnold
/// `light_filter`-style node (`barndoor`, `light_blocker`, `gobo`,
/// `passthrough_filter`, etc.) and pulls its parameters from
/// `arnold:` namespaced attributes on the prim. Lights consume the filter
/// node by calling `GetFilterNode()` during their own sync.
///
/// This lives alongside the legacy `ArnoldNodeGraph`-as-filter path so
/// existing Hydra-1 scenes keep working. `HdArnoldLight` checks the new
/// Sprim type first, then falls back to the node-graph lookup.

#pragma once

#include "api.h"

#include <pxr/pxr.h>
#include <pxr/imaging/hd/sprim.h>

#include <ai.h>

#include <mutex>

#include "hdarnold.h"

PXR_NAMESPACE_OPEN_SCOPE

class HdArnoldRenderDelegate;
class HdRenderIndex;

/// Sprim adapter for `UsdLuxLightFilter` prims.
///
/// The Arnold node type is read from the `arnold:node_entry` attribute on
/// the prim (e.g. `"barndoor"`, `"light_blocker"`, `"gobo"`,
/// `"passthrough_filter"`). All Arnold parameters of that node entry are
/// pulled from `arnold:<paramName>` attributes on the prim. Transform is
/// applied if the node entry has a `matrix` parameter (`light_blocker`,
/// `gobo`).
class HdArnoldLightFilter final : public HdSprim {
public:
    enum DirtyBits : HdDirtyBits {
        Clean      = 0,
        DirtyParams    = 1 << 0,
        DirtyTransform = 1 << 1,
        AllDirty       = DirtyParams | DirtyTransform,
    };

    HDARNOLD_API
    HdArnoldLightFilter(HdArnoldRenderDelegate* delegate, const SdfPath& id);

    HDARNOLD_API
    ~HdArnoldLightFilter() override;

    HDARNOLD_API
    void Sync(HdSceneDelegate* sceneDelegate, HdRenderParam* renderParam,
              HdDirtyBits* dirtyBits) override;

    HDARNOLD_API
    HdDirtyBits GetInitialDirtyBitsMask() const override;

    HDARNOLD_API
    void Finalize(HdRenderParam* renderParam) override;

    /// Returns the Arnold light-filter node owned by this Sprim, or null
    /// if the prim has never been synced or has no resolvable Arnold
    /// node type. Called by `HdArnoldLight` while building its
    /// `filters[]` array.
    AtNode* GetFilterNode() const { return _filterNode; }

    /// Convenience accessor that matches the lookup `HdArnoldLight` performs.
    /// Returns the `HdArnoldLightFilter` Sprim registered at `path`, or null.
    HDARNOLD_API
    static HdArnoldLightFilter* GetLightFilter(HdRenderIndex& renderIndex, const SdfPath& path);

private:
    /// Destroys the current Arnold node (if any) and clears the cached
    /// type token. Called when the requested node type changes between
    /// syncs.
    void _DestroyArnoldNode();

    HdArnoldRenderDelegate* _delegate = nullptr;
    AtNode* _filterNode = nullptr;
    AtString _nodeEntryType;  ///< Cached `arnold:node_entry` value.
    std::mutex _syncMutex;
};

PXR_NAMESPACE_CLOSE_SCOPE
