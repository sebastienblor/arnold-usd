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

#include "light_filter.h"

#include <pxr/imaging/hd/light.h>
#include <pxr/imaging/hd/sceneDelegate.h>
#include <pxr/imaging/hd/tokens.h>

#include <constant_strings.h>

#include "render_delegate.h"
#include "render_param.h"
#include "utils.h"

PXR_NAMESPACE_OPEN_SCOPE

namespace {

TF_DEFINE_PRIVATE_TOKENS(
    _tokens,
    ((nodeEntry, "arnold:node_entry"))
    (matrix)
);

/// Reads the `arnold:node_entry` value off the prim, returning an empty
/// AtString when the attribute is missing or non-string.
AtString _ReadNodeEntryType(HdSceneDelegate* sceneDelegate, const SdfPath& id)
{
    const VtValue val = sceneDelegate->Get(id, _tokens->nodeEntry);
    if (val.IsEmpty()) {
        return AtString();
    }
    if (val.IsHolding<TfToken>()) {
        return AtString(val.UncheckedGet<TfToken>().GetText());
    }
    if (val.IsHolding<std::string>()) {
        return AtString(val.UncheckedGet<std::string>().c_str());
    }
    return AtString();
}

} // namespace

HdArnoldLightFilter::HdArnoldLightFilter(HdArnoldRenderDelegate* delegate, const SdfPath& id)
    : HdSprim(id), _delegate(delegate)
{
}

HdArnoldLightFilter::~HdArnoldLightFilter() = default;

void HdArnoldLightFilter::Finalize(HdRenderParam* renderParam)
{
    HdArnoldRenderParamInterrupt(renderParam).Interrupt();
    std::lock_guard<std::mutex> lock(_syncMutex);
    _DestroyArnoldNode();
    if (_delegate) {
        _delegate->ClearDependencies(GetId());
    }
}

HdDirtyBits HdArnoldLightFilter::GetInitialDirtyBitsMask() const
{
    return HdLight::DirtyParams | HdLight::DirtyTransform;
}

void HdArnoldLightFilter::_DestroyArnoldNode()
{
    if (_filterNode && _delegate) {
        _delegate->DestroyArnoldNode(_filterNode);
    }
    _filterNode = nullptr;
    _nodeEntryType = AtString();
}

void HdArnoldLightFilter::Sync(HdSceneDelegate* sceneDelegate, HdRenderParam* renderParam,
                               HdDirtyBits* dirtyBits)
{
    if (!_delegate || !_delegate->CanUpdateScene()) {
        *dirtyBits = Clean;
        return;
    }

    HdArnoldRenderParamInterrupt(renderParam).Interrupt();
    std::lock_guard<std::mutex> lock(_syncMutex);

    const SdfPath& id = GetId();
    const bool dirtyParams = (*dirtyBits & HdLight::DirtyParams) != 0;
    const bool dirtyTransform = (*dirtyBits & HdLight::DirtyTransform) != 0;

    // Resolve the requested Arnold node-entry type. Without it, we have no
    // way to know which Arnold light-filter node to instantiate — leave
    // `_filterNode` null so the light skips us, mirroring how missing
    // ArnoldNodeGraph filters are skipped today.
    AtString requestedType = _ReadNodeEntryType(sceneDelegate, id);
    if (requestedType.empty()) {
        _DestroyArnoldNode();
        *dirtyBits = Clean;
        return;
    }

    // (Re)create the Arnold node when the type changes between syncs.
    const bool typeChanged = (requestedType != _nodeEntryType);
    if (typeChanged) {
        _DestroyArnoldNode();
        const AtNodeEntry* entry = AiNodeEntryLookUp(requestedType);
        if (!entry) {
            TF_WARN("HdArnoldLightFilter %s: unknown Arnold node entry '%s'",
                    id.GetText(), requestedType.c_str());
            *dirtyBits = Clean;
            return;
        }
        _filterNode = _delegate->CreateArnoldNode(requestedType, AtString(id.GetText()));
        if (!_filterNode) {
            TF_WARN("HdArnoldLightFilter %s: failed to create '%s'",
                    id.GetText(), requestedType.c_str());
            *dirtyBits = Clean;
            return;
        }
        _nodeEntryType = requestedType;
    }

    if (!_filterNode) {
        *dirtyBits = Clean;
        return;
    }

    const AtNodeEntry* nodeEntry = AiNodeGetNodeEntry(_filterNode);

    // Pull `arnold:<paramName>` for every parameter of the node entry.
    // Forcing a full re-read on type-change as well, so a freshly created
    // node lands in a fully configured state.
    if (dirtyParams || typeChanged) {
        AtParamIterator* paramIter = AiNodeEntryGetParamIterator(nodeEntry);
        while (!AiParamIteratorFinished(paramIter)) {
            const AtParamEntry* param = AiParamIteratorGetNext(paramIter);
            const AtString paramName = AiParamGetName(param);
            // `matrix` is sourced from the prim transform, not an attribute.
            if (paramName == str::matrix) {
                continue;
            }
            const TfToken attrToken{TfStringPrintf("arnold:%s", paramName.c_str())};
            const VtValue val = sceneDelegate->Get(id, attrToken);
            if (val.IsEmpty()) {
                continue;
            }
            HdArnoldSetParameter(_filterNode, param, val, _delegate);
        }
        AiParamIteratorDestroy(paramIter);
    }

    // Apply transform when the node has a `matrix` parameter (e.g.
    // `light_blocker`, `gobo`). Other filter types ignore transform.
    if ((dirtyTransform || typeChanged) &&
        AiNodeEntryLookUpParameter(nodeEntry, str::matrix) != nullptr) {
        const GfMatrix4d xform = sceneDelegate->GetTransform(id);
        AtMatrix arnoldMatrix;
        ConvertValue(arnoldMatrix, xform);
        AiNodeSetMatrix(_filterNode, str::matrix, arnoldMatrix);
    }

    *dirtyBits = Clean;
}

HdArnoldLightFilter*
HdArnoldLightFilter::GetLightFilter(HdRenderIndex& renderIndex, const SdfPath& path)
{
    return dynamic_cast<HdArnoldLightFilter*>(
        renderIndex.GetSprim(HdPrimTypeTokens->lightFilter, path));
}

PXR_NAMESPACE_CLOSE_SCOPE
