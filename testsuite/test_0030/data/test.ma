//Maya ASCII 2011 scene
//Name: test.ma
//Last modified: Mon, Jul 18, 2011 05:33:53 PM
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
	setAttr ".translate" -type "double3" 8.2209683823296658 6.1657262420436485 8.2209683525273434 ;
	setAttr ".rotate" -type "double3" -27.938352729602322 45.000000000000007 -2.2489917831974721e-15 ;
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
	addAttr -cachedInternally true -keyable true -shortName "filtermap" -longName "filtermap" 
		-attributeType "message";
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
	addAttr -cachedInternally true -usedAsColor -keyable true -shortName "uv_remap" 
		-longName "uvRemap" -attributeType "float3" -numberOfChildren 3;
	addAttr -cachedInternally true -keyable true -shortName "uv_remapr" -longName "uvRemapR" 
		-attributeType "float" -parent "uvRemap";
	addAttr -cachedInternally true -keyable true -shortName "uv_remapg" -longName "uvRemapG" 
		-attributeType "float" -parent "uvRemap";
	addAttr -cachedInternally true -keyable true -shortName "uv_remapb" -longName "uvRemapB" 
		-attributeType "float" -parent "uvRemap";
	addAttr -cachedInternally true -hidden true -keyable true -shortName "uv_remapa" 
		-longName "uvRemapA" -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "arntr" -longName "arnoldTranslator" 
		-dataType "string";
	addAttr -cachedInternally true -keyable true -shortName "fov" -longName "fov" -defaultValue 
		90 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "autocrop" -longName "autocrop" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "horizontal_fov" -longName "horizontalFov" 
		-defaultValue 60 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "vertical_fov" -longName "verticalFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 180 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "projective" -longName "projective" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	setAttr -keyable off ".visibility" no;
	setAttr ".overscan" 1.3;
	setAttr ".focalLength" 34.999999999999986;
	setAttr ".centerOfInterest" 13.159970380601656;
	setAttr ".imageName" -type "string" "persp";
	setAttr ".depthName" -type "string" "persp_depth";
	setAttr ".maskName" -type "string" "persp_mask";
	setAttr ".tumblePivot" -type "double3" 5.9604644775390625e-08 0 2.9802322387695312e-08 ;
	setAttr ".homeCommand" -type "string" "viewSet -p %camera";
	setAttr ".displayResolution" yes;
	setAttr -keyable on ".aiTranslator" -type "string" "perspective";
	setAttr -keyable on ".arnoldTranslator" -type "string" "perspective";
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
	addAttr -cachedInternally true -keyable true -shortName "filtermap" -longName "filtermap" 
		-attributeType "message";
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
	addAttr -cachedInternally true -usedAsColor -keyable true -shortName "uv_remap" 
		-longName "uvRemap" -attributeType "float3" -numberOfChildren 3;
	addAttr -cachedInternally true -keyable true -shortName "uv_remapr" -longName "uvRemapR" 
		-attributeType "float" -parent "uvRemap";
	addAttr -cachedInternally true -keyable true -shortName "uv_remapg" -longName "uvRemapG" 
		-attributeType "float" -parent "uvRemap";
	addAttr -cachedInternally true -keyable true -shortName "uv_remapb" -longName "uvRemapB" 
		-attributeType "float" -parent "uvRemap";
	addAttr -cachedInternally true -hidden true -keyable true -shortName "uv_remapa" 
		-longName "uvRemapA" -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "arntr" -longName "arnoldTranslator" 
		-dataType "string";
	addAttr -cachedInternally true -keyable true -shortName "fov" -longName "fov" -defaultValue 
		90 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "autocrop" -longName "autocrop" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "horizontal_fov" -longName "horizontalFov" 
		-defaultValue 60 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "vertical_fov" -longName "verticalFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 180 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "projective" -longName "projective" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
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
	setAttr -keyable on ".arnoldTranslator" -type "string" "orthographic";
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
	addAttr -cachedInternally true -keyable true -shortName "filtermap" -longName "filtermap" 
		-attributeType "message";
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
	addAttr -cachedInternally true -usedAsColor -keyable true -shortName "uv_remap" 
		-longName "uvRemap" -attributeType "float3" -numberOfChildren 3;
	addAttr -cachedInternally true -keyable true -shortName "uv_remapr" -longName "uvRemapR" 
		-attributeType "float" -parent "uvRemap";
	addAttr -cachedInternally true -keyable true -shortName "uv_remapg" -longName "uvRemapG" 
		-attributeType "float" -parent "uvRemap";
	addAttr -cachedInternally true -keyable true -shortName "uv_remapb" -longName "uvRemapB" 
		-attributeType "float" -parent "uvRemap";
	addAttr -cachedInternally true -hidden true -keyable true -shortName "uv_remapa" 
		-longName "uvRemapA" -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "arntr" -longName "arnoldTranslator" 
		-dataType "string";
	addAttr -cachedInternally true -keyable true -shortName "fov" -longName "fov" -defaultValue 
		90 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "autocrop" -longName "autocrop" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "horizontal_fov" -longName "horizontalFov" 
		-defaultValue 60 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "vertical_fov" -longName "verticalFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 180 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "projective" -longName "projective" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
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
	setAttr -keyable on ".arnoldTranslator" -type "string" "orthographic";
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
	addAttr -cachedInternally true -keyable true -shortName "filtermap" -longName "filtermap" 
		-attributeType "message";
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
	addAttr -cachedInternally true -usedAsColor -keyable true -shortName "uv_remap" 
		-longName "uvRemap" -attributeType "float3" -numberOfChildren 3;
	addAttr -cachedInternally true -keyable true -shortName "uv_remapr" -longName "uvRemapR" 
		-attributeType "float" -parent "uvRemap";
	addAttr -cachedInternally true -keyable true -shortName "uv_remapg" -longName "uvRemapG" 
		-attributeType "float" -parent "uvRemap";
	addAttr -cachedInternally true -keyable true -shortName "uv_remapb" -longName "uvRemapB" 
		-attributeType "float" -parent "uvRemap";
	addAttr -cachedInternally true -hidden true -keyable true -shortName "uv_remapa" 
		-longName "uvRemapA" -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "arntr" -longName "arnoldTranslator" 
		-dataType "string";
	addAttr -cachedInternally true -keyable true -shortName "fov" -longName "fov" -defaultValue 
		90 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "autocrop" -longName "autocrop" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "horizontal_fov" -longName "horizontalFov" 
		-defaultValue 60 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "vertical_fov" -longName "verticalFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 180 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "projective" -longName "projective" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
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
	setAttr -keyable on ".arnoldTranslator" -type "string" "orthographic";
