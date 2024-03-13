//
// SPDX-License-Identifier: Apache-2.0
//

// Copyright 2022 Autodesk, Inc.
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

#include <ai_nodes.h>

#include <pxr/usd/usd/prim.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "prim_writer.h"

PXR_NAMESPACE_USING_DIRECTIVE

// Register writers for USD builtin geometries
REGISTER_PRIM_WRITER(UsdArnoldWriteMesh);
REGISTER_PRIM_WRITER(UsdArnoldWriteCurves);
REGISTER_PRIM_WRITER(UsdArnoldWritePoints);

// Writer for custom procedurals
class UsdArnoldWriteProceduralCustom : public UsdArnoldPrimWriter {
public:
    UsdArnoldWriteProceduralCustom(const std::string &nodeEntry) : UsdArnoldPrimWriter(), _nodeEntry(nodeEntry) {}

    void Write(const AtNode *node, UsdArnoldWriter &writer) override;

private:
    std::string _nodeEntry;
};

/**
 *   Ginstance nodes require a special treatment, because of the behaviour of
 *   default values. In general we can skip authoring an attribute if the value
 *   is different from default, but that's not the case for instances. Here,
 *   we'll compare the value of the attribute with the corresponding value
 *   for the instanced node. If it is different we will write it, even if the
 *   value is equal to default
 **/
class UsdArnoldWriteGinstance : public UsdArnoldPrimWriter {
public:
    UsdArnoldWriteGinstance() : UsdArnoldPrimWriter() {}

    void Write(const AtNode *node, UsdArnoldWriter &writer) override;

    void _ProcessInstanceAttribute(
        UsdPrim &prim, const AtNode *node, const AtNode *target, const char *attrName, int attrType, UsdArnoldWriter &writer);

};
