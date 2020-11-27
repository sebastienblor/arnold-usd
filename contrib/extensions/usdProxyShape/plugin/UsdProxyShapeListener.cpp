//
// Copyright 2018 Pixar
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

#ifdef ENABLE_USD
#include "UsdProxyShapeListener.h"

#include <pxr/base/tf/notice.h>
#include <pxr/base/tf/weakBase.h>
#include <pxr/usd/usd/notice.h>
#include <pxr/usd/usd/stage.h>

PXR_NAMESPACE_OPEN_SCOPE

/* virtual */
CUsdMtoAListener::~CUsdMtoAListener()
{
    if (_stageContentsChangedKey.IsValid()) {
        TfNotice::Revoke(_stageContentsChangedKey);
    }
    if (_stageObjectsChangedKey.IsValid()) {
        TfNotice::Revoke(_stageObjectsChangedKey);
    }
}

void CUsdMtoAListener::SetStage(const UsdStageWeakPtr& stage)
{
    _stage = stage;

    _UpdateStageContentsChangedRegistration();
}

void CUsdMtoAListener::SetStageContentsChangedCallback(
    const StageContentsChangedCallback& callback)
{
    _stageContentsChangedCallback = callback;

    _UpdateStageContentsChangedRegistration();
}

void CUsdMtoAListener::SetStageObjectsChangedCallback(
    const StageObjectsChangedCallback& callback)
{
    _stageObjectsChangedCallback = callback;

    _UpdateStageContentsChangedRegistration();
}

void CUsdMtoAListener::_UpdateStageContentsChangedRegistration()
{
    if (_stage && _stageContentsChangedCallback) {
        // Register for notices if we're not already listening.
        if (!_stageContentsChangedKey.IsValid()) {
            _stageContentsChangedKey = TfNotice::Register(
                TfCreateWeakPtr(this), &CUsdMtoAListener::_OnStageContentsChanged);
        }
    } else {
        // Either the stage or the callback is invalid, so stop listening for
        // notices.
        if (_stageContentsChangedKey.IsValid()) {
            TfNotice::Revoke(_stageContentsChangedKey);
        }
    }

    if (_stage && _stageObjectsChangedCallback) {
        // Register for notices if we're not already listening.
        if (!_stageObjectsChangedKey.IsValid()) {
            _stageObjectsChangedKey = TfNotice::Register(
                TfCreateWeakPtr(this), &CUsdMtoAListener::_OnStageObjectsChanged, _stage);
        }
    } else {
        // Either the stage or the callback is invalid, so stop listening for
        // notices.
        if (_stageObjectsChangedKey.IsValid()) {
            TfNotice::Revoke(_stageObjectsChangedKey);
        }
    }
}

void CUsdMtoAListener::_OnStageContentsChanged(
    const UsdNotice::StageContentsChanged& notice) const
{
    if (notice.GetStage() == _stage && _stageContentsChangedCallback) {
        _stageContentsChangedCallback(notice);
    }
}

void CUsdMtoAListener::_OnStageObjectsChanged(
    const UsdNotice::ObjectsChanged& notice,
    const UsdStageWeakPtr&           sender) const
{
    if (notice.GetStage() == _stage && _stageObjectsChangedCallback) {
        _stageObjectsChangedCallback(notice);
    }
}

PXR_NAMESPACE_CLOSE_SCOPE

#endif