//Maya ASCII 2011 scene
//Name: test.ma
//Last modified: Wed, Feb 29, 2012 01:57:06 PM
//Codeset: 1252
requires maya "2011";
requires "mtoa" "0.14.0";
requires "Mayatomr" "2011.0m - 3.8.1.33 ";
requires "stereoCamera" "10.0";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2011";
fileInfo "version" "2011 x64";
fileInfo "cutIdentifier" "201009060330-781623";
fileInfo "osv" "Microsoft Windows 7 Business Edition, 64-bit Windows 7 Service Pack 1 (Build 7601)\n";
fileInfo "saveHistory0" "/Volumes/sv-dev01/devRepo/chad/arnold/mtoa/testsuite/test_0045/data/test.ma&chad&ws-050.luma-pictures.com&Tuesday, October 18 2011 @ 06:31:48 PM (PDT)";
fileInfo "saveHistory1" "/Volumes/sv-dev01/devRepo/chad/arnold/mtoa/testsuite/test_0045/data/test.ma&chad&ws-050.luma-pictures.com&Tuesday, October 18 2011 @ 06:29:10 PM (PDT)";
fileInfo "saveHistory2" "/Volumes/sv-dev01/devRepo/chad/arnold/mtoa/testsuite/test_0041/data/test.ma&chad&ws-050.luma-pictures.com&Tuesday, October 18 2011 @ 11:59:19 AM (PDT)";
fileInfo "saveHistory3" "/Volumes/sv-dev01/devRepo/chad/arnold/mtoa/testsuite/test_0041/data/test.ma&chad&ws-050.luma-pictures.com&Tuesday, October 18 2011 @ 11:56:44 AM (PDT)";
fileInfo "nexSGFix" "True";
createNode transform -s -n "persp";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -3.1443942773107727 5.9729462931136723 4.3015195927180283 ;
	setAttr ".r" -type "double3" -44.138352729560033 -399.39999999999196 0 ;
	setAttr ".lumaID" -type "string" "a:1292543810.78:0.842974524452";
createNode camera -s -n "perspShape" -p "persp";
	addAttr -ci true -k true -sn "ai_filtermap" -ln "aiFiltermap" -at "message";
	addAttr -ci true -sn "ai_horizontal_fov" -ln "aiHorizontalFov" -dv 60 -min 0 -max 
		1000000000 -smx 360 -at "float";
	addAttr -ci true -sn "ai_vertical_fov" -ln "aiVerticalFov" -dv 90 -min 0 -max 1000000000 
		-smx 180 -at "float";
	addAttr -ci true -sn "ai_projective" -ln "aiProjective" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_focus_distance" -ln "aiFocusDistance" -dv 1 -min 0 -max 
		1000000000 -smn 0 -smx 200 -at "float";
	addAttr -ci true -sn "ai_aperture_size" -ln "aiApertureSize" -min 0 -max 20 -smn 
		0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_aperture_blades" -ln "aiApertureBlades" -min 3 -max 40 
		-at "long";
	addAttr -ci true -sn "ai_aperture_blade_curvature" -ln "aiApertureBladeCurvature" 
		-min -20 -max 20 -smn -5 -smx 5 -at "float";
	addAttr -ci true -sn "ai_aperture_rotation" -ln "aiApertureRotation" -min 0 -max 
		360 -smn 0 -smx 50 -at "float";
	addAttr -ci true -k true -sn "ai_edof" -ln "aiEnableDOF" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_fov" -ln "aiFov" -dv 90 -min 0 -max 1000000000 -smx 360 
		-at "float";
	addAttr -ci true -sn "ai_autocrop" -ln "aiAutocrop" -min 0 -max 1 -at "bool";
	addAttr -ci true -h true -sn "ai_uv_remapa" -ln "aiUvRemapA" -at "float";
	addAttr -ci true -uac -sn "ai_uv_remap" -ln "aiUvRemap" -at "float3" -nc 3;
	addAttr -ci true -sn "ai_uv_remapr" -ln "aiUvRemapR" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapg" -ln "aiUvRemapG" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapb" -ln "aiUvRemapB" -at "float" -p "aiUvRemap";
	addAttr -ci true -k true -sn "ai_translator" -ln "aiTranslator" -dt "string";
	addAttr -ci true -k true -sn "filtermap" -ln "filtermap" -at "message";
	addAttr -ci true -k true -sn "focal_distance" -ln "focalDistance" -dv 5 -min 0 -max 
		1000000000 -smn 0 -smx 200 -at "float";
	addAttr -ci true -k true -sn "aperture_size" -ln "apertureSize" -dv 0.20000000298023224 
		-min 0 -max 20 -smn 0 -smx 0.5 -at "float";
	addAttr -ci true -k true -sn "aperture_blades" -ln "apertureBlades" -dv 6 -min 3 
		-max 40 -at "long";
	addAttr -ci true -k true -sn "aperture_blade_curvature" -ln "apertureBladeCurvature" 
		-min -20 -max 20 -smn -5 -smx 5 -at "float";
	addAttr -ci true -k true -sn "aperture_rotation" -ln "apertureRotation" -min 0 -max 
		360 -smn 0 -smx 50 -at "float";
	addAttr -ci true -k true -sn "edof" -ln "enableDOF" -min 0 -max 1 -at "bool";
	addAttr -ci true -uac -k true -sn "uv_remap" -ln "uvRemap" -at "float3" -nc 3;
	addAttr -ci true -k true -sn "uv_remapr" -ln "uvRemapR" -at "float" -p "uvRemap";
	addAttr -ci true -k true -sn "uv_remapg" -ln "uvRemapG" -at "float" -p "uvRemap";
	addAttr -ci true -k true -sn "uv_remapb" -ln "uvRemapB" -at "float" -p "uvRemap";
	addAttr -ci true -h true -k true -sn "uv_remapa" -ln "uvRemapA" -at "float";
	addAttr -ci true -k true -sn "arntr" -ln "arnoldTranslator" -dt "string";
	addAttr -ci true -k true -sn "fov" -ln "fov" -dv 90 -min 0 -max 1000000000 -smx 
		360 -at "float";
	addAttr -ci true -k true -sn "autocrop" -ln "autocrop" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "horizontal_fov" -ln "horizontalFov" -dv 60 -min 0 
		-max 1000000000 -smx 360 -at "float";
	addAttr -ci true -k true -sn "vertical_fov" -ln "verticalFov" -dv 90 -min 0 -max 
		1000000000 -smx 180 -at "float";
	addAttr -ci true -k true -sn "projective" -ln "projective" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".fl" 34.999999999999993;
	setAttr ".coi" 7.5714796510852072;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".hc" -type "string" "viewSet -p %camera";
	setAttr ".dr" yes;
	setAttr -k on ".ai_translator" -type "string" "perspective";
	setAttr -k on ".focal_distance" 4;
	setAttr -k on ".aperture_size" 0.10000000149011612;
	setAttr -k on ".arntr" -type "string" "perspective";
	setAttr ".lumaID" -type "string" "a:1292543810.78:0.369170061596";
createNode transform -s -n "top";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 100.1 0 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
	setAttr ".lumaID" -type "string" "a:1292543810.78:0.701369956448";
createNode camera -s -n "topShape" -p "top";
	addAttr -ci true -k true -sn "ai_filtermap" -ln "aiFiltermap" -at "message";
	addAttr -ci true -sn "ai_horizontal_fov" -ln "aiHorizontalFov" -dv 60 -min 0 -max 
		1000000000 -smx 360 -at "float";
	addAttr -ci true -sn "ai_vertical_fov" -ln "aiVerticalFov" -dv 90 -min 0 -max 1000000000 
		-smx 180 -at "float";
	addAttr -ci true -sn "ai_projective" -ln "aiProjective" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_focus_distance" -ln "aiFocusDistance" -dv 1 -min 0 -max 
		1000000000 -smn 0 -smx 200 -at "float";
	addAttr -ci true -sn "ai_aperture_size" -ln "aiApertureSize" -min 0 -max 20 -smn 
		0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_aperture_blades" -ln "aiApertureBlades" -min 3 -max 40 
		-at "long";
	addAttr -ci true -sn "ai_aperture_blade_curvature" -ln "aiApertureBladeCurvature" 
		-min -20 -max 20 -smn -5 -smx 5 -at "float";
	addAttr -ci true -sn "ai_aperture_rotation" -ln "aiApertureRotation" -min 0 -max 
		360 -smn 0 -smx 50 -at "float";
	addAttr -ci true -k true -sn "ai_edof" -ln "aiEnableDOF" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_fov" -ln "aiFov" -dv 90 -min 0 -max 1000000000 -smx 360 
		-at "float";
	addAttr -ci true -sn "ai_autocrop" -ln "aiAutocrop" -min 0 -max 1 -at "bool";
	addAttr -ci true -h true -sn "ai_uv_remapa" -ln "aiUvRemapA" -at "float";
	addAttr -ci true -uac -sn "ai_uv_remap" -ln "aiUvRemap" -at "float3" -nc 3;
	addAttr -ci true -sn "ai_uv_remapr" -ln "aiUvRemapR" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapg" -ln "aiUvRemapG" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapb" -ln "aiUvRemapB" -at "float" -p "aiUvRemap";
	addAttr -ci true -k true -sn "ai_translator" -ln "aiTranslator" -dt "string";
	addAttr -ci true -k true -sn "filtermap" -ln "filtermap" -at "message";
	addAttr -ci true -k true -sn "focal_distance" -ln "focalDistance" -dv 5 -min 0 -max 
		1000000000 -smn 0 -smx 200 -at "float";
	addAttr -ci true -k true -sn "aperture_size" -ln "apertureSize" -dv 0.20000000298023224 
		-min 0 -max 20 -smn 0 -smx 0.5 -at "float";
	addAttr -ci true -k true -sn "aperture_blades" -ln "apertureBlades" -dv 6 -min 3 
		-max 40 -at "long";
	addAttr -ci true -k true -sn "aperture_blade_curvature" -ln "apertureBladeCurvature" 
		-min -20 -max 20 -smn -5 -smx 5 -at "float";
	addAttr -ci true -k true -sn "aperture_rotation" -ln "apertureRotation" -min 0 -max 
		360 -smn 0 -smx 50 -at "float";
	addAttr -ci true -k true -sn "edof" -ln "enableDOF" -min 0 -max 1 -at "bool";
	addAttr -ci true -uac -k true -sn "uv_remap" -ln "uvRemap" -at "float3" -nc 3;
	addAttr -ci true -k true -sn "uv_remapr" -ln "uvRemapR" -at "float" -p "uvRemap";
	addAttr -ci true -k true -sn "uv_remapg" -ln "uvRemapG" -at "float" -p "uvRemap";
	addAttr -ci true -k true -sn "uv_remapb" -ln "uvRemapB" -at "float" -p "uvRemap";
	addAttr -ci true -h true -k true -sn "uv_remapa" -ln "uvRemapA" -at "float";
	addAttr -ci true -k true -sn "arntr" -ln "arnoldTranslator" -dt "string";
	addAttr -ci true -k true -sn "fov" -ln "fov" -dv 90 -min 0 -max 1000000000 -smx 
		360 -at "float";
	addAttr -ci true -k true -sn "autocrop" -ln "autocrop" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "horizontal_fov" -ln "horizontalFov" -dv 60 -min 0 
		-max 1000000000 -smx 360 -at "float";
	addAttr -ci true -k true -sn "vertical_fov" -ln "verticalFov" -dv 90 -min 0 -max 
		1000000000 -smx 180 -at "float";
	addAttr -ci true -k true -sn "projective" -ln "projective" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
	setAttr -k on ".ai_translator" -type "string" "orthographic";
	setAttr -k on ".arntr" -type "string" "orthographic";
	setAttr ".lumaID" -type "string" "a:1292543810.78:0.0901379666454";
createNode transform -s -n "front";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 100.1 ;
	setAttr ".lumaID" -type "string" "a:1292543810.78:0.868060622798";
createNode camera -s -n "frontShape" -p "front";
	addAttr -ci true -k true -sn "ai_filtermap" -ln "aiFiltermap" -at "message";
	addAttr -ci true -sn "ai_horizontal_fov" -ln "aiHorizontalFov" -dv 60 -min 0 -max 
		1000000000 -smx 360 -at "float";
	addAttr -ci true -sn "ai_vertical_fov" -ln "aiVerticalFov" -dv 90 -min 0 -max 1000000000 
		-smx 180 -at "float";
	addAttr -ci true -sn "ai_projective" -ln "aiProjective" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_focus_distance" -ln "aiFocusDistance" -dv 1 -min 0 -max 
		1000000000 -smn 0 -smx 200 -at "float";
	addAttr -ci true -sn "ai_aperture_size" -ln "aiApertureSize" -min 0 -max 20 -smn 
		0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_aperture_blades" -ln "aiApertureBlades" -min 3 -max 40 
		-at "long";
	addAttr -ci true -sn "ai_aperture_blade_curvature" -ln "aiApertureBladeCurvature" 
		-min -20 -max 20 -smn -5 -smx 5 -at "float";
	addAttr -ci true -sn "ai_aperture_rotation" -ln "aiApertureRotation" -min 0 -max 
		360 -smn 0 -smx 50 -at "float";
	addAttr -ci true -k true -sn "ai_edof" -ln "aiEnableDOF" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_fov" -ln "aiFov" -dv 90 -min 0 -max 1000000000 -smx 360 
		-at "float";
	addAttr -ci true -sn "ai_autocrop" -ln "aiAutocrop" -min 0 -max 1 -at "bool";
	addAttr -ci true -h true -sn "ai_uv_remapa" -ln "aiUvRemapA" -at "float";
	addAttr -ci true -uac -sn "ai_uv_remap" -ln "aiUvRemap" -at "float3" -nc 3;
	addAttr -ci true -sn "ai_uv_remapr" -ln "aiUvRemapR" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapg" -ln "aiUvRemapG" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapb" -ln "aiUvRemapB" -at "float" -p "aiUvRemap";
	addAttr -ci true -k true -sn "ai_translator" -ln "aiTranslator" -dt "string";
	addAttr -ci true -k true -sn "filtermap" -ln "filtermap" -at "message";
	addAttr -ci true -k true -sn "focal_distance" -ln "focalDistance" -dv 5 -min 0 -max 
		1000000000 -smn 0 -smx 200 -at "float";
	addAttr -ci true -k true -sn "aperture_size" -ln "apertureSize" -dv 0.20000000298023224 
		-min 0 -max 20 -smn 0 -smx 0.5 -at "float";
	addAttr -ci true -k true -sn "aperture_blades" -ln "apertureBlades" -dv 6 -min 3 
		-max 40 -at "long";
	addAttr -ci true -k true -sn "aperture_blade_curvature" -ln "apertureBladeCurvature" 
		-min -20 -max 20 -smn -5 -smx 5 -at "float";
	addAttr -ci true -k true -sn "aperture_rotation" -ln "apertureRotation" -min 0 -max 
		360 -smn 0 -smx 50 -at "float";
	addAttr -ci true -k true -sn "edof" -ln "enableDOF" -min 0 -max 1 -at "bool";
	addAttr -ci true -uac -k true -sn "uv_remap" -ln "uvRemap" -at "float3" -nc 3;
	addAttr -ci true -k true -sn "uv_remapr" -ln "uvRemapR" -at "float" -p "uvRemap";
	addAttr -ci true -k true -sn "uv_remapg" -ln "uvRemapG" -at "float" -p "uvRemap";
	addAttr -ci true -k true -sn "uv_remapb" -ln "uvRemapB" -at "float" -p "uvRemap";
	addAttr -ci true -h true -k true -sn "uv_remapa" -ln "uvRemapA" -at "float";
	addAttr -ci true -k true -sn "arntr" -ln "arnoldTranslator" -dt "string";
	addAttr -ci true -k true -sn "fov" -ln "fov" -dv 90 -min 0 -max 1000000000 -smx 
		360 -at "float";
	addAttr -ci true -k true -sn "autocrop" -ln "autocrop" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "horizontal_fov" -ln "horizontalFov" -dv 60 -min 0 
		-max 1000000000 -smx 360 -at "float";
	addAttr -ci true -k true -sn "vertical_fov" -ln "verticalFov" -dv 90 -min 0 -max 
		1000000000 -smx 180 -at "float";
	addAttr -ci true -k true -sn "projective" -ln "projective" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
	setAttr -k on ".ai_translator" -type "string" "orthographic";
	setAttr -k on ".arntr" -type "string" "orthographic";
	setAttr ".lumaID" -type "string" "a:1292543810.78:0.477657204049";
createNode transform -s -n "side";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 100.1 0 0 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
	setAttr ".lumaID" -type "string" "a:1292543810.78:0.0689165906814";
createNode camera -s -n "sideShape" -p "side";
	addAttr -ci true -k true -sn "ai_filtermap" -ln "aiFiltermap" -at "message";
	addAttr -ci true -sn "ai_horizontal_fov" -ln "aiHorizontalFov" -dv 60 -min 0 -max 
		1000000000 -smx 360 -at "float";
	addAttr -ci true -sn "ai_vertical_fov" -ln "aiVerticalFov" -dv 90 -min 0 -max 1000000000 
		-smx 180 -at "float";
	addAttr -ci true -sn "ai_projective" -ln "aiProjective" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_focus_distance" -ln "aiFocusDistance" -dv 1 -min 0 -max 
		1000000000 -smn 0 -smx 200 -at "float";
	addAttr -ci true -sn "ai_aperture_size" -ln "aiApertureSize" -min 0 -max 20 -smn 
		0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_aperture_blades" -ln "aiApertureBlades" -min 3 -max 40 
		-at "long";
	addAttr -ci true -sn "ai_aperture_blade_curvature" -ln "aiApertureBladeCurvature" 
		-min -20 -max 20 -smn -5 -smx 5 -at "float";
	addAttr -ci true -sn "ai_aperture_rotation" -ln "aiApertureRotation" -min 0 -max 
		360 -smn 0 -smx 50 -at "float";
	addAttr -ci true -k true -sn "ai_edof" -ln "aiEnableDOF" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_fov" -ln "aiFov" -dv 90 -min 0 -max 1000000000 -smx 360 
		-at "float";
	addAttr -ci true -sn "ai_autocrop" -ln "aiAutocrop" -min 0 -max 1 -at "bool";
	addAttr -ci true -h true -sn "ai_uv_remapa" -ln "aiUvRemapA" -at "float";
	addAttr -ci true -uac -sn "ai_uv_remap" -ln "aiUvRemap" -at "float3" -nc 3;
	addAttr -ci true -sn "ai_uv_remapr" -ln "aiUvRemapR" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapg" -ln "aiUvRemapG" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapb" -ln "aiUvRemapB" -at "float" -p "aiUvRemap";
	addAttr -ci true -k true -sn "ai_translator" -ln "aiTranslator" -dt "string";
	addAttr -ci true -k true -sn "filtermap" -ln "filtermap" -at "message";
	addAttr -ci true -k true -sn "focal_distance" -ln "focalDistance" -dv 5 -min 0 -max 
		1000000000 -smn 0 -smx 200 -at "float";
	addAttr -ci true -k true -sn "aperture_size" -ln "apertureSize" -dv 0.20000000298023224 
		-min 0 -max 20 -smn 0 -smx 0.5 -at "float";
	addAttr -ci true -k true -sn "aperture_blades" -ln "apertureBlades" -dv 6 -min 3 
		-max 40 -at "long";
	addAttr -ci true -k true -sn "aperture_blade_curvature" -ln "apertureBladeCurvature" 
		-min -20 -max 20 -smn -5 -smx 5 -at "float";
	addAttr -ci true -k true -sn "aperture_rotation" -ln "apertureRotation" -min 0 -max 
		360 -smn 0 -smx 50 -at "float";
	addAttr -ci true -k true -sn "edof" -ln "enableDOF" -min 0 -max 1 -at "bool";
	addAttr -ci true -uac -k true -sn "uv_remap" -ln "uvRemap" -at "float3" -nc 3;
	addAttr -ci true -k true -sn "uv_remapr" -ln "uvRemapR" -at "float" -p "uvRemap";
	addAttr -ci true -k true -sn "uv_remapg" -ln "uvRemapG" -at "float" -p "uvRemap";
	addAttr -ci true -k true -sn "uv_remapb" -ln "uvRemapB" -at "float" -p "uvRemap";
	addAttr -ci true -h true -k true -sn "uv_remapa" -ln "uvRemapA" -at "float";
	addAttr -ci true -k true -sn "arntr" -ln "arnoldTranslator" -dt "string";
	addAttr -ci true -k true -sn "fov" -ln "fov" -dv 90 -min 0 -max 1000000000 -smx 
		360 -at "float";
	addAttr -ci true -k true -sn "autocrop" -ln "autocrop" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "horizontal_fov" -ln "horizontalFov" -dv 60 -min 0 
		-max 1000000000 -smx 360 -at "float";
	addAttr -ci true -k true -sn "vertical_fov" -ln "verticalFov" -dv 90 -min 0 -max 
		1000000000 -smx 180 -at "float";
	addAttr -ci true -k true -sn "projective" -ln "projective" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
	setAttr -k on ".ai_translator" -type "string" "orthographic";
	setAttr -k on ".arntr" -type "string" "orthographic";
	setAttr ".lumaID" -type "string" "a:1292543810.78:0.883109732476";