createNode transform -name "pSphere1";
createNode mesh -name "pSphereShape1" -parent "pSphere1";
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
	addAttr -cachedInternally true -shortName "mso" -longName "miShadingSamplesOverride" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -shortName "msh" -longName "miShadingSamples" -minValue 
		0 -softMaxValue 8 -attributeType "float";
	addAttr -cachedInternally true -shortName "mdo" -longName "miMaxDisplaceOverride" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -shortName "mmd" -longName "miMaxDisplace" -minValue 
		0 -softMaxValue 1 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "sss_use_gi" -longName "sssUseGi" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "sss_max_samples" -longName "sssMaxSamples" 
		-defaultValue 100000 -minValue 0 -softMaxValue 1000000 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "sss_sample_spacing" -longName "sssSampleSpacing" 
		-defaultValue 0.10000000149011612 -minValue 0 -softMaxValue 1 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "self_shadows" -longName "selfShadows" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "opaque" -longName "opaque" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "vid" -longName "visibleInDiffuse" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "vig" -longName "visibleInGlossy" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -shortName "subdiv_type" -longName "subdivType" -minValue 
		0 -maxValue 2 -enumName "none:catclark:linear" -attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "subdiv_iterations" -longName "subdivIterations" 
		-defaultValue 1 -minValue 0 -maxValue 100 -softMinValue 0 -softMaxValue 10 -attributeType "long";
	addAttr -cachedInternally true -shortName "subdiv_adaptive_metric" -longName "subdivAdaptiveMetric" 
		-minValue 0 -maxValue 2 -enumName "auto:edge_length:flatness" -attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "subdiv_pixel_error" -longName "subdivPixelError" 
		-minValue 0 -softMaxValue 10 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "subdiv_dicing_camera" -longName "subdivDicingCamera" 
		-attributeType "message";
	addAttr -cachedInternally true -shortName "subdiv_uv_smoothing" -longName "subdivUvSmoothing" 
		-minValue 0 -maxValue 3 -enumName "pin_corners:pin_borders:linear:smooth" -attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "exptan" -longName "exportTangents" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "expcol" -longName "exportColors" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	setAttr -keyable off ".visibility";
	setAttr ".visibleInReflections" yes;
	setAttr ".visibleInRefractions" yes;
	setAttr ".uvSet[0].uvSetName" -type "string" "map1";
	setAttr ".currentUVSet" -type "string" "map1";
	setAttr ".displayColorChannel" -type "string" "Ambient+Diffuse";
	setAttr ".collisionOffsetVelocityMultiplier[0]"  0 1 1;
	setAttr ".collisionDepthVelocityMultiplier[0]"  0 1 1;
createNode transform -name "pointLight1";
	setAttr ".translate" -type "double3" 0.8946263483617648 1.8998226219594425 2.2074515762557692 ;
createNode pointLight -name "pointLightShape1" -parent "pointLight1";
	addAttr -cachedInternally true -keyable true -shortName "ai_cast_shadows" -longName "aiCastShadows" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_exposure" -longName "aiExposure" 
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
	addAttr -cachedInternally true -keyable true -multi -shortName "ai_filters" -longName "aiFilters" 
		-attributeType "message";
	addAttr -cachedInternally true -keyable true -shortName "ai_oss" -longName "aiOverrideSssSamples" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_sss_samples" -longName "aiSssSamples" 
		-defaultValue 1 -minValue 0 -maxValue 100 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "ai_affect_volumetrics" 
		-longName "aiAffectVolumetrics" -defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_cast_volumetric_shadows" 
		-longName "aiCastVolumetricShadows" -defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_radius" -longName "aiRadius" 
		-minValue 0 -softMaxValue 10 -attributeType "float";
	setAttr -keyable off ".visibility";
	setAttr ".useOnlySingleDmap" no;
createNode transform -name "hairSystem1";
createNode hairSystem -name "hairSystemShape1" -parent "hairSystem1";
	addAttr -cachedInternally true -keyable true -shortName "ai_sss_use_gi" -longName "aiSssUseGi" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -shortName "ai_sss_sample_distribution" -longName "aiSssSampleDistribution" 
		-minValue 0 -maxValue 3 -enumName "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "ai_sss_sample_spacing" 
		-longName "aiSssSampleSpacing" -defaultValue 0.10000000149011612 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_self_shadows" -longName "aiSelfShadows" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_opaque" -longName "aiOpaque" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_vid" -longName "aiVisibleInDiffuse" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_vig" -longName "aiVisibleInGlossy" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_min_pixel_width" -longName "aiMinPixelWidth" 
		-attributeType "float";
	addAttr -cachedInternally true -shortName "ai_mode" -longName "aiMode" -minValue 
		0 -maxValue 1 -enumName "ribbon:thick" -attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "ai_export_hair_ids" -longName "aiExportHairIDs" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_override_hair" -longName "aiOverrideHair" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_hair_shader" -longName "aiHairShader" 
		-attributeType "message";
	addAttr -cachedInternally true -keyable true -shortName "ai_sss_max_samples" -longName "aiSssMaxSamples" 
		-defaultValue 100000 -attributeType "long";
	addAttr -cachedInternally true -shortName "mso" -longName "miShadingSamplesOverride" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -shortName "msh" -longName "miShadingSamples" -minValue 
		0 -softMaxValue 8 -attributeType "float";
	addAttr -cachedInternally true -shortName "mdo" -longName "miMaxDisplaceOverride" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -shortName "mmd" -longName "miMaxDisplace" -minValue 
		0 -softMaxValue 1 -attributeType "float";
	setAttr -keyable off ".visibility";
	setAttr -size 64 ".inputHair";
	setAttr ".evaluationOrder" 0;
	setAttr ".stiffness" 0.1;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
	setAttr ".subSegments" 10;
	setAttr ".clumpWidth" 0.61589403937737275;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.63999999 3;
	setAttr ".clumpTwist" 0.45695364187835463;
	setAttr ".clumpCurl[0]"  0 0.5 1;
	setAttr ".clumpFlatness[0]"  0 0 1;
	setAttr -size 2 ".hairWidthScale[0:1]"  0.68695652 1 3 1 0.2 3;
	setAttr -size 3 ".hairColorScale";
	setAttr ".hairColorScale[0].hairColorScale_Position" 0;
	setAttr ".hairColorScale[0].hairColorScale_Color" -type "float3" 0.37099999 0.24690048 
		0.12280098 ;
	setAttr ".hairColorScale[0].hairColorScale_Interp" 3;
	setAttr ".hairColorScale[1].hairColorScale_Position" 0.30000001192092896;
	setAttr ".hairColorScale[1].hairColorScale_Color" -type "float3" 0.43799999 0.31864497 
		0.19928999 ;
	setAttr ".hairColorScale[1].hairColorScale_Interp" 3;
	setAttr ".hairColorScale[2].hairColorScale_Position" 1;
	setAttr ".hairColorScale[2].hairColorScale_Color" -type "float3" 0.62900001 0.54320443 
		0.462944 ;
	setAttr ".hairColorScale[2].hairColorScale_Interp" 3;
	setAttr ".hairsPerClump" 26;
	setAttr ".thinning" 0.15231788093655907;
	setAttr ".hueRand" 0.068027210548570777;
	setAttr ".satRand" 0.10204081628851745;
	setAttr ".valRand" 0.12925170106673931;
	setAttr ".displacementScale[0]"  0 1 1;
createNode transform -name "hairSystem1Follicles";
createNode transform -name "pSphere1Follicle606" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape606" -parent "pSphere1Follicle606";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.0625;
	setAttr ".parameterV" 0.0625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve1" -parent "pSphere1Follicle606";
createNode nurbsCurve -name "curveShape1" -parent "curve1";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle619" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape619" -parent "pSphere1Follicle619";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.0625;
	setAttr ".parameterV" 0.1875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve2" -parent "pSphere1Follicle619";
createNode nurbsCurve -name "curveShape2" -parent "curve2";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle631" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape631" -parent "pSphere1Follicle631";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.0625;
	setAttr ".parameterV" 0.3125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve3" -parent "pSphere1Follicle631";
createNode nurbsCurve -name "curveShape3" -parent "curve3";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle643" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape643" -parent "pSphere1Follicle643";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.0625;
	setAttr ".parameterV" 0.4375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve4" -parent "pSphere1Follicle643";
createNode nurbsCurve -name "curveShape4" -parent "curve4";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle656" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape656" -parent "pSphere1Follicle656";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.0625;
	setAttr ".parameterV" 0.5625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve5" -parent "pSphere1Follicle656";
createNode nurbsCurve -name "curveShape5" -parent "curve5";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle668" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape668" -parent "pSphere1Follicle668";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.0625;
	setAttr ".parameterV" 0.6875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve6" -parent "pSphere1Follicle668";
