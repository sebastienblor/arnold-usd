//Maya ASCII 2011 scene
//Name: test.ma
//Last modified: Mon, Jul 18, 2011 06:14:35 PM
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
	setAttr ".translate" -type "double3" 16.722772317035005 14.739098013784576 17.196404143463287 ;
	setAttr ".rotate" -type "double3" -35.738352729602418 44.2 -2.2182365887199574e-15 ;
	setAttr ".rotatePivot" -type "double3" -3.7305311785434307e-15 4.1302409908546105e-15 
		-7.1054273576010019e-15 ;
	setAttr ".rotatePivotTranslate" -type "double3" -1.4878539151390208e-14 -1.7318315130879441e-15 
		3.0664719809111371e-15 ;
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
	setAttr ".centerOfInterest" 28.024519899009473;
	setAttr ".orthographicWidth" 30;
	setAttr ".imageName" -type "string" "persp";
	setAttr ".depthName" -type "string" "persp_depth";
	setAttr ".maskName" -type "string" "persp_mask";
	setAttr ".homeCommand" -type "string" "viewSet -p %camera";
	setAttr ".displayResolution" yes;
	setAttr -keyable on ".aiTranslator" -type "string" "perspective";
	setAttr -keyable on ".arnoldTranslator" -type "string" "orthographic";
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
	setAttr ".centerOfInterest" 100.1;
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
	setAttr ".centerOfInterest" 100.1;
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
	setAttr ".centerOfInterest" 100.1;
	setAttr ".orthographicWidth" 30;
	setAttr ".imageName" -type "string" "side";
	setAttr ".depthName" -type "string" "side_depth";
	setAttr ".maskName" -type "string" "side_mask";
	setAttr ".homeCommand" -type "string" "viewSet -s %camera";
	setAttr ".orthographic" yes;
	setAttr -keyable on ".aiTranslator" -type "string" "orthographic";
	setAttr -keyable on ".arnoldTranslator" -type "string" "orthographic";
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
	addAttr -cachedInternally true -keyable true -shortName "sss_use_gi" -longName "sssUseGi" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "sss_max_samples" -longName "sssMaxSamples" 
		-defaultValue 100000 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "sss_sample_spacing" -longName "sssSampleSpacing" 
		-defaultValue 0.10000000149011612 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "self_shadows" -longName "selfShadows" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "opaque" -longName "opaque" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "vid" -longName "visibleInDiffuse" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "vig" -longName "visibleInGlossy" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "min_pixel_width" -longName "minPixelWidth" 
		-attributeType "float";
	addAttr -cachedInternally true -shortName "mode" -longName "mode" -minValue 0 -maxValue 
		1 -enumName "ribbon:thick" -attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "override_hair" -longName "overrideHair" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "hair_shader" -longName "hairShader" 
		-attributeType "message";
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
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr ".clumpCurl[0]"  0 0.5 1;
	setAttr ".clumpFlatness[0]"  0 0 1;
	setAttr ".hairWidth" 0.3;
	setAttr -size 2 ".hairWidthScale[0:1]"  0.80000001 1 3 1 0.2 3;
	setAttr ".hairColor" -type "float3" 1 1 1 ;
	setAttr -size 3 ".hairColorScale";
	setAttr ".hairColorScale[0].hairColorScale_Position" 0;
	setAttr ".hairColorScale[0].hairColorScale_Color" -type "float3" 0.5 0.5 0.5 ;
	setAttr ".hairColorScale[0].hairColorScale_Interp" 3;
	setAttr ".hairColorScale[1].hairColorScale_Position" 0.30000001192092896;
	setAttr ".hairColorScale[1].hairColorScale_Color" -type "float3" 0.80000001 0.80000001 
		0.80000001 ;
	setAttr ".hairColorScale[1].hairColorScale_Interp" 3;
	setAttr ".hairColorScale[2].hairColorScale_Position" 1;
	setAttr ".hairColorScale[2].hairColorScale_Color" -type "float3" 1 1 1 ;
	setAttr ".hairColorScale[2].hairColorScale_Interp" 3;
	setAttr ".hairsPerClump" 1;
	setAttr ".specularColor" -type "float3" 1 1 1 ;
	setAttr ".displacementScale[0]"  0 1 1;
createNode transform -name "hairSystem1Follicles";
createNode transform -name "pPlane1Follicle606" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape606" -parent "pPlane1Follicle606";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.0625;
	setAttr ".parameterV" 0.0625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve1" -parent "pPlane1Follicle606";
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
createNode transform -name "pPlane1Follicle619" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape619" -parent "pPlane1Follicle619";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.0625;
	setAttr ".parameterV" 0.1875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve2" -parent "pPlane1Follicle619";
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
createNode transform -name "pPlane1Follicle631" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape631" -parent "pPlane1Follicle631";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.0625;
	setAttr ".parameterV" 0.3125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve3" -parent "pPlane1Follicle631";
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
createNode transform -name "pPlane1Follicle643" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape643" -parent "pPlane1Follicle643";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.0625;
	setAttr ".parameterV" 0.4375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve4" -parent "pPlane1Follicle643";
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
createNode transform -name "pPlane1Follicle656" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape656" -parent "pPlane1Follicle656";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.0625;
	setAttr ".parameterV" 0.5625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve5" -parent "pPlane1Follicle656";
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
createNode transform -name "pPlane1Follicle668" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape668" -parent "pPlane1Follicle668";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.0625;
	setAttr ".parameterV" 0.6875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve6" -parent "pPlane1Follicle668";
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
createNode transform -name "pPlane1Follicle680" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape680" -parent "pPlane1Follicle680";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.0625;
	setAttr ".parameterV" 0.8125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve7" -parent "pPlane1Follicle680";
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
createNode transform -name "pPlane1Follicle693" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape693" -parent "pPlane1Follicle693";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.0625;
	setAttr ".parameterV" 0.9375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve8" -parent "pPlane1Follicle693";
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
createNode transform -name "pPlane1Follicle1906" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape1906" -parent "pPlane1Follicle1906";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.1875;
	setAttr ".parameterV" 0.0625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve9" -parent "pPlane1Follicle1906";
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
createNode transform -name "pPlane1Follicle1919" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape1919" -parent "pPlane1Follicle1919";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.1875;
	setAttr ".parameterV" 0.1875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve10" -parent "pPlane1Follicle1919";
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
createNode transform -name "pPlane1Follicle1931" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape1931" -parent "pPlane1Follicle1931";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.1875;
	setAttr ".parameterV" 0.3125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve11" -parent "pPlane1Follicle1931";
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
createNode transform -name "pPlane1Follicle1943" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape1943" -parent "pPlane1Follicle1943";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.1875;
	setAttr ".parameterV" 0.4375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve12" -parent "pPlane1Follicle1943";
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
createNode transform -name "pPlane1Follicle1956" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape1956" -parent "pPlane1Follicle1956";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.1875;
	setAttr ".parameterV" 0.5625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve13" -parent "pPlane1Follicle1956";
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
createNode transform -name "pPlane1Follicle1968" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape1968" -parent "pPlane1Follicle1968";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.1875;
	setAttr ".parameterV" 0.6875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve14" -parent "pPlane1Follicle1968";
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
createNode transform -name "pPlane1Follicle1980" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape1980" -parent "pPlane1Follicle1980";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.1875;
	setAttr ".parameterV" 0.8125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve15" -parent "pPlane1Follicle1980";
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
createNode transform -name "pPlane1Follicle1993" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape1993" -parent "pPlane1Follicle1993";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.1875;
	setAttr ".parameterV" 0.9375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve16" -parent "pPlane1Follicle1993";
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
createNode transform -name "pPlane1Follicle3106" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape3106" -parent "pPlane1Follicle3106";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.3125;
	setAttr ".parameterV" 0.0625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve17" -parent "pPlane1Follicle3106";
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
createNode transform -name "pPlane1Follicle3119" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape3119" -parent "pPlane1Follicle3119";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.3125;
	setAttr ".parameterV" 0.1875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve18" -parent "pPlane1Follicle3119";
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
createNode transform -name "pPlane1Follicle3131" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape3131" -parent "pPlane1Follicle3131";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.3125;
	setAttr ".parameterV" 0.3125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve19" -parent "pPlane1Follicle3131";
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
createNode transform -name "pPlane1Follicle3143" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape3143" -parent "pPlane1Follicle3143";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.3125;
	setAttr ".parameterV" 0.4375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve20" -parent "pPlane1Follicle3143";
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
createNode transform -name "pPlane1Follicle3156" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape3156" -parent "pPlane1Follicle3156";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.3125;
	setAttr ".parameterV" 0.5625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve21" -parent "pPlane1Follicle3156";
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
createNode transform -name "pPlane1Follicle3168" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape3168" -parent "pPlane1Follicle3168";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.3125;
	setAttr ".parameterV" 0.6875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve22" -parent "pPlane1Follicle3168";
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
createNode transform -name "pPlane1Follicle3180" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape3180" -parent "pPlane1Follicle3180";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.3125;
	setAttr ".parameterV" 0.8125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve23" -parent "pPlane1Follicle3180";
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
createNode transform -name "pPlane1Follicle3193" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape3193" -parent "pPlane1Follicle3193";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.3125;
	setAttr ".parameterV" 0.9375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve24" -parent "pPlane1Follicle3193";
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
createNode transform -name "pPlane1Follicle4306" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape4306" -parent "pPlane1Follicle4306";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.4375;
	setAttr ".parameterV" 0.0625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve25" -parent "pPlane1Follicle4306";
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
createNode transform -name "pPlane1Follicle4319" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape4319" -parent "pPlane1Follicle4319";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.4375;
	setAttr ".parameterV" 0.1875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve26" -parent "pPlane1Follicle4319";
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
createNode transform -name "pPlane1Follicle4331" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape4331" -parent "pPlane1Follicle4331";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.4375;
	setAttr ".parameterV" 0.3125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve27" -parent "pPlane1Follicle4331";
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
createNode transform -name "pPlane1Follicle4343" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape4343" -parent "pPlane1Follicle4343";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.4375;
	setAttr ".parameterV" 0.4375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve28" -parent "pPlane1Follicle4343";
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
createNode transform -name "pPlane1Follicle4356" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape4356" -parent "pPlane1Follicle4356";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.4375;
	setAttr ".parameterV" 0.5625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve29" -parent "pPlane1Follicle4356";
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
createNode transform -name "pPlane1Follicle4368" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape4368" -parent "pPlane1Follicle4368";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.4375;
	setAttr ".parameterV" 0.6875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve30" -parent "pPlane1Follicle4368";
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
createNode transform -name "pPlane1Follicle4380" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape4380" -parent "pPlane1Follicle4380";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.4375;
	setAttr ".parameterV" 0.8125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve31" -parent "pPlane1Follicle4380";
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
createNode transform -name "pPlane1Follicle4393" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape4393" -parent "pPlane1Follicle4393";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.4375;
	setAttr ".parameterV" 0.9375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve32" -parent "pPlane1Follicle4393";
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
createNode transform -name "pPlane1Follicle5606" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape5606" -parent "pPlane1Follicle5606";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.5625;
	setAttr ".parameterV" 0.0625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve33" -parent "pPlane1Follicle5606";
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
createNode transform -name "pPlane1Follicle5619" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape5619" -parent "pPlane1Follicle5619";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.5625;
	setAttr ".parameterV" 0.1875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve34" -parent "pPlane1Follicle5619";
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
createNode transform -name "pPlane1Follicle5631" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape5631" -parent "pPlane1Follicle5631";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.5625;
	setAttr ".parameterV" 0.3125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve35" -parent "pPlane1Follicle5631";
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
createNode transform -name "pPlane1Follicle5643" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape5643" -parent "pPlane1Follicle5643";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.5625;
	setAttr ".parameterV" 0.4375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve36" -parent "pPlane1Follicle5643";
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
createNode transform -name "pPlane1Follicle5656" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape5656" -parent "pPlane1Follicle5656";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.5625;
	setAttr ".parameterV" 0.5625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve37" -parent "pPlane1Follicle5656";
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
createNode transform -name "pPlane1Follicle5668" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape5668" -parent "pPlane1Follicle5668";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.5625;
	setAttr ".parameterV" 0.6875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve38" -parent "pPlane1Follicle5668";
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
createNode transform -name "pPlane1Follicle5680" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape5680" -parent "pPlane1Follicle5680";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.5625;
	setAttr ".parameterV" 0.8125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve39" -parent "pPlane1Follicle5680";
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
createNode transform -name "pPlane1Follicle5693" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape5693" -parent "pPlane1Follicle5693";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.5625;
	setAttr ".parameterV" 0.9375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve40" -parent "pPlane1Follicle5693";
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
createNode transform -name "pPlane1Follicle6806" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape6806" -parent "pPlane1Follicle6806";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.6875;
	setAttr ".parameterV" 0.0625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve41" -parent "pPlane1Follicle6806";
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
createNode transform -name "pPlane1Follicle6819" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape6819" -parent "pPlane1Follicle6819";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.6875;
	setAttr ".parameterV" 0.1875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve42" -parent "pPlane1Follicle6819";
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
createNode transform -name "pPlane1Follicle6831" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape6831" -parent "pPlane1Follicle6831";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.6875;
	setAttr ".parameterV" 0.3125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve43" -parent "pPlane1Follicle6831";
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
createNode transform -name "pPlane1Follicle6843" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape6843" -parent "pPlane1Follicle6843";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.6875;
	setAttr ".parameterV" 0.4375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve44" -parent "pPlane1Follicle6843";
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
createNode transform -name "pPlane1Follicle6856" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape6856" -parent "pPlane1Follicle6856";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.6875;
	setAttr ".parameterV" 0.5625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve45" -parent "pPlane1Follicle6856";
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
createNode transform -name "pPlane1Follicle6868" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape6868" -parent "pPlane1Follicle6868";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.6875;
	setAttr ".parameterV" 0.6875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve46" -parent "pPlane1Follicle6868";
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
createNode transform -name "pPlane1Follicle6880" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape6880" -parent "pPlane1Follicle6880";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.6875;
	setAttr ".parameterV" 0.8125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve47" -parent "pPlane1Follicle6880";
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
createNode transform -name "pPlane1Follicle6893" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape6893" -parent "pPlane1Follicle6893";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.6875;
	setAttr ".parameterV" 0.9375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve48" -parent "pPlane1Follicle6893";
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
createNode transform -name "pPlane1Follicle8006" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape8006" -parent "pPlane1Follicle8006";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.8125;
	setAttr ".parameterV" 0.0625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve49" -parent "pPlane1Follicle8006";
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
createNode transform -name "pPlane1Follicle8019" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape8019" -parent "pPlane1Follicle8019";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.8125;
	setAttr ".parameterV" 0.1875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve50" -parent "pPlane1Follicle8019";
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
createNode transform -name "pPlane1Follicle8031" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape8031" -parent "pPlane1Follicle8031";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.8125;
	setAttr ".parameterV" 0.3125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve51" -parent "pPlane1Follicle8031";
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
createNode transform -name "pPlane1Follicle8043" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape8043" -parent "pPlane1Follicle8043";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.8125;
	setAttr ".parameterV" 0.4375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve52" -parent "pPlane1Follicle8043";
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
createNode transform -name "pPlane1Follicle8056" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape8056" -parent "pPlane1Follicle8056";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.8125;
	setAttr ".parameterV" 0.5625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve53" -parent "pPlane1Follicle8056";
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
createNode transform -name "pPlane1Follicle8068" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape8068" -parent "pPlane1Follicle8068";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.8125;
	setAttr ".parameterV" 0.6875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve54" -parent "pPlane1Follicle8068";
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
createNode transform -name "pPlane1Follicle8080" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape8080" -parent "pPlane1Follicle8080";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.8125;
	setAttr ".parameterV" 0.8125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve55" -parent "pPlane1Follicle8080";
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
createNode transform -name "pPlane1Follicle8093" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape8093" -parent "pPlane1Follicle8093";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.8125;
	setAttr ".parameterV" 0.9375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve56" -parent "pPlane1Follicle8093";
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
createNode transform -name "pPlane1Follicle9306" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape9306" -parent "pPlane1Follicle9306";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.9375;
	setAttr ".parameterV" 0.0625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve57" -parent "pPlane1Follicle9306";
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
createNode transform -name "pPlane1Follicle9319" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape9319" -parent "pPlane1Follicle9319";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.9375;
	setAttr ".parameterV" 0.1875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve58" -parent "pPlane1Follicle9319";
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
createNode transform -name "pPlane1Follicle9331" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape9331" -parent "pPlane1Follicle9331";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.9375;
	setAttr ".parameterV" 0.3125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve59" -parent "pPlane1Follicle9331";
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
createNode transform -name "pPlane1Follicle9343" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape9343" -parent "pPlane1Follicle9343";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.9375;
	setAttr ".parameterV" 0.4375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve60" -parent "pPlane1Follicle9343";
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
createNode transform -name "pPlane1Follicle9356" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape9356" -parent "pPlane1Follicle9356";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.9375;
	setAttr ".parameterV" 0.5625;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve61" -parent "pPlane1Follicle9356";
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
createNode transform -name "pPlane1Follicle9368" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape9368" -parent "pPlane1Follicle9368";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.9375;
	setAttr ".parameterV" 0.6875;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve62" -parent "pPlane1Follicle9368";
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
createNode transform -name "pPlane1Follicle9380" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape9380" -parent "pPlane1Follicle9380";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.9375;
	setAttr ".parameterV" 0.8125;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve63" -parent "pPlane1Follicle9380";
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
createNode transform -name "pPlane1Follicle9393" -parent "hairSystem1Follicles";
createNode follicle -name "pPlane1FollicleShape9393" -parent "pPlane1Follicle9393";
	setAttr -keyable off ".visibility";
	setAttr ".parameterU" 0.9375;
	setAttr ".parameterV" 0.9375;
	setAttr -size 2 ".stiffnessScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".clumpWidthScale[0:1]"  0 1 3 1 0.2 3;
	setAttr -size 2 ".attractionScale[0:1]"  0 1 3 1 0.2 3;
createNode transform -name "curve64" -parent "pPlane1Follicle9393";
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
createNode transform -name "directionalLight1";
	setAttr ".translate" -type "double3" 0 15 0 ;
	setAttr ".rotate" -type "double3" -90 0 0 ;
createNode directionalLight -name "directionalLightShape1" -parent "directionalLight1";
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
	addAttr -cachedInternally true -keyable true -shortName "ai_angle" -longName "aiAngle" 
		-minValue 0 -softMaxValue 10 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "cast_shadows" -longName "castShadows" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "exposure" -longName "exposure" 
		-attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "angle" -longName "angle" 
		-attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "samples" -longName "samples" 
		-defaultValue 1 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "mis" -longName "mis" -defaultValue 
		1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "normalize" -longName "normalize" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "bounce_factor" -longName "bounceFactor" 
		-defaultValue 1 -minValue 0 -softMaxValue 20 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "bounces" -longName "bounces" 
		-defaultValue 999 -minValue 0 -maxValue 10000 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "oss" -longName "override_sss_samples" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "sss_samples" -longName "sssSamples" 
		-defaultValue -1 -minValue 0 -maxValue 100 -attributeType "long";
	setAttr -keyable off ".visibility";
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
	setAttr ".GIDiffuseDepth" 3;
	setAttr ".GIGlossyDepth" 3;
	setAttr ".GIReflectionDepth" 3;
	setAttr ".GIRefractionDepth" 3;
	setAttr -keyable on ".driverTiled" no;
createNode lightLinker -shared -name "lightLinker1";
	setAttr -size 3 ".link";
	setAttr -size 3 ".shadowLink";
createNode displayLayerManager -name "layerManager";
createNode displayLayer -name "defaultLayer";
createNode renderLayerManager -name "renderLayerManager";
createNode renderLayer -name "defaultRenderLayer";
	setAttr ".global" yes;
createNode polyPlane -name "polyPlane1";
	setAttr ".width" 20;
	setAttr ".height" 20;
	setAttr ".subdivisionsWidth" 1;
	setAttr ".subdivisionsHeight" 1;
	setAttr ".createUVs" 2;
createNode script -name "sceneConfigurationScriptNode";
	setAttr ".before" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".scriptType" 6;
createNode aiStandard -name "aiStandard1";
	setAttr ".Kd" 1;
createNode shadingEngine -name "aiStandard1SG";
	setAttr ".isHistoricallyInteresting" 0;
	setAttr ".renderableOnlySet" yes;
createNode materialInfo -name "materialInfo1";
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
connectAttr "polyPlane1.output" "pPlaneShape1.inMesh";
connectAttr ":time1.outTime" "hairSystemShape1.currentTime";
connectAttr "pPlane1FollicleShape606.outHair" "hairSystemShape1.inputHair[0]";
connectAttr "pPlane1FollicleShape619.outHair" "hairSystemShape1.inputHair[1]";
connectAttr "pPlane1FollicleShape631.outHair" "hairSystemShape1.inputHair[2]";
connectAttr "pPlane1FollicleShape643.outHair" "hairSystemShape1.inputHair[3]";
connectAttr "pPlane1FollicleShape656.outHair" "hairSystemShape1.inputHair[4]";
connectAttr "pPlane1FollicleShape668.outHair" "hairSystemShape1.inputHair[5]";
connectAttr "pPlane1FollicleShape680.outHair" "hairSystemShape1.inputHair[6]";
connectAttr "pPlane1FollicleShape693.outHair" "hairSystemShape1.inputHair[7]";
connectAttr "pPlane1FollicleShape1906.outHair" "hairSystemShape1.inputHair[8]";
connectAttr "pPlane1FollicleShape1919.outHair" "hairSystemShape1.inputHair[9]";
connectAttr "pPlane1FollicleShape1931.outHair" "hairSystemShape1.inputHair[10]";
connectAttr "pPlane1FollicleShape1943.outHair" "hairSystemShape1.inputHair[11]";
connectAttr "pPlane1FollicleShape1956.outHair" "hairSystemShape1.inputHair[12]";
connectAttr "pPlane1FollicleShape1968.outHair" "hairSystemShape1.inputHair[13]";
connectAttr "pPlane1FollicleShape1980.outHair" "hairSystemShape1.inputHair[14]";
connectAttr "pPlane1FollicleShape1993.outHair" "hairSystemShape1.inputHair[15]";
connectAttr "pPlane1FollicleShape3106.outHair" "hairSystemShape1.inputHair[16]";
connectAttr "pPlane1FollicleShape3119.outHair" "hairSystemShape1.inputHair[17]";
connectAttr "pPlane1FollicleShape3131.outHair" "hairSystemShape1.inputHair[18]";
connectAttr "pPlane1FollicleShape3143.outHair" "hairSystemShape1.inputHair[19]";
connectAttr "pPlane1FollicleShape3156.outHair" "hairSystemShape1.inputHair[20]";
connectAttr "pPlane1FollicleShape3168.outHair" "hairSystemShape1.inputHair[21]";
connectAttr "pPlane1FollicleShape3180.outHair" "hairSystemShape1.inputHair[22]";
connectAttr "pPlane1FollicleShape3193.outHair" "hairSystemShape1.inputHair[23]";
connectAttr "pPlane1FollicleShape4306.outHair" "hairSystemShape1.inputHair[24]";
connectAttr "pPlane1FollicleShape4319.outHair" "hairSystemShape1.inputHair[25]";
connectAttr "pPlane1FollicleShape4331.outHair" "hairSystemShape1.inputHair[26]";
connectAttr "pPlane1FollicleShape4343.outHair" "hairSystemShape1.inputHair[27]";
connectAttr "pPlane1FollicleShape4356.outHair" "hairSystemShape1.inputHair[28]";
connectAttr "pPlane1FollicleShape4368.outHair" "hairSystemShape1.inputHair[29]";
connectAttr "pPlane1FollicleShape4380.outHair" "hairSystemShape1.inputHair[30]";
connectAttr "pPlane1FollicleShape4393.outHair" "hairSystemShape1.inputHair[31]";
connectAttr "pPlane1FollicleShape5606.outHair" "hairSystemShape1.inputHair[32]";
connectAttr "pPlane1FollicleShape5619.outHair" "hairSystemShape1.inputHair[33]";
connectAttr "pPlane1FollicleShape5631.outHair" "hairSystemShape1.inputHair[34]";
connectAttr "pPlane1FollicleShape5643.outHair" "hairSystemShape1.inputHair[35]";
connectAttr "pPlane1FollicleShape5656.outHair" "hairSystemShape1.inputHair[36]";
connectAttr "pPlane1FollicleShape5668.outHair" "hairSystemShape1.inputHair[37]";
connectAttr "pPlane1FollicleShape5680.outHair" "hairSystemShape1.inputHair[38]";
connectAttr "pPlane1FollicleShape5693.outHair" "hairSystemShape1.inputHair[39]";
connectAttr "pPlane1FollicleShape6806.outHair" "hairSystemShape1.inputHair[40]";
connectAttr "pPlane1FollicleShape6819.outHair" "hairSystemShape1.inputHair[41]";
connectAttr "pPlane1FollicleShape6831.outHair" "hairSystemShape1.inputHair[42]";
connectAttr "pPlane1FollicleShape6843.outHair" "hairSystemShape1.inputHair[43]";
connectAttr "pPlane1FollicleShape6856.outHair" "hairSystemShape1.inputHair[44]";
connectAttr "pPlane1FollicleShape6868.outHair" "hairSystemShape1.inputHair[45]";
connectAttr "pPlane1FollicleShape6880.outHair" "hairSystemShape1.inputHair[46]";
connectAttr "pPlane1FollicleShape6893.outHair" "hairSystemShape1.inputHair[47]";
connectAttr "pPlane1FollicleShape8006.outHair" "hairSystemShape1.inputHair[48]";
connectAttr "pPlane1FollicleShape8019.outHair" "hairSystemShape1.inputHair[49]";
connectAttr "pPlane1FollicleShape8031.outHair" "hairSystemShape1.inputHair[50]";
connectAttr "pPlane1FollicleShape8043.outHair" "hairSystemShape1.inputHair[51]";
connectAttr "pPlane1FollicleShape8056.outHair" "hairSystemShape1.inputHair[52]";
connectAttr "pPlane1FollicleShape8068.outHair" "hairSystemShape1.inputHair[53]";
connectAttr "pPlane1FollicleShape8080.outHair" "hairSystemShape1.inputHair[54]";
connectAttr "pPlane1FollicleShape8093.outHair" "hairSystemShape1.inputHair[55]";
connectAttr "pPlane1FollicleShape9306.outHair" "hairSystemShape1.inputHair[56]";
connectAttr "pPlane1FollicleShape9319.outHair" "hairSystemShape1.inputHair[57]";
connectAttr "pPlane1FollicleShape9331.outHair" "hairSystemShape1.inputHair[58]";
connectAttr "pPlane1FollicleShape9343.outHair" "hairSystemShape1.inputHair[59]";
connectAttr "pPlane1FollicleShape9356.outHair" "hairSystemShape1.inputHair[60]";
connectAttr "pPlane1FollicleShape9368.outHair" "hairSystemShape1.inputHair[61]";
connectAttr "pPlane1FollicleShape9380.outHair" "hairSystemShape1.inputHair[62]";
connectAttr "pPlane1FollicleShape9393.outHair" "hairSystemShape1.inputHair[63]";
connectAttr "pPlane1FollicleShape606.outTranslate" "pPlane1Follicle606.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape606.outRotate" "pPlane1Follicle606.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape606.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape606.inputMesh";
connectAttr "curveShape1.worldSpace" "pPlane1FollicleShape606.startPosition";
connectAttr "pPlane1FollicleShape619.outTranslate" "pPlane1Follicle619.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape619.outRotate" "pPlane1Follicle619.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape619.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape619.inputMesh";
connectAttr "curveShape2.worldSpace" "pPlane1FollicleShape619.startPosition";
connectAttr "pPlane1FollicleShape631.outTranslate" "pPlane1Follicle631.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape631.outRotate" "pPlane1Follicle631.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape631.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape631.inputMesh";
connectAttr "curveShape3.worldSpace" "pPlane1FollicleShape631.startPosition";
connectAttr "pPlane1FollicleShape643.outTranslate" "pPlane1Follicle643.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape643.outRotate" "pPlane1Follicle643.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape643.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape643.inputMesh";
connectAttr "curveShape4.worldSpace" "pPlane1FollicleShape643.startPosition";
connectAttr "pPlane1FollicleShape656.outTranslate" "pPlane1Follicle656.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape656.outRotate" "pPlane1Follicle656.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape656.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape656.inputMesh";
connectAttr "curveShape5.worldSpace" "pPlane1FollicleShape656.startPosition";
connectAttr "pPlane1FollicleShape668.outTranslate" "pPlane1Follicle668.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape668.outRotate" "pPlane1Follicle668.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape668.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape668.inputMesh";
connectAttr "curveShape6.worldSpace" "pPlane1FollicleShape668.startPosition";
connectAttr "pPlane1FollicleShape680.outTranslate" "pPlane1Follicle680.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape680.outRotate" "pPlane1Follicle680.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape680.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape680.inputMesh";
connectAttr "curveShape7.worldSpace" "pPlane1FollicleShape680.startPosition";
connectAttr "pPlane1FollicleShape693.outTranslate" "pPlane1Follicle693.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape693.outRotate" "pPlane1Follicle693.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape693.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape693.inputMesh";
connectAttr "curveShape8.worldSpace" "pPlane1FollicleShape693.startPosition";
connectAttr "pPlane1FollicleShape1906.outTranslate" "pPlane1Follicle1906.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape1906.outRotate" "pPlane1Follicle1906.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape1906.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape1906.inputMesh";
connectAttr "curveShape9.worldSpace" "pPlane1FollicleShape1906.startPosition";
connectAttr "pPlane1FollicleShape1919.outTranslate" "pPlane1Follicle1919.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape1919.outRotate" "pPlane1Follicle1919.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape1919.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape1919.inputMesh";
connectAttr "curveShape10.worldSpace" "pPlane1FollicleShape1919.startPosition";
connectAttr "pPlane1FollicleShape1931.outTranslate" "pPlane1Follicle1931.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape1931.outRotate" "pPlane1Follicle1931.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape1931.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape1931.inputMesh";
connectAttr "curveShape11.worldSpace" "pPlane1FollicleShape1931.startPosition";
connectAttr "pPlane1FollicleShape1943.outTranslate" "pPlane1Follicle1943.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape1943.outRotate" "pPlane1Follicle1943.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape1943.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape1943.inputMesh";
connectAttr "curveShape12.worldSpace" "pPlane1FollicleShape1943.startPosition";
connectAttr "pPlane1FollicleShape1956.outTranslate" "pPlane1Follicle1956.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape1956.outRotate" "pPlane1Follicle1956.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape1956.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape1956.inputMesh";
connectAttr "curveShape13.worldSpace" "pPlane1FollicleShape1956.startPosition";
connectAttr "pPlane1FollicleShape1968.outTranslate" "pPlane1Follicle1968.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape1968.outRotate" "pPlane1Follicle1968.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape1968.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape1968.inputMesh";
connectAttr "curveShape14.worldSpace" "pPlane1FollicleShape1968.startPosition";
connectAttr "pPlane1FollicleShape1980.outTranslate" "pPlane1Follicle1980.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape1980.outRotate" "pPlane1Follicle1980.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape1980.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape1980.inputMesh";
connectAttr "curveShape15.worldSpace" "pPlane1FollicleShape1980.startPosition";
connectAttr "pPlane1FollicleShape1993.outTranslate" "pPlane1Follicle1993.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape1993.outRotate" "pPlane1Follicle1993.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape1993.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape1993.inputMesh";
connectAttr "curveShape16.worldSpace" "pPlane1FollicleShape1993.startPosition";
connectAttr "pPlane1FollicleShape3106.outTranslate" "pPlane1Follicle3106.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape3106.outRotate" "pPlane1Follicle3106.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape3106.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape3106.inputMesh";
connectAttr "curveShape17.worldSpace" "pPlane1FollicleShape3106.startPosition";
connectAttr "pPlane1FollicleShape3119.outTranslate" "pPlane1Follicle3119.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape3119.outRotate" "pPlane1Follicle3119.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape3119.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape3119.inputMesh";
connectAttr "curveShape18.worldSpace" "pPlane1FollicleShape3119.startPosition";
connectAttr "pPlane1FollicleShape3131.outTranslate" "pPlane1Follicle3131.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape3131.outRotate" "pPlane1Follicle3131.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape3131.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape3131.inputMesh";
connectAttr "curveShape19.worldSpace" "pPlane1FollicleShape3131.startPosition";
connectAttr "pPlane1FollicleShape3143.outTranslate" "pPlane1Follicle3143.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape3143.outRotate" "pPlane1Follicle3143.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape3143.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape3143.inputMesh";
connectAttr "curveShape20.worldSpace" "pPlane1FollicleShape3143.startPosition";
connectAttr "pPlane1FollicleShape3156.outTranslate" "pPlane1Follicle3156.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape3156.outRotate" "pPlane1Follicle3156.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape3156.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape3156.inputMesh";
connectAttr "curveShape21.worldSpace" "pPlane1FollicleShape3156.startPosition";
connectAttr "pPlane1FollicleShape3168.outTranslate" "pPlane1Follicle3168.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape3168.outRotate" "pPlane1Follicle3168.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape3168.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape3168.inputMesh";
connectAttr "curveShape22.worldSpace" "pPlane1FollicleShape3168.startPosition";
connectAttr "pPlane1FollicleShape3180.outTranslate" "pPlane1Follicle3180.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape3180.outRotate" "pPlane1Follicle3180.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape3180.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape3180.inputMesh";
connectAttr "curveShape23.worldSpace" "pPlane1FollicleShape3180.startPosition";
connectAttr "pPlane1FollicleShape3193.outTranslate" "pPlane1Follicle3193.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape3193.outRotate" "pPlane1Follicle3193.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape3193.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape3193.inputMesh";
connectAttr "curveShape24.worldSpace" "pPlane1FollicleShape3193.startPosition";
connectAttr "pPlane1FollicleShape4306.outTranslate" "pPlane1Follicle4306.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape4306.outRotate" "pPlane1Follicle4306.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape4306.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape4306.inputMesh";
connectAttr "curveShape25.worldSpace" "pPlane1FollicleShape4306.startPosition";
connectAttr "pPlane1FollicleShape4319.outTranslate" "pPlane1Follicle4319.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape4319.outRotate" "pPlane1Follicle4319.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape4319.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape4319.inputMesh";
connectAttr "curveShape26.worldSpace" "pPlane1FollicleShape4319.startPosition";
connectAttr "pPlane1FollicleShape4331.outTranslate" "pPlane1Follicle4331.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape4331.outRotate" "pPlane1Follicle4331.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape4331.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape4331.inputMesh";
connectAttr "curveShape27.worldSpace" "pPlane1FollicleShape4331.startPosition";
connectAttr "pPlane1FollicleShape4343.outTranslate" "pPlane1Follicle4343.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape4343.outRotate" "pPlane1Follicle4343.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape4343.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape4343.inputMesh";
connectAttr "curveShape28.worldSpace" "pPlane1FollicleShape4343.startPosition";
connectAttr "pPlane1FollicleShape4356.outTranslate" "pPlane1Follicle4356.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape4356.outRotate" "pPlane1Follicle4356.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape4356.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape4356.inputMesh";
connectAttr "curveShape29.worldSpace" "pPlane1FollicleShape4356.startPosition";
connectAttr "pPlane1FollicleShape4368.outTranslate" "pPlane1Follicle4368.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape4368.outRotate" "pPlane1Follicle4368.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape4368.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape4368.inputMesh";
connectAttr "curveShape30.worldSpace" "pPlane1FollicleShape4368.startPosition";
connectAttr "pPlane1FollicleShape4380.outTranslate" "pPlane1Follicle4380.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape4380.outRotate" "pPlane1Follicle4380.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape4380.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape4380.inputMesh";
connectAttr "curveShape31.worldSpace" "pPlane1FollicleShape4380.startPosition";
connectAttr "pPlane1FollicleShape4393.outTranslate" "pPlane1Follicle4393.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape4393.outRotate" "pPlane1Follicle4393.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape4393.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape4393.inputMesh";
connectAttr "curveShape32.worldSpace" "pPlane1FollicleShape4393.startPosition";
connectAttr "pPlane1FollicleShape5606.outTranslate" "pPlane1Follicle5606.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape5606.outRotate" "pPlane1Follicle5606.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape5606.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape5606.inputMesh";
connectAttr "curveShape33.worldSpace" "pPlane1FollicleShape5606.startPosition";
connectAttr "pPlane1FollicleShape5619.outTranslate" "pPlane1Follicle5619.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape5619.outRotate" "pPlane1Follicle5619.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape5619.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape5619.inputMesh";
connectAttr "curveShape34.worldSpace" "pPlane1FollicleShape5619.startPosition";
connectAttr "pPlane1FollicleShape5631.outTranslate" "pPlane1Follicle5631.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape5631.outRotate" "pPlane1Follicle5631.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape5631.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape5631.inputMesh";
connectAttr "curveShape35.worldSpace" "pPlane1FollicleShape5631.startPosition";
connectAttr "pPlane1FollicleShape5643.outTranslate" "pPlane1Follicle5643.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape5643.outRotate" "pPlane1Follicle5643.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape5643.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape5643.inputMesh";
connectAttr "curveShape36.worldSpace" "pPlane1FollicleShape5643.startPosition";
connectAttr "pPlane1FollicleShape5656.outTranslate" "pPlane1Follicle5656.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape5656.outRotate" "pPlane1Follicle5656.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape5656.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape5656.inputMesh";
connectAttr "curveShape37.worldSpace" "pPlane1FollicleShape5656.startPosition";
connectAttr "pPlane1FollicleShape5668.outTranslate" "pPlane1Follicle5668.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape5668.outRotate" "pPlane1Follicle5668.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape5668.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape5668.inputMesh";
connectAttr "curveShape38.worldSpace" "pPlane1FollicleShape5668.startPosition";
connectAttr "pPlane1FollicleShape5680.outTranslate" "pPlane1Follicle5680.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape5680.outRotate" "pPlane1Follicle5680.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape5680.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape5680.inputMesh";
connectAttr "curveShape39.worldSpace" "pPlane1FollicleShape5680.startPosition";
connectAttr "pPlane1FollicleShape5693.outTranslate" "pPlane1Follicle5693.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape5693.outRotate" "pPlane1Follicle5693.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape5693.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape5693.inputMesh";
connectAttr "curveShape40.worldSpace" "pPlane1FollicleShape5693.startPosition";
connectAttr "pPlane1FollicleShape6806.outTranslate" "pPlane1Follicle6806.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape6806.outRotate" "pPlane1Follicle6806.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape6806.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape6806.inputMesh";
connectAttr "curveShape41.worldSpace" "pPlane1FollicleShape6806.startPosition";
connectAttr "pPlane1FollicleShape6819.outTranslate" "pPlane1Follicle6819.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape6819.outRotate" "pPlane1Follicle6819.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape6819.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape6819.inputMesh";
connectAttr "curveShape42.worldSpace" "pPlane1FollicleShape6819.startPosition";
connectAttr "pPlane1FollicleShape6831.outTranslate" "pPlane1Follicle6831.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape6831.outRotate" "pPlane1Follicle6831.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape6831.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape6831.inputMesh";
connectAttr "curveShape43.worldSpace" "pPlane1FollicleShape6831.startPosition";
connectAttr "pPlane1FollicleShape6843.outTranslate" "pPlane1Follicle6843.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape6843.outRotate" "pPlane1Follicle6843.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape6843.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape6843.inputMesh";
connectAttr "curveShape44.worldSpace" "pPlane1FollicleShape6843.startPosition";
connectAttr "pPlane1FollicleShape6856.outTranslate" "pPlane1Follicle6856.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape6856.outRotate" "pPlane1Follicle6856.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape6856.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape6856.inputMesh";
connectAttr "curveShape45.worldSpace" "pPlane1FollicleShape6856.startPosition";
connectAttr "pPlane1FollicleShape6868.outTranslate" "pPlane1Follicle6868.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape6868.outRotate" "pPlane1Follicle6868.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape6868.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape6868.inputMesh";
connectAttr "curveShape46.worldSpace" "pPlane1FollicleShape6868.startPosition";
connectAttr "pPlane1FollicleShape6880.outTranslate" "pPlane1Follicle6880.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape6880.outRotate" "pPlane1Follicle6880.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape6880.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape6880.inputMesh";
connectAttr "curveShape47.worldSpace" "pPlane1FollicleShape6880.startPosition";
connectAttr "pPlane1FollicleShape6893.outTranslate" "pPlane1Follicle6893.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape6893.outRotate" "pPlane1Follicle6893.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape6893.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape6893.inputMesh";
connectAttr "curveShape48.worldSpace" "pPlane1FollicleShape6893.startPosition";
connectAttr "pPlane1FollicleShape8006.outTranslate" "pPlane1Follicle8006.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape8006.outRotate" "pPlane1Follicle8006.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape8006.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape8006.inputMesh";
connectAttr "curveShape49.worldSpace" "pPlane1FollicleShape8006.startPosition";
connectAttr "pPlane1FollicleShape8019.outTranslate" "pPlane1Follicle8019.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape8019.outRotate" "pPlane1Follicle8019.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape8019.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape8019.inputMesh";
connectAttr "curveShape50.worldSpace" "pPlane1FollicleShape8019.startPosition";
connectAttr "pPlane1FollicleShape8031.outTranslate" "pPlane1Follicle8031.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape8031.outRotate" "pPlane1Follicle8031.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape8031.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape8031.inputMesh";
connectAttr "curveShape51.worldSpace" "pPlane1FollicleShape8031.startPosition";
connectAttr "pPlane1FollicleShape8043.outTranslate" "pPlane1Follicle8043.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape8043.outRotate" "pPlane1Follicle8043.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape8043.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape8043.inputMesh";
connectAttr "curveShape52.worldSpace" "pPlane1FollicleShape8043.startPosition";
connectAttr "pPlane1FollicleShape8056.outTranslate" "pPlane1Follicle8056.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape8056.outRotate" "pPlane1Follicle8056.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape8056.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape8056.inputMesh";
connectAttr "curveShape53.worldSpace" "pPlane1FollicleShape8056.startPosition";
connectAttr "pPlane1FollicleShape8068.outTranslate" "pPlane1Follicle8068.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape8068.outRotate" "pPlane1Follicle8068.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape8068.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape8068.inputMesh";
connectAttr "curveShape54.worldSpace" "pPlane1FollicleShape8068.startPosition";
connectAttr "pPlane1FollicleShape8080.outTranslate" "pPlane1Follicle8080.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape8080.outRotate" "pPlane1Follicle8080.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape8080.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape8080.inputMesh";
connectAttr "curveShape55.worldSpace" "pPlane1FollicleShape8080.startPosition";
connectAttr "pPlane1FollicleShape8093.outTranslate" "pPlane1Follicle8093.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape8093.outRotate" "pPlane1Follicle8093.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape8093.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape8093.inputMesh";
connectAttr "curveShape56.worldSpace" "pPlane1FollicleShape8093.startPosition";
connectAttr "pPlane1FollicleShape9306.outTranslate" "pPlane1Follicle9306.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape9306.outRotate" "pPlane1Follicle9306.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape9306.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape9306.inputMesh";
connectAttr "curveShape57.worldSpace" "pPlane1FollicleShape9306.startPosition";
connectAttr "pPlane1FollicleShape9319.outTranslate" "pPlane1Follicle9319.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape9319.outRotate" "pPlane1Follicle9319.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape9319.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape9319.inputMesh";
connectAttr "curveShape58.worldSpace" "pPlane1FollicleShape9319.startPosition";
connectAttr "pPlane1FollicleShape9331.outTranslate" "pPlane1Follicle9331.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape9331.outRotate" "pPlane1Follicle9331.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape9331.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape9331.inputMesh";
connectAttr "curveShape59.worldSpace" "pPlane1FollicleShape9331.startPosition";
connectAttr "pPlane1FollicleShape9343.outTranslate" "pPlane1Follicle9343.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape9343.outRotate" "pPlane1Follicle9343.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape9343.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape9343.inputMesh";
connectAttr "curveShape60.worldSpace" "pPlane1FollicleShape9343.startPosition";
connectAttr "pPlane1FollicleShape9356.outTranslate" "pPlane1Follicle9356.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape9356.outRotate" "pPlane1Follicle9356.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape9356.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape9356.inputMesh";
connectAttr "curveShape61.worldSpace" "pPlane1FollicleShape9356.startPosition";
connectAttr "pPlane1FollicleShape9368.outTranslate" "pPlane1Follicle9368.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape9368.outRotate" "pPlane1Follicle9368.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape9368.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape9368.inputMesh";
connectAttr "curveShape62.worldSpace" "pPlane1FollicleShape9368.startPosition";
connectAttr "pPlane1FollicleShape9380.outTranslate" "pPlane1Follicle9380.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape9380.outRotate" "pPlane1Follicle9380.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape9380.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape9380.inputMesh";
connectAttr "curveShape63.worldSpace" "pPlane1FollicleShape9380.startPosition";
connectAttr "pPlane1FollicleShape9393.outTranslate" "pPlane1Follicle9393.translate"
		 -lock on;
connectAttr "pPlane1FollicleShape9393.outRotate" "pPlane1Follicle9393.rotate" -lock
		 on;
connectAttr "pPlaneShape1.worldMatrix" "pPlane1FollicleShape9393.inputWorldMatrix"
		;
connectAttr "pPlaneShape1.outMesh" "pPlane1FollicleShape9393.inputMesh";
connectAttr "curveShape64.worldSpace" "pPlane1FollicleShape9393.startPosition";
connectAttr "hairSystemShape1.outputRenderHairs" "pfxHairShape1.renderHairs";
connectAttr ":time1.outTime" ":defaultArnoldRenderOptions.AA_seed";
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
connectAttr "pPlaneShape1.instObjGroups" "aiStandard1SG.dagSetMembers" -nextAvailable
		;
connectAttr "aiStandard1SG.message" "materialInfo1.shadingGroup";
connectAttr "aiStandard1.message" "materialInfo1.material";
connectAttr "aiStandard1.message" "materialInfo1.texture" -nextAvailable;
connectAttr "aiStandard1SG.partition" ":renderPartition.sets" -nextAvailable;
connectAttr "aiStandard1.message" ":defaultShaderList1.shaders" -nextAvailable;
connectAttr "directionalLightShape1.lightData" ":lightList1.lights" -nextAvailable
		;
connectAttr "directionalLight1.instObjGroups" ":defaultLightSet.dagSetMembers" -nextAvailable
		;
// End of test.ma
