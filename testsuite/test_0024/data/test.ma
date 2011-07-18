//Maya ASCII 2011 scene
//Name: test.ma
//Last modified: Mon, Jul 18, 2011 06:21:25 PM
//Codeset: UTF-8
requires maya "2011";
requires "mtoa" "0.9.0";
currentUnit -linear centimeter -angle degree -time film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2011";
fileInfo "version" "2011 x64";
fileInfo "cutIdentifier" "201009060248-781623";
fileInfo "osv" "Linux 2.6.18-194.32.1.el5 #1 SMP Wed Jan 5 17:52:25 EST 2011 x86_64";
createNode transform -shared -name "persp";
	setAttr ".visibility" no;
	setAttr ".translate" -type "double3" 28 21 28 ;
	setAttr ".rotate" -type "double3" -27.938352729602379 44.999999999999972 -5.172681101354183e-14 ;
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
	setAttr ".focalLength" 34.999999999999993;
	setAttr ".centerOfInterest" 44.82186966202994;
	setAttr ".imageName" -type "string" "persp";
	setAttr ".depthName" -type "string" "persp_depth";
	setAttr ".maskName" -type "string" "persp_mask";
	setAttr ".homeCommand" -type "string" "viewSet -p %camera";
	setAttr -keyable on ".aiTranslator" -type "string" "perspective";
createNode transform -shared -name "top";
	setAttr ".visibility" no;
	setAttr ".translate" -type "double3" 0 1.0000000000000002 2.2204460492503131e-16 ;
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
	setAttr ".centerOfInterest" 100.1;
	setAttr ".orthographicWidth" 30;
	setAttr ".imageName" -type "string" "top";
	setAttr ".depthName" -type "string" "top_depth";
	setAttr ".maskName" -type "string" "top_mask";
	setAttr ".homeCommand" -type "string" "viewSet -t %camera";
	setAttr ".displayResolution" yes;
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
createNode transform -name "pPlane1";
createNode mesh -name "pPlaneShape1" -parent "pPlane1";
	addAttr -cachedInternally true -keyable true -shortName "ai_sss_use_gi" -longName "aiSssUseGi" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -shortName "ai_sss_sample_distribution" -longName "aiSssSampleDistribution" 
		-minValue 0 -maxValue 3 -enumName "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-attributeType "enum";
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
	addAttr -cachedInternally true -keyable true -shortName "ai_sss_max_samples" -longName "aiSssMaxSamples" 
		-defaultValue 100000 -minValue 0 -softMaxValue 1000000 -attributeType "long";
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
createNode aiOptions -shared -name "defaultArnoldRenderOptions";
	addAttr -cachedInternally true -shortName "driver_exr_compression" -longName "driverExrCompression" 
		-defaultValue 2 -minValue 0 -maxValue 4 -enumName "none:rle:zip:piz:pxr24" -attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "driver_half_precision" 
		-longName "driverHalfPrecision" -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "driver_tiled" -longName "driverTiled" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "driver_preserve_layer_name" 
		-longName "driverPreserveLayerName" -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "driver_quality" -longName "driverQuality" 
		-defaultValue 100 -minValue 0 -maxValue 100 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "driver_output_padded" -longName "driverOutputPadded" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "driver_gamma" -longName "driverGamma" 
		-defaultValue 2.2000000476837158 -minValue 9.9999997473787516e-05 -softMaxValue 5 
		-attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "driver_dither_amplitude" 
		-longName "driverDitherAmplitude" -defaultValue 1 -attributeType "float";
	addAttr -cachedInternally true -shortName "driver_png_format" -longName "driverPngFormat" 
		-minValue 0 -maxValue 1 -enumName "int8:int16" -attributeType "enum";
	addAttr -cachedInternally true -shortName "driver_tiff_compression" -longName "driverTiffCompression" 
		-minValue 0 -maxValue 4 -enumName "none:lzw:ccittrle:zip:packbits" -attributeType "enum";
	addAttr -cachedInternally true -shortName "driver_tiff_format" -longName "driverTiffFormat" 
		-minValue 0 -maxValue 3 -enumName "int8:int16:int32:float32" -attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "driver_unpremult_alpha" 
		-longName "driverUnpremultAlpha" -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "filter_width" -longName "filterWidth" 
		-defaultValue 3 -attributeType "float";
	addAttr -cachedInternally true -shortName "filter_domain" -longName "filterDomain" 
		-minValue 0 -maxValue 1 -enumName "first_hit:all_hits" -attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "filter_minimum" -longName "filterMinimum" 
		-attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "filter_maximum" -longName "filterMaximum" 
		-defaultValue 1 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "filter_scalar_mode" -longName "filterScalarMode" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	setAttr ".imageFormat" -type "string" "tiff";
	setAttr ".filterType" -type "string" "gaussian";
	setAttr -keyable on ".driverTiled" no;