createNode nurbsCurve -name "curveShape6" -parent "curve6";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle680" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape680" -parent "pSphere1Follicle680";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.0625;
	setAttr ".parameterV" 0.8125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve7" -parent "pSphere1Follicle680";
createNode nurbsCurve -name "curveShape7" -parent "curve7";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle693" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape693" -parent "pSphere1Follicle693";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.0625;
	setAttr ".parameterV" 0.9375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve8" -parent "pSphere1Follicle693";
createNode nurbsCurve -name "curveShape8" -parent "curve8";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle1906" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape1906" -parent "pSphere1Follicle1906";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.1875;
	setAttr ".parameterV" 0.0625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve9" -parent "pSphere1Follicle1906";
createNode nurbsCurve -name "curveShape9" -parent "curve9";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle1919" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape1919" -parent "pSphere1Follicle1919";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.1875;
	setAttr ".parameterV" 0.1875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve10" -parent "pSphere1Follicle1919";
createNode nurbsCurve -name "curveShape10" -parent "curve10";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle1931" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape1931" -parent "pSphere1Follicle1931";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.1875;
	setAttr ".parameterV" 0.3125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve11" -parent "pSphere1Follicle1931";
createNode nurbsCurve -name "curveShape11" -parent "curve11";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle1943" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape1943" -parent "pSphere1Follicle1943";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.1875;
	setAttr ".parameterV" 0.4375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve12" -parent "pSphere1Follicle1943";
createNode nurbsCurve -name "curveShape12" -parent "curve12";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle1956" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape1956" -parent "pSphere1Follicle1956";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.1875;
	setAttr ".parameterV" 0.5625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve13" -parent "pSphere1Follicle1956";
createNode nurbsCurve -name "curveShape13" -parent "curve13";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle1968" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape1968" -parent "pSphere1Follicle1968";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.1875;
	setAttr ".parameterV" 0.6875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve14" -parent "pSphere1Follicle1968";
createNode nurbsCurve -name "curveShape14" -parent "curve14";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle1980" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape1980" -parent "pSphere1Follicle1980";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.1875;
	setAttr ".parameterV" 0.8125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve15" -parent "pSphere1Follicle1980";
createNode nurbsCurve -name "curveShape15" -parent "curve15";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle1993" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape1993" -parent "pSphere1Follicle1993";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.1875;
	setAttr ".parameterV" 0.9375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve16" -parent "pSphere1Follicle1993";
createNode nurbsCurve -name "curveShape16" -parent "curve16";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle3106" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape3106" -parent "pSphere1Follicle3106";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.3125;
	setAttr ".parameterV" 0.0625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve17" -parent "pSphere1Follicle3106";
createNode nurbsCurve -name "curveShape17" -parent "curve17";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle3119" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape3119" -parent "pSphere1Follicle3119";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.3125;
	setAttr ".parameterV" 0.1875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve18" -parent "pSphere1Follicle3119";
createNode nurbsCurve -name "curveShape18" -parent "curve18";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle3131" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape3131" -parent "pSphere1Follicle3131";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.3125;
	setAttr ".parameterV" 0.3125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve19" -parent "pSphere1Follicle3131";
createNode nurbsCurve -name "curveShape19" -parent "curve19";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle3143" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape3143" -parent "pSphere1Follicle3143";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.3125;
	setAttr ".parameterV" 0.4375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve20" -parent "pSphere1Follicle3143";
createNode nurbsCurve -name "curveShape20" -parent "curve20";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle3156" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape3156" -parent "pSphere1Follicle3156";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.3125;
	setAttr ".parameterV" 0.5625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve21" -parent "pSphere1Follicle3156";
createNode nurbsCurve -name "curveShape21" -parent "curve21";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle3168" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape3168" -parent "pSphere1Follicle3168";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.3125;
	setAttr ".parameterV" 0.6875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve22" -parent "pSphere1Follicle3168";
createNode nurbsCurve -name "curveShape22" -parent "curve22";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle3180" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape3180" -parent "pSphere1Follicle3180";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.3125;
	setAttr ".parameterV" 0.8125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve23" -parent "pSphere1Follicle3180";
createNode nurbsCurve -name "curveShape23" -parent "curve23";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle3193" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape3193" -parent "pSphere1Follicle3193";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.3125;
	setAttr ".parameterV" 0.9375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve24" -parent "pSphere1Follicle3193";
createNode nurbsCurve -name "curveShape24" -parent "curve24";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle4306" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape4306" -parent "pSphere1Follicle4306";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.4375;
	setAttr ".parameterV" 0.0625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve25" -parent "pSphere1Follicle4306";
createNode nurbsCurve -name "curveShape25" -parent "curve25";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle4319" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape4319" -parent "pSphere1Follicle4319";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.4375;
	setAttr ".parameterV" 0.1875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve26" -parent "pSphere1Follicle4319";
createNode nurbsCurve -name "curveShape26" -parent "curve26";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle4331" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape4331" -parent "pSphere1Follicle4331";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.4375;
	setAttr ".parameterV" 0.3125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve27" -parent "pSphere1Follicle4331";
createNode nurbsCurve -name "curveShape27" -parent "curve27";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle4343" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape4343" -parent "pSphere1Follicle4343";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.4375;
	setAttr ".parameterV" 0.4375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve28" -parent "pSphere1Follicle4343";
createNode nurbsCurve -name "curveShape28" -parent "curve28";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle4356" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape4356" -parent "pSphere1Follicle4356";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.4375;
	setAttr ".parameterV" 0.5625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve29" -parent "pSphere1Follicle4356";
createNode nurbsCurve -name "curveShape29" -parent "curve29";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle4368" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape4368" -parent "pSphere1Follicle4368";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.4375;
	setAttr ".parameterV" 0.6875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve30" -parent "pSphere1Follicle4368";
createNode nurbsCurve -name "curveShape30" -parent "curve30";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle4380" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape4380" -parent "pSphere1Follicle4380";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.4375;
	setAttr ".parameterV" 0.8125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve31" -parent "pSphere1Follicle4380";
createNode nurbsCurve -name "curveShape31" -parent "curve31";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle4393" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape4393" -parent "pSphere1Follicle4393";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.4375;
	setAttr ".parameterV" 0.9375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve32" -parent "pSphere1Follicle4393";
createNode nurbsCurve -name "curveShape32" -parent "curve32";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle5606" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape5606" -parent "pSphere1Follicle5606";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.5625;
	setAttr ".parameterV" 0.0625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve33" -parent "pSphere1Follicle5606";
createNode nurbsCurve -name "curveShape33" -parent "curve33";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle5619" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape5619" -parent "pSphere1Follicle5619";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.5625;
	setAttr ".parameterV" 0.1875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve34" -parent "pSphere1Follicle5619";
createNode nurbsCurve -name "curveShape34" -parent "curve34";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle5631" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape5631" -parent "pSphere1Follicle5631";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.5625;
	setAttr ".parameterV" 0.3125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve35" -parent "pSphere1Follicle5631";
createNode nurbsCurve -name "curveShape35" -parent "curve35";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle5643" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape5643" -parent "pSphere1Follicle5643";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.5625;
	setAttr ".parameterV" 0.4375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve36" -parent "pSphere1Follicle5643";
createNode nurbsCurve -name "curveShape36" -parent "curve36";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle5656" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape5656" -parent "pSphere1Follicle5656";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.5625;
	setAttr ".parameterV" 0.5625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve37" -parent "pSphere1Follicle5656";
createNode nurbsCurve -name "curveShape37" -parent "curve37";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle5668" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape5668" -parent "pSphere1Follicle5668";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.5625;
	setAttr ".parameterV" 0.6875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve38" -parent "pSphere1Follicle5668";
createNode nurbsCurve -name "curveShape38" -parent "curve38";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle5680" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape5680" -parent "pSphere1Follicle5680";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.5625;
	setAttr ".parameterV" 0.8125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve39" -parent "pSphere1Follicle5680";
createNode nurbsCurve -name "curveShape39" -parent "curve39";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle5693" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape5693" -parent "pSphere1Follicle5693";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.5625;
	setAttr ".parameterV" 0.9375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve40" -parent "pSphere1Follicle5693";
createNode nurbsCurve -name "curveShape40" -parent "curve40";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle6806" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape6806" -parent "pSphere1Follicle6806";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.6875;
	setAttr ".parameterV" 0.0625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve41" -parent "pSphere1Follicle6806";
createNode nurbsCurve -name "curveShape41" -parent "curve41";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle6819" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape6819" -parent "pSphere1Follicle6819";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.6875;
	setAttr ".parameterV" 0.1875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve42" -parent "pSphere1Follicle6819";
createNode nurbsCurve -name "curveShape42" -parent "curve42";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle6831" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape6831" -parent "pSphere1Follicle6831";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.6875;
	setAttr ".parameterV" 0.3125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve43" -parent "pSphere1Follicle6831";
createNode nurbsCurve -name "curveShape43" -parent "curve43";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle6843" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape6843" -parent "pSphere1Follicle6843";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.6875;
	setAttr ".parameterV" 0.4375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve44" -parent "pSphere1Follicle6843";
createNode nurbsCurve -name "curveShape44" -parent "curve44";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle6856" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape6856" -parent "pSphere1Follicle6856";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.6875;
	setAttr ".parameterV" 0.5625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve45" -parent "pSphere1Follicle6856";
createNode nurbsCurve -name "curveShape45" -parent "curve45";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle6868" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape6868" -parent "pSphere1Follicle6868";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.6875;
	setAttr ".parameterV" 0.6875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve46" -parent "pSphere1Follicle6868";
createNode nurbsCurve -name "curveShape46" -parent "curve46";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle6880" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape6880" -parent "pSphere1Follicle6880";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.6875;
	setAttr ".parameterV" 0.8125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve47" -parent "pSphere1Follicle6880";
createNode nurbsCurve -name "curveShape47" -parent "curve47";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle6893" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape6893" -parent "pSphere1Follicle6893";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.6875;
	setAttr ".parameterV" 0.9375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve48" -parent "pSphere1Follicle6893";
createNode nurbsCurve -name "curveShape48" -parent "curve48";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle8006" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape8006" -parent "pSphere1Follicle8006";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.8125;
	setAttr ".parameterV" 0.0625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve49" -parent "pSphere1Follicle8006";
createNode nurbsCurve -name "curveShape49" -parent "curve49";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle8019" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape8019" -parent "pSphere1Follicle8019";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.8125;
	setAttr ".parameterV" 0.1875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve50" -parent "pSphere1Follicle8019";
createNode nurbsCurve -name "curveShape50" -parent "curve50";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle8031" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape8031" -parent "pSphere1Follicle8031";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.8125;
	setAttr ".parameterV" 0.3125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve51" -parent "pSphere1Follicle8031";
createNode nurbsCurve -name "curveShape51" -parent "curve51";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle8043" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape8043" -parent "pSphere1Follicle8043";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.8125;
	setAttr ".parameterV" 0.4375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve52" -parent "pSphere1Follicle8043";
createNode nurbsCurve -name "curveShape52" -parent "curve52";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle8056" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape8056" -parent "pSphere1Follicle8056";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.8125;
	setAttr ".parameterV" 0.5625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve53" -parent "pSphere1Follicle8056";
createNode nurbsCurve -name "curveShape53" -parent "curve53";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle8068" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape8068" -parent "pSphere1Follicle8068";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.8125;
	setAttr ".parameterV" 0.6875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve54" -parent "pSphere1Follicle8068";
createNode nurbsCurve -name "curveShape54" -parent "curve54";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle8080" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape8080" -parent "pSphere1Follicle8080";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.8125;
	setAttr ".parameterV" 0.8125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve55" -parent "pSphere1Follicle8080";
createNode nurbsCurve -name "curveShape55" -parent "curve55";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle8093" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape8093" -parent "pSphere1Follicle8093";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.8125;
	setAttr ".parameterV" 0.9375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve56" -parent "pSphere1Follicle8093";
createNode nurbsCurve -name "curveShape56" -parent "curve56";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle9306" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape9306" -parent "pSphere1Follicle9306";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.9375;
	setAttr ".parameterV" 0.0625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve57" -parent "pSphere1Follicle9306";
createNode nurbsCurve -name "curveShape57" -parent "curve57";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle9319" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape9319" -parent "pSphere1Follicle9319";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.9375;
	setAttr ".parameterV" 0.1875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve58" -parent "pSphere1Follicle9319";
createNode nurbsCurve -name "curveShape58" -parent "curve58";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle9331" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape9331" -parent "pSphere1Follicle9331";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.9375;
	setAttr ".parameterV" 0.3125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve59" -parent "pSphere1Follicle9331";
createNode nurbsCurve -name "curveShape59" -parent "curve59";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle9343" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape9343" -parent "pSphere1Follicle9343";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.9375;
	setAttr ".parameterV" 0.4375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve60" -parent "pSphere1Follicle9343";
createNode nurbsCurve -name "curveShape60" -parent "curve60";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle9356" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape9356" -parent "pSphere1Follicle9356";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.9375;
	setAttr ".parameterV" 0.5625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve61" -parent "pSphere1Follicle9356";
createNode nurbsCurve -name "curveShape61" -parent "curve61";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle9368" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape9368" -parent "pSphere1Follicle9368";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.9375;
	setAttr ".parameterV" 0.6875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve62" -parent "pSphere1Follicle9368";
createNode nurbsCurve -name "curveShape62" -parent "curve62";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle9380" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape9380" -parent "pSphere1Follicle9380";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.9375;
	setAttr ".parameterV" 0.8125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve63" -parent "pSphere1Follicle9380";
createNode nurbsCurve -name "curveShape63" -parent "curve63";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pSphere1Follicle9393" -parent "hairSystem1Follicles";
createNode follicle -name "pSphere1FollicleShape9393" -parent "pSphere1Follicle9393";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.9375;
	setAttr ".parameterV" 0.9375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve64" -parent "pSphere1Follicle9393";
createNode nurbsCurve -name "curveShape64" -parent "curve64";
	setAttr -keyable off ".visibility";
	setAttr ".intermediateObject" yes;
	setAttr ".cached" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.55555555560000003
		0 0 1.111111111
		0 0 1.6666666670000001
		0 0 2.2222222220000001
		0 0 2.7777777779999999
		0 0 3.3333333330000001
		0 0 3.888888889
		0 0 4.4444444440000002
		0 0 5
		;
createNode transform -name "pfxHair1";
createNode pfxHair -name "pfxHairShape1" -parent "pfxHair1";
	setAttr -keyable off ".visibility";
	setAttr ".displayPercent" 100;
	setAttr ".drawAsMesh" no;
createNode lightLinker -shared -name "lightLinker1";
	setAttr -size 5 ".link";
	setAttr -size 5 ".shadowLink";
createNode displayLayerManager -name "layerManager";
createNode displayLayer -name "defaultLayer";
createNode renderLayerManager -name "renderLayerManager";
createNode renderLayer -name "defaultRenderLayer";
	setAttr ".global" yes;
createNode polySphere -name "polySphere1";
	setAttr ".subdivisionsAxis" 40;
	setAttr ".subdivisionsHeight" 40;
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
createNode shadingEngine -name "aiStandard1SG";
	setAttr ".isHistoricallyInteresting" 0;
	setAttr ".renderableOnlySet" yes;
createNode materialInfo -name "materialInfo1";
createNode script -name "sceneConfigurationScriptNode";
	setAttr ".before" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".scriptType" 6;
createNode shadingEngine -name "surfaceShader1SG";
	setAttr ".isHistoricallyInteresting" 0;
	setAttr ".renderableOnlySet" yes;
createNode materialInfo -name "materialInfo2";
createNode aiStandard -name "aiStandard2";
createNode shadingEngine -name "aiStandard2SG";
	setAttr ".isHistoricallyInteresting" 0;
	setAttr ".renderableOnlySet" yes;
createNode materialInfo -name "materialInfo3";
select -noExpand :time1;
	setAttr ".outTime" 1;
	setAttr ".unwarpedTime" 1;
select -noExpand :renderPartition;
	setAttr -size 5 ".sets";
select -noExpand :initialShadingGroup;
	setAttr ".renderableOnlySet" yes;
select -noExpand :initialParticleSE;
	setAttr ".renderableOnlySet" yes;
select -noExpand :defaultShaderList1;
	setAttr -size 3 ".shaders";
select -noExpand :lightList1;
select -noExpand :postProcessList1;
	setAttr -size 2 ".postProcesses";
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
select -noExpand :defaultLightSet;
select -noExpand :hardwareRenderGlobals;
	setAttr ".colorTextureResolution" 256;
	setAttr ".bumpTextureResolution" 512;
select -noExpand :defaultHardwareRenderGlobals;
	setAttr ".filename" -type "string" "im";
	setAttr ".resolution" -type "string" "ntsc_4d 646 485 1.333";
connectAttr "polySphere1.output" "pSphereShape1.inMesh";
connectAttr ":time1.outTime" "hairSystemShape1.currentTime";
connectAttr "pSphere1FollicleShape606.outHair" "hairSystemShape1.inputHair[0]";
connectAttr "pSphere1FollicleShape619.outHair" "hairSystemShape1.inputHair[1]";
connectAttr "pSphere1FollicleShape631.outHair" "hairSystemShape1.inputHair[2]";
connectAttr "pSphere1FollicleShape643.outHair" "hairSystemShape1.inputHair[3]";
connectAttr "pSphere1FollicleShape656.outHair" "hairSystemShape1.inputHair[4]";
connectAttr "pSphere1FollicleShape668.outHair" "hairSystemShape1.inputHair[5]";
connectAttr "pSphere1FollicleShape680.outHair" "hairSystemShape1.inputHair[6]";
connectAttr "pSphere1FollicleShape693.outHair" "hairSystemShape1.inputHair[7]";
connectAttr "pSphere1FollicleShape1906.outHair" "hairSystemShape1.inputHair[8]";
connectAttr "pSphere1FollicleShape1919.outHair" "hairSystemShape1.inputHair[9]";
connectAttr "pSphere1FollicleShape1931.outHair" "hairSystemShape1.inputHair[10]"
		;
connectAttr "pSphere1FollicleShape1943.outHair" "hairSystemShape1.inputHair[11]"
		;
connectAttr "pSphere1FollicleShape1956.outHair" "hairSystemShape1.inputHair[12]"
		;
connectAttr "pSphere1FollicleShape1968.outHair" "hairSystemShape1.inputHair[13]"
		;
connectAttr "pSphere1FollicleShape1980.outHair" "hairSystemShape1.inputHair[14]"
		;
connectAttr "pSphere1FollicleShape1993.outHair" "hairSystemShape1.inputHair[15]"
		;
connectAttr "pSphere1FollicleShape3106.outHair" "hairSystemShape1.inputHair[16]"
		;
connectAttr "pSphere1FollicleShape3119.outHair" "hairSystemShape1.inputHair[17]"
		;
connectAttr "pSphere1FollicleShape3131.outHair" "hairSystemShape1.inputHair[18]"
		;
connectAttr "pSphere1FollicleShape3143.outHair" "hairSystemShape1.inputHair[19]"
		;
connectAttr "pSphere1FollicleShape3156.outHair" "hairSystemShape1.inputHair[20]"
		;
connectAttr "pSphere1FollicleShape3168.outHair" "hairSystemShape1.inputHair[21]"
		;
connectAttr "pSphere1FollicleShape3180.outHair" "hairSystemShape1.inputHair[22]"
		;
connectAttr "pSphere1FollicleShape3193.outHair" "hairSystemShape1.inputHair[23]"
		;
connectAttr "pSphere1FollicleShape4306.outHair" "hairSystemShape1.inputHair[24]"
		;
connectAttr "pSphere1FollicleShape4319.outHair" "hairSystemShape1.inputHair[25]"
		;
connectAttr "pSphere1FollicleShape4331.outHair" "hairSystemShape1.inputHair[26]"
		;
connectAttr "pSphere1FollicleShape4343.outHair" "hairSystemShape1.inputHair[27]"
		;
connectAttr "pSphere1FollicleShape4356.outHair" "hairSystemShape1.inputHair[28]"
		;
connectAttr "pSphere1FollicleShape4368.outHair" "hairSystemShape1.inputHair[29]"
		;
connectAttr "pSphere1FollicleShape4380.outHair" "hairSystemShape1.inputHair[30]"
		;
connectAttr "pSphere1FollicleShape4393.outHair" "hairSystemShape1.inputHair[31]"
		;
connectAttr "pSphere1FollicleShape5606.outHair" "hairSystemShape1.inputHair[32]"
		;
connectAttr "pSphere1FollicleShape5619.outHair" "hairSystemShape1.inputHair[33]"
		;
connectAttr "pSphere1FollicleShape5631.outHair" "hairSystemShape1.inputHair[34]"
		;
connectAttr "pSphere1FollicleShape5643.outHair" "hairSystemShape1.inputHair[35]"
		;
connectAttr "pSphere1FollicleShape5656.outHair" "hairSystemShape1.inputHair[36]"
		;
connectAttr "pSphere1FollicleShape5668.outHair" "hairSystemShape1.inputHair[37]"
		;
connectAttr "pSphere1FollicleShape5680.outHair" "hairSystemShape1.inputHair[38]"
		;
connectAttr "pSphere1FollicleShape5693.outHair" "hairSystemShape1.inputHair[39]"
		;
connectAttr "pSphere1FollicleShape6806.outHair" "hairSystemShape1.inputHair[40]"
		;
connectAttr "pSphere1FollicleShape6819.outHair" "hairSystemShape1.inputHair[41]"
		;
connectAttr "pSphere1FollicleShape6831.outHair" "hairSystemShape1.inputHair[42]"
		;
connectAttr "pSphere1FollicleShape6843.outHair" "hairSystemShape1.inputHair[43]"
		;
connectAttr "pSphere1FollicleShape6856.outHair" "hairSystemShape1.inputHair[44]"
		;
connectAttr "pSphere1FollicleShape6868.outHair" "hairSystemShape1.inputHair[45]"
		;
connectAttr "pSphere1FollicleShape6880.outHair" "hairSystemShape1.inputHair[46]"
		;
connectAttr "pSphere1FollicleShape6893.outHair" "hairSystemShape1.inputHair[47]"
		;
connectAttr "pSphere1FollicleShape8006.outHair" "hairSystemShape1.inputHair[48]"
		;
connectAttr "pSphere1FollicleShape8019.outHair" "hairSystemShape1.inputHair[49]"
		;
connectAttr "pSphere1FollicleShape8031.outHair" "hairSystemShape1.inputHair[50]"
		;
connectAttr "pSphere1FollicleShape8043.outHair" "hairSystemShape1.inputHair[51]"
		;
connectAttr "pSphere1FollicleShape8056.outHair" "hairSystemShape1.inputHair[52]"
		;
connectAttr "pSphere1FollicleShape8068.outHair" "hairSystemShape1.inputHair[53]"
		;
connectAttr "pSphere1FollicleShape8080.outHair" "hairSystemShape1.inputHair[54]"
		;
connectAttr "pSphere1FollicleShape8093.outHair" "hairSystemShape1.inputHair[55]"
		;
connectAttr "pSphere1FollicleShape9306.outHair" "hairSystemShape1.inputHair[56]"
		;
connectAttr "pSphere1FollicleShape9319.outHair" "hairSystemShape1.inputHair[57]"
		;
connectAttr "pSphere1FollicleShape9331.outHair" "hairSystemShape1.inputHair[58]"
		;
connectAttr "pSphere1FollicleShape9343.outHair" "hairSystemShape1.inputHair[59]"
		;
connectAttr "pSphere1FollicleShape9356.outHair" "hairSystemShape1.inputHair[60]"
		;
connectAttr "pSphere1FollicleShape9368.outHair" "hairSystemShape1.inputHair[61]"
		;
connectAttr "pSphere1FollicleShape9380.outHair" "hairSystemShape1.inputHair[62]"
		;
connectAttr "pSphere1FollicleShape9393.outHair" "hairSystemShape1.inputHair[63]"
		;
connectAttr "pSphere1FollicleShape606.outTranslate" "pSphere1Follicle606.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape606.outRotate" "pSphere1Follicle606.rotate" -lock
		 on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape606.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape606.inputMesh";
connectAttr "curveShape1.worldSpace" "pSphere1FollicleShape606.startPosition";
connectAttr "pSphere1FollicleShape619.outTranslate" "pSphere1Follicle619.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape619.outRotate" "pSphere1Follicle619.rotate" -lock
		 on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape619.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape619.inputMesh";
connectAttr "curveShape2.worldSpace" "pSphere1FollicleShape619.startPosition";
connectAttr "pSphere1FollicleShape631.outTranslate" "pSphere1Follicle631.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape631.outRotate" "pSphere1Follicle631.rotate" -lock
		 on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape631.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape631.inputMesh";
connectAttr "curveShape3.worldSpace" "pSphere1FollicleShape631.startPosition";
connectAttr "pSphere1FollicleShape643.outTranslate" "pSphere1Follicle643.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape643.outRotate" "pSphere1Follicle643.rotate" -lock
		 on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape643.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape643.inputMesh";
connectAttr "curveShape4.worldSpace" "pSphere1FollicleShape643.startPosition";
connectAttr "pSphere1FollicleShape656.outTranslate" "pSphere1Follicle656.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape656.outRotate" "pSphere1Follicle656.rotate" -lock
		 on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape656.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape656.inputMesh";
connectAttr "curveShape5.worldSpace" "pSphere1FollicleShape656.startPosition";
connectAttr "pSphere1FollicleShape668.outTranslate" "pSphere1Follicle668.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape668.outRotate" "pSphere1Follicle668.rotate" -lock
		 on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape668.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape668.inputMesh";
connectAttr "curveShape6.worldSpace" "pSphere1FollicleShape668.startPosition";
connectAttr "pSphere1FollicleShape680.outTranslate" "pSphere1Follicle680.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape680.outRotate" "pSphere1Follicle680.rotate" -lock
		 on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape680.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape680.inputMesh";
connectAttr "curveShape7.worldSpace" "pSphere1FollicleShape680.startPosition";
connectAttr "pSphere1FollicleShape693.outTranslate" "pSphere1Follicle693.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape693.outRotate" "pSphere1Follicle693.rotate" -lock
		 on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape693.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape693.inputMesh";
