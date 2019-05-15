//Maya ASCII 2018ff09 scene
//Name: test.ma
//Last modified: Sat, Apr 27, 2019 04:18:16 PM
//Codeset: 1252
requires maya "2018ff09";
requires -nodeType "aiOptions" -nodeType "aiAOVDriver" -nodeType "aiAOVFilter" -nodeType "aiAmbientOcclusion"
		 -nodeType "aiStandard" -nodeType "aiWriteColor" "mtoa" "3.2.1.wip";
requires "stereoCamera" "10.0";
requires "stereoCamera" "10.0";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2018";
fileInfo "version" "2018";
fileInfo "cutIdentifier" "201807182215-4524384375";
fileInfo "osv" "Microsoft Windows 7 Enterprise Edition, 64-bit Windows 7 Service Pack 1 (Build 7601)\n";
fileInfo "saveHistory0" "/Volumes/sv-dev01/devRepo/chad/arnold/mtoa/testsuite/test_0045/data/test.ma&chad&ws-050.luma-pictures.com&Tuesday, October 18 2011 @ 06:31:48 PM (PDT)";
fileInfo "saveHistory1" "/Volumes/sv-dev01/devRepo/chad/arnold/mtoa/testsuite/test_0045/data/test.ma&chad&ws-050.luma-pictures.com&Tuesday, October 18 2011 @ 06:29:10 PM (PDT)";
fileInfo "saveHistory2" "/Volumes/sv-dev01/devRepo/chad/arnold/mtoa/testsuite/test_0041/data/test.ma&chad&ws-050.luma-pictures.com&Tuesday, October 18 2011 @ 11:59:19 AM (PDT)";
fileInfo "saveHistory3" "/Volumes/sv-dev01/devRepo/chad/arnold/mtoa/testsuite/test_0041/data/test.ma&chad&ws-050.luma-pictures.com&Tuesday, October 18 2011 @ 11:56:44 AM (PDT)";
fileInfo "nexSGFix" "True";
createNode transform -s -n "persp";
	rename -uid "80DEAE3F-4644-EDAF-DA7B-139AB9F9B01D";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -3.1443942773107727 5.9729462931136723 4.3015195927180283 ;
	setAttr ".r" -type "double3" -44.138352729560033 -399.39999999999196 0 ;
	setAttr ".lumaID" -type "string" "a:1292543810.78:0.842974524452";
createNode camera -s -n "perspShape" -p "persp";
	rename -uid "83109800-4312-CA00-02BF-6F9AECAEBA1D";
	addAttr -s false -ci true -k true -sn "filtermap" -ln "filtermap" -at "message";
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
	rename -uid "499CB78B-432F-73AB-0ECA-AB89C91EC8A2";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 100.1 0 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
	setAttr ".lumaID" -type "string" "a:1292543810.78:0.701369956448";
createNode camera -s -n "topShape" -p "top";
	rename -uid "C410E2E6-436F-9017-0058-6E96536BCF6D";
	addAttr -s false -ci true -k true -sn "filtermap" -ln "filtermap" -at "message";
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
	rename -uid "10B910A9-47AE-44BF-83B7-C793918B15FC";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 100.1 ;
	setAttr ".lumaID" -type "string" "a:1292543810.78:0.868060622798";
createNode camera -s -n "frontShape" -p "front";
	rename -uid "6EE8F83A-4284-7D31-D218-9B9FE316E5B1";
	addAttr -s false -ci true -k true -sn "filtermap" -ln "filtermap" -at "message";
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
	rename -uid "B8A37A76-4FE7-9B75-0728-6DB8EEA7363A";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 100.1 0 0 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
	setAttr ".lumaID" -type "string" "a:1292543810.78:0.0689165906814";
createNode camera -s -n "sideShape" -p "side";
	rename -uid "63D084B4-46FA-61DC-E4CC-C1A17BFD9F88";
	addAttr -s false -ci true -k true -sn "filtermap" -ln "filtermap" -at "message";
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
	rename -uid "28575036-4358-9309-289A-5FA5CE33D615";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".t" -type "double3" -3.4230987654118594e-05 1.6605839956969397 -0.0098064157742902697 ;
	setAttr ".r" -type "double3" -90.338352729604253 0.19999999999997223 3.7272352249325446e-17 ;
	setAttr ".lumaID" -type "string" "a:1317168474.93:0.86833865998";
