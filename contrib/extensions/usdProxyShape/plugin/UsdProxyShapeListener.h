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
#ifndef MTOA_USDPROXYSHAPE_LISTENER_H
#define MTOA_USDPROXYSHAPE_LISTENER_H

#ifdef ENABLE_USD

#include <pxr/base/tf/notice.h>
#include <pxr/base/tf/weakBase.h>
#include <pxr/pxr.h>
#include <pxr/usd/usd/notice.h>
#include <pxr/usd/usd/stage.h>

#include <functional>

PXR_NAMESPACE_OPEN_SCOPE

class CUsdMtoAListener : public TfWeakBase
{
public:
    CUsdMtoAListener() = default;

    virtual ~CUsdMtoAListener();

    /// Set the USD stage for which this instance will listen for notices.
    void SetStage(const UsdStageWeakPtr& stage);

    /// Callback type for stage notices.
    using StageContentsChangedCallback
        = std::function<void(const UsdNotice::StageContentsChanged& notice)>;
    using StageObjectsChangedCallback
        = std::function<void(const UsdNotice::ObjectsChanged& notice)>;

    /// Sets the callback to be invoked when the listener receives a
    /// StageContentsChanged notice.
    
    void SetStageContentsChangedCallback(const StageContentsChangedCallback& callback);

    /// Sets the callback to be invoked when the listener receives a
    /// ObjectsChanged notice.
    
    void SetStageObjectsChangedCallback(const StageObjectsChangedCallback& callback);

private:
    CUsdMtoAListener(const CUsdMtoAListener&) = delete;
    CUsdMtoAListener& operator=(const CUsdMtoAListener&) = delete;

    UsdStageWeakPtr _stage;

    /// Handling for UsdNotices
    TfNotice::Key                _stageContentsChangedKey {};
    StageContentsChangedCallback _stageContentsChangedCallback {};

    TfNotice::Key               _stageObjectsChangedKey {};
    StageObjectsChangedCallback _stageObjectsChangedCallback {};

    void _UpdateStageContentsChangedRegistration();
    void _OnStageContentsChanged(const UsdNotice::StageContentsChanged& notice) const;
    void _OnStageObjectsChanged(
        const UsdNotice::ObjectsChanged& notice,
        const UsdStageWeakPtr&           sender) const;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif

#endif