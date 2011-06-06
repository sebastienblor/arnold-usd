//Maya ASCII 2011 scene
//Name: test.ma
//Last modified: Tue, May 31, 2011 07:48:06 PM
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
	setAttr ".translate" -type "double3" 28 21 28 ;
	setAttr ".rotate" -type "double3" -27.938352729602379 44.999999999999972 -5.172681101354183e-014 ;
createNode camera -shared -name "perspShape" -parent "persp";
	addAttr -cachedInternally true -keyable true -shortName "ai_filtermap" -longName "aiFiltermap" 
		-attributeType "message";
	addAttr -cachedInternally true -keyable true -shortName "ai_horizontal_fov" -longName "aiHorizontalFov" 
		-defaultValue 60 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_vertical_fov" -longName "aiVerticalFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 180 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_projective" -longName "aiProjective" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
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
	addAttr -cachedInternally true -keyable true -shortName "ai_focus_distance" -longName "aiFocusDistance" 
		-defaultValue 5 -minValue 0 -maxValue 1000000000 -softMinValue 0 -softMaxValue 200 
		-attributeType "float";
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
	addAttr -cachedInternally true -keyable true -shortName "focal_distance" -longName "focalDistance" 
		-defaultValue 5 -minValue 0 -maxValue 1000000000 -softMinValue 0 -softMaxValue 200 
		-attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "aperture_size" -longName "apertureSize" 
		-defaultValue 0.20000000298023224 -minValue 0 -maxValue 20 -softMinValue 0 -softMaxValue 
		0.5 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "aperture_blades" -longName "apertureBlades" 
		-defaultValue 6 -minValue 3 -maxValue 40 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "aperture_blade_curvature" 
		-longName "apertureBladeCurvature" -minValue -20 -maxValue 20 -softMinValue -5 -softMaxValue 
		5 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "aperture_rotation" -longName "apertureRotation" 
		-minValue 0 -maxValue 360 -softMinValue 0 -softMaxValue 50 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "edof" -longName "enableDOF" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	setAttr -keyable off ".visibility" no;
	setAttr ".overscan" 1.3;
	setAttr ".focalLength" 34.999999999999993;
	setAttr ".centerOfInterest" 44.82186966202994;
	setAttr ".imageName" -type "string" "persp";
	setAttr ".depthName" -type "string" "persp_depth";
	setAttr ".maskName" -type "string" "persp_mask";
	setAttr ".homeCommand" -type "string" "viewSet -p %camera";
	setAttr ".displayResolution" yes;
	setAttr -keyable on ".aiTranslator" -type "string" "perspective";
createNode transform -shared -name "top";
	setAttr ".visibility" no;
	setAttr ".translate" -type "double3" 0 100.1 0 ;
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
	addAttr -cachedInternally true -keyable true -shortName "ai_focus_distance" -longName "aiFocusDistance" 
		-defaultValue 5 -minValue 0 -maxValue 1000000000 -softMinValue 0 -softMaxValue 200 
		-attributeType "float";
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
	addAttr -cachedInternally true -keyable true -shortName "focal_distance" -longName "focalDistance" 
		-defaultValue 5 -minValue 0 -maxValue 1000000000 -softMinValue 0 -softMaxValue 200 
		-attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "aperture_size" -longName "apertureSize" 
		-defaultValue 0.20000000298023224 -minValue 0 -maxValue 20 -softMinValue 0 -softMaxValue 
		0.5 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "aperture_blades" -longName "apertureBlades" 
		-defaultValue 6 -minValue 3 -maxValue 40 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "aperture_blade_curvature" 
		-longName "apertureBladeCurvature" -minValue -20 -maxValue 20 -softMinValue -5 -softMaxValue 
		5 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "aperture_rotation" -longName "apertureRotation" 
		-minValue 0 -maxValue 360 -softMinValue 0 -softMaxValue 50 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "edof" -longName "enableDOF" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	setAttr -keyable off ".visibility" no;
	setAttr ".renderable" no;
	setAttr ".centerOfInterest" 100.1;
	setAttr ".orthographicWidth" 30;
	setAttr ".imageName" -type "string" "top";
	setAttr ".depthName" -type "string" "top_depth";
	setAttr ".maskName" -type "string" "top_mask";
	setAttr ".homeCommand" -type "string" "viewSet -t %camera";
	setAttr ".orthographic" yes;
	setAttr -keyable on ".aiTranslator" -type "string" "orthographic";
createNode transform -shared -name "front";
	setAttr ".visibility" no;
	setAttr ".translate" -type "double3" 0 0 100.1 ;
createNode camera -shared -name "frontShape" -parent "front";
	addAttr -cachedInternally true -keyable true -shortName "ai_filtermap" -longName "aiFiltermap" 
		-attributeType "message";
	addAttr -cachedInternally true -keyable true -shortName "ai_horizontal_fov" -longName "aiHorizontalFov" 
		-defaultValue 60 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_vertical_fov" -longName "aiVerticalFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 180 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_projective" -longName "aiProjective" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
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
	addAttr -cachedInternally true -keyable true -shortName "ai_focus_distance" -longName "aiFocusDistance" 
		-defaultValue 5 -minValue 0 -maxValue 1000000000 -softMinValue 0 -softMaxValue 200 
		-attributeType "float";
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
	addAttr -cachedInternally true -keyable true -shortName "focal_distance" -longName "focalDistance" 
		-defaultValue 5 -minValue 0 -maxValue 1000000000 -softMinValue 0 -softMaxValue 200 
		-attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "aperture_size" -longName "apertureSize" 
		-defaultValue 0.20000000298023224 -minValue 0 -maxValue 20 -softMinValue 0 -softMaxValue 
		0.5 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "aperture_blades" -longName "apertureBlades" 
		-defaultValue 6 -minValue 3 -maxValue 40 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "aperture_blade_curvature" 
		-longName "apertureBladeCurvature" -minValue -20 -maxValue 20 -softMinValue -5 -softMaxValue 
		5 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "aperture_rotation" -longName "apertureRotation" 
		-minValue 0 -maxValue 360 -softMinValue 0 -softMaxValue 50 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "edof" -longName "enableDOF" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	setAttr -keyable off ".visibility" no;
	setAttr ".renderable" no;
	setAttr ".centerOfInterest" 100.1;
	setAttr ".orthographicWidth" 30;
	setAttr ".imageName" -type "string" "front";
	setAttr ".depthName" -type "string" "front_depth";
	setAttr ".maskName" -type "string" "front_mask";
	setAttr ".homeCommand" -type "string" "viewSet -f %camera";
	setAttr ".orthographic" yes;
	setAttr -keyable on ".aiTranslator" -type "string" "orthographic";
createNode transform -shared -name "side";
	setAttr ".visibility" no;
	setAttr ".translate" -type "double3" 100.1 0 0 ;
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
	addAttr -cachedInternally true -keyable true -shortName "ai_focus_distance" -longName "aiFocusDistance" 
		-defaultValue 5 -minValue 0 -maxValue 1000000000 -softMinValue 0 -softMaxValue 200 
		-attributeType "float";
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
	addAttr -cachedInternally true -keyable true -shortName "focal_distance" -longName "focalDistance" 
		-defaultValue 5 -minValue 0 -maxValue 1000000000 -softMinValue 0 -softMaxValue 200 
		-attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "aperture_size" -longName "apertureSize" 
		-defaultValue 0.20000000298023224 -minValue 0 -maxValue 20 -softMinValue 0 -softMaxValue 
		0.5 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "aperture_blades" -longName "apertureBlades" 
		-defaultValue 6 -minValue 3 -maxValue 40 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "aperture_blade_curvature" 
		-longName "apertureBladeCurvature" -minValue -20 -maxValue 20 -softMinValue -5 -softMaxValue 
		5 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "aperture_rotation" -longName "apertureRotation" 
		-minValue 0 -maxValue 360 -softMinValue 0 -softMaxValue 50 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "edof" -longName "enableDOF" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	setAttr -keyable off ".visibility" no;
	setAttr ".renderable" no;
	setAttr ".centerOfInterest" 100.1;
	setAttr ".orthographicWidth" 30;
	setAttr ".imageName" -type "string" "side";
	setAttr ".depthName" -type "string" "side_depth";
	setAttr ".maskName" -type "string" "side_mask";
	setAttr ".homeCommand" -type "string" "viewSet -s %camera";
	setAttr ".orthographic" yes;
	setAttr -keyable on ".aiTranslator" -type "string" "orthographic";
createNode transform -name "pSphere1";
createNode mesh -name "pSphereShape1" -parent "pSphere1";
	addAttr -cachedInternally true -keyable true -shortName "ai_sss_use_gi" -longName "aiSssUseGi" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_sss_max_samples" -longName "aiSssMaxSamples" 
		-defaultValue 100000 -minValue 0 -softMaxValue 1000000 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "ai_sss_sample_spacing" 
		-longName "aiSssSampleSpacing" -defaultValue 0.10000000149011612 -minValue 0 -softMaxValue 
		1 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_self_shadows" -longName "aiSelfShadows" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_opaque" -longName "aiOpaque" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_vid" -longName "aiVisibleInDiffuse" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_vig" -longName "aiVisibleInGlossy" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -shortName "ai_subdiv_type" -longName "aiSubdivType" 
		-minValue 0 -maxValue 2 -enumName "none:catclark:linear" -attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "ai_subdiv_iterations" -longName "aiSubdivIterations" 
		-defaultValue 1 -minValue 0 -maxValue 100 -softMinValue 0 -softMaxValue 10 -attributeType "long";
	addAttr -cachedInternally true -shortName "ai_subdiv_adaptive_metric" -longName "aiSubdivAdaptiveMetric" 
		-minValue 0 -maxValue 2 -enumName "auto:edge_length:flatness" -attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "ai_subdiv_pixel_error" 
		-longName "aiSubdivPixelError" -minValue 0 -softMaxValue 10 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_subdiv_dicing_camera" 
		-longName "aiSubdivDicingCamera" -attributeType "message";
	addAttr -cachedInternally true -shortName "ai_subdiv_uv_smoothing" -longName "aiSubdivUvSmoothing" 
		-minValue 0 -maxValue 3 -enumName "pin_corners:pin_borders:linear:smooth" -attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "ai_exptan" -longName "aiExportTangents" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_expcol" -longName "aiExportColors" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "sss_use_gi" -longName "sssUseGi" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "sss_max_samples" -longName "sssMaxSamples" 
		-defaultValue 100000 -minValue 0 -maxValue 1000000000 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "sss_sample_spacing" -longName "sssSampleSpacing" 
		-defaultValue 0.10000000149011612 -minValue 0 -maxValue 10 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "self_shadows" -longName "selfShadows" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "opaque" -longName "opaque" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "vid" -longName "visibleInDiffuse" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "vig" -longName "visibleInGlossy" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -shortName "subdiv_type" -longName "subdivType" -minValue 
		0 -maxValue 1 -enumName "none:catclark" -attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "subdiv_iterations" -longName "subdivIterations" 
		-defaultValue 1 -minValue 0 -maxValue 100 -attributeType "long";
	addAttr -cachedInternally true -shortName "subdiv_adaptive_metric" -longName "subdivAdaptiveMetric" 
		-minValue 0 -maxValue 2 -enumName "auto:edge_length:flatness" -attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "subdiv_pixel_error" -longName "subdivPixelError" 
		-minValue 0 -maxValue 50 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "subdiv_dicing_camera" -longName "subdivDicingCamera" 
		-attributeType "message";
	addAttr -cachedInternally true -shortName "subdiv_uv_smoothing" -longName "subdivUvSmoothing" 
		-minValue 0 -maxValue 1 -enumName "pin_corners:pin_borders" -attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "exptan" -longName "exportTangents" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "expcol" -longName "exportColors" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -shortName "mso" -longName "miShadingSamplesOverride" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -shortName "msh" -longName "miShadingSamples" -minValue 
		0 -softMaxValue 8 -attributeType "float";
	addAttr -cachedInternally true -shortName "mdo" -longName "miMaxDisplaceOverride" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -shortName "mmd" -longName "miMaxDisplace" -minValue 
		0 -softMaxValue 1 -attributeType "float";
	setAttr -keyable off ".visibility";
	setAttr ".visibleInReflections" yes;
	setAttr ".visibleInRefractions" yes;
	setAttr ".uvSet[0].uvSetName" -type "string" "map1";
	setAttr ".currentUVSet" -type "string" "map1";
	setAttr ".displayColorChannel" -type "string" "Ambient+Diffuse";
	setAttr ".collisionOffsetVelocityMultiplier[0]"  0 1 1;
	setAttr ".collisionDepthVelocityMultiplier[0]"  0 1 1;
createNode transform -name "aiSky";
createNode aiSky -name "aiSkyShape" -parent "aiSky";
	setAttr -keyable off ".visibility";
createNode aiOptions -shared -name "defaultArnoldRenderOptions";
	setAttr ".arnoldRenderImageFormat" 1;
	setAttr ".renderType" 1;
	setAttr ".filter_width" 4;
	setAttr ".driver_gamma" 1;
	setAttr ".light_gamma" 1;
	setAttr ".shader_gamma" 1;
	setAttr ".texture_gamma" 1;
	setAttr ".GIDiffuseDepth" 2;
	setAttr ".GIGlossyDepth" 2;
createNode lightLinker -shared -name "lightLinker1";
	setAttr -size 3 ".link";
	setAttr -size 3 ".shadowLink";
createNode displayLayerManager -name "layerManager";
createNode displayLayer -name "defaultLayer";
createNode renderLayerManager -name "renderLayerManager";
createNode renderLayer -name "defaultRenderLayer";
	setAttr ".global" yes;
createNode polySphere -name "polySphere1";
	setAttr ".radius" 11;
	setAttr ".subdivisionsAxis" 50;
	setAttr ".subdivisionsHeight" 50;
createNode aiStandard -name "aiStandard1";
	setAttr ".KdColor" -type "float3" 1 0.27499998 0.27499998 ;
createNode shadingEngine -name "aiStandard1SG";
	setAttr ".isHistoricallyInteresting" 0;
	setAttr ".renderableOnlySet" yes;
createNode materialInfo -name "materialInfo1";
createNode script -name "sceneConfigurationScriptNode";
	setAttr ".before" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".scriptType" 6;
select -noExpand :time1;
	setAttr ".outTime" 1;
	setAttr ".unwarpedTime" 1;
select -noExpand :renderPartition;
	setAttr -size 3 ".sets";
select -noExpand :initialShadingGroup;
	setAttr ".renderableOnlySet" yes;
select -noExpand :initialParticleSE;
	setAttr ".renderableOnlySet" yes;
select -noExpand :defaultShaderList1;
	setAttr -size 4 ".shaders";
select -noExpand :postProcessList1;
	setAttr -size 2 ".postProcesses";
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
select -noExpand :hardwareRenderGlobals;
	setAttr ".colorTextureResolution" 256;
	setAttr ".bumpTextureResolution" 512;
select -noExpand :defaultHardwareRenderGlobals;
	setAttr ".filename" -type "string" "im";
	setAttr ".resolution" -type "string" "ntsc_4d 646 485 1.333";
connectAttr "polySphere1.output" "pSphereShape1.inMesh";
connectAttr ":time1.outTime" ":defaultArnoldRenderOptions.AA_seed";
connectAttr "aiSkyShape.message" ":defaultArnoldRenderOptions.background";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
connectAttr "layerManager.displayLayerId[0]" "defaultLayer.identification";
connectAttr "renderLayerManager.renderLayerId[0]" "defaultRenderLayer.identification"
		;
connectAttr "aiStandard1.outColor" "aiStandard1SG.surfaceShader";
connectAttr "pSphereShape1.instObjGroups" "aiStandard1SG.dagSetMembers" -nextAvailable
		;
connectAttr "aiStandard1SG.message" "materialInfo1.shadingGroup";
connectAttr "aiStandard1.message" "materialInfo1.material";
connectAttr "aiStandard1.message" "materialInfo1.texture" -nextAvailable;
connectAttr "aiStandard1SG.partition" ":renderPartition.sets" -nextAvailable;
connectAttr "aiStandard1.message" ":defaultShaderList1.shaders" -nextAvailable;
connectAttr "aiSkyShape.message" ":defaultShaderList1.shaders" -nextAvailable;
// End of test.ma