createNode camera -n "persp1Shape" -p "persp1";
	rename -uid "B9F77AC7-4674-BCB5-339B-859FAB5985BE";
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
	rename -uid "CDA11955-46F9-7980-F578-B88FB8C86B49";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1317170712.86:0.154491558146";
createNode transform -n "pPlane1" -p "group1";
	rename -uid "C85BC815-4C42-EEEA-B87F-8684BA9B5823";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1317168465.15:0.931902382119";
createNode mesh -n "pPlaneShape1" -p "pPlane1";
	rename -uid "5F1BAA3F-46BB-96EF-DF94-32979248DE7D";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -ci true -k true -sn "ai_vid" -ln "aiVisibleInDiffuse" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_vig" -ln "aiVisibleInGlossy" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -s false -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
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
	setAttr ".ugsdt" no;
	setAttr ".vnm" 0;
	setAttr ".ai_translator" -type "string" "polymesh";
	setAttr ".lumaID" -type "string" "a:1317168465.15:0.434052028585";
createNode place3dTexture -n "place3dTexture2" -p "group1";
	rename -uid "70AB4A24-4768-60A1-10C6-F4BC385A19F2";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".r" -type "double3" -90 0 0 ;
	setAttr ".lumaID" -type "string" "a:1317170510.25:0.71444603152";
createNode aiOptions -s -n "defaultArnoldRenderOptions";
	rename -uid "7F44EBD7-45E2-C353-764A-01A000D619E3";
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
		-min 9.9999997473787516e-05 -smx 5 -at "float";
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
	addAttr -ci true -sn "ARV_options" -ln "ARV_options" -dt "string";
	setAttr ".img" -type "string" "tif";
	setAttr ".AA_samples" 8;
	setAttr ".GI_diffuse_samples" 0;
	setAttr ".fltr" -type "string" "gaussian";
	setAttr ".mb_en" yes;
	setAttr ".mb_sen" yes;
	setAttr ".oamask" 255;
	setAttr -k on ".driver_half_precision" yes;
	setAttr -k on ".driver_tiled" no;
	setAttr ".lumaID" -type "string" "a:1292543823.37:0.763113719731";
createNode lightLinker -s -n "lightLinker1";
	rename -uid "A34BEC81-489A-2DA9-0864-26AF6C534221";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr -s 12 ".lnk";
	setAttr -s 12 ".slnk";
	setAttr ".lumaID" -type "string" "a:1292543810.78:0.167805055758";
createNode displayLayerManager -n "layerManager";
	rename -uid "38ED3442-41A2-EB4B-014E-10BAC7EF090A";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292543811.36:0.590663748025";
createNode displayLayer -n "defaultLayer";
	rename -uid "0E3393F5-4F14-914B-498C-019E587BCFBB";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292543811.36:0.738199646578";
createNode renderLayerManager -n "renderLayerManager";
	rename -uid "2C66C16F-447B-BD54-D2E4-6F8548FCCD1B";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292543811.36:0.0651947789961";
createNode renderLayer -n "defaultRenderLayer";
	rename -uid "3D4534C1-4190-4233-B4A3-0AA41450A313";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".g" yes;
	setAttr ".lumaID" -type "string" "a:1292543811.36:0.984934109685";
createNode shadingEngine -n "ArnoldStandardShader1SG";
	rename -uid "07A71B0D-4FA5-9619-266A-E997896DBCBB";
	addAttr -s false -ci true -k true -m -sn "ai_aovs" -ln "aiAOVs" -at "message";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".lumaID" -type "string" "a:1292543852.89:0.822631597294";
createNode materialInfo -n "materialInfo1";
	rename -uid "9970A283-4E3D-D68F-9B01-1EA9DF373304";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292543852.89:0.97871350292";
createNode script -n "sceneConfigurationScriptNode";
	rename -uid "A3A21E41-41DB-6A81-9688-3F8FFF2A73E7";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".st" 6;
	setAttr ".lumaID" -type "string" "a:1292543919.2:0.0285823201715";
createNode file -n "file1";
	rename -uid "26F47D67-4592-4A3B-46A7-15B25DA373ED";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	addAttr -ci true -sn "resolution" -ln "resolution" -dv 32 -at "long";
	setAttr ".ftn" -type "string" "/Volumes/luma/dev/byPrefix/DEV/DEV_000_CDO/lite/maya//sourceimages/pPlaneShape1.jpg";
	setAttr ".cs" -type "string" "sRGB";
	setAttr ".lumaID" -type "string" "a:1292615508.8:0.610688249863";
	setAttr ".resolution" 256;
createNode surfaceShader -n "surfaceShader1";
	rename -uid "0A2617B8-4BFB-5D09-666D-18A77E851C41";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292615518.55:0.011718405467";
createNode shadingEngine -n "surfaceShader1SG";
	rename -uid "A0DB9878-45A5-2A87-A257-08AB8A67B86E";
	addAttr -s false -ci true -k true -m -sn "ai_aovs" -ln "aiAOVs" -at "message";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".lumaID" -type "string" "a:1292615518.57:0.731679023426";
createNode materialInfo -n "materialInfo2";
	rename -uid "DFFDA62F-4E8E-6194-F936-CB82BEB37C06";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292615518.57:0.330743273861";
createNode shadingEngine -n "displacementShader1SG";
	rename -uid "F62FA7B6-4558-3378-85D7-ADBFACB16E27";
	addAttr -s false -ci true -k true -m -sn "ai_aovs" -ln "aiAOVs" -at "message";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".lumaID" -type "string" "a:1292637323.63:0.375187587026";
createNode materialInfo -n "materialInfo3";
	rename -uid "70C45851-4FE0-F91A-D28F-EB82E89BADED";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292637323.63:0.605764353569";
createNode shadingEngine -n "ArnoldDisplacementShader1SG";
	rename -uid "1BDFB26A-4D6C-B9BE-9CD2-1197A5DD3C75";
	addAttr -s false -ci true -k true -m -sn "ai_aovs" -ln "aiAOVs" -at "message";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".lumaID" -type "string" "a:1292637325.9:0.989227967657";
createNode materialInfo -n "materialInfo4";
	rename -uid "FEB1B562-45C3-A618-FAE8-70990312B5FD";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292637325.9:0.130860848452";
createNode surfaceShader -n "surfaceShader2";
	rename -uid "4C7824F6-466D-C48D-59F3-1D9E9D022A97";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292637343.42:0.786098897522";
createNode shadingEngine -n "surfaceShader2SG";
	rename -uid "DC465E81-4745-FADF-3B23-DAAF8CC73501";
	addAttr -s false -ci true -k true -m -sn "ai_aovs" -ln "aiAOVs" -at "message";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".lumaID" -type "string" "a:1292637343.42:0.371353149395";
createNode materialInfo -n "materialInfo5";
	rename -uid "5B80492B-4C23-1F0D-D599-2FA57AA852D3";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292637343.42:0.138685312503";
createNode ramp -n "ramp1";
	rename -uid "639C9D37-40EC-BFED-45FC-63869948F4CA";
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
	rename -uid "C81E3C35-4E71-6616-2375-848F24F85E44";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1295462112.13:0.445756576247";
createNode shadingEngine -n "ArnoldStandardShader2SG";
	rename -uid "1EB1AD93-43A9-3884-FC4F-F8A57B56D52F";
	addAttr -s false -ci true -k true -m -sn "ai_aovs" -ln "aiAOVs" -at "message";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".lumaID" -type "string" "a:1295554445.08:0.727766031669";
createNode materialInfo -n "materialInfo6";
	rename -uid "ACE4830C-40E3-837D-E701-49B9B7FF55B8";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1295554445.08:0.565720536964";
createNode aiStandard -n "aiStandard1";
	rename -uid "0904D31E-4E03-9492-C7E4-5D8514C3EF73";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1303323616.31:0.428443242721";
createNode shadingEngine -n "aiStandard1SG";
	rename -uid "F7670176-4422-863C-F838-1A9AA544C382";
	addAttr -s false -ci true -k true -m -sn "ai_aovs" -ln "aiAOVs" -at "message";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".lumaID" -type "string" "a:1303323616.33:0.612520181505";
createNode materialInfo -n "materialInfo7";
	rename -uid "A4540EE2-4A0F-5841-1C92-7D9610D4BDC0";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1303323616.33:0.817786567497";
createNode aiWriteColor -n "aiWriteColor1";
	rename -uid "1670E5E7-4808-D4B5-0DB5-08B03C52C543";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".aov_name" -type "string" "customColo";
	setAttr ".lumaID" -type "string" "a:1303513193.2:0.763179307074";
createNode surfaceShader -n "surfaceShader3";
	rename -uid "EACE68B7-46CC-42F7-7764-6D9C8DEBE8A1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1310166001.9:0.241648970725";
createNode shadingEngine -n "surfaceShader3SG";
	rename -uid "F2920970-4D01-6255-0350-D7A2FDC8BF37";
	addAttr -s false -ci true -k true -m -sn "ai_aovs" -ln "aiAOVs" -at "message";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".lumaID" -type "string" "a:1310166001.91:0.487384395222";
createNode materialInfo -n "materialInfo8";
	rename -uid "7B1C889F-42E4-A18C-5F6E-3EBF79316384";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1310166001.91:0.648140939226";
createNode ramp -n "ramp2";
	rename -uid "5CDB3CA7-4536-3257-C8C2-9F954F6DEF10";
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
	rename -uid "334938CD-450E-5DC6-8075-3793DA3BAAB4";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1310166007.18:0.162886383048";
createNode samplerInfo -n "samplerInfo1";
	rename -uid "B10785F1-4D33-5A30-98D8-68972CD79FD2";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".n" -type "float3" 0 0 0.99999994 ;
	setAttr ".r" -type "float3" 0 0 0.99999994 ;
	setAttr ".lumaID" -type "string" "a:1310167358.3:0.484836311764";
createNode clamp -n "color_comp_to_point2_comp";
	rename -uid "674F0316-4FC9-9C6C-2A39-4781FF029855";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".mn" -type "float3" -0.5 -0.5 -0.5 ;
	setAttr ".mx" -type "float3" 0.89999998 0.89999998 0.89999998 ;
	setAttr ".lumaID" -type "string" "a:1310167376.34:0.768693323095";
createNode plusMinusAverage -n "vec_comp_to_vec_comp";
	rename -uid "BF4EF6D6-476C-6C15-0E3F-0EBAB3735C29";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".i2[0]" -type "float2" 0 0;
	setAttr ".lumaID" -type "string" "a:1310598985.02:0.205080308206";
createNode remapValue -n "float_to_vec_component";
	rename -uid "AE7CA7ED-4B93-C9F1-DE2B-9595252F41F4";
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
	rename -uid "E2372126-44DA-40A7-8B1D-AA847F618B3E";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1310599121.82:0.85752713221";
createNode multiplyDivide -n "multiplyDivide2";
	rename -uid "7E3BC71E-4591-1F9B-DB19-4CA5CC076F6B";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1310600436.64:0.304362595478";
createNode checker -n "outAlpha";
	rename -uid "624A8D68-4134-2A44-2D6C-2CB076AE054F";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1312496695.47:0.49259582169";
createNode place2dTexture -n "place2dTexture3";
	rename -uid "53CF38B1-45D6-BBAB-D88B-21B3762AADF9";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".re" -type "float2" 4 4 ;
	setAttr ".lumaID" -type "string" "a:1312496695.51:0.51221712097";
createNode aiStandard -n "aiStandard2";
	rename -uid "31921A33-4F00-C97D-D7E8-F8A547A67375";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1312497208.46:0.424666439728";
createNode surfaceShader -n "surfaceShader4";
	rename -uid "3EFBA7A2-43A5-B0CA-46DD-FE88A93BA850";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1318963997.57:0.81346545914";
createNode shadingEngine -n "surfaceShader4SG";
	rename -uid "E3127B88-4E87-23C8-05CC-0A8A1497EBF8";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".lumaID" -type "string" "a:1318963997.59:0.459891105116";
createNode materialInfo -n "materialInfo9";
	rename -uid "DE2AD859-4908-FA7A-9FBC-A2B836623991";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1318963997.59:0.325681427104";
createNode aiAmbientOcclusion -n "aiAmbientOcclusion1";
	rename -uid "58568C27-40D9-F95D-055D-FC8524ECC87E";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".spread" 0.5;
	setAttr ".white" -type "float3" 0.46865034 0.46865034 0.46865034 ;
	setAttr ".lumaID" -type "string" "a:1318964090.42:0.00242041854394";