connectAttr "curveShape8.worldSpace" "pSphere1FollicleShape693.startPosition";
connectAttr "pSphere1FollicleShape1906.outTranslate" "pSphere1Follicle1906.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape1906.outRotate" "pSphere1Follicle1906.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape1906.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape1906.inputMesh";
connectAttr "curveShape9.worldSpace" "pSphere1FollicleShape1906.startPosition";
connectAttr "pSphere1FollicleShape1919.outTranslate" "pSphere1Follicle1919.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape1919.outRotate" "pSphere1Follicle1919.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape1919.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape1919.inputMesh";
connectAttr "curveShape10.worldSpace" "pSphere1FollicleShape1919.startPosition";
connectAttr "pSphere1FollicleShape1931.outTranslate" "pSphere1Follicle1931.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape1931.outRotate" "pSphere1Follicle1931.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape1931.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape1931.inputMesh";
connectAttr "curveShape11.worldSpace" "pSphere1FollicleShape1931.startPosition";
connectAttr "pSphere1FollicleShape1943.outTranslate" "pSphere1Follicle1943.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape1943.outRotate" "pSphere1Follicle1943.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape1943.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape1943.inputMesh";
connectAttr "curveShape12.worldSpace" "pSphere1FollicleShape1943.startPosition";
connectAttr "pSphere1FollicleShape1956.outTranslate" "pSphere1Follicle1956.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape1956.outRotate" "pSphere1Follicle1956.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape1956.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape1956.inputMesh";
connectAttr "curveShape13.worldSpace" "pSphere1FollicleShape1956.startPosition";
connectAttr "pSphere1FollicleShape1968.outTranslate" "pSphere1Follicle1968.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape1968.outRotate" "pSphere1Follicle1968.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape1968.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape1968.inputMesh";
connectAttr "curveShape14.worldSpace" "pSphere1FollicleShape1968.startPosition";
connectAttr "pSphere1FollicleShape1980.outTranslate" "pSphere1Follicle1980.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape1980.outRotate" "pSphere1Follicle1980.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape1980.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape1980.inputMesh";
connectAttr "curveShape15.worldSpace" "pSphere1FollicleShape1980.startPosition";
connectAttr "pSphere1FollicleShape1993.outTranslate" "pSphere1Follicle1993.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape1993.outRotate" "pSphere1Follicle1993.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape1993.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape1993.inputMesh";
connectAttr "curveShape16.worldSpace" "pSphere1FollicleShape1993.startPosition";
connectAttr "pSphere1FollicleShape3106.outTranslate" "pSphere1Follicle3106.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape3106.outRotate" "pSphere1Follicle3106.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape3106.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape3106.inputMesh";
connectAttr "curveShape17.worldSpace" "pSphere1FollicleShape3106.startPosition";
connectAttr "pSphere1FollicleShape3119.outTranslate" "pSphere1Follicle3119.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape3119.outRotate" "pSphere1Follicle3119.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape3119.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape3119.inputMesh";
connectAttr "curveShape18.worldSpace" "pSphere1FollicleShape3119.startPosition";
connectAttr "pSphere1FollicleShape3131.outTranslate" "pSphere1Follicle3131.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape3131.outRotate" "pSphere1Follicle3131.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape3131.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape3131.inputMesh";
connectAttr "curveShape19.worldSpace" "pSphere1FollicleShape3131.startPosition";
connectAttr "pSphere1FollicleShape3143.outTranslate" "pSphere1Follicle3143.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape3143.outRotate" "pSphere1Follicle3143.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape3143.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape3143.inputMesh";
connectAttr "curveShape20.worldSpace" "pSphere1FollicleShape3143.startPosition";
connectAttr "pSphere1FollicleShape3156.outTranslate" "pSphere1Follicle3156.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape3156.outRotate" "pSphere1Follicle3156.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape3156.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape3156.inputMesh";
connectAttr "curveShape21.worldSpace" "pSphere1FollicleShape3156.startPosition";
connectAttr "pSphere1FollicleShape3168.outTranslate" "pSphere1Follicle3168.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape3168.outRotate" "pSphere1Follicle3168.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape3168.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape3168.inputMesh";
connectAttr "curveShape22.worldSpace" "pSphere1FollicleShape3168.startPosition";
connectAttr "pSphere1FollicleShape3180.outTranslate" "pSphere1Follicle3180.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape3180.outRotate" "pSphere1Follicle3180.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape3180.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape3180.inputMesh";
connectAttr "curveShape23.worldSpace" "pSphere1FollicleShape3180.startPosition";
connectAttr "pSphere1FollicleShape3193.outTranslate" "pSphere1Follicle3193.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape3193.outRotate" "pSphere1Follicle3193.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape3193.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape3193.inputMesh";
connectAttr "curveShape24.worldSpace" "pSphere1FollicleShape3193.startPosition";
connectAttr "pSphere1FollicleShape4306.outTranslate" "pSphere1Follicle4306.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape4306.outRotate" "pSphere1Follicle4306.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape4306.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape4306.inputMesh";
connectAttr "curveShape25.worldSpace" "pSphere1FollicleShape4306.startPosition";
connectAttr "pSphere1FollicleShape4319.outTranslate" "pSphere1Follicle4319.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape4319.outRotate" "pSphere1Follicle4319.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape4319.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape4319.inputMesh";
connectAttr "curveShape26.worldSpace" "pSphere1FollicleShape4319.startPosition";
connectAttr "pSphere1FollicleShape4331.outTranslate" "pSphere1Follicle4331.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape4331.outRotate" "pSphere1Follicle4331.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape4331.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape4331.inputMesh";
connectAttr "curveShape27.worldSpace" "pSphere1FollicleShape4331.startPosition";
connectAttr "pSphere1FollicleShape4343.outTranslate" "pSphere1Follicle4343.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape4343.outRotate" "pSphere1Follicle4343.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape4343.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape4343.inputMesh";
connectAttr "curveShape28.worldSpace" "pSphere1FollicleShape4343.startPosition";
connectAttr "pSphere1FollicleShape4356.outTranslate" "pSphere1Follicle4356.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape4356.outRotate" "pSphere1Follicle4356.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape4356.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape4356.inputMesh";
connectAttr "curveShape29.worldSpace" "pSphere1FollicleShape4356.startPosition";
connectAttr "pSphere1FollicleShape4368.outTranslate" "pSphere1Follicle4368.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape4368.outRotate" "pSphere1Follicle4368.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape4368.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape4368.inputMesh";
connectAttr "curveShape30.worldSpace" "pSphere1FollicleShape4368.startPosition";
connectAttr "pSphere1FollicleShape4380.outTranslate" "pSphere1Follicle4380.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape4380.outRotate" "pSphere1Follicle4380.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape4380.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape4380.inputMesh";
connectAttr "curveShape31.worldSpace" "pSphere1FollicleShape4380.startPosition";
connectAttr "pSphere1FollicleShape4393.outTranslate" "pSphere1Follicle4393.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape4393.outRotate" "pSphere1Follicle4393.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape4393.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape4393.inputMesh";
connectAttr "curveShape32.worldSpace" "pSphere1FollicleShape4393.startPosition";
connectAttr "pSphere1FollicleShape5606.outTranslate" "pSphere1Follicle5606.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape5606.outRotate" "pSphere1Follicle5606.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape5606.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape5606.inputMesh";
connectAttr "curveShape33.worldSpace" "pSphere1FollicleShape5606.startPosition";
connectAttr "pSphere1FollicleShape5619.outTranslate" "pSphere1Follicle5619.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape5619.outRotate" "pSphere1Follicle5619.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape5619.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape5619.inputMesh";
connectAttr "curveShape34.worldSpace" "pSphere1FollicleShape5619.startPosition";
connectAttr "pSphere1FollicleShape5631.outTranslate" "pSphere1Follicle5631.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape5631.outRotate" "pSphere1Follicle5631.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape5631.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape5631.inputMesh";
connectAttr "curveShape35.worldSpace" "pSphere1FollicleShape5631.startPosition";
connectAttr "pSphere1FollicleShape5643.outTranslate" "pSphere1Follicle5643.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape5643.outRotate" "pSphere1Follicle5643.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape5643.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape5643.inputMesh";
connectAttr "curveShape36.worldSpace" "pSphere1FollicleShape5643.startPosition";
connectAttr "pSphere1FollicleShape5656.outTranslate" "pSphere1Follicle5656.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape5656.outRotate" "pSphere1Follicle5656.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape5656.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape5656.inputMesh";
connectAttr "curveShape37.worldSpace" "pSphere1FollicleShape5656.startPosition";
connectAttr "pSphere1FollicleShape5668.outTranslate" "pSphere1Follicle5668.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape5668.outRotate" "pSphere1Follicle5668.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape5668.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape5668.inputMesh";
connectAttr "curveShape38.worldSpace" "pSphere1FollicleShape5668.startPosition";
connectAttr "pSphere1FollicleShape5680.outTranslate" "pSphere1Follicle5680.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape5680.outRotate" "pSphere1Follicle5680.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape5680.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape5680.inputMesh";
connectAttr "curveShape39.worldSpace" "pSphere1FollicleShape5680.startPosition";
connectAttr "pSphere1FollicleShape5693.outTranslate" "pSphere1Follicle5693.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape5693.outRotate" "pSphere1Follicle5693.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape5693.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape5693.inputMesh";
connectAttr "curveShape40.worldSpace" "pSphere1FollicleShape5693.startPosition";
connectAttr "pSphere1FollicleShape6806.outTranslate" "pSphere1Follicle6806.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape6806.outRotate" "pSphere1Follicle6806.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape6806.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape6806.inputMesh";
connectAttr "curveShape41.worldSpace" "pSphere1FollicleShape6806.startPosition";
connectAttr "pSphere1FollicleShape6819.outTranslate" "pSphere1Follicle6819.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape6819.outRotate" "pSphere1Follicle6819.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape6819.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape6819.inputMesh";
connectAttr "curveShape42.worldSpace" "pSphere1FollicleShape6819.startPosition";
connectAttr "pSphere1FollicleShape6831.outTranslate" "pSphere1Follicle6831.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape6831.outRotate" "pSphere1Follicle6831.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape6831.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape6831.inputMesh";
connectAttr "curveShape43.worldSpace" "pSphere1FollicleShape6831.startPosition";
connectAttr "pSphere1FollicleShape6843.outTranslate" "pSphere1Follicle6843.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape6843.outRotate" "pSphere1Follicle6843.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape6843.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape6843.inputMesh";
connectAttr "curveShape44.worldSpace" "pSphere1FollicleShape6843.startPosition";
connectAttr "pSphere1FollicleShape6856.outTranslate" "pSphere1Follicle6856.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape6856.outRotate" "pSphere1Follicle6856.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape6856.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape6856.inputMesh";
connectAttr "curveShape45.worldSpace" "pSphere1FollicleShape6856.startPosition";
connectAttr "pSphere1FollicleShape6868.outTranslate" "pSphere1Follicle6868.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape6868.outRotate" "pSphere1Follicle6868.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape6868.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape6868.inputMesh";
connectAttr "curveShape46.worldSpace" "pSphere1FollicleShape6868.startPosition";
connectAttr "pSphere1FollicleShape6880.outTranslate" "pSphere1Follicle6880.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape6880.outRotate" "pSphere1Follicle6880.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape6880.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape6880.inputMesh";
connectAttr "curveShape47.worldSpace" "pSphere1FollicleShape6880.startPosition";
connectAttr "pSphere1FollicleShape6893.outTranslate" "pSphere1Follicle6893.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape6893.outRotate" "pSphere1Follicle6893.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape6893.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape6893.inputMesh";
connectAttr "curveShape48.worldSpace" "pSphere1FollicleShape6893.startPosition";
connectAttr "pSphere1FollicleShape8006.outTranslate" "pSphere1Follicle8006.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape8006.outRotate" "pSphere1Follicle8006.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape8006.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape8006.inputMesh";
connectAttr "curveShape49.worldSpace" "pSphere1FollicleShape8006.startPosition";
connectAttr "pSphere1FollicleShape8019.outTranslate" "pSphere1Follicle8019.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape8019.outRotate" "pSphere1Follicle8019.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape8019.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape8019.inputMesh";
connectAttr "curveShape50.worldSpace" "pSphere1FollicleShape8019.startPosition";
connectAttr "pSphere1FollicleShape8031.outTranslate" "pSphere1Follicle8031.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape8031.outRotate" "pSphere1Follicle8031.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape8031.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape8031.inputMesh";
connectAttr "curveShape51.worldSpace" "pSphere1FollicleShape8031.startPosition";
connectAttr "pSphere1FollicleShape8043.outTranslate" "pSphere1Follicle8043.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape8043.outRotate" "pSphere1Follicle8043.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape8043.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape8043.inputMesh";
connectAttr "curveShape52.worldSpace" "pSphere1FollicleShape8043.startPosition";
connectAttr "pSphere1FollicleShape8056.outTranslate" "pSphere1Follicle8056.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape8056.outRotate" "pSphere1Follicle8056.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape8056.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape8056.inputMesh";
connectAttr "curveShape53.worldSpace" "pSphere1FollicleShape8056.startPosition";
connectAttr "pSphere1FollicleShape8068.outTranslate" "pSphere1Follicle8068.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape8068.outRotate" "pSphere1Follicle8068.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape8068.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape8068.inputMesh";
connectAttr "curveShape54.worldSpace" "pSphere1FollicleShape8068.startPosition";
connectAttr "pSphere1FollicleShape8080.outTranslate" "pSphere1Follicle8080.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape8080.outRotate" "pSphere1Follicle8080.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape8080.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape8080.inputMesh";
connectAttr "curveShape55.worldSpace" "pSphere1FollicleShape8080.startPosition";
connectAttr "pSphere1FollicleShape8093.outTranslate" "pSphere1Follicle8093.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape8093.outRotate" "pSphere1Follicle8093.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape8093.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape8093.inputMesh";
connectAttr "curveShape56.worldSpace" "pSphere1FollicleShape8093.startPosition";
connectAttr "pSphere1FollicleShape9306.outTranslate" "pSphere1Follicle9306.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape9306.outRotate" "pSphere1Follicle9306.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape9306.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape9306.inputMesh";
connectAttr "curveShape57.worldSpace" "pSphere1FollicleShape9306.startPosition";
connectAttr "pSphere1FollicleShape9319.outTranslate" "pSphere1Follicle9319.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape9319.outRotate" "pSphere1Follicle9319.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape9319.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape9319.inputMesh";
connectAttr "curveShape58.worldSpace" "pSphere1FollicleShape9319.startPosition";
connectAttr "pSphere1FollicleShape9331.outTranslate" "pSphere1Follicle9331.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape9331.outRotate" "pSphere1Follicle9331.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape9331.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape9331.inputMesh";
connectAttr "curveShape59.worldSpace" "pSphere1FollicleShape9331.startPosition";
connectAttr "pSphere1FollicleShape9343.outTranslate" "pSphere1Follicle9343.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape9343.outRotate" "pSphere1Follicle9343.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape9343.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape9343.inputMesh";
connectAttr "curveShape60.worldSpace" "pSphere1FollicleShape9343.startPosition";
connectAttr "pSphere1FollicleShape9356.outTranslate" "pSphere1Follicle9356.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape9356.outRotate" "pSphere1Follicle9356.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape9356.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape9356.inputMesh";
connectAttr "curveShape61.worldSpace" "pSphere1FollicleShape9356.startPosition";
connectAttr "pSphere1FollicleShape9368.outTranslate" "pSphere1Follicle9368.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape9368.outRotate" "pSphere1Follicle9368.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape9368.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape9368.inputMesh";
connectAttr "curveShape62.worldSpace" "pSphere1FollicleShape9368.startPosition";
connectAttr "pSphere1FollicleShape9380.outTranslate" "pSphere1Follicle9380.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape9380.outRotate" "pSphere1Follicle9380.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape9380.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape9380.inputMesh";
connectAttr "curveShape63.worldSpace" "pSphere1FollicleShape9380.startPosition";
connectAttr "pSphere1FollicleShape9393.outTranslate" "pSphere1Follicle9393.translate"
		 -lock on;
connectAttr "pSphere1FollicleShape9393.outRotate" "pSphere1Follicle9393.rotate" 
		-lock on;
connectAttr "pSphereShape1.worldMatrix" "pSphere1FollicleShape9393.inputWorldMatrix"
		;
connectAttr "pSphereShape1.outMesh" "pSphere1FollicleShape9393.inputMesh";
connectAttr "curveShape64.worldSpace" "pSphere1FollicleShape9393.startPosition";
connectAttr "hairSystemShape1.outputRenderHairs" "pfxHairShape1.renderHairs";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "surfaceShader1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "surfaceShader1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard2SG.message" ":defaultLightSet.message";
connectAttr "layerManager.displayLayerId[0]" "defaultLayer.identification";
connectAttr "renderLayerManager.renderLayerId[0]" "defaultRenderLayer.identification"
		;
connectAttr ":time1.outTime" ":defaultArnoldRenderOptions.AA_seed";
connectAttr "aiStandard1SG.message" "materialInfo1.shadingGroup";
connectAttr "surfaceShader1SG.message" "materialInfo2.shadingGroup";
connectAttr "aiStandard2.outColor" "aiStandard2SG.surfaceShader";
connectAttr "pSphereShape1.instObjGroups" "aiStandard2SG.dagSetMembers" -nextAvailable
		;
connectAttr "aiStandard2SG.message" "materialInfo3.shadingGroup";
connectAttr "aiStandard2.message" "materialInfo3.material";
connectAttr "aiStandard2.message" "materialInfo3.texture" -nextAvailable;
connectAttr "aiStandard1SG.partition" ":renderPartition.sets" -nextAvailable;
connectAttr "surfaceShader1SG.partition" ":renderPartition.sets" -nextAvailable;
connectAttr "aiStandard2SG.partition" ":renderPartition.sets" -nextAvailable;
connectAttr "aiStandard2.message" ":defaultShaderList1.shaders" -nextAvailable;
connectAttr "pointLightShape1.lightData" ":lightList1.lights" -nextAvailable;
connectAttr "pointLight1.instObjGroups" ":defaultLightSet.dagSetMembers" -nextAvailable
		;
// End of test.ma