createNode lightLinker -shared -name "lightLinker1";
	setAttr -size 8 ".link";
	setAttr -size 8 ".shadowLink";
createNode displayLayerManager -name "layerManager";
createNode displayLayer -name "defaultLayer";
createNode renderLayerManager -name "renderLayerManager";
createNode renderLayer -name "defaultRenderLayer";
	setAttr ".global" yes;
createNode polyPlane -name "polyPlane1";
	setAttr ".width" 30;
	setAttr ".height" 30;
	setAttr ".subdivisionsWidth" 1;
	setAttr ".subdivisionsHeight" 1;
	setAttr ".createUVs" 2;
createNode script -name "sceneConfigurationScriptNode";
	setAttr ".before" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".scriptType" 6;
createNode shadingEngine -name "layeredShader1SG";
	setAttr ".isHistoricallyInteresting" 0;
	setAttr ".renderableOnlySet" yes;
createNode materialInfo -name "materialInfo1";
createNode shadingEngine -name "ArnoldStandardShader1SG";
	setAttr ".isHistoricallyInteresting" 0;
	setAttr ".renderableOnlySet" yes;
createNode materialInfo -name "materialInfo2";
createNode shadingEngine -name "ArnoldStandardShader2SG";
	setAttr ".isHistoricallyInteresting" 0;
	setAttr ".renderableOnlySet" yes;
createNode materialInfo -name "materialInfo3";
createNode shadingEngine -name "surfaceShader1SG";
	setAttr ".isHistoricallyInteresting" 0;
	setAttr ".renderableOnlySet" yes;
createNode materialInfo -name "materialInfo4";
createNode shadingEngine -name "aiStandard1SG";
	setAttr ".isHistoricallyInteresting" 0;
	setAttr ".renderableOnlySet" yes;
createNode materialInfo -name "materialInfo5";
createNode checker -name "checker1";
createNode place2dTexture -name "place2dTexture2";
	setAttr ".repeatUV" -type "float2" 4 4 ;
createNode surfaceShader -name "surfaceShader2";
createNode shadingEngine -name "surfaceShader2SG";
	setAttr ".isHistoricallyInteresting" 0;
	setAttr ".renderableOnlySet" yes;
createNode materialInfo -name "materialInfo6";
createNode ramp -name "ramp1";
	setAttr -size 5 ".colorEntryList";
	setAttr ".colorEntryList[0].position" 0;
	setAttr ".colorEntryList[0].color" -type "float3" 1 0 0 ;
	setAttr ".colorEntryList[1].position" 0.5;
	setAttr ".colorEntryList[2].position" 0.40000000596046448;
	setAttr ".colorEntryList[2].color" -type "float3" 0 0 1 ;
	setAttr ".colorEntryList[3].position" 0.60000002384185791;
	setAttr ".colorEntryList[3].color" -type "float3" 0.025 0.025 0.97500002 ;
	setAttr ".colorEntryList[4].position" 1;
	setAttr ".colorEntryList[4].color" -type "float3" 0.92199999 0.0020000001 0.078000002 ;
	setAttr ".vWave" 0.05000000074505806;
createNode place2dTexture -name "place2dTexture3";
select -noExpand :time1;
	setAttr ".outTime" 1;
	setAttr ".unwarpedTime" 1;