createNode aiAmbientOcclusion -n "aiAmbientOcclusion2";
	rename -uid "E880EC6E-4E55-A80B-A9D9-3B9969055206";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1318964094.75:0.368413419687";
createNode animCurveTL -n "group1_translateX";
	rename -uid "9F38BDAA-4C3D-A1FF-96CE-0DAFA02F08BE";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 3 ".ktv[0:2]"  1 -0.92967018813264624 3 0 5 1.4399295410216557;
	setAttr ".lumaID" -type "string" "a:1317170718.23:0.493394429026";
createNode animCurveTL -n "group1_translateY";
	rename -uid "8BF51F59-4609-9B9B-684E-B599B1902B9C";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  3 0;
	setAttr ".lumaID" -type "string" "a:1317170718.23:0.891778835236";
createNode animCurveTL -n "group1_translateZ";
	rename -uid "1548D119-4D94-A5EC-192E-019181CB4E7C";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  3 0;
	setAttr ".lumaID" -type "string" "a:1317170718.23:0.35262924947";
createNode polyPlane -n "polyPlane1";
	rename -uid "41390E5E-4228-815F-4B18-D892C0169AE2";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".sw" 1;
	setAttr ".sh" 1;
	setAttr ".cuv" 2;
	setAttr ".lumaID" -type "string" "a:1317168465.14:0.951407156813";
createNode materialInfo -n "tempExport_CDO_materialInfo1";
	rename -uid "BFEBF630-49E1-0E7C-A903-6BB53A0EB0A3";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1317168496.14:0.903125533971";
createNode shadingEngine -n "tempExport_CDO_surfaceShader1SG";
	rename -uid "5EEEA54F-4F6C-6323-9BD0-11940127816F";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".lumaID" -type "string" "a:1317168496.14:0.673086117312";
createNode surfaceShader -n "tempExport_CDO_surfaceShader1";
	rename -uid "3E8E5B2C-4948-5196-93D8-8FB2286F94A9";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1317168496.12:0.938402515383";
createNode projection -n "projection2";
	rename -uid "DCB56173-40DD-3EE7-DE75-998A5BA8FC2A";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ua" 180.00000500895632;
	setAttr ".va" 90.000002504478161;
	setAttr ".vt1" -type "float2" 0.5 0.5 ;
	setAttr ".vt2" -type "float2" 0.5 0.5 ;
	setAttr ".vt3" -type "float2" 0.5 0.5 ;
	setAttr ".lumaID" -type "string" "a:1317170501.37:0.466037154435";
createNode checker -n "checker1";
	rename -uid "1CBC43A9-48C4-2F7E-BDAC-4BA118C7B261";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1317168502.78:0.706495120423";
createNode place2dTexture -n "tempExport_CDO_place2dTexture1";
	rename -uid "10DDAAA6-4928-7C3E-911C-1FA457F99B0B";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".re" -type "float2" 4 4 ;
	setAttr ".lumaID" -type "string" "a:1317168502.79:0.802971792062";
createNode aiAOVFilter -s -n "defaultArnoldFilter";
	rename -uid "CBE6C673-40FA-0B71-A0BA-52BEB3F5B44B";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVDriver -s -n "defaultArnoldDriver";
	rename -uid "8A1C1E08-4DC7-85CA-6413-B18B896C6819";
	addAttr -ci true -sn "gamma" -ln "gamma" -dv 2.2000000476837158 -min 9.9999997473787516e-05 
		-smx 5 -at "float";
	addAttr -ci true -sn "dither_amplitude" -ln "ditherAmplitude" -dv 1 -at "float";
	setAttr ".tiled" no;
	setAttr ".ai_translator" -type "string" "tif";