createNode transform -n "persp1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".t" -type "double3" -3.4230987654118594e-005 1.6605839956969397 -0.0098064157742902697 ;
	setAttr ".r" -type "double3" -90.338352729604253 0.19999999999997223 3.7272352249325446e-017 ;
	setAttr ".lumaID" -type "string" "a:1317168474.93:0.86833865998";
createNode camera -n "persp1Shape" -p "persp1";
	addAttr -ci true -k true -sn "ai_filtermap" -ln "aiFiltermap" -at "message";
	addAttr -ci true -sn "ai_horizontal_fov" -ln "aiHorizontalFov" -dv 60 -min 0 -max 
		1000000000 -smx 360 -at "float";
	addAttr -ci true -sn "ai_vertical_fov" -ln "aiVerticalFov" -dv 90 -min 0 -max 1000000000 
		-smx 180 -at "float";
	addAttr -ci true -sn "ai_projective" -ln "aiProjective" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_focus_distance" -ln "aiFocusDistance" -dv 1 -min 0 -max 
		1000000000 -smn 0 -smx 200 -at "float";
	addAttr -ci true -sn "ai_aperture_size" -ln "aiApertureSize" -min 0 -max 20 -smn 
		0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_aperture_blades" -ln "aiApertureBlades" -min 3 -max 40 
		-at "long";
	addAttr -ci true -sn "ai_aperture_blade_curvature" -ln "aiApertureBladeCurvature" 
		-min -20 -max 20 -smn -5 -smx 5 -at "float";
	addAttr -ci true -sn "ai_aperture_rotation" -ln "aiApertureRotation" -min 0 -max 
		360 -smn 0 -smx 50 -at "float";
	addAttr -ci true -k true -sn "ai_edof" -ln "aiEnableDOF" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_fov" -ln "aiFov" -dv 90 -min 0 -max 1000000000 -smx 360 
		-at "float";
	addAttr -ci true -sn "ai_autocrop" -ln "aiAutocrop" -min 0 -max 1 -at "bool";
	addAttr -ci true -h true -sn "ai_uv_remapa" -ln "aiUvRemapA" -at "float";
	addAttr -ci true -uac -sn "ai_uv_remap" -ln "aiUvRemap" -at "float3" -nc 3;
	addAttr -ci true -sn "ai_uv_remapr" -ln "aiUvRemapR" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapg" -ln "aiUvRemapG" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapb" -ln "aiUvRemapB" -at "float" -p "aiUvRemap";
	addAttr -ci true -k true -sn "ai_translator" -ln "aiTranslator" -dt "string";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr -k off ".v";
	setAttr ".fl" 34.999999999999986;
	setAttr ".coi" 1.6606129512101384;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".hc" -type "string" "viewSet -p %camera";
	setAttr -k on ".ai_translator" -type "string" "perspective";
	setAttr ".lumaID" -type "string" "a:1317168474.93:0.295984196428";
createNode transform -n "group1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1317170712.86:0.154491558146";
createNode transform -n "pPlane1" -p "group1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1317168465.15:0.931902382119";
createNode mesh -n "pPlaneShape1" -p "pPlane1";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_self_shadows" -ln "aiSelfShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_opaque" -ln "aiOpaque" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_vid" -ln "aiVisibleInDiffuse" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_vig" -ln "aiVisibleInGlossy" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "ai_subdiv_type" -ln "aiSubdivType" -min 0 -max 2 -en "none:catclark:linear" 
		-at "enum";
	addAttr -ci true -sn "ai_subdiv_iterations" -ln "aiSubdivIterations" -dv 1 -min 
		0 -max 100 -smn 0 -smx 10 -at "long";
	addAttr -ci true -sn "ai_subdiv_adaptive_metric" -ln "aiSubdivAdaptiveMetric" -min 
		0 -max 2 -en "auto:edge_length:flatness" -at "enum";
	addAttr -ci true -sn "ai_subdiv_pixel_error" -ln "aiSubdivPixelError" -min 0 -smx 
		10 -at "float";
	addAttr -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -sn "ai_subdiv_uv_smoothing" -ln "aiSubdivUvSmoothing" -min 0 -max 
		3 -en "pin_corners:pin_borders:linear:smooth" -at "enum";
	addAttr -ci true -sn "ai_subdiv_smooth_derivs" -ln "aiSubdivSmoothDerivs" -min 0 
		-max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exptan" -ln "aiExportTangents" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_expcol" -ln "aiExportColors" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprpt" -ln "aiExportRefPoints" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprnrm" -ln "aiExportRefNormals" -min 0 -max 1 
		-at "bool";
	addAttr -ci true -k true -sn "ai_exprtan" -ln "aiExportRefTangents" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	addAttr -ci true -sn "mso" -ln "miShadingSamplesOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "msh" -ln "miShadingSamples" -min 0 -smx 8 -at "float";
	addAttr -ci true -sn "mdo" -ln "miMaxDisplaceOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "mmd" -ln "miMaxDisplace" -min 0 -smx 1 -at "float";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".lumaID" -type "string" "a:1317168465.15:0.434052028585";
createNode place3dTexture -n "place3dTexture2" -p "group1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".r" -type "double3" -90 0 0 ;
	setAttr ".lumaID" -type "string" "a:1317170510.25:0.71444603152";
createNode aiOptions -s -n "defaultArnoldRenderOptions";
	addAttr -ci true -sn "driver_exr_compression" -ln "driverExrCompression" -dv 2 
		-min 0 -max 4 -en "none:rle:zip:piz:pxr24" -at "enum";
	addAttr -ci true -k true -sn "driver_half_precision" -ln "driverHalfPrecision" -min 
		0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "driver_tiled" -ln "driverTiled" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "driver_preserve_layer_name" -ln "driverPreserveLayerName" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "driver_quality" -ln "driverQuality" -dv 100 -min 0 
		-max 100 -at "long";
	addAttr -ci true -k true -sn "driver_output_padded" -ln "driverOutputPadded" -min 
		0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "driver_gamma" -ln "driverGamma" -dv 2.2000000476837158 
		-min 9.9999997473787516e-005 -smx 5 -at "float";
	addAttr -ci true -k true -sn "driver_dither_amplitude" -ln "driverDitherAmplitude" 
		-dv 1 -at "float";
	addAttr -ci true -sn "driver_png_format" -ln "driverPngFormat" -min 0 -max 1 -en 
		"int8:int16" -at "enum";
	addAttr -ci true -sn "driver_tiff_compression" -ln "driverTiffCompression" -dv 1 
		-min 0 -max 4 -en "none:lzw:ccittrle:zip:packbits" -at "enum";
	addAttr -ci true -sn "driver_tiff_format" -ln "driverTiffFormat" -min 0 -max 3 -en 
		"int8:int16:int32:float32" -at "enum";
	addAttr -ci true -k true -sn "driver_unpremult_alpha" -ln "driverUnpremultAlpha" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "filter_width" -ln "filterWidth" -dv 3 -at "float";
	addAttr -ci true -sn "filter_domain" -ln "filterDomain" -min 0 -max 1 -en "first_hit:all_hits" 
		-at "enum";
	addAttr -ci true -k true -sn "filter_minimum" -ln "filterMinimum" -at "float";
	addAttr -ci true -k true -sn "filter_maximum" -ln "filterMaximum" -dv 1 -at "float";
	addAttr -ci true -k true -sn "filter_scalar_mode" -ln "filterScalarMode" -min 0 
		-max 1 -at "bool";
	addAttr -ci true -k true -sn "driver_filename" -ln "driverFilename" -dt "string";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".img" -type "string" "tif";
	setAttr ".GI_diffuse_samples" 0;
	setAttr ".GI_glossy_samples" 0;
	setAttr ".GI_refraction_samples" 0;
	setAttr ".sss_sample_factor" 1;
	setAttr ".fltr" -type "string" "gaussian";
	setAttr ".mb_en" yes;
	setAttr ".oamask" 255;
	setAttr -k on ".driver_half_precision" yes;
	setAttr -k on ".driver_tiled" no;
	setAttr ".lumaID" -type "string" "a:1292543823.37:0.763113719731";
	setAttr ".AA_samples" 8;
createNode lightLinker -s -n "lightLinker1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr -s 12 ".lnk";
	setAttr -s 12 ".slnk";
	setAttr ".lumaID" -type "string" "a:1292543810.78:0.167805055758";
createNode displayLayerManager -n "layerManager";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292543811.36:0.590663748025";
createNode displayLayer -n "defaultLayer";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292543811.36:0.738199646578";
createNode renderLayerManager -n "renderLayerManager";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292543811.36:0.0651947789961";
createNode renderLayer -n "defaultRenderLayer";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".g" yes;
	setAttr ".lumaID" -type "string" "a:1292543811.36:0.984934109685";
createNode shadingEngine -n "ArnoldStandardShader1SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -k true -m -sn "ai_aovs" -ln "aiAOVs" -at "message";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".lumaID" -type "string" "a:1292543852.89:0.822631597294";
createNode materialInfo -n "materialInfo1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292543852.89:0.97871350292";
createNode mentalrayItemsList -s -n "mentalrayItemsList";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr -s 17 ".opt";
	setAttr ".lumaID" -type "string" "a:1292543868.14:0.16508281247";
createNode mentalrayGlobals -s -n "mentalrayGlobals";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	addAttr -ci true -h true -sn "sunAndSkyShader" -ln "sunAndSkyShader" -at "message";
	setAttr ".rvb" 5;
	setAttr ".ivb" no;
	setAttr ".xpfx" no;
	setAttr ".pct" no;
	setAttr ".ptt" no;
	setAttr ".lumaID" -type "string" "a:1292543868.14:0.926473562544";
createNode mentalrayOptions -s -n "miDefaultOptions";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	addAttr -ci true -m -sn "stringOptions" -ln "stringOptions" -at "compound" -nc 3;
	addAttr -ci true -sn "name" -ln "name" -dt "string" -p "stringOptions";
	addAttr -ci true -sn "value" -ln "value" -dt "string" -p "stringOptions";
	addAttr -ci true -sn "type" -ln "type" -dt "string" -p "stringOptions";
	setAttr ".rapc" 5;
	setAttr ".maxr" 2;
	setAttr ".lumaID" -type "string" "a:1292543868.14:0.708056388642";
	setAttr -s 28 ".stringOptions";
	setAttr ".stringOptions[0].name" -type "string" "rast motion factor";
	setAttr ".stringOptions[0].value" -type "string" "1.0";
	setAttr ".stringOptions[0].type" -type "string" "scalar";
	setAttr ".stringOptions[1].name" -type "string" "rast transparency depth";
	setAttr ".stringOptions[1].value" -type "string" "8";
	setAttr ".stringOptions[1].type" -type "string" "integer";
	setAttr ".stringOptions[2].name" -type "string" "rast useopacity";
	setAttr ".stringOptions[2].value" -type "string" "true";
	setAttr ".stringOptions[2].type" -type "string" "boolean";
	setAttr ".stringOptions[3].name" -type "string" "importon";
	setAttr ".stringOptions[3].value" -type "string" "false";
	setAttr ".stringOptions[3].type" -type "string" "boolean";
	setAttr ".stringOptions[4].name" -type "string" "importon density";
	setAttr ".stringOptions[4].value" -type "string" "1.0";
	setAttr ".stringOptions[4].type" -type "string" "scalar";
	setAttr ".stringOptions[5].name" -type "string" "importon merge";
	setAttr ".stringOptions[5].value" -type "string" "0.0";
	setAttr ".stringOptions[5].type" -type "string" "scalar";
	setAttr ".stringOptions[6].name" -type "string" "importon trace depth";
	setAttr ".stringOptions[6].value" -type "string" "0";
	setAttr ".stringOptions[6].type" -type "string" "integer";
	setAttr ".stringOptions[7].name" -type "string" "importon traverse";
	setAttr ".stringOptions[7].value" -type "string" "true";
	setAttr ".stringOptions[7].type" -type "string" "boolean";
	setAttr ".stringOptions[8].name" -type "string" "shadowmap pixel samples";
	setAttr ".stringOptions[8].value" -type "string" "3";
	setAttr ".stringOptions[8].type" -type "string" "integer";
	setAttr ".stringOptions[9].name" -type "string" "ambient occlusion";
	setAttr ".stringOptions[9].value" -type "string" "false";
	setAttr ".stringOptions[9].type" -type "string" "boolean";
	setAttr ".stringOptions[10].name" -type "string" "ambient occlusion rays";
	setAttr ".stringOptions[10].value" -type "string" "256";
	setAttr ".stringOptions[10].type" -type "string" "integer";
	setAttr ".stringOptions[11].name" -type "string" "ambient occlusion cache";
	setAttr ".stringOptions[11].value" -type "string" "false";
	setAttr ".stringOptions[11].type" -type "string" "boolean";
	setAttr ".stringOptions[12].name" -type "string" "ambient occlusion cache density";
	setAttr ".stringOptions[12].value" -type "string" "1.0";
	setAttr ".stringOptions[12].type" -type "string" "scalar";
	setAttr ".stringOptions[13].name" -type "string" "ambient occlusion cache points";
	setAttr ".stringOptions[13].value" -type "string" "64";
	setAttr ".stringOptions[13].type" -type "string" "integer";
	setAttr ".stringOptions[14].name" -type "string" "irradiance particles";
	setAttr ".stringOptions[14].value" -type "string" "false";
	setAttr ".stringOptions[14].type" -type "string" "boolean";
	setAttr ".stringOptions[15].name" -type "string" "irradiance particles rays";
	setAttr ".stringOptions[15].value" -type "string" "256";
	setAttr ".stringOptions[15].type" -type "string" "integer";
	setAttr ".stringOptions[16].name" -type "string" "irradiance particles interpolate";
	setAttr ".stringOptions[16].value" -type "string" "1";
	setAttr ".stringOptions[16].type" -type "string" "integer";
	setAttr ".stringOptions[17].name" -type "string" "irradiance particles interppoints";
	setAttr ".stringOptions[17].value" -type "string" "64";
	setAttr ".stringOptions[17].type" -type "string" "integer";
	setAttr ".stringOptions[18].name" -type "string" "irradiance particles indirect passes";
	setAttr ".stringOptions[18].value" -type "string" "0";
	setAttr ".stringOptions[18].type" -type "string" "integer";
	setAttr ".stringOptions[19].name" -type "string" "irradiance particles scale";
	setAttr ".stringOptions[19].value" -type "string" "1.0";
	setAttr ".stringOptions[19].type" -type "string" "scalar";
	setAttr ".stringOptions[20].name" -type "string" "irradiance particles env";
	setAttr ".stringOptions[20].value" -type "string" "true";
	setAttr ".stringOptions[20].type" -type "string" "boolean";
	setAttr ".stringOptions[21].name" -type "string" "irradiance particles env rays";
	setAttr ".stringOptions[21].value" -type "string" "256";
	setAttr ".stringOptions[21].type" -type "string" "integer";
	setAttr ".stringOptions[22].name" -type "string" "irradiance particles env scale";
	setAttr ".stringOptions[22].value" -type "string" "1";
	setAttr ".stringOptions[22].type" -type "string" "integer";
	setAttr ".stringOptions[23].name" -type "string" "irradiance particles rebuild";
	setAttr ".stringOptions[23].value" -type "string" "true";
	setAttr ".stringOptions[23].type" -type "string" "boolean";
	setAttr ".stringOptions[24].name" -type "string" "irradiance particles file";
	setAttr ".stringOptions[24].value" -type "string" "";
	setAttr ".stringOptions[24].type" -type "string" "string";
	setAttr ".stringOptions[25].name" -type "string" "geom displace motion factor";
	setAttr ".stringOptions[25].value" -type "string" "1.0";
	setAttr ".stringOptions[25].type" -type "string" "scalar";
	setAttr ".stringOptions[26].name" -type "string" "contrast all buffers";
	setAttr ".stringOptions[26].value" -type "string" "true";
	setAttr ".stringOptions[26].type" -type "string" "boolean";
	setAttr ".stringOptions[27].name" -type "string" "finalgather normal tolerance";
	setAttr ".stringOptions[27].value" -type "string" "25.842";
	setAttr ".stringOptions[27].type" -type "string" "scalar";
createNode mentalrayFramebuffer -s -n "miDefaultFramebuffer";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292543868.15:0.77701318097";
createNode script -n "sceneConfigurationScriptNode";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".st" 6;
	setAttr ".lumaID" -type "string" "a:1292543919.2:0.0285823201715";
createNode file -n "file1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	addAttr -ci true -sn "resolution" -ln "resolution" -dv 32 -at "long";
	setAttr ".ftn" -type "string" "/Volumes/luma/dev/byPrefix/DEV/DEV_000_CDO/lite/maya//sourceimages/pPlaneShape1.jpg";
	setAttr ".lumaID" -type "string" "a:1292615508.8:0.610688249863";
	setAttr ".resolution" 256;
createNode surfaceShader -n "surfaceShader1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292615518.55:0.011718405467";
createNode shadingEngine -n "surfaceShader1SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -k true -m -sn "ai_aovs" -ln "aiAOVs" -at "message";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".lumaID" -type "string" "a:1292615518.57:0.731679023426";
createNode materialInfo -n "materialInfo2";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292615518.57:0.330743273861";
createNode shadingEngine -n "displacementShader1SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -k true -m -sn "ai_aovs" -ln "aiAOVs" -at "message";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".lumaID" -type "string" "a:1292637323.63:0.375187587026";
createNode materialInfo -n "materialInfo3";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292637323.63:0.605764353569";
createNode shadingEngine -n "ArnoldDisplacementShader1SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -k true -m -sn "ai_aovs" -ln "aiAOVs" -at "message";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".lumaID" -type "string" "a:1292637325.9:0.989227967657";
createNode materialInfo -n "materialInfo4";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292637325.9:0.130860848452";
createNode surfaceShader -n "surfaceShader2";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292637343.42:0.786098897522";
createNode shadingEngine -n "surfaceShader2SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -k true -m -sn "ai_aovs" -ln "aiAOVs" -at "message";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".lumaID" -type "string" "a:1292637343.42:0.371353149395";
createNode materialInfo -n "materialInfo5";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292637343.42:0.138685312503";
createNode ramp -n "ramp1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr -s 3 ".cel";
	setAttr ".cel[0].ep" 0;
	setAttr ".cel[0].ec" -type "float3" 1 0 0 ;
	setAttr ".cel[1].ep" 0.5;
	setAttr ".cel[1].ec" -type "float3" 0 1 0 ;
	setAttr ".cel[2].ep" 1;
	setAttr ".cel[2].ec" -type "float3" 0 0 1 ;
	setAttr ".lumaID" -type "string" "a:1295462112.04:0.757327071153";
createNode place2dTexture -n "place2dTexture1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1295462112.13:0.445756576247";
createNode mentalrayOptions -s -n "miContourPreset";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1295552742.68:0.265652434205";
createNode mentalrayOptions -s -n "Draft";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".rapc" 5;
	setAttr ".maxr" 2;
	setAttr ".lumaID" -type "string" "a:1295552742.68:0.247876576293";
createNode mentalrayOptions -s -n "DraftMotionBlur";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".rapc" 5;
	setAttr ".maxr" 2;
	setAttr ".mb" 1;
	setAttr ".tconr" 1;
	setAttr ".tcong" 1;
	setAttr ".tconb" 1;
	setAttr ".tcona" 1;
	setAttr ".lumaID" -type "string" "a:1295552742.68:0.372206228205";
createNode mentalrayOptions -s -n "DraftRapidMotion";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".scan" 3;
	setAttr ".rapc" 6;
	setAttr ".raps" 0.25;
	setAttr ".maxr" 2;
	setAttr ".mb" 1;
	setAttr ".tconr" 1;
	setAttr ".tcong" 1;
	setAttr ".tconb" 1;
	setAttr ".tcona" 1;
	setAttr ".lumaID" -type "string" "a:1295552742.68:0.766622745283";
createNode mentalrayOptions -s -n "Preview";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".minsp" -1;
	setAttr ".maxsp" 1;
	setAttr ".fil" 1;
	setAttr ".rapc" 6;
	setAttr ".raps" 3;
	setAttr ".rflr" 2;
	setAttr ".rfrr" 2;
	setAttr ".maxr" 4;
	setAttr ".lumaID" -type "string" "a:1295552742.68:0.412697133773";
createNode mentalrayOptions -s -n "PreviewMotionblur";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".minsp" -1;
	setAttr ".maxsp" 1;
	setAttr ".fil" 1;
	setAttr ".rapc" 6;
	setAttr ".raps" 3;
	setAttr ".rflr" 2;
	setAttr ".rfrr" 2;
	setAttr ".maxr" 4;
	setAttr ".mb" 1;
	setAttr ".tconr" 0.5;
	setAttr ".tcong" 0.5;
	setAttr ".tconb" 0.5;
	setAttr ".tcona" 0.5;
	setAttr ".lumaID" -type "string" "a:1295552742.68:0.329312645643";
createNode mentalrayOptions -s -n "PreviewRapidMotion";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".minsp" -1;
	setAttr ".maxsp" 1;
	setAttr ".fil" 1;
	setAttr ".scan" 3;
	setAttr ".rapc" 3;
	setAttr ".rflr" 2;
	setAttr ".rfrr" 2;
	setAttr ".maxr" 4;
	setAttr ".mb" 1;
	setAttr ".tconr" 0.5;
	setAttr ".tcong" 0.5;
	setAttr ".tconb" 0.5;
	setAttr ".tcona" 0.5;
	setAttr ".lumaID" -type "string" "a:1295552742.68:0.345565148008";
createNode mentalrayOptions -s -n "PreviewCaustics";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".minsp" -1;
	setAttr ".maxsp" 1;
	setAttr ".fil" 1;
	setAttr ".rapc" 6;
	setAttr ".raps" 3;
	setAttr ".rflr" 2;
	setAttr ".rfrr" 2;
	setAttr ".maxr" 4;
	setAttr ".ca" yes;
	setAttr ".cc" 1;
	setAttr ".cr" 1;
	setAttr ".lumaID" -type "string" "a:1295552742.68:0.0417519332973";
createNode mentalrayOptions -s -n "PreviewGlobalIllum";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".minsp" -1;
	setAttr ".maxsp" 1;
	setAttr ".fil" 1;
	setAttr ".rapc" 6;
	setAttr ".raps" 3;
	setAttr ".rflr" 2;
	setAttr ".rfrr" 2;
	setAttr ".maxr" 4;
	setAttr ".gi" yes;
	setAttr ".gc" 1;
	setAttr ".gr" 1;
	setAttr ".lumaID" -type "string" "a:1295552742.68:0.139756219622";
createNode mentalrayOptions -s -n "PreviewFinalGather";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".minsp" -1;
	setAttr ".maxsp" 1;
	setAttr ".fil" 1;
	setAttr ".rapc" 6;
	setAttr ".raps" 3;
	setAttr ".rflr" 2;
	setAttr ".rfrr" 2;
	setAttr ".maxr" 4;
	setAttr ".fg" yes;
	setAttr ".lumaID" -type "string" "a:1295552742.69:0.733150745801";
createNode mentalrayOptions -s -n "Production";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".minsp" 0;
	setAttr ".maxsp" 2;
	setAttr ".fil" 2;
	setAttr ".rapc" 8;
	setAttr ".raps" 4;
	setAttr ".rflr" 10;
	setAttr ".rfrr" 10;
	setAttr ".maxr" 20;
	setAttr ".lumaID" -type "string" "a:1295552742.69:0.623892455497";
createNode mentalrayOptions -s -n "ProductionMotionblur";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".minsp" 0;
	setAttr ".maxsp" 2;
	setAttr ".fil" 2;
	setAttr ".rapc" 8;
	setAttr ".raps" 4;
	setAttr ".rflr" 10;
	setAttr ".rfrr" 10;
	setAttr ".maxr" 20;
	setAttr ".mb" 2;
	setAttr ".lumaID" -type "string" "a:1295552742.69:0.392979727637";
createNode mentalrayOptions -s -n "ProductionRapidMotion";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".minsp" 0;
	setAttr ".maxsp" 2;
	setAttr ".fil" 2;
	setAttr ".scan" 3;
	setAttr ".rapc" 8;
	setAttr ".raps" 2;
	setAttr ".rflr" 10;
	setAttr ".rfrr" 10;
	setAttr ".maxr" 20;
	setAttr ".mb" 2;
	setAttr ".lumaID" -type "string" "a:1295552742.69:0.771077694131";
createNode mentalrayOptions -s -n "ProductionFineTrace";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".conr" 0.019999999552965164;
	setAttr ".cong" 0.019999999552965164;
	setAttr ".conb" 0.019999999552965164;
	setAttr ".minsp" 1;
	setAttr ".maxsp" 2;
	setAttr ".fil" 1;
	setAttr ".filw" 0.75;
	setAttr ".filh" 0.75;
	setAttr ".jit" yes;
	setAttr ".scan" 0;
	setAttr ".rapc" 6;
	setAttr ".raps" 3;
	setAttr ".lumaID" -type "string" "a:1295552742.69:0.570050407279";
createNode mentalrayOptions -s -n "ProductionRapidFur";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".conr" 0.039999999105930328;
	setAttr ".cong" 0.029999999329447746;
	setAttr ".conb" 0.070000000298023224;
	setAttr ".minsp" 0;
	setAttr ".maxsp" 2;
	setAttr ".fil" 1;
	setAttr ".filw" 1.1449999809265137;
	setAttr ".filh" 1.1449999809265137;
	setAttr ".jit" yes;
	setAttr ".scan" 3;
	setAttr ".rapc" 3;
	setAttr ".raps" 0.25;
	setAttr ".ray" no;
	setAttr ".shmap" 3;
	setAttr ".mbsm" no;
	setAttr ".bism" 0.019999999552965164;
	setAttr ".lumaID" -type "string" "a:1295552742.69:0.118368502083";
createNode mentalrayOptions -s -n "ProductionRapidHair";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".conr" 0.039999999105930328;
	setAttr ".cong" 0.029999999329447746;
	setAttr ".conb" 0.070000000298023224;
	setAttr ".minsp" 0;
	setAttr ".maxsp" 2;
	setAttr ".fil" 1;
	setAttr ".filw" 1.1449999809265137;
	setAttr ".filh" 1.1449999809265137;
	setAttr ".jit" yes;
	setAttr ".scan" 3;
	setAttr ".rapc" 6;
	setAttr ".ray" no;
	setAttr ".shmap" 3;
	setAttr ".mbsm" no;
	setAttr ".bism" 0.019999999552965164;
	setAttr ".lumaID" -type "string" "a:1295552742.69:0.953182114998";
createNode shadingEngine -n "ArnoldStandardShader2SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -k true -m -sn "ai_aovs" -ln "aiAOVs" -at "message";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".lumaID" -type "string" "a:1295554445.08:0.727766031669";
createNode materialInfo -n "materialInfo6";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1295554445.08:0.565720536964";
createNode aiStandard -n "aiStandard1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1303323616.31:0.428443242721";
createNode shadingEngine -n "aiStandard1SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -k true -m -sn "ai_aovs" -ln "aiAOVs" -at "message";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".lumaID" -type "string" "a:1303323616.33:0.612520181505";
createNode materialInfo -n "materialInfo7";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1303323616.33:0.817786567497";
createNode aiWriteColor -n "aiWriteColor1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".aov_name" -type "string" "customColo";
	setAttr ".lumaID" -type "string" "a:1303513193.2:0.763179307074";
createNode surfaceShader -n "surfaceShader3";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1310166001.9:0.241648970725";
createNode shadingEngine -n "surfaceShader3SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -k true -m -sn "ai_aovs" -ln "aiAOVs" -at "message";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".lumaID" -type "string" "a:1310166001.91:0.487384395222";
createNode materialInfo -n "materialInfo8";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1310166001.91:0.648140939226";
createNode ramp -n "ramp2";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr -s 3 ".cel";
	setAttr ".cel[0].ep" 0;
	setAttr ".cel[0].ec" -type "float3" 1 0 0 ;
	setAttr ".cel[1].ep" 0.5;
	setAttr ".cel[1].ec" -type "float3" 0 1 0 ;
	setAttr ".cel[2].ep" 1;
	setAttr ".cel[2].ec" -type "float3" 0 0 1 ;
	setAttr ".lumaID" -type "string" "a:1310166007.16:0.0766800892567";
createNode place2dTexture -n "point2_comp_to_point2_comp";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1310166007.18:0.162886383048";
createNode samplerInfo -n "samplerInfo1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".n" -type "float3" 0 0 0.99999994 ;
	setAttr ".r" -type "float3" 0 0 0.99999994 ;
	setAttr ".lumaID" -type "string" "a:1310167358.3:0.484836311764";
createNode clamp -n "color_comp_to_point2_comp";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".mn" -type "float3" -0.5 -0.5 -0.5 ;
	setAttr ".mx" -type "float3" 0.89999998 0.89999998 0.89999998 ;
	setAttr ".lumaID" -type "string" "a:1310167376.34:0.768693323095";
createNode mia_exposure_simple -n "mia_exposure_simple1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".S02" 0;
	setAttr ".S03" 0;
	setAttr ".lumaID" -type "string" "a:1310598901.52:0.167014046806";
createNode plusMinusAverage -n "vec_comp_to_vec_comp";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".i2[0]" -type "float2" 0 0;
	setAttr ".lumaID" -type "string" "a:1310598985.02:0.205080308206";
createNode remapValue -n "float_to_vec_component";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr -s 3 ".vl[0:2]"  0 0 2 1 1 1 0.23478261 0.77999997 
		2;
	setAttr -s 2 ".cl";
	setAttr ".cl[0].cli" 1;
	setAttr ".cl[1].clp" 1;
	setAttr ".cl[1].clc" -type "float3" 1 1 1 ;
	setAttr ".cl[1].cli" 1;
	setAttr ".lumaID" -type "string" "a:1310599083.56:0.811129207641";
createNode multiplyDivide -n "multiplyDivide1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1310599121.82:0.85752713221";
createNode multiplyDivide -n "multiplyDivide2";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1310600436.64:0.304362595478";
createNode checker -n "outAlpha";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1312496695.47:0.49259582169";
createNode place2dTexture -n "place2dTexture3";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".re" -type "float2" 4 4 ;
	setAttr ".lumaID" -type "string" "a:1312496695.51:0.51221712097";
createNode aiStandard -n "aiStandard2";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1312497208.46:0.424666439728";
createNode surfaceShader -n "surfaceShader4";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1318963997.57:0.81346545914";
createNode shadingEngine -n "surfaceShader4SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".lumaID" -type "string" "a:1318963997.59:0.459891105116";
createNode materialInfo -n "materialInfo9";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1318963997.59:0.325681427104";
createNode aiAmbientOcclusion -n "aiAmbientOcclusion1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".spread" 0.5;
	setAttr ".white" -type "float3" 0.46865034 0.46865034 0.46865034 ;
	setAttr ".lumaID" -type "string" "a:1318964090.42:0.00242041854394";
createNode aiAmbientOcclusion -n "aiAmbientOcclusion2";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1318964094.75:0.368413419687";
createNode animCurveTL -n "group1_translateX";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 3 ".ktv[0:2]"  1 -0.92967018813264624 3 0 5 1.4399295410216557;
	setAttr ".lumaID" -type "string" "a:1317170718.23:0.493394429026";
createNode animCurveTL -n "group1_translateY";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  3 0;
	setAttr ".lumaID" -type "string" "a:1317170718.23:0.891778835236";
createNode animCurveTL -n "group1_translateZ";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  3 0;
	setAttr ".lumaID" -type "string" "a:1317170718.23:0.35262924947";
createNode polyPlane -n "polyPlane1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".sw" 1;
	setAttr ".sh" 1;
	setAttr ".cuv" 2;
	setAttr ".lumaID" -type "string" "a:1317168465.14:0.951407156813";
createNode materialInfo -n "tempExport_CDO_materialInfo1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1317168496.14:0.903125533971";
createNode shadingEngine -n "tempExport_CDO_surfaceShader1SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".lumaID" -type "string" "a:1317168496.14:0.673086117312";
createNode surfaceShader -n "tempExport_CDO_surfaceShader1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1317168496.12:0.938402515383";
createNode projection -n "projection2";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ua" 180.00000500895632;
	setAttr ".va" 90.000002504478161;
	setAttr ".vt1" -type "float2" 0.5 0.5 ;
	setAttr ".vt2" -type "float2" 0.5 0.5 ;
	setAttr ".vt3" -type "float2" 0.5 0.5 ;
	setAttr ".lumaID" -type "string" "a:1317170501.37:0.466037154435";
createNode checker -n "checker1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1317168502.78:0.706495120423";
createNode place2dTexture -n "tempExport_CDO_place2dTexture1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".re" -type "float2" 4 4 ;
	setAttr ".lumaID" -type "string" "a:1317168502.79:0.802971792062";
createNode aiAOVFilter -s -n "defaultArnoldFilter";
	addAttr -ci true -sn "width" -ln "width" -dv 2 -at "float";
	addAttr -ci true -sn "domain" -ln "domain" -min 0 -max 1 -en "first_hit:all_hits" 
		-at "enum";
	addAttr -ci true -sn "minimum" -ln "minimum" -at "float";
	addAttr -ci true -sn "maximum" -ln "maximum" -dv 1 -at "float";
	addAttr -ci true -sn "scalar_mode" -ln "scalarMode" -min 0 -max 1 -at "bool";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVDriver -s -n "defaultArnoldDriver";
	addAttr -ci true -sn "exr_compression" -ln "exrCompression" -dv 2 -min 0 -max 4 
		-en "none:rle:zip:piz:pxr24" -at "enum";
	addAttr -ci true -sn "half_precision" -ln "halfPrecision" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "tiled" -ln "tiled" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "preserve_layer_name" -ln "preserveLayerName" -min 0 -max 1 
		-at "bool";
	addAttr -ci true -sn "quality" -ln "quality" -dv 100 -min 0 -max 100 -at "long";
	addAttr -ci true -sn "output_padded" -ln "outputPadded" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "gamma" -ln "gamma" -dv 2.2000000476837158 -min 9.9999997473787516e-005 
		-smx 5 -at "float";
	addAttr -ci true -sn "dither_amplitude" -ln "ditherAmplitude" -dv 1 -at "float";
	addAttr -ci true -sn "png_format" -ln "pngFormat" -min 0 -max 1 -en "int8:int16" 
		-at "enum";
	addAttr -ci true -sn "tiff_compression" -ln "tiffCompression" -dv 1 -min 0 -max 
		4 -en "none:lzw:ccittrle:zip:packbits" -at "enum";
	addAttr -ci true -sn "tiff_format" -ln "tiffFormat" -min 0 -max 3 -en "int8:int16:int32:float32" 
		-at "enum";
	addAttr -ci true -sn "unpremult_alpha" -ln "unpremultAlpha" -min 0 -max 1 -at "bool";
	setAttr ".ai_translator" -type "string" "tif";
	setAttr ".tiled" no;
createNode script -n "uiConfigurationScriptNode";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"top\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n"
		+ "                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n"
		+ "                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n"
		+ "                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n"
		+ "            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n"
		+ "            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -shadows 0\n"
		+ "            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"side\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n"
		+ "                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n"
		+ "                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n"
		+ "                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"side\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n"
		+ "            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n"
		+ "            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n"
		+ "                -camera \"front\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n"
		+ "                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n"
		+ "                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n"
		+ "            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n"
		+ "            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n"
		+ "            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n"
		+ "                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n"
		+ "                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n"
		+ "                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n"
		+ "            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n"
		+ "            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 0\n                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 1\n                -showAssets 1\n                -showContainedOnly 1\n                -showPublishedAsConnected 0\n                -showContainerContents 1\n                -ignoreDagHierarchy 0\n                -expandConnections 0\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n                -autoSelectNewObjects 0\n"
		+ "                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"defaultSetFilter\" \n                -showSetMembers 1\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 1\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n"
		+ "            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"graphEditor\" -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n"
		+ "                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n"
		+ "                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n"
		+ "                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n"
		+ "                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n"
		+ "                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n"
		+ "                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dopeSheetPanel\" -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n"
		+ "                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n"
		+ "                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n"
		+ "                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n"
		+ "                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n"
		+ "                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"clipEditorPanel\" -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"sequenceEditorPanel\" -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n"
		+ "                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperGraphPanel\" -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n"
		+ "                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range -1 -1 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n"
		+ "                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range -1 -1 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperShadePanel\" -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"visorPanel\" -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Texture Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"polyTexturePlacementPanel\" -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"renderWindowPanel\" -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\tif ($useSceneConfig) {\n\t\tscriptedPanel -e -to $panelName;\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"blendShapePanel\" (localizedPanelLabel(\"Blend Shape\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tblendShapePanel -unParent -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tblendShapePanel -edit -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynRelEdPanel\" -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"relationshipPanel\" -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"referenceEditorPanel\" -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"componentEditorPanel\" -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynPaintScriptedPanelType\" -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"scriptEditorPanel\" -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"Stereo\" (localizedPanelLabel(\"Stereo\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"Stereo\" -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels `;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n"
		+ "                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -colorResolution 4 4 \n                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n"
		+ "                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n                -displayMode \"centerEye\" \n                -viewColor 0 0 0 1 \n                $editorName;\nstereoCameraView -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels  $panelName;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n"
		+ "                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -colorResolution 4 4 \n                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n"
		+ "                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n                -displayMode \"centerEye\" \n                -viewColor 0 0 0 1 \n                $editorName;\nstereoCameraView -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n"
		+ "        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-defaultImage \"\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        setFocus `paneLayout -q -p1 $gMainPane`;\n        sceneUIReplacement -deleteRemaining;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
select -ne :time1;
	setAttr -av -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".o" 3;
	setAttr ".unw" 3;
select -ne :renderPartition;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 12 ".st";
	setAttr -cb on ".an";
	setAttr -cb on ".pt";
select -ne :initialShadingGroup;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -av -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".mwc";
	setAttr -cb on ".an";
	setAttr -cb on ".il";
	setAttr -cb on ".vo";
	setAttr -cb on ".eo";
	setAttr -cb on ".fo";
	setAttr -cb on ".epo";
	setAttr ".ro" yes;
	setAttr -cb on ".mimt";
	setAttr -cb on ".miop";
	setAttr -cb on ".mise";
	setAttr -cb on ".mism";
	setAttr -cb on ".mice";
	setAttr -av -cb on ".micc";
	setAttr -cb on ".mica";
	setAttr -cb on ".micw";
	setAttr -cb on ".mirw";
select -ne :initialParticleSE;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".mwc";
	setAttr -cb on ".an";
	setAttr -cb on ".il";
	setAttr -cb on ".vo";
	setAttr -cb on ".eo";
	setAttr -cb on ".fo";
	setAttr -cb on ".epo";
	setAttr ".ro" yes;
	setAttr -cb on ".mimt";
	setAttr -cb on ".miop";
	setAttr -cb on ".mise";
	setAttr -cb on ".mism";
	setAttr -cb on ".mice";
	setAttr -cb on ".micc";
	setAttr -cb on ".mica";
	setAttr -cb on ".micw";
	setAttr -cb on ".mirw";
select -ne :defaultShaderList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 12 ".s";
select -ne :defaultTextureList1;
	setAttr -s 5 ".tx";
select -ne :postProcessList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
	setAttr -s 12 ".u";
select -ne :renderGlobalsList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
select -ne :defaultRenderGlobals;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".macc";
	setAttr -k on ".macd";
	setAttr -k on ".macq";
	setAttr -k on ".mcfr";
	setAttr -cb on ".ifg";
	setAttr -k on ".clip";
	setAttr -k on ".edm";
	setAttr -k on ".edl";
	setAttr -cb on ".ren" -type "string" "arnold";
	setAttr -av -k on ".esr";
	setAttr -k on ".ors";
	setAttr -cb on ".sdf";
	setAttr -av -k on ".outf" 51;
	setAttr -cb on ".imfkey" -type "string" "tif";
	setAttr -k on ".gama";
	setAttr -k on ".ar";
	setAttr -k on ".fs";
	setAttr -k on ".ef";
	setAttr -av -k on ".bfs";
	setAttr -k on ".me";
	setAttr -k on ".se";
	setAttr -k on ".be";
	setAttr ".ep" 4;
	setAttr -k on ".fec";
	setAttr -k on ".ofc";
	setAttr -cb on ".ofe";
	setAttr -cb on ".efe";
	setAttr -k on ".oft";
	setAttr -cb on ".umfn";
	setAttr -cb on ".ufe";
	setAttr ".pff" yes;
	setAttr -cb on ".peie";
	setAttr -cb on ".ifp" -type "string" "testrender";
	setAttr -k on ".comp";
	setAttr -k on ".cth";
	setAttr -k on ".soll";
	setAttr -k on ".rd";
	setAttr -k on ".lp";
	setAttr -av -k on ".sp";
	setAttr -k on ".shs";
	setAttr -k on ".lpr";
	setAttr -cb on ".gv";
	setAttr -cb on ".sv";
	setAttr -k on ".mm";
	setAttr -k on ".npu";
	setAttr -k on ".itf";
	setAttr -k on ".shp";
	setAttr -cb on ".isp";
	setAttr -k on ".uf";
	setAttr -k on ".oi";
	setAttr -k on ".rut";
	setAttr -k on ".mb";
	setAttr -av -k on ".mbf";
	setAttr -k on ".afp";
	setAttr -k on ".pfb";
	setAttr -k on ".pram";
	setAttr -k on ".poam";
	setAttr -k on ".prlm";
	setAttr -k on ".polm";
	setAttr -cb on ".prm";
	setAttr -cb on ".pom";
	setAttr -cb on ".pfrm";
	setAttr -cb on ".pfom";
	setAttr -av -k on ".bll";
	setAttr -k on ".bls";
	setAttr -av -k on ".smv";
	setAttr -k on ".ubc";
	setAttr -k on ".mbc";
	setAttr -cb on ".mbt";
	setAttr -k on ".udbx";
	setAttr -k on ".smc";
	setAttr -k on ".kmv";
	setAttr -cb on ".isl";
	setAttr -cb on ".ism";
	setAttr -cb on ".imb";
	setAttr -k on ".rlen";
	setAttr -av -k on ".frts";
	setAttr -k on ".tlwd";
	setAttr -k on ".tlht";
	setAttr -k on ".jfc";
	setAttr -cb on ".rsb";
	setAttr -k on ".ope";
	setAttr -k on ".oppf";
	setAttr -cb on ".hbl";
select -ne :defaultResolution;
	addAttr -ci true -sn "projectWidth" -ln "projectWidth" -at "double";
	addAttr -ci true -sn "projectHeight" -ln "projectHeight" -at "double";
	addAttr -ci true -sn "projectPixelAspect" -ln "projectPixelAspect" -at "double";
	addAttr -ci true -sn "overscanWidth" -ln "overscanWidth" -at "double";
	addAttr -ci true -sn "overscanHeight" -ln "overscanHeight" -at "double";
	addAttr -ci true -sn "projectResEnabled" -ln "projectResEnabled" -at "double";
	addAttr -ci true -sn "applyOverscan" -ln "applyOverscan" -at "double";
	addAttr -ci true -sn "correctPixelAspect" -ln "correctPixelAspect" -at "double";
	addAttr -ci true -sn "resolutionMultiplier" -ln "resolutionMultiplier" -min 0.01 
		-max 10 -at "double";
	setAttr -k on ".cch";
	setAttr -k on ".ihi";
	setAttr -k on ".nds";
	setAttr -k on ".bnm";
	setAttr -av ".w" 320;
	setAttr -av ".h" 240;
	setAttr -av -k on ".pa" 1;
	setAttr -k on ".al";
	setAttr -av ".dar" 1.3329999446868896;
	setAttr -k on ".ldar";
	setAttr -k on ".dpi";
	setAttr -k on ".off";
	setAttr -k on ".fld";
	setAttr -k on ".zsl";
	setAttr -k on ".isu";
	setAttr -k on ".pdu";
	setAttr ".projectWidth" 2048;
	setAttr ".projectHeight" 1556;
	setAttr ".projectPixelAspect" 1;
	setAttr ".overscanWidth" 1;
	setAttr ".overscanHeight" 1;
	setAttr ".applyOverscan" 1;
	setAttr ".correctPixelAspect" 1;
	setAttr ".resolutionMultiplier" 1;
select -ne :defaultLightSet;
	setAttr -k on ".cch";
	setAttr -k on ".ihi";
	setAttr -k on ".nds";
	setAttr -k on ".bnm";
	setAttr -k on ".mwc";
	setAttr -k on ".an";
	setAttr -k on ".il";
	setAttr -k on ".vo";
	setAttr -k on ".eo";
	setAttr -k on ".fo";
	setAttr -k on ".epo";
	setAttr ".ro" yes;
select -ne :defaultObjectSet;
	setAttr ".ro" yes;
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
select -ne :defaultHardwareRenderGlobals;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".rp";
	setAttr -k on ".cai";
	setAttr -k on ".coi";
	setAttr -cb on ".bc";
	setAttr -av -k on ".bcb";
	setAttr -av -k on ".bcg";
	setAttr -av -k on ".bcr";
	setAttr -k on ".ei";
	setAttr -k on ".ex";
	setAttr -av -k on ".es";
	setAttr -av -k on ".ef";
	setAttr -k on ".bf";
	setAttr -k on ".fii";
	setAttr -av -k on ".sf";
	setAttr -k on ".gr";
	setAttr -k on ".li";
	setAttr -k on ".ls";
	setAttr -k on ".mb";
	setAttr -k on ".ti";
	setAttr -k on ".txt";
	setAttr -k on ".mpr";
	setAttr -k on ".wzd";
	setAttr ".fn" -type "string" "im";
	setAttr -k on ".if";
	setAttr ".res" -type "string" "ntsc_4d 646 485 1.333";
	setAttr -k on ".as";
	setAttr -k on ".ds";
	setAttr -k on ".lm";
	setAttr -k on ".fir";
	setAttr -k on ".aap";
	setAttr -k on ".gh";
	setAttr -cb on ".sd";
select -ne :strokeGlobals;
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292543810.78:0.491419832703";
connectAttr "mia_exposure_simple1.msg" ":perspShape.mils";
connectAttr "group1_translateX.o" "group1.tx";
connectAttr "group1_translateY.o" "group1.ty";
connectAttr "group1_translateZ.o" "group1.tz";
connectAttr "polyPlane1.out" "pPlaneShape1.i";
connectAttr ":time1.o" ":defaultArnoldRenderOptions.aaseed";
connectAttr ":defaultArnoldFilter.msg" ":defaultArnoldRenderOptions.filt";
connectAttr ":defaultArnoldDriver.msg" ":defaultArnoldRenderOptions.drvr";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "ArnoldStandardShader1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "surfaceShader1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "displacementShader1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "ArnoldDisplacementShader1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "surfaceShader2SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "ArnoldStandardShader2SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "surfaceShader3SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "surfaceShader4SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "tempExport_CDO_surfaceShader1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "ArnoldStandardShader1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "surfaceShader1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "displacementShader1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "ArnoldDisplacementShader1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "surfaceShader2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "ArnoldStandardShader2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "surfaceShader3SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "surfaceShader4SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "tempExport_CDO_surfaceShader1SG.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr "ArnoldStandardShader1SG.msg" "materialInfo1.sg";
connectAttr ":mentalrayGlobals.msg" ":mentalrayItemsList.glb";
connectAttr ":miDefaultOptions.msg" ":mentalrayItemsList.opt" -na;
connectAttr ":miContourPreset.msg" ":mentalrayItemsList.opt" -na;
connectAttr ":Draft.msg" ":mentalrayItemsList.opt" -na;
connectAttr ":DraftMotionBlur.msg" ":mentalrayItemsList.opt" -na;
connectAttr ":DraftRapidMotion.msg" ":mentalrayItemsList.opt" -na;
connectAttr ":Preview.msg" ":mentalrayItemsList.opt" -na;
connectAttr ":PreviewMotionblur.msg" ":mentalrayItemsList.opt" -na;
connectAttr ":PreviewRapidMotion.msg" ":mentalrayItemsList.opt" -na;
connectAttr ":PreviewCaustics.msg" ":mentalrayItemsList.opt" -na;
connectAttr ":PreviewGlobalIllum.msg" ":mentalrayItemsList.opt" -na;
connectAttr ":PreviewFinalGather.msg" ":mentalrayItemsList.opt" -na;
connectAttr ":Production.msg" ":mentalrayItemsList.opt" -na;
connectAttr ":ProductionMotionblur.msg" ":mentalrayItemsList.opt" -na;
connectAttr ":ProductionRapidMotion.msg" ":mentalrayItemsList.opt" -na;
connectAttr ":ProductionFineTrace.msg" ":mentalrayItemsList.opt" -na;
connectAttr ":ProductionRapidFur.msg" ":mentalrayItemsList.opt" -na;
connectAttr ":ProductionRapidHair.msg" ":mentalrayItemsList.opt" -na;
connectAttr ":miDefaultFramebuffer.msg" ":mentalrayItemsList.fb" -na;
connectAttr ":miDefaultOptions.msg" ":mentalrayGlobals.opt";
connectAttr ":miDefaultFramebuffer.msg" ":mentalrayGlobals.fb";
connectAttr "file1.oc" "surfaceShader1.oc";
connectAttr "surfaceShader1.oc" "surfaceShader1SG.ss";
connectAttr "surfaceShader1SG.msg" "materialInfo2.sg";
connectAttr "surfaceShader1.msg" "materialInfo2.m";
connectAttr "file1.msg" "materialInfo2.t" -na;
connectAttr "file1.oc" "materialInfo2.tc";
connectAttr "displacementShader1SG.msg" "materialInfo3.sg";
connectAttr "ArnoldDisplacementShader1SG.msg" "materialInfo4.sg";
connectAttr "surfaceShader2.oc" "surfaceShader2SG.ss";
connectAttr "surfaceShader2SG.msg" "materialInfo5.sg";
connectAttr "surfaceShader2.msg" "materialInfo5.m";
connectAttr "surfaceShader2.msg" "materialInfo5.t" -na;
connectAttr "place2dTexture1.o" "ramp1.uv";
connectAttr "place2dTexture1.ofs" "ramp1.fs";
connectAttr "ArnoldStandardShader2SG.msg" "materialInfo6.sg";
connectAttr "aiStandard1.out" "aiStandard1SG.ss";
connectAttr "aiStandard1SG.msg" "materialInfo7.sg";
connectAttr "aiStandard1.msg" "materialInfo7.m";
connectAttr "aiStandard1.msg" "materialInfo7.t" -na;
connectAttr "multiplyDivide1.oy" "surfaceShader3.ocg";
connectAttr "multiplyDivide1.oz" "surfaceShader3.ocb";
connectAttr "outAlpha.oa" "surfaceShader3.ocr";
connectAttr "surfaceShader3.oc" "surfaceShader3SG.ss";
connectAttr "surfaceShader3SG.msg" "materialInfo8.sg";
connectAttr "surfaceShader3.msg" "materialInfo8.m";
connectAttr "surfaceShader3.msg" "materialInfo8.t" -na;
connectAttr "point2_comp_to_point2_comp.ov" "ramp2.v";
connectAttr "point2_comp_to_point2_comp.ofs" "ramp2.fs";
connectAttr "samplerInfo1.fr" "color_comp_to_point2_comp.ipr";
connectAttr "aiAmbientOcclusion1.outr" "color_comp_to_point2_comp.ipg";
connectAttr "aiAmbientOcclusion2.outg" "color_comp_to_point2_comp.ipb";
connectAttr "ramp2.oc" "vec_comp_to_vec_comp.i3[0]";
connectAttr "vec_comp_to_vec_comp.o3x" "float_to_vec_component.i";
connectAttr "vec_comp_to_vec_comp.o3y" "multiplyDivide1.i1y";
connectAttr "vec_comp_to_vec_comp.o3z" "multiplyDivide1.i1z";
connectAttr "float_to_vec_component.ov" "multiplyDivide1.i1x";
connectAttr "vec_comp_to_vec_comp.o3" "multiplyDivide2.i1";
connectAttr "place2dTexture3.o" "outAlpha.uv";
connectAttr "place2dTexture3.ofs" "outAlpha.fs";
connectAttr "ramp2.oc" "outAlpha.c1";
connectAttr "color_comp_to_point2_comp.op" "surfaceShader4.oc";
connectAttr "surfaceShader4.oc" "surfaceShader4SG.ss";
connectAttr "surfaceShader4SG.msg" "materialInfo9.sg";
connectAttr "surfaceShader4.msg" "materialInfo9.m";
connectAttr "surfaceShader4.msg" "materialInfo9.t" -na;
connectAttr "aiAmbientOcclusion1.spread" "aiAmbientOcclusion2.spread";
connectAttr "aiAmbientOcclusion1.white" "aiAmbientOcclusion2.white";
connectAttr "tempExport_CDO_surfaceShader1SG.msg" "tempExport_CDO_materialInfo1.sg"
		;
connectAttr "tempExport_CDO_surfaceShader1.msg" "tempExport_CDO_materialInfo1.m"
		;
connectAttr "tempExport_CDO_surfaceShader1.msg" "tempExport_CDO_materialInfo1.t"
		 -na;
connectAttr "tempExport_CDO_surfaceShader1.oc" "tempExport_CDO_surfaceShader1SG.ss"
		;
connectAttr "pPlaneShape1.iog" "tempExport_CDO_surfaceShader1SG.dsm" -na;
connectAttr "projection2.oc" "tempExport_CDO_surfaceShader1.oc";
connectAttr "checker1.oc" "projection2.im";
connectAttr "place3dTexture2.wim" "projection2.pm";
connectAttr "tempExport_CDO_place2dTexture1.o" "checker1.uv";
connectAttr "tempExport_CDO_place2dTexture1.ofs" "checker1.fs";
connectAttr "ArnoldStandardShader1SG.pa" ":renderPartition.st" -na;
connectAttr "surfaceShader1SG.pa" ":renderPartition.st" -na;
connectAttr "displacementShader1SG.pa" ":renderPartition.st" -na;
connectAttr "ArnoldDisplacementShader1SG.pa" ":renderPartition.st" -na;
connectAttr "surfaceShader2SG.pa" ":renderPartition.st" -na;
connectAttr "ArnoldStandardShader2SG.pa" ":renderPartition.st" -na;
connectAttr "aiStandard1SG.pa" ":renderPartition.st" -na;
connectAttr "surfaceShader3SG.pa" ":renderPartition.st" -na;
connectAttr "surfaceShader4SG.pa" ":renderPartition.st" -na;
connectAttr "tempExport_CDO_surfaceShader1SG.pa" ":renderPartition.st" -na;
connectAttr "surfaceShader1.msg" ":defaultShaderList1.s" -na;
connectAttr "surfaceShader2.msg" ":defaultShaderList1.s" -na;
connectAttr "aiStandard1.msg" ":defaultShaderList1.s" -na;
connectAttr "aiWriteColor1.msg" ":defaultShaderList1.s" -na;
connectAttr "surfaceShader3.msg" ":defaultShaderList1.s" -na;
connectAttr "aiStandard2.msg" ":defaultShaderList1.s" -na;
connectAttr "surfaceShader4.msg" ":defaultShaderList1.s" -na;
connectAttr "aiAmbientOcclusion1.msg" ":defaultShaderList1.s" -na;
connectAttr "aiAmbientOcclusion2.msg" ":defaultShaderList1.s" -na;
connectAttr "tempExport_CDO_surfaceShader1.msg" ":defaultShaderList1.s" -na;
connectAttr "file1.msg" ":defaultTextureList1.tx" -na;
connectAttr "ramp1.msg" ":defaultTextureList1.tx" -na;
connectAttr "ramp2.msg" ":defaultTextureList1.tx" -na;
connectAttr "outAlpha.msg" ":defaultTextureList1.tx" -na;
connectAttr "checker1.msg" ":defaultTextureList1.tx" -na;
connectAttr "place2dTexture1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "point2_comp_to_point2_comp.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "samplerInfo1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "color_comp_to_point2_comp.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "mia_exposure_simple1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "vec_comp_to_vec_comp.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "float_to_vec_component.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "multiplyDivide1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "multiplyDivide2.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture3.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "tempExport_CDO_place2dTexture1.msg" ":defaultRenderUtilityList1.u" 
		-na;
connectAttr "projection2.msg" ":defaultRenderUtilityList1.u" -na;
// End of test.ma
