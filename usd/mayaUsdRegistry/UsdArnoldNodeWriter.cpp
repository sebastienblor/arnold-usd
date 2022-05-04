//
// Copyright 2020 Autodesk
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
//
#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma warning(disable:4244)
#pragma warning(disable:4267)
#pragma warning(disable:4305)
#elif defined(_LINUX) || defined(_DARWIN)
#include <dlfcn.h>
#endif


#include <ai.h>
#include <pxr/base/tf/registryManager.h>
#include <pxr/base/tf/staticTokens.h>
#include <pxr/base/tf/stringUtils.h>
#include <pxr/base/tf/token.h>
#include <pxr/pxr.h>
#include <pxr/usd/usd/stageCache.h>
#include <pxr/usd/usdUtils/stageCache.h>

#include "writer.h"
#include "registry.h"
#include "prim_writer.h"

#include "platform/Platform.h"

#include <iostream>

PXR_NAMESPACE_OPEN_SCOPE
extern "C"
{
    // This function can be invoked from a code that doesn't link against USD. It will provide
    // a cache Id, which allows to retrieve a UsdStage through the UsdStageCache. 
    // We can also be given a specific frame that will determine how the data is authored to USD
    DLLEXPORT void WriteUsdStageCache ( int cacheId, bool defaultFrame, float frame )
    {
        // Get the UsdStageCache, it's common to all libraries linking against the same USD libs
        UsdStageCache &stageCache = UsdUtilsStageCache::Get();
        UsdStageCache::Id id = UsdStageCache::Id::FromLongInt(cacheId);
        // Retrieve the UsdStage associated to this cache ID.
        UsdStageRefPtr stage = (id.IsValid()) ? stageCache.Find(id) : nullptr;
        if (!stage) {
            AiMsgError("[usd] Cache ID not valid %d", cacheId);
            return;
        }
        // Create an Arnold-USD writer, that can write an Arnold univers to a UsdStage
        UsdArnoldWriter writer;
        writer.SetUsdStage(stage); 

        if (!defaultFrame)
            writer.SetFrame(frame);
        writer.Write(nullptr);
    }
}
   
PXR_NAMESPACE_CLOSE_SCOPE