createNode script -n "uiConfigurationScriptNode";
	rename -uid "F9EFBB72-4797-2F52-BED5-5781CC5F425C";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $nodeEditorPanelVisible = stringArrayContains(\"nodeEditorPanel1\", `getPanel -vis`);\n\tint    $nodeEditorWorkspaceControlOpen = (`workspaceControl -exists nodeEditorPanel1Window` && `workspaceControl -q -visible nodeEditorPanel1Window`);\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\n\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n"
		+ "            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n"
		+ "            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n"
		+ "\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"side\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n"
		+ "            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n"
		+ "            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n"
		+ "            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n"
		+ "            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n"
		+ "            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n"
		+ "            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n"
		+ "            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n"
		+ "            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n"
		+ "            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 383\n            -height 717\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 1\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 0\n            -showReferenceMembers 0\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -organizeByClip 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showParentContainers 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n"
		+ "            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -organizeByClip 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showParentContainers 1\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n"
		+ "                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n"
		+ "                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 1\n                -autoFitTime 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -showCurveNames 0\n                -showActiveCurveNames 0\n"
		+ "                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                -valueLinesToggle 1\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n"
		+ "                -showMuteInfo 0\n                -organizeByLayer 1\n                -organizeByClip 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showParentContainers 1\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n"
		+ "                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n"
		+ "                -displayValues 0\n                -autoFit 0\n                -autoFitTime 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n"
		+ "                -displayValues 0\n                -autoFit 0\n                -autoFitTime 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -autoFitTime 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n"
		+ "                -initialized 0\n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n"
		+ "                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range -1 -1 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n"
		+ "\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n"
		+ "\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"contentBrowserPanel\" (localizedPanelLabel(\"Content Browser\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Content Browser\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"Stereo\" (localizedPanelLabel(\"Stereo\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels  $panelName;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n"
		+ "                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 4 4 \n"
		+ "                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -controllers 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n"
		+ "                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 0\n                -height 0\n                -sceneRenderFilter 0\n                -displayMode \"centerEye\" \n                -viewColor 0 0 0 1 \n                -useCustomBackground 1\n                $editorName;\n            stereoCameraView -e -viewSelected 0 $editorName;\n            stereoCameraView -e \n"
		+ "                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"blendShapePanel\" (localizedPanelLabel(\"Blend Shape\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tblendShapePanel -edit -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"nodeEditorPanel\" (localizedPanelLabel(\"Node Editor\")) `;\n\tif ($nodeEditorPanelVisible || $nodeEditorWorkspaceControlOpen) {\n\t\tif (\"\" == $panelName) {\n\t\t\tif ($useSceneConfig) {\n\t\t\t\t$panelName = `scriptedPanel -unParent  -type \"nodeEditorPanel\" -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n"
		+ "                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -connectNodeOnCreation 0\n                -connectOnDrop 0\n                -copyConnectionsOnPaste 0\n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -crosshairOnEdgeDragging 0\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -editorMode \"default\" \n                $editorName;\n\t\t\t}\n\t\t} else {\n\t\t\t$label = `panel -q -label $panelName`;\n\t\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -connectNodeOnCreation 0\n                -connectOnDrop 0\n                -copyConnectionsOnPaste 0\n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -crosshairOnEdgeDragging 0\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n"
		+ "                -editorMode \"default\" \n                $editorName;\n\t\t\tif (!$useSceneConfig) {\n\t\t\t\tpanel -e -l $label $panelName;\n\t\t\t}\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"createNodePanel\" (localizedPanelLabel(\"Create Node\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"shapePanel\" (localizedPanelLabel(\"Shape Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tshapePanel -edit -l (localizedPanelLabel(\"Shape Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"posePanel\" (localizedPanelLabel(\"Pose Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tposePanel -edit -l (localizedPanelLabel(\"Pose Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"profilerPanel\" (localizedPanelLabel(\"Profiler Tool\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Profiler Tool\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"timeEditorPanel\" (localizedPanelLabel(\"Time Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Time Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"ToggledOutliner\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"ToggledOutliner\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 0\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 1\n            -showReferenceMembers 1\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -organizeByClip 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showParentContainers 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n"
		+ "            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -isSet 0\n            -isSetMember 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            -renderFilterIndex 0\n            -selectionOrder \"chronological\" \n            -expandAttribute 0\n            $editorName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-userCreated false\n\t\t\t\t-defaultImage \"vacantCell.xP:/\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -controllers 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 383\\n    -height 717\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -controllers 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 383\\n    -height 717\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode shapeEditorManager -n "shapeEditorManager";
	rename -uid "1CC64AE3-4A8A-28F0-62AC-85A30B2DA8CB";
createNode poseInterpolatorManager -n "poseInterpolatorManager";
	rename -uid "1B841D13-4EE6-8C06-BC09-FC9981DB41E6";
createNode aiAOVDriver -s -n "defaultArnoldDisplayDriver";
	rename -uid "BE53ADB8-4CD7-595E-0D4E-728ABDD631ED";
	setAttr ".output_mode" 0;
	setAttr ".ai_translator" -type "string" "maya";
createNode nodeGraphEditorInfo -n "hyperShadePrimaryNodeEditorSavedTabsInfo";
	rename -uid "FB2CC9ED-4DB4-DD01-D7E1-06B6DA8C0AAD";
	setAttr ".tgi[0].tn" -type "string" "Untitled_1";
	setAttr ".tgi[0].vl" -type "double2" -933.33329624600037 -474.99998112519671 ;
	setAttr ".tgi[0].vh" -type "double2" 933.33329624600037 473.80950498202571 ;
	setAttr -s 3 ".tgi[0].ni";
	setAttr ".tgi[0].ni[0].x" -101.42857360839844;
	setAttr ".tgi[0].ni[0].y" 118.57142639160156;
	setAttr ".tgi[0].ni[0].nvs" 2114;
	setAttr ".tgi[0].ni[1].x" -84.285713195800781;
	setAttr ".tgi[0].ni[1].y" -48.571430206298828;
	setAttr ".tgi[0].ni[1].nvs" 1922;
	setAttr ".tgi[0].ni[2].x" -612.85711669921875;
	setAttr ".tgi[0].ni[2].y" 45.714286804199219;
	setAttr ".tgi[0].ni[2].nvs" 1922;
select -ne :time1;
	setAttr -av -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".o" 3;
	setAttr ".unw" 3;
select -ne :hardwareRenderingGlobals;
	setAttr ".vac" 2;
	setAttr ".etmr" no;
	setAttr ".tmr" 4096;
select -ne :renderPartition;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 12 ".st";
	setAttr -cb on ".an";
	setAttr -cb on ".pt";
select -ne :renderGlobalsList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
select -ne :defaultShaderList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 14 ".s";
select -ne :postProcessList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
	setAttr -s 11 ".u";
select -ne :defaultRenderingList1;
select -ne :defaultTextureList1;
	setAttr -s 5 ".tx";
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
	addAttr -ci true -sn "resolutionMultiplier" -ln "resolutionMultiplier" -dv 0.01 
		-min 0.01 -max 10 -at "double";
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
select -ne :defaultColorMgtGlobals;
	setAttr ".cme" no;
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
select -ne :strokeGlobals;
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1292543810.78:0.491419832703";
connectAttr "group1_translateX.o" "group1.tx";
connectAttr "group1_translateY.o" "group1.ty";
connectAttr "group1_translateZ.o" "group1.tz";
connectAttr "polyPlane1.out" "pPlaneShape1.i";
connectAttr ":time1.o" ":defaultArnoldRenderOptions.aaseed";
connectAttr ":defaultArnoldFilter.msg" ":defaultArnoldRenderOptions.filt";
connectAttr ":defaultArnoldDriver.msg" ":defaultArnoldRenderOptions.drvr";
connectAttr ":defaultArnoldDisplayDriver.msg" ":defaultArnoldRenderOptions.drivers"
		 -na;
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
connectAttr ":defaultColorMgtGlobals.cme" "file1.cme";
connectAttr ":defaultColorMgtGlobals.cfe" "file1.cmcf";
connectAttr ":defaultColorMgtGlobals.cfp" "file1.cmcp";
connectAttr ":defaultColorMgtGlobals.wsn" "file1.ws";
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
connectAttr "shapeEditorManager.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[0].dn"
		;
connectAttr "poseInterpolatorManager.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[1].dn"
		;
connectAttr ":defaultArnoldDisplayDriver.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[2].dn"
		;
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
connectAttr "place2dTexture1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "point2_comp_to_point2_comp.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "samplerInfo1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "color_comp_to_point2_comp.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "vec_comp_to_vec_comp.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "float_to_vec_component.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "multiplyDivide1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "multiplyDivide2.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture3.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "tempExport_CDO_place2dTexture1.msg" ":defaultRenderUtilityList1.u" 
		-na;
connectAttr "projection2.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "file1.msg" ":defaultTextureList1.tx" -na;
connectAttr "ramp1.msg" ":defaultTextureList1.tx" -na;
connectAttr "ramp2.msg" ":defaultTextureList1.tx" -na;
connectAttr "outAlpha.msg" ":defaultTextureList1.tx" -na;
connectAttr "checker1.msg" ":defaultTextureList1.tx" -na;
// End of test.ma