select -noExpand :renderPartition;
	setAttr -size 8 ".sets";
select -noExpand :initialShadingGroup;
	setAttr ".renderableOnlySet" yes;
select -noExpand :initialParticleSE;
	setAttr ".renderableOnlySet" yes;
select -noExpand :defaultShaderList1;
	setAttr -size 3 ".shaders";
select -noExpand :defaultTextureList1;
	setAttr -size 2 ".textures";
select -noExpand :postProcessList1;
	setAttr -size 2 ".postProcesses";
select -noExpand :defaultRenderUtilityList1;
	setAttr -size 2 ".utilities";
select -noExpand :renderGlobalsList1;
select -noExpand :defaultRenderGlobals;
	setAttr ".currentRenderer" -type "string" "arnold";
	setAttr ".imageFormat" 51;
	setAttr ".imfPluginKey" -type "string" "tiff";
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
connectAttr "polyPlane1.output" "pPlaneShape1.inMesh";
connectAttr ":time1.outTime" ":defaultArnoldRenderOptions.AA_seed";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "layeredShader1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "ArnoldStandardShader1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "ArnoldStandardShader2SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "surfaceShader1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "surfaceShader2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "layeredShader1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "ArnoldStandardShader1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "ArnoldStandardShader2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "surfaceShader1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "surfaceShader2SG.message" ":defaultLightSet.message";
connectAttr "layerManager.displayLayerId[0]" "defaultLayer.identification";
connectAttr "renderLayerManager.renderLayerId[0]" "defaultRenderLayer.identification"
		;
connectAttr "layeredShader1SG.message" "materialInfo1.shadingGroup";
connectAttr "ArnoldStandardShader1SG.message" "materialInfo2.shadingGroup";
connectAttr "ArnoldStandardShader2SG.message" "materialInfo3.shadingGroup";
connectAttr "surfaceShader1SG.message" "materialInfo4.shadingGroup";
connectAttr "aiStandard1SG.message" "materialInfo5.shadingGroup";
connectAttr "place2dTexture2.outUV" "checker1.uvCoord";
connectAttr "place2dTexture2.outUvFilterSize" "checker1.uvFilterSize";
connectAttr "ramp1.outColor" "surfaceShader2.outColor";
connectAttr "surfaceShader2.outColor" "surfaceShader2SG.surfaceShader";
connectAttr "pPlaneShape1.instObjGroups" "surfaceShader2SG.dagSetMembers" -nextAvailable
		;
connectAttr "surfaceShader2SG.message" "materialInfo6.shadingGroup";
connectAttr "surfaceShader2.message" "materialInfo6.material";
connectAttr "surfaceShader2.message" "materialInfo6.texture" -nextAvailable;
connectAttr "place2dTexture3.outUV" "ramp1.uvCoord";
connectAttr "place2dTexture3.outUvFilterSize" "ramp1.uvFilterSize";
connectAttr "checker1.outColor" "ramp1.colorEntryList[1].color";
connectAttr "layeredShader1SG.partition" ":renderPartition.sets" -nextAvailable;
connectAttr "ArnoldStandardShader1SG.partition" ":renderPartition.sets" -nextAvailable
		;
connectAttr "ArnoldStandardShader2SG.partition" ":renderPartition.sets" -nextAvailable
		;
connectAttr "surfaceShader1SG.partition" ":renderPartition.sets" -nextAvailable;
connectAttr "aiStandard1SG.partition" ":renderPartition.sets" -nextAvailable;
connectAttr "surfaceShader2SG.partition" ":renderPartition.sets" -nextAvailable;
connectAttr "surfaceShader2.message" ":defaultShaderList1.shaders" -nextAvailable
		;
connectAttr "checker1.message" ":defaultTextureList1.textures" -nextAvailable;
connectAttr "ramp1.message" ":defaultTextureList1.textures" -nextAvailable;
connectAttr "place2dTexture2.message" ":defaultRenderUtilityList1.utilities" -nextAvailable
		;
connectAttr "place2dTexture3.message" ":defaultRenderUtilityList1.utilities" -nextAvailable
		;
// End of test.ma
