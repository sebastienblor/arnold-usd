// Copyright 2019 Autodesk, Inc.
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
#include "registry.h"

#include <ai.h>

#include "../utils/utils.h"
#include "write_arnold_type.h"
#include "write_shader.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

//-*************************************************************************

PXR_NAMESPACE_USING_DIRECTIVE

// For now we're not registering any writer
UsdArnoldWriterRegistry::UsdArnoldWriterRegistry()
{
    // TODO: write to builtin USD types. For now we're creating these nodes as
    // Arnold-Typed primitives at the end of this function
    /*
       // First, let's register all the prim writers that we've hardcoded for
       USD builtin types registerWriter("polymesh", new UsdArnoldWriteMesh());
       registerWriter("curves", new UsdArnoldWriteCurves());
       registerWriter("points", new UsdArnoldWritePoints());
       registerWriter("box", new UsdArnoldWriteCube());
       registerWriter("sphere", new UsdArnoldWritephere());
       registerWriter("cylinder", new UsdArnoldWriteCylinder());
       registerWriter("cone", new UsdArnoldWriteCone());
       registerWriter("nurbs", new UsdArnoldWriteUnsupported("nurbs"));

       // Arnold nodes that can be exported as USD builtin Lights
       registerWriter("distant_light", new UsdArnoldWriteDistantLight());
       registerWriter("skydome_light", new UsdArnoldWriteDomeLight());
       registerWriter("disk_light", new UsdArnoldWriteDiskLight());
       registerWriter("point_light", new UsdArnoldWriteSphereLight());
       registerWriter("quad_light", new UsdArnoldWriteRectLight());
       registerWriter("mesh_light", new UsdArnoldWriteGeometryLight());
    */

    // Now let's iterate over all the arnold classes known at this point
    bool universeCreated = false;
    // If a universe is already active, we can just use it, otherwise we need to
    // call AiBegin.
    //  But if we do so, we'll have to call AiEnd() when we finish
    if (!AiUniverseIsActive()) {
        AiBegin();
        universeCreated = true;
        // FIXME: should we call AiLoadPlugins here ?
    }

    // Iterate over all node types
    AtNodeEntryIterator *nodeEntryIter = AiUniverseGetNodeEntryIterator(AI_NODE_ALL);
    while (!AiNodeEntryIteratorFinished(nodeEntryIter)) {
        AtNodeEntry *nodeEntry = AiNodeEntryIteratorGetNext(nodeEntryIter);
        std::string entryName = AiNodeEntryGetName(nodeEntry);

        // if a primWriter is already registed for this AtNodeEntry (i.e. from
        // the above list), then we should skip it. We want these nodes to be
        // exported as USD native primitive
        if (getPrimWriter(entryName)) {
            continue;
        }

        std::string entryTypeName = AiNodeEntryGetTypeName(nodeEntry);

        std::string usdName = makeCamelCase(entryName);
        if (usdName.length() == 0) {
            continue;
        }
        usdName[0] = toupper(usdName[0]);
        usdName = std::string("Arnold") + usdName;

        /*
                if (entryTypeName == "shader") {
                    // We want to export all shaders as a UsdShader primitive,
                    // and set the shader type in info:id
                    registerWriter(
                        entryName, new UsdArnoldWriteShader(entryName, usdName));
                } else */
        {
            // Generic writer for arnold nodes.
            registerWriter(entryName, new UsdArnoldWriteArnoldType(entryName, usdName, entryTypeName));
        }
    }
    AiNodeEntryIteratorDestroy(nodeEntryIter);

    if (universeCreated) {
        AiEnd();
    }
}
UsdArnoldWriterRegistry::~UsdArnoldWriterRegistry()
{
    // Delete all the prim readers that were registed here
    std::unordered_map<std::string, UsdArnoldPrimWriter *>::iterator it = _writersMap.begin();
    std::unordered_map<std::string, UsdArnoldPrimWriter *>::iterator itEnd = _writersMap.end();

    for (; it != itEnd; ++it) {
        delete it->second;
    }
}

/**
 *   Register a prim writer for a given Arnold node type, overriding the
 *eventual existing one.
 *
 **/
void UsdArnoldWriterRegistry::registerWriter(const std::string &primName, UsdArnoldPrimWriter *primWriter)
{
    std::unordered_map<std::string, UsdArnoldPrimWriter *>::iterator it = _writersMap.find(primName);
    if (it != _writersMap.end()) {
        // we have already registered a reader for this node type, let's delete
        // the existing one and override it
        delete it->second;
    }
    _writersMap[primName] = primWriter;
}
