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
/// @file node_registry/tokens.h
///
/// Tokens for nodeRegistryArnold.
#pragma once
#include <pxr/pxr.h>

#include "api.h"

#include <pxr/base/tf/staticTokens.h>

PXR_NAMESPACE_OPEN_SCOPE

// clang-format off
#define NDRARNOLD_TOKENS         \
    (nodeRegistryArnoldFilename)          \
    (nodeRegistryArnoldEnumOptions)       \
    (nodeRegistryArnoldParamType)         \
    (nodeRegistryArnoldArrayElemType)     \
    (nodeRegistryArnoldMetadata)          \
    (nodeRegistryArnoldOutputType)        \
// clang-format on

TF_DECLARE_PUBLIC_TOKENS(NdrArnoldTokens, NDRARNOLD_API, NDRARNOLD_TOKENS);

PXR_NAMESPACE_CLOSE_SCOPE
