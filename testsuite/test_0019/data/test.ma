//Maya ASCII 2011 scene
//Name: test.ma
//Last modified: Fri, Jun 10, 2011 10:08:28 PM
//Codeset: 1252
requires maya "2011";
requires "mtoa" "0.7.0";
currentUnit -linear centimeter -angle degree -time film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2011";
fileInfo "version" "2011 x64";
fileInfo "cutIdentifier" "201009060330-781623";
fileInfo "osv" "Microsoft Windows 7 Business Edition, 64-bit Windows 7  (Build 7600)\n";
createNode transform -shared -name "persp";
	setAttr ".visibility" no;
	setAttr ".translate" -type "double3" 32.364702040154384 21.773633774227896 31.81567579885289 ;
	setAttr ".rotate" -type "double3" -27.938352729602403 44.200000000000038 0 ;
createNode camera -shared -name "perspShape" -parent "persp";
	addAttr -cachedInternally true -keyable true -shortName "ai_filtermap" -longName "aiFiltermap" 
		-attributeType "message";
	addAttr -cachedInternally true -keyable true -shortName "ai_horizontal_fov" -longName "aiHorizontalFov" 
		-defaultValue 60 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_vertical_fov" -longName "aiVerticalFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 180 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_projective" -longName "aiProjective" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_focus_distance" -longName "aiFocusDistance" 
		-defaultValue 1 -minValue 0 -maxValue 1000000000 -softMinValue 0 -softMaxValue 200 
		-attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_size" -longName "aiApertureSize" 
		-minValue 0 -maxValue 20 -softMinValue 0 -softMaxValue 1 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_blades" -longName "aiApertureBlades" 
		-minValue 3 -maxValue 40 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_blade_curvature" 
		-longName "aiApertureBladeCurvature" -minValue -20 -maxValue 20 -softMinValue -5 
		-softMaxValue 5 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_rotation" -longName "aiApertureRotation" 
		-minValue 0 -maxValue 360 -softMinValue 0 -softMaxValue 50 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_edof" -longName "aiEnableDOF" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_fov" -longName "aiFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_autocrop" -longName "aiAutocrop" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -usedAsColor -keyable true -shortName "ai_uv_remap" 
		-longName "aiUvRemap" -attributeType "float3" -numberOfChildren 3;
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapr" -longName "aiUvRemapR" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapg" -longName "aiUvRemapG" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapb" -longName "aiUvRemapB" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -hidden true -keyable true -shortName "ai_uv_remapa" 
		-longName "aiUvRemapA" -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_translator" -longName "aiTranslator" 
		-dataType "string";
	setAttr -keyable off ".visibility" no;
	setAttr ".overscan" 1.3;
	setAttr ".focalLength" 34.999999999999986;
	setAttr ".centerOfInterest" 50.303220597829224;
	setAttr ".imageName" -type "string" "persp";
	setAttr ".depthName" -type "string" "persp_depth";
	setAttr ".maskName" -type "string" "persp_mask";
	setAttr ".tumblePivot" -type "double3" 5.9604644775390625e-008 0 2.9802322387695313e-008 ;
	setAttr ".homeCommand" -type "string" "viewSet -p %camera";
	setAttr ".displayResolution" yes;
	setAttr -keyable on ".aiTranslator" -type "string" "perspective";
createNode transform -shared -name "top";
	setAttr ".visibility" no;
	setAttr ".translate" -type "double3" 0 1000.1 0 ;
	setAttr ".rotate" -type "double3" -89.999999999999986 0 0 ;
createNode camera -shared -name "topShape" -parent "top";
	addAttr -cachedInternally true -keyable true -shortName "ai_filtermap" -longName "aiFiltermap" 
		-attributeType "message";
	addAttr -cachedInternally true -keyable true -shortName "ai_horizontal_fov" -longName "aiHorizontalFov" 
		-defaultValue 60 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_vertical_fov" -longName "aiVerticalFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 180 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_projective" -longName "aiProjective" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_focus_distance" -longName "aiFocusDistance" 
		-defaultValue 1 -minValue 0 -maxValue 1000000000 -softMinValue 0 -softMaxValue 200 
		-attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_size" -longName "aiApertureSize" 
		-minValue 0 -maxValue 20 -softMinValue 0 -softMaxValue 1 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_blades" -longName "aiApertureBlades" 
		-minValue 3 -maxValue 40 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_blade_curvature" 
		-longName "aiApertureBladeCurvature" -minValue -20 -maxValue 20 -softMinValue -5 
		-softMaxValue 5 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_rotation" -longName "aiApertureRotation" 
		-minValue 0 -maxValue 360 -softMinValue 0 -softMaxValue 50 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_edof" -longName "aiEnableDOF" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_fov" -longName "aiFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_autocrop" -longName "aiAutocrop" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -usedAsColor -keyable true -shortName "ai_uv_remap" 
		-longName "aiUvRemap" -attributeType "float3" -numberOfChildren 3;
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapr" -longName "aiUvRemapR" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapg" -longName "aiUvRemapG" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapb" -longName "aiUvRemapB" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -hidden true -keyable true -shortName "ai_uv_remapa" 
		-longName "aiUvRemapA" -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_translator" -longName "aiTranslator" 
		-dataType "string";
	setAttr -keyable off ".visibility" no;
	setAttr ".renderable" no;
	setAttr ".centerOfInterest" 1000.1;
	setAttr ".orthographicWidth" 30;
	setAttr ".imageName" -type "string" "top";
	setAttr ".depthName" -type "string" "top_depth";
	setAttr ".maskName" -type "string" "top_mask";
	setAttr ".homeCommand" -type "string" "viewSet -t %camera";
	setAttr ".orthographic" yes;
	setAttr -keyable on ".aiTranslator" -type "string" "orthographic";
createNode transform -shared -name "front";
	setAttr ".visibility" no;
	setAttr ".translate" -type "double3" 0 0 1000.1 ;
createNode camera -shared -name "frontShape" -parent "front";
	addAttr -cachedInternally true -keyable true -shortName "ai_filtermap" -longName "aiFiltermap" 
		-attributeType "message";
	addAttr -cachedInternally true -keyable true -shortName "ai_horizontal_fov" -longName "aiHorizontalFov" 
		-defaultValue 60 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_vertical_fov" -longName "aiVerticalFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 180 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_projective" -longName "aiProjective" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_focus_distance" -longName "aiFocusDistance" 
		-defaultValue 1 -minValue 0 -maxValue 1000000000 -softMinValue 0 -softMaxValue 200 
		-attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_size" -longName "aiApertureSize" 
		-minValue 0 -maxValue 20 -softMinValue 0 -softMaxValue 1 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_blades" -longName "aiApertureBlades" 
		-minValue 3 -maxValue 40 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_blade_curvature" 
		-longName "aiApertureBladeCurvature" -minValue -20 -maxValue 20 -softMinValue -5 
		-softMaxValue 5 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_rotation" -longName "aiApertureRotation" 
		-minValue 0 -maxValue 360 -softMinValue 0 -softMaxValue 50 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_edof" -longName "aiEnableDOF" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_fov" -longName "aiFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_autocrop" -longName "aiAutocrop" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -usedAsColor -keyable true -shortName "ai_uv_remap" 
		-longName "aiUvRemap" -attributeType "float3" -numberOfChildren 3;
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapr" -longName "aiUvRemapR" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapg" -longName "aiUvRemapG" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapb" -longName "aiUvRemapB" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -hidden true -keyable true -shortName "ai_uv_remapa" 
		-longName "aiUvRemapA" -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_translator" -longName "aiTranslator" 
		-dataType "string";
	setAttr -keyable off ".visibility" no;
	setAttr ".renderable" no;
	setAttr ".centerOfInterest" 1000.1;
	setAttr ".orthographicWidth" 30;
	setAttr ".imageName" -type "string" "front";
	setAttr ".depthName" -type "string" "front_depth";
	setAttr ".maskName" -type "string" "front_mask";
	setAttr ".homeCommand" -type "string" "viewSet -f %camera";
	setAttr ".orthographic" yes;
	setAttr -keyable on ".aiTranslator" -type "string" "orthographic";
createNode transform -shared -name "side";
	setAttr ".visibility" no;
	setAttr ".translate" -type "double3" 1000.1 0 0 ;
	setAttr ".rotate" -type "double3" 0 89.999999999999986 0 ;
createNode camera -shared -name "sideShape" -parent "side";
	addAttr -cachedInternally true -keyable true -shortName "ai_filtermap" -longName "aiFiltermap" 
		-attributeType "message";
	addAttr -cachedInternally true -keyable true -shortName "ai_horizontal_fov" -longName "aiHorizontalFov" 
		-defaultValue 60 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_vertical_fov" -longName "aiVerticalFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 180 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_projective" -longName "aiProjective" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_focus_distance" -longName "aiFocusDistance" 
		-defaultValue 1 -minValue 0 -maxValue 1000000000 -softMinValue 0 -softMaxValue 200 
		-attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_size" -longName "aiApertureSize" 
		-minValue 0 -maxValue 20 -softMinValue 0 -softMaxValue 1 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_blades" -longName "aiApertureBlades" 
		-minValue 3 -maxValue 40 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_blade_curvature" 
		-longName "aiApertureBladeCurvature" -minValue -20 -maxValue 20 -softMinValue -5 
		-softMaxValue 5 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_rotation" -longName "aiApertureRotation" 
		-minValue 0 -maxValue 360 -softMinValue 0 -softMaxValue 50 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_edof" -longName "aiEnableDOF" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_fov" -longName "aiFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_autocrop" -longName "aiAutocrop" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -usedAsColor -keyable true -shortName "ai_uv_remap" 
		-longName "aiUvRemap" -attributeType "float3" -numberOfChildren 3;
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapr" -longName "aiUvRemapR" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapg" -longName "aiUvRemapG" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapb" -longName "aiUvRemapB" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -hidden true -keyable true -shortName "ai_uv_remapa" 
		-longName "aiUvRemapA" -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_translator" -longName "aiTranslator" 
		-dataType "string";
	setAttr -keyable off ".visibility" no;
	setAttr ".renderable" no;
	setAttr ".centerOfInterest" 1000.1;
	setAttr ".orthographicWidth" 30;
	setAttr ".imageName" -type "string" "side";
	setAttr ".depthName" -type "string" "side_depth";
	setAttr ".maskName" -type "string" "side_mask";
	setAttr ".homeCommand" -type "string" "viewSet -s %camera";
	setAttr ".orthographic" yes;
	setAttr -keyable on ".aiTranslator" -type "string" "orthographic";
createNode transform -name "ArnoldStandIn";
createNode aiStandIn -name "ArnoldStandInShape" -parent "ArnoldStandIn";
	setAttr -keyable off ".visibility";
	setAttr ".collisionOffsetVelocityMultiplier[0]"  0 1 1;
	setAttr ".collisionDepthVelocityMultiplier[0]"  0 1 1;
	setAttr ".dso" -type "string" "soccer.so";
	setAttr ".MinBoundingBox" -type "float3" -12 -12 -12 ;
	setAttr ".MaxBoundingBox" -type "float3" 12 12 12 ;
createNode transform -name "pointLight1";
	setAttr ".translate" -type "double3" 24.793086314131887 19.626588310430417 28.776564590043222 ;
createNode pointLight -name "pointLightShape1" -parent "pointLight1";
	addAttr -cachedInternally true -keyable true -shortName "ai_cast_shadows" -longName "aiCastShadows" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_exposure" -longName "aiExposure" 
		-minValue 0 -softMaxValue 10 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_radius" -longName "aiRadius" 
		-minValue 0 -softMaxValue 10 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_samples" -longName "aiSamples" 
		-defaultValue 1 -minValue 0 -maxValue 100 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "ai_mis" -longName "aiMis" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_normalize" -longName "aiNormalize" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_bounce_factor" -longName "aiBounceFactor" 
		-defaultValue 1 -minValue 0 -softMaxValue 20 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_bounces" -longName "aiBounces" 
		-defaultValue 999 -minValue 0 -maxValue 10000 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "ai_oss" -longName "aiOverrideSssSamples" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_sss_samples" -longName "aiSssSamples" 
		-defaultValue 1 -minValue 0 -maxValue 100 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "ai_affect_volumetrics" 
		-longName "aiAffectVolumetrics" -defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_cast_volumetric_shadows" 
		-longName "aiCastVolumetricShadows" -defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	setAttr -keyable off ".visibility";
	setAttr ".useOnlySingleDmap" no;
createNode transform -name "pointLight2";
	setAttr ".translate" -type "double3" 31.066538685512125 19.626588310430417 27.063197844091434 ;
createNode pointLight -name "pointLightShape2" -parent "pointLight2";
	addAttr -cachedInternally true -keyable true -shortName "ai_cast_shadows" -longName "aiCastShadows" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_exposure" -longName "aiExposure" 
		-minValue 0 -softMaxValue 10 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_radius" -longName "aiRadius" 
		-minValue 0 -softMaxValue 10 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_samples" -longName "aiSamples" 
		-defaultValue 1 -minValue 0 -maxValue 100 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "ai_mis" -longName "aiMis" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_normalize" -longName "aiNormalize" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_bounce_factor" -longName "aiBounceFactor" 
		-defaultValue 1 -minValue 0 -softMaxValue 20 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_bounces" -longName "aiBounces" 
		-defaultValue 999 -minValue 0 -maxValue 10000 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "ai_oss" -longName "aiOverrideSssSamples" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_sss_samples" -longName "aiSssSamples" 
		-defaultValue 1 -minValue 0 -maxValue 100 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "ai_affect_volumetrics" 
		-longName "aiAffectVolumetrics" -defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_cast_volumetric_shadows" 
		-longName "aiCastVolumetricShadows" -defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	setAttr -keyable off ".visibility";
	setAttr ".useOnlySingleDmap" no;
createNode lightLinker -shared -name "lightLinker1";
	setAttr -size 5 ".link";
	setAttr -size 5 ".shadowLink";
createNode displayLayerManager -name "layerManager";
createNode displayLayer -name "defaultLayer";
createNode renderLayerManager -name "renderLayerManager";
createNode renderLayer -name "defaultRenderLayer";
	setAttr ".global" yes;
createNode aiOptions -shared -name "defaultArnoldRenderOptions";
	setAttr ".arnoldRenderImageFormat" 1;
	setAttr ".output_ass_mask" 255;
createNode shadingEngine -name "aiStandard1SG";
	setAttr ".isHistoricallyInteresting" 0;
	setAttr ".renderableOnlySet" yes;
createNode materialInfo -name "materialInfo1";
createNode shadingEngine -name "aiStandard2SG";
	setAttr ".isHistoricallyInteresting" 0;
	setAttr ".renderableOnlySet" yes;
createNode materialInfo -name "materialInfo2";
createNode script -name "sceneConfigurationScriptNode";
	setAttr ".before" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".scriptType" 6;
createNode aiStandard -name "aiStandard3";
createNode shadingEngine -name "aiStandard3SG";
	setAttr ".isHistoricallyInteresting" 0;
	setAttr ".renderableOnlySet" yes;
createNode materialInfo -name "materialInfo3";
createNode file -name "file1";
	setAttr ".fileTextureName" -type "string" "ball.tif";
createNode place2dTexture -name "place2dTexture1";
select -noExpand :time1;
	setAttr ".outTime" 3;
	setAttr ".unwarpedTime" 3;
select -noExpand :renderPartition;
	setAttr -size 5 ".sets";
select -noExpand :initialShadingGroup;
	setAttr ".renderableOnlySet" yes;
select -noExpand :initialParticleSE;
	setAttr ".renderableOnlySet" yes;
select -noExpand :defaultShaderList1;
	setAttr -size 3 ".shaders";
select -noExpand :defaultTextureList1;
select -noExpand :lightList1;
	setAttr -size 2 ".lights";
select -noExpand :postProcessList1;
	setAttr -size 2 ".postProcesses";
select -noExpand :defaultRenderUtilityList1;
select -noExpand :renderGlobalsList1;
select -noExpand :defaultRenderGlobals;
	setAttr ".currentRenderer" -type "string" "arnold";
	setAttr ".imageFormat" 0;
	setAttr ".imageFilePrefix" -type "string" "testrender";
select -noExpand :defaultResolution;
	setAttr ".width" 160;
	setAttr ".height" 120;
	setAttr ".pixelAspect" 1;
	setAttr ".deviceAspectRatio" 1.3333333730697632;
select -noExpand :defaultLightSet;
	setAttr -size 2 ".dagSetMembers";
select -noExpand :hardwareRenderGlobals;
	setAttr ".colorTextureResolution" 256;
	setAttr ".bumpTextureResolution" 512;
select -noExpand :defaultHardwareRenderGlobals;
	setAttr ".filename" -type "string" "im";
	setAttr ".resolution" -type "string" "ntsc_4d 646 485 1.333";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard2SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard3SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard3SG.message" ":defaultLightSet.message";
connectAttr "layerManager.displayLayerId[0]" "defaultLayer.identification";
connectAttr "renderLayerManager.renderLayerId[0]" "defaultRenderLayer.identification"
		;
connectAttr "aiStandard1SG.message" "materialInfo1.shadingGroup";
connectAttr "aiStandard2SG.message" "materialInfo2.shadingGroup";
connectAttr "file1.outColor" "aiStandard3.KdColor";
connectAttr "aiStandard3.outColor" "aiStandard3SG.surfaceShader";
connectAttr "ArnoldStandInShape.instObjGroups" "aiStandard3SG.dagSetMembers" -nextAvailable
		;
connectAttr "aiStandard3SG.message" "materialInfo3.shadingGroup";
connectAttr "aiStandard3.message" "materialInfo3.material";
connectAttr "aiStandard3.message" "materialInfo3.texture" -nextAvailable;
connectAttr "place2dTexture1.coverage" "file1.coverage";
connectAttr "place2dTexture1.translateFrame" "file1.translateFrame";
connectAttr "place2dTexture1.rotateFrame" "file1.rotateFrame";
connectAttr "place2dTexture1.mirrorU" "file1.mirrorU";
connectAttr "place2dTexture1.mirrorV" "file1.mirrorV";
connectAttr "place2dTexture1.stagger" "file1.stagger";
connectAttr "place2dTexture1.wrapU" "file1.wrapU";
connectAttr "place2dTexture1.wrapV" "file1.wrapV";
connectAttr "place2dTexture1.repeatUV" "file1.repeatUV";
connectAttr "place2dTexture1.offset" "file1.offset";
connectAttr "place2dTexture1.rotateUV" "file1.rotateUV";
connectAttr "place2dTexture1.noiseUV" "file1.noiseUV";
connectAttr "place2dTexture1.vertexUvOne" "file1.vertexUvOne";
connectAttr "place2dTexture1.vertexUvTwo" "file1.vertexUvTwo";
connectAttr "place2dTexture1.vertexUvThree" "file1.vertexUvThree";
connectAttr "place2dTexture1.vertexCameraOne" "file1.vertexCameraOne";
connectAttr "place2dTexture1.outUV" "file1.uvCoord";
connectAttr "place2dTexture1.outUvFilterSize" "file1.uvFilterSize";
connectAttr "aiStandard1SG.partition" ":renderPartition.sets" -nextAvailable;
connectAttr "aiStandard2SG.partition" ":renderPartition.sets" -nextAvailable;
connectAttr "aiStandard3SG.partition" ":renderPartition.sets" -nextAvailable;
connectAttr "aiStandard3.message" ":defaultShaderList1.shaders" -nextAvailable;
connectAttr "file1.message" ":defaultTextureList1.textures" -nextAvailable;
connectAttr "pointLightShape1.lightData" ":lightList1.lights" -nextAvailable;
connectAttr "pointLightShape2.lightData" ":lightList1.lights" -nextAvailable;
connectAttr "place2dTexture1.message" ":defaultRenderUtilityList1.utilities" -nextAvailable
		;
connectAttr "pointLight1.instObjGroups" ":defaultLightSet.dagSetMembers" -nextAvailable
		;
connectAttr "pointLight2.instObjGroups" ":defaultLightSet.dagSetMembers" -nextAvailable
		;
// End of test.ma
