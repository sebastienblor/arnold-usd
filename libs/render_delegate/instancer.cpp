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
#include "instancer.h"

#include <pxr/base/gf/quaternion.h>
#include <pxr/base/gf/rotation.h>
#include <pxr/imaging/hd/sceneDelegate.h>
#include <constant_strings.h>

PXR_NAMESPACE_OPEN_SCOPE

// clang-format off
TF_DEFINE_PRIVATE_TOKENS(_tokens,
    (angularVelocities)
    (instanceTransform)
    (rotate)
    (scale)
    (translate)
    ((instanceTransforms,   "hydra:instanceTransforms"))
    ((instanceRotations,    "hydra:instanceRotations"))
    ((instanceScales,       "hydra:instanceScales"))
    ((instanceTranslations, "hydra:instanceTranslations"))
    ((matte, "arnold:matte"))
    ((visibility, "arnold:visibility"))
    ((visibilityPrefix, "arnold:visibility:"))
    ((visibilityCamera, "arnold:visibility:camera"))
    ((visibilityShadow, "arnold:visibility:shadow"))
    ((visibilityDiffuseTransmit, "arnold:visibility:diffuse_transmit"))
    ((visibilitySpecularTransmit, "arnold:visibility:specular_transmit"))
    ((visibilityDiffuseReflect, "arnold:visibility:diffuse_reflect"))
    ((visibilitySpecularReflect, "arnold:visibility:specular_reflect"))
    ((visibilityVolume, "arnold:visibility:volume"))
    ((visibilitySubsurface, "arnold:visibility:subsurface"))
);

#if PXR_VERSION >= 2311
// see https://github.com/PixarAnimationStudios/OpenUSD/commit/7469ddd8bddd6a82ee5b3e7f8d6d92f9f06b2c51
inline const TfToken & GetInstanceTransformsToken() {return _tokens->instanceTransforms;}
inline const TfToken & GetRotateToken() {return _tokens->instanceRotations;}
inline const TfToken & GetScaleToken() {return _tokens->instanceScales;}
inline const TfToken & GetTranslateToken() {return _tokens->instanceTranslations;}
#else
inline const TfToken & GetInstanceTransformsToken() {return _tokens->instanceTransform;}
inline const TfToken & GetRotateToken() {return _tokens->rotate;}
inline const TfToken & GetScaleToken() {return _tokens->scale;}
inline const TfToken & GetTranslateToken() {return _tokens->translate;}
#endif
// clang-format on

namespace {

template <typename T1, typename T2>
void _AccumulateSampleTimes(const HdArnoldSampledType<T1>& in, HdArnoldSampledType<T2>& out)
{
    if (in.count > out.count) {
        out.Resize(in.count);
        out.times = in.times;
    }
}

} // namespace

HdArnoldInstancer::HdArnoldInstancer(
    HdArnoldRenderDelegate* renderDelegate, HdSceneDelegate* sceneDelegate, const SdfPath& id)
    : HdInstancer(sceneDelegate, id), _delegate(renderDelegate)
{
}

void HdArnoldInstancer::Sync(HdSceneDelegate* sceneDelegate, HdRenderParam* renderParam, HdDirtyBits* dirtyBits)
{
    if (!_delegate->CanUpdateScene())
        return;
 
    _UpdateInstancer(sceneDelegate, dirtyBits);

    if (HdChangeTracker::IsAnyPrimvarDirty(*dirtyBits, GetId())) {
        HdArnoldRenderParam *param = reinterpret_cast<HdArnoldRenderParam*>(renderParam);
        _SyncPrimvars(*dirtyBits, param);
    }
}

// Sample a primvar, check that the keys have the correct number of instances otherwise get only the sample at the keyframe
// We have to do this because hydra SamplePrimvar
template <typename VectorT>

static void SamplePrimvarChecked(
    HdSceneDelegate* delegate, const SdfPath& id, const TfToken& key, const GfVec2f& shutterRange, VectorT& out)
{
    HdArnoldSampledPrimvarType sample;
    SamplePrimvar(delegate, id, key, shutterRange, &sample);
    HdArnoldEnsureSamplesCount(shutterRange, sample);
    if (sample.count >= 1) {
        // We expect SamplePrimvar to return the same number of elements in sampled arrays.
        // However this number might be different than the number of element at the frame.
        const VtValue& firstSample = sample.values[0];
        if (firstSample.IsArrayValued()) {
            VtValue valueAtFrame = delegate->Get(id, key); // value at time 0
            if (firstSample.GetArraySize() != valueAtFrame.GetArraySize()) {
                for (size_t i=0; i < sample.count; ++i) {
                    sample.values[i] = valueAtFrame;
                }
            }
        }
    }

    HdArnoldUnboxResample(sample, shutterRange, out);
}

void HdArnoldInstancer::_SyncPrimvars(HdDirtyBits dirtyBits, HdArnoldRenderParam* renderParam)
{
    auto& changeTracker = GetDelegate()->GetRenderIndex().GetChangeTracker();
    const auto& id = GetId();

    if (!HdChangeTracker::IsAnyPrimvarDirty(dirtyBits, id)) {
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    dirtyBits = changeTracker.GetInstancerDirtyBits(id);

    // We want to read the deformkeys first as it is used to determine the number of sample
    VtValue deformKeysVal = GetDelegate()->Get(id, str::t_deformKeys);
    if (deformKeysVal != VtValue() && deformKeysVal.IsHolding<int>()) {
        _deformKeys = deformKeysVal.Get<int>();
        _deformKeys = _deformKeys > 0 ? _deformKeys : 0; 
    } else {
        _deformKeys = -1; // -1 means there is no value set
    }

    if (HdChangeTracker::IsAnyPrimvarDirty(dirtyBits, id)) {
        for (const auto& primvar : GetDelegate()->GetPrimvarDescriptors(id, HdInterpolationInstance)) {
            if (!HdChangeTracker::IsPrimvarDirty(dirtyBits, id, primvar.name)) {
                continue;
            }
            if (primvar.name == GetInstanceTransformsToken()) {
                SamplePrimvarChecked(GetDelegate(), id, primvar.name, renderParam->GetShutterRange(), _transforms);
            } else if (primvar.name == GetRotateToken()) {
                SamplePrimvarChecked(GetDelegate(), id, primvar.name, renderParam->GetShutterRange(), _rotates);
            } else if (primvar.name == GetScaleToken()) {
                SamplePrimvarChecked(GetDelegate(), id, primvar.name, renderParam->GetShutterRange(), _scales);
            } else if (primvar.name == GetTranslateToken()) {
                SamplePrimvarChecked(GetDelegate(), id, primvar.name, renderParam->GetShutterRange(), _translates);
            } else {
                HdArnoldInsertPrimvar(
                    _primvars, primvar.name, primvar.role, primvar.interpolation, GetDelegate()->Get(id, primvar.name),
                    {});
            }
        }
    }

    // NOTE: it shouldn't be necessary to mark the instancer clean as it is done later on by hydra
    changeTracker.MarkInstancerClean(id);
}

void HdArnoldInstancer::ResampleInstancePrimvars()
{
    const auto& id = GetId();
    std::lock_guard<std::mutex> lock(_mutex);
    // Recompute the sampled primvars only if they were previously sampled
    if (_transforms.count) {
        SamplePrimvarChecked(GetDelegate(), id, GetInstanceTransformsToken(), _samplingInterval, _transforms);
    }
    if (_rotates.count) {
        SamplePrimvarChecked(GetDelegate(), id, GetRotateToken(), _samplingInterval, _rotates);
    }
    if (_scales.count) {
        SamplePrimvarChecked(GetDelegate(), id, GetScaleToken(), _samplingInterval, _scales);
    }
    if (_translates.count) {
        SamplePrimvarChecked(GetDelegate(), id, GetTranslateToken(), _samplingInterval, _translates);
    }
}

void HdArnoldInstancer::CalculateInstanceMatrices(HdArnoldRenderDelegate* renderDelegate, 
    const SdfPath& prototypeId, std::vector<AtNode *> &instancers)
{
    const SdfPath& instancerId = GetId();
    HdArnoldRenderParam * renderParam = reinterpret_cast<HdArnoldRenderParam*>(renderDelegate->GetRenderParam());

    // If the sampling interval has changed we need to resample the translate, orientations and scales
    if (UpdateSamplingInterval(renderParam->GetShutterRange())){
        ResampleInstancePrimvars();
    }

    const auto instanceIndices = GetDelegate()->GetInstanceIndices(instancerId, prototypeId);
    if (instanceIndices.empty()) {
        return;
    }
    HdArnoldSampledMatrixArrayType sampleArray;
    
    const auto numInstances = instanceIndices.size();

    HdArnoldSampledType<GfMatrix4d> instancerTransforms;
    SampleInstancerTransform(GetDelegate(), instancerId, _samplingInterval, &instancerTransforms);
    HdArnoldEnsureSamplesCount(_samplingInterval, instancerTransforms);

    // Similarly to the HdPrman render delegate, we take a look at the sampled values, and take the one with the
    // most samples and use its time range.
    // TODO(pal): Improve this further by using the widest time range and calculate sample count based on that.
    _AccumulateSampleTimes(instancerTransforms, sampleArray);
    {
        // Another mesh can be resampling the instances primvars, we need to lock 
        std::lock_guard<std::mutex> lock(_mutex);
        _AccumulateSampleTimes(_transforms, sampleArray);
        _AccumulateSampleTimes(_translates, sampleArray);
        _AccumulateSampleTimes(_rotates, sampleArray);
        _AccumulateSampleTimes(_scales, sampleArray);
    }


    // By default _deformKeys will take over sample counts
    if (sampleArray.count <= 2 && _deformKeys < 2 && _deformKeys > -1 ) { 
        sampleArray.Resize(1);
        sampleArray.times[0] = 0.0;
    } else if (_deformKeys > 1 /*&& _deformKeys > sampleArray.times.size()*/) {
        const float minTime = *std::min_element(sampleArray.times.begin(), sampleArray.times.end());
        const float maxTime = *std::max_element(sampleArray.times.begin(), sampleArray.times.end());
        sampleArray.Resize(_deformKeys);
        for(int i = 0; i < _deformKeys; ++i) {
            sampleArray.times[i] = minTime + i * (maxTime - minTime) / (_deformKeys - 1);   
        }
    }
    const auto numSamples = sampleArray.count;
    if (numSamples == 0) {
        return;
    }

    const float fps = 1.0f / (reinterpret_cast<HdArnoldRenderParam*>(renderDelegate->GetRenderParam())->GetFPS());
    const float fps2 = fps * fps;
    VtValue velValue = GetDelegate()->Get(instancerId, HdTokens->velocities);
    VtVec3fArray emptyVelocities;
    const VtVec3fArray& velocities =
        velValue.IsHolding<VtVec3fArray>() ? velValue.UncheckedGet<VtVec3fArray>() : emptyVelocities;

    VtValue accelValue = GetDelegate()->Get(instancerId, HdTokens->accelerations);
    VtVec3fArray emptyAccelerations;
    const VtVec3fArray& accelerations =
        accelValue.IsHolding<VtVec3fArray>() ? accelValue.UncheckedGet<VtVec3fArray>() : emptyAccelerations;

    VtValue angularVelocitiesValue = GetDelegate()->Get(instancerId, _tokens->angularVelocities);
    VtVec3fArray emptyAngularVelocities;
    const VtVec3fArray& angularVelocities =
        angularVelocitiesValue.IsHolding<VtVec3fArray>() ? angularVelocitiesValue.UncheckedGet<VtVec3fArray>() : emptyAngularVelocities;

    const bool hasVelocities = !velocities.empty();
    const bool hasAccelerations = !accelerations.empty();
    const bool hasAngularVelocities = !angularVelocities.empty();
    const bool velBlur = hasAccelerations || hasVelocities || hasAngularVelocities;

    // TODO(pal): This resamples the values for all the instance indices, not only the ones belonging to the processed prototype.
    for (auto sample = decltype(numSamples){0}; sample < numSamples; sample += 1) {
        const float t = sampleArray.times[sample];
        const float t2 = t * t;

        sampleArray.values[sample].resize(numInstances);

        GfMatrix4d instancerTransform(1.0);
        if (instancerTransforms.count > 0) {
            instancerTransform = instancerTransforms.Resample(t);
        }
        const VtMatrix4dArray transforms = _transforms.count > 0 ? _transforms.Resample(t) : VtMatrix4dArray();
        const VtVec3fArray translates = _translates.count > 0 ? _translates.Resample(velBlur ? 0.f : t) : VtVec3fArray();
        const VtQuathArray rotates =_rotates.count > 0 ? _rotates.Resample(velBlur ? 0.f : t) : VtQuathArray();
        const VtVec3fArray scales = _scales.count > 0 ? _scales.Resample(velBlur ? 0.f : t) : VtVec3fArray();

        for (auto instance = decltype(numInstances){0}; instance < numInstances; instance += 1) {
            const auto instanceIndex = instanceIndices[instance];
            auto matrix = instancerTransform;
            if (translates.size() > static_cast<size_t>(instanceIndex)) {
                GfMatrix4d m(1.0);
                GfVec3f translate = translates[instanceIndex];
                // For velocity blur, we add the velocity and/or acceleration 
                // to the current position
                if (hasVelocities) {
                    translate += velocities[instanceIndex] * fps * t;
                }
                if (hasAccelerations) {
                    translate += accelerations[instanceIndex] * fps2 * t2 * 0.5f;
                }
                m.SetTranslate(translate);
                matrix = m * matrix;
            }
            if (rotates.size() > static_cast<size_t>(instanceIndex)) {
                GfMatrix4d m(1.0);
                m.SetRotate(rotates[instanceIndex]);
                matrix = m * matrix;
                if (hasAngularVelocities) {
                    GfVec3f angularVelocity = angularVelocities[instanceIndex];
                    GfMatrix4d rotation;
                    rotation.SetRotate(GfRotation(angularVelocity, fps * t * angularVelocity.GetLength()));
                    matrix = rotation * matrix;
                }
            }
            if (scales.size() > static_cast<size_t>(instanceIndex)) {
                GfMatrix4d m(1.0);
                m.SetScale(scales[instanceIndex]);
                matrix = m * matrix;
            }
            if (transforms.size() > static_cast<size_t>(instanceIndex)) {
                matrix = transforms[instanceIndex] * matrix;
            }
            sampleArray.values[sample][instance] = matrix;
        }
    }

    std::stringstream ss;
    ss << prototypeId << "_instancer";
    AtNode *instancerNode = renderDelegate->CreateArnoldNode(str::instancer, AtString(ss.str().c_str()));
    instancers.push_back(instancerNode);

    AiNodeDeclare(instancerNode, str::instance_inherit_xform, "constant array BOOL");
    AiNodeSetArray(instancerNode, str::instance_inherit_xform, AiArray(1, 1, AI_TYPE_BOOLEAN, true));

    if (sampleArray.count == 0 || sampleArray.values.front().empty()) {
        AiNodeResetParameter(instancerNode, str::instance_matrix);
        AiNodeResetParameter(instancerNode, str::node_idxs);
        AiNodeResetParameter(instancerNode, str::instance_visibility);
    } else {
        const auto sampleCount = sampleArray.count;
        const auto instanceCount = sampleArray.values.front().size();
        
        auto* matrixArray = AiArrayAllocate(instanceCount, sampleCount, AI_TYPE_MATRIX);
        auto* nodeIdxsArray = AiArrayAllocate(instanceCount, sampleCount, AI_TYPE_UINT);
        auto* matrices = static_cast<AtMatrix*>(AiArrayMap(matrixArray));
        auto* nodeIdxs = static_cast<uint32_t*>(AiArrayMap(nodeIdxsArray));
        std::fill(nodeIdxs, nodeIdxs + instanceCount*sampleCount, 0);
        AiArrayUnmap(nodeIdxsArray);
        auto convertMatrices = [&](size_t sample) {
            std::transform(
                sampleArray.values[sample].begin(), sampleArray.values[sample].end(),
                matrices + sample * instanceCount,
                [](const GfMatrix4d& in) -> AtMatrix { AtMatrix res; ConvertValue(res, in); return res; });
        };
        convertMatrices(0);
        for (auto sample = decltype(sampleCount){1}; sample < sampleCount; sample += 1) {
            // We check if there is enough data to do the conversion, otherwise we are reusing the first sample.
            if (ARCH_UNLIKELY(sampleArray.values[sample].size() != instanceCount)) {
                std::copy(matrices, matrices + instanceCount, matrices + sample * instanceCount);
            } else {
                convertMatrices(sample);
            }
        }
        auto setMotionParam = [&](const char* name, float value) {
            if (AiNodeLookUpUserParameter(instancerNode, AtString(name)) == nullptr) {
                AiNodeDeclare(instancerNode, AtString(name), str::constantArrayFloat);
            }
            AiNodeSetArray(instancerNode, AtString(name), AiArray(1, 1, AI_TYPE_FLOAT, value));
        };
        if (sampleCount > 1) {
            setMotionParam(str::instance_motion_start, sampleArray.times.front());
            setMotionParam(str::instance_motion_end, sampleArray.times[sampleCount - 1]);
        } else {
            setMotionParam(str::instance_motion_start, 0.0f);
            setMotionParam(str::instance_motion_end, 1.0f);
        }
        AiArrayUnmap(matrixArray);
        AiNodeSetArray(instancerNode, str::instance_matrix, matrixArray);
        AiNodeSetArray(instancerNode, str::node_idxs, nodeIdxsArray);
        SetPrimvars(instancerNode, prototypeId, instanceCount, renderDelegate);
    }
    VtValue matteVal = GetDelegate()->Get(instancerId, _tokens->matte);
    if (!matteVal.IsEmpty())
        AiNodeSetBool(instancerNode, str::matte, VtValueGetBool(matteVal));

    VtValue visVal = GetDelegate()->Get(instancerId, _tokens->visibility);
    if (!visVal.IsEmpty()) {
        AiNodeSetInt(instancerNode, str::visibility, VtValueGetInt(visVal));
    } else {
        bool assignVisibility = false;
        HdArnoldRayFlags rayFlags{AI_RAY_ALL};
        auto applyRayFlags = [&](const TfToken& attr) {
            visVal = GetDelegate()->Get(instancerId, attr);
            if (!visVal.IsEmpty()) {
                assignVisibility = true;
                const char* rayName = attr.GetText() + _tokens->visibilityPrefix.size();
                rayFlags.SetRayFlag(rayName, visVal);
            }
        };
        applyRayFlags(_tokens->visibilityCamera);
        applyRayFlags(_tokens->visibilityShadow);
        applyRayFlags(_tokens->visibilityDiffuseTransmit);
        applyRayFlags(_tokens->visibilitySpecularTransmit);
        applyRayFlags(_tokens->visibilityDiffuseReflect);
        applyRayFlags(_tokens->visibilitySpecularReflect);
        applyRayFlags(_tokens->visibilityVolume);
        applyRayFlags(_tokens->visibilitySubsurface);
        if (assignVisibility)
            AiNodeSetByte(instancerNode, str::visibility, rayFlags.Compose());
    }
    
    const auto parentId = GetParentId();
    if (parentId.IsEmpty()) {
        return;
    }
    auto* parentInstancer = dynamic_cast<HdArnoldInstancer*>(GetDelegate()->GetRenderIndex().GetInstancer(parentId));
    if (ARCH_UNLIKELY(parentInstancer == nullptr)) {
        return;
    }
    parentInstancer->CalculateInstanceMatrices(renderDelegate, instancerId, instancers);
    AiNodeSetByte(instancerNode, str::visibility, 0);
}


void HdArnoldInstancer::SetPrimvars(AtNode* node, const SdfPath& prototypeId, size_t totalInstanceCount, HdArnoldRenderDelegate* renderDelegate )
{

    VtIntArray instanceIndices = GetDelegate()->GetInstanceIndices(GetId(), prototypeId);
    size_t instanceCount = instanceIndices.size();

    if (instanceCount == 0 || instanceCount != totalInstanceCount)
        return;
        
    // We can receive primvars that have visibility components (e.g. visibility:camera, sidedness:reflection, etc...)
    // In that case we need to concatenate all the component values before we compose them into a single 
    // AtByte visibility. Since each instance can have different data, we need to store a HdArnoldRayFlags for
    // each instance
    std::vector<HdArnoldRayFlags> visibilityFlags;
    std::vector<HdArnoldRayFlags> sidednessFlags;
    std::vector<HdArnoldRayFlags> autobumpVisibilityFlags;

    // Loop over this instancer primvars
    for (auto& primvar : _primvars) {
        auto& desc = primvar.second;
        const char* paramName = primvar.first.GetText();
        
        // We don't need to call NeedsUpdate here, as this function is called once per Prototype, not
        // once per instancer.        

        // For arnold primvars, we want to remove the arnold: prefix in the primvar name. This way, 
        // primvars:arnold:matte will end up as instance_matte in the arnold instancer, which is supported.
       
        auto charStartsWithToken = [&](const char *c, const TfToken &t) { return strncmp(c, t.GetText(), t.size()) == 0; };
        if (charStartsWithToken(paramName, str::t_arnold_prefix)) {
            // extract the arnold prefix from the primvar name
            paramName = primvar.first.GetText() + str::t_arnold_prefix.size();    
    
            // Apply each component value to the corresponding ray flag
            auto applyRayFlags = [&](const char *primvar, const TfToken& prefix, const VtValue &value, std::vector<HdArnoldRayFlags> &rayFlags) {
                // check if the primvar name starts with the provided prefix
                if (!charStartsWithToken(primvar, prefix))
                    return false;

                // Store a default HdArnoldRayFalgs, with the proper values
                HdArnoldRayFlags defaultFlags;
                defaultFlags.SetHydraFlag(AI_RAY_ALL);
               
                if (value.IsHolding<VtBoolArray>()) {
                    const VtBoolArray &array = value.UncheckedGet<VtBoolArray>();
                    if (array.size() > rayFlags.size()) {                        
                        rayFlags.resize(array.size(), defaultFlags);
                    }
                    // extract the attribute namespace, to get the ray type component (camera, etc...)
                    const auto* rayName = primvar + prefix.size();                    
                    for (size_t i = 0; i < array.size(); ++i) {
                        // apply the ray flag for each instance
                        rayFlags[i].SetRayFlag(rayName, VtValue(array[i]));
                    }
                }
                return true;
            };

            if (applyRayFlags(paramName, str::t_visibility_prefix, desc.value, visibilityFlags))
                continue;
            if (applyRayFlags(paramName, str::t_sidedness_prefix, desc.value, sidednessFlags))
                continue;
            if (applyRayFlags(paramName, str::t_autobump_visibility_prefix, desc.value, autobumpVisibilityFlags))
                continue;
            
        }
        HdArnoldSetInstancePrimvar(node, TfToken(paramName), desc.role, instanceIndices, desc.value, renderDelegate);
    }
    // Compose the ray flags and get a single AtByte value for each instance. Then make it a single array VtValue
    // and provide it to HdArnoldSetInstancePrimvar
    auto getRayInstanceValue = [&](std::vector<HdArnoldRayFlags> &rayFlags, const TfToken &attrName, AtNode *node,
                VtIntArray &instanceIndices) {
        if (rayFlags.empty())
            return false;

        VtUCharArray valueArray;
        valueArray.reserve(rayFlags.size());
        for (auto &rayFlag : rayFlags) {
            valueArray.push_back(rayFlag.Compose());
        }
        HdArnoldSetInstancePrimvar(node, attrName, HdPrimvarRoleTokens->none, instanceIndices, 
            VtValue(valueArray), renderDelegate);
        return true;    
    };
     
    getRayInstanceValue(visibilityFlags, str::t_visibility, node, instanceIndices);
    getRayInstanceValue(sidednessFlags, str::t_sidedness, node, instanceIndices);
    getRayInstanceValue(autobumpVisibilityFlags, str::t_autobump_visibility, node, instanceIndices);

}

PXR_NAMESPACE_CLOSE_SCOPE
