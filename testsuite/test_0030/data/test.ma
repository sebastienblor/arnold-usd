//Maya ASCII 2017 scene
//Name: test.ma
//Last modified: Fri, Feb 17, 2017 11:05:13 AM
//Codeset: 1252
requires maya "2017";
requires -nodeType "aiOptions" -nodeType "aiAOVDriver" -nodeType "aiAOVFilter" -nodeType "aiStandard"
		 "mtoa" "2.0.0.0";
requires "stereoCamera" "10.0";
requires "mtoa" "0.14.0";
requires "stereoCamera" "10.0";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2017";
fileInfo "version" "2017";
fileInfo "cutIdentifier" "201606150345-997974";
fileInfo "osv" "Microsoft Windows 7 Enterprise Edition, 64-bit Windows 7 Service Pack 1 (Build 7601)\n";
createNode transform -s -n "persp";
	rename -uid "823D6783-4545-DAF1-E574-10AB3201E451";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 8.2209683823296658 6.1657262420436485 8.2209683525273434 ;
	setAttr ".r" -type "double3" -27.938352729602322 45.000000000000007 -2.2489917831974721e-015 ;
createNode camera -s -n "perspShape" -p "persp";
	rename -uid "C9358D8C-4550-0E1E-CA6F-C898481DA759";
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
	setAttr -k off ".v" no;
	setAttr ".ovr" 1.3;
	setAttr ".fl" 34.999999999999986;
	setAttr ".coi" 13.159970380601656;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".tp" -type "double3" 5.9604644775390625e-008 0 2.9802322387695313e-008 ;
	setAttr ".hc" -type "string" "viewSet -p %camera";
	setAttr ".dr" yes;
	setAttr ".ai_translator" -type "string" "perspective";
	setAttr -k on ".arntr" -type "string" "perspective";
createNode transform -s -n "top";
	rename -uid "BE794EBF-4ABE-E527-FCF9-26A68853FFB2";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 1000.1 0 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	rename -uid "19560D53-43ED-2556-E6FB-77AC85A4B828";
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
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 1000.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
	setAttr ".ai_translator" -type "string" "orthographic";
	setAttr -k on ".arntr" -type "string" "orthographic";
createNode transform -s -n "front";
	rename -uid "29DBBCEB-4FDF-8EF4-2515-5191E17E5357";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 1000.1 ;
createNode camera -s -n "frontShape" -p "front";
	rename -uid "3EBEE8A3-4F02-76FD-95A3-3B8F5172E5FD";
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
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 1000.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
	setAttr ".ai_translator" -type "string" "orthographic";
	setAttr -k on ".arntr" -type "string" "orthographic";
createNode transform -s -n "side";
	rename -uid "DFA88925-4995-5F08-E46C-999CAB25E92B";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 1000.1 0 0 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	rename -uid "F786785C-4EFD-DFB7-AFCD-24BE882D2369";
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
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 1000.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
	setAttr ".ai_translator" -type "string" "orthographic";
	setAttr -k on ".arntr" -type "string" "orthographic";
createNode transform -n "pSphere1";
	rename -uid "7D962496-41A1-99E3-6FAD-70B68142E7C3";
createNode mesh -n "pSphereShape1" -p "pSphere1";
	rename -uid "C42C5003-4B14-B0BA-AE70-6B8D354BE9FD";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -s false -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -k true -sn "ai_sss_use_gi" -ln "aiSssUseGi" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_sss_max_samples" -ln "aiSssMaxSamples" -dv 100000 
		-min 0 -smx 1000000 -at "long";
	addAttr -ci true -sn "mso" -ln "miShadingSamplesOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "msh" -ln "miShadingSamples" -min 0 -smx 8 -at "float";
	addAttr -ci true -sn "mdo" -ln "miMaxDisplaceOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "mmd" -ln "miMaxDisplace" -min 0 -smx 1 -at "float";
	addAttr -ci true -k true -sn "sss_use_gi" -ln "sssUseGi" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "sss_max_samples" -ln "sssMaxSamples" -dv 100000 -min 
		0 -smx 1000000 -at "long";
	addAttr -ci true -k true -sn "sss_sample_spacing" -ln "sssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -ci true -k true -sn "self_shadows" -ln "selfShadows" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "opaque" -ln "opaque" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "vid" -ln "visibleInDiffuse" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "vig" -ln "visibleInGlossy" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "subdiv_type" -ln "subdivType" -min 0 -max 2 -en "none:catclark:linear" 
		-at "enum";
	addAttr -ci true -k true -sn "subdiv_iterations" -ln "subdivIterations" -dv 1 -min 
		0 -max 100 -smn 0 -smx 10 -at "long";
	addAttr -ci true -sn "subdiv_adaptive_metric" -ln "subdivAdaptiveMetric" -min 0 
		-max 2 -en "auto:edge_length:flatness" -at "enum";
	addAttr -ci true -k true -sn "subdiv_pixel_error" -ln "subdivPixelError" -min 0 
		-smx 10 -at "float";
	addAttr -s false -ci true -k true -sn "subdiv_dicing_camera" -ln "subdivDicingCamera" 
		-at "message";
	addAttr -ci true -sn "subdiv_uv_smoothing" -ln "subdivUvSmoothing" -min 0 -max 3 
		-en "pin_corners:pin_borders:linear:smooth" -at "enum";
	addAttr -ci true -k true -sn "exptan" -ln "exportTangents" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "expcol" -ln "exportColors" -min 0 -max 1 -at "bool";
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
createNode transform -n "pointLight1";
	rename -uid "A0149405-468B-63E6-9099-D084581341D4";
	setAttr ".t" -type "double3" 0.8946263483617648 1.8998226219594425 2.2074515762557692 ;
createNode pointLight -n "pointLightShape1" -p "pointLight1";
	rename -uid "5527845F-4B81-8B07-0438-A1862F700F28";
	addAttr -ci true -sn "ai_bounce_factor" -ln "aiBounceFactor" -dv 1 -min 0 -smx 
		20 -at "float";
	addAttr -ci true -sn "ai_bounces" -ln "aiBounces" -dv 999 -min 0 -max 10000 -at "long";
	addAttr -ci true -sn "ai_decay_type" -ln "aiDecayType" -dv 1 -min 0 -max 1 -en "constant:quadratic" 
		-at "enum";
	addAttr -ci true -sn "ai_affect_volumetrics" -ln "aiAffectVolumetrics" -dv 1 -min 
		0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_mis" -ln "aiMis" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_oss" -ln "aiOverrideSssSamples" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_sss_samples" -ln "aiSssSamples" -dv 1 -min 0 -max 
		100 -at "long";
	setAttr -k off ".v";
	setAttr ".urs" no;
	setAttr ".rdl" 1;
	setAttr ".us" no;
	setAttr ".ai_exposure" 4;
	setAttr -k on ".ai_decay_type" 0;
createNode transform -n "hairSystem1";
	rename -uid "8D4161CD-4896-624A-9390-D2A015C19A13";
createNode hairSystem -n "hairSystemShape1" -p "hairSystem1";
	rename -uid "8F747633-4502-0A8D-406A-E88E072B353A";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	addAttr -ci true -k true -sn "ai_sss_use_gi" -ln "aiSssUseGi" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_sss_max_samples" -ln "aiSssMaxSamples" -dv 100000 
		-at "long";
	addAttr -ci true -sn "mso" -ln "miShadingSamplesOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "msh" -ln "miShadingSamples" -min 0 -smx 8 -at "float";
	addAttr -ci true -sn "mdo" -ln "miMaxDisplaceOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "mmd" -ln "miMaxDisplace" -min 0 -smx 1 -at "float";
	setAttr -k off ".v";
	setAttr -s 64 ".ih";
	setAttr ".evo" 0;
	setAttr ".sfn" 0.1;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
	setAttr ".ssg" 10;
	setAttr ".cwd" 0.61589403937737275;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.63999999 3;
	setAttr ".ctw" 0.45695364187835463;
	setAttr ".clc[0]"  0 0.5 1;
	setAttr ".cfl[0]"  0 0 1;
	setAttr -s 2 ".hws[0:1]"  0.68695652 1 3 1 0.2 3;
	setAttr -s 3 ".hcs";
	setAttr ".hcs[0].hcsp" 0;
	setAttr ".hcs[0].hcsc" -type "float3" 0.37099999 0.24690048 0.12280098 ;
	setAttr ".hcs[0].hcsi" 3;
	setAttr ".hcs[1].hcsp" 0.30000001192092896;
	setAttr ".hcs[1].hcsc" -type "float3" 0.43799999 0.31864497 0.19928999 ;
	setAttr ".hcs[1].hcsi" 3;
	setAttr ".hcs[2].hcsp" 1;
	setAttr ".hcs[2].hcsc" -type "float3" 0.62900001 0.54320443 0.462944 ;
	setAttr ".hcs[2].hcsi" 3;
	setAttr ".hpc" 26;
	setAttr ".thn" 0.15231788093655907;
	setAttr ".chr" 0.068027210548570777;
	setAttr ".csr" 0.10204081628851745;
	setAttr ".cvr" 0.12925170106673931;
	setAttr ".dsc[0]"  0 1 1;
createNode transform -n "hairSystem1Follicles";
	rename -uid "A11CDF38-4C61-B778-2A53-2B93C06FA3EF";
createNode transform -n "pSphere1Follicle606" -p "hairSystem1Follicles";
	rename -uid "BFADDC80-45D2-A570-CAFF-4997938B5504";
createNode follicle -n "pSphere1FollicleShape606" -p "pSphere1Follicle606";
	rename -uid "F4CCD52F-4F86-AA29-4316-3E8C616C097E";
	setAttr -k off ".v";
	setAttr ".pu" 0.0625;
	setAttr ".pv" 0.0625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve1" -p "pSphere1Follicle606";
	rename -uid "417ACC82-4B77-73F3-A335-7EAAB343BB96";
createNode nurbsCurve -n "curveShape1" -p "curve1";
	rename -uid "3FE7C00C-4464-307B-5409-42B0D7E964A3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle619" -p "hairSystem1Follicles";
	rename -uid "EB5B76E1-406C-DE7C-0872-9DB334A470CD";
createNode follicle -n "pSphere1FollicleShape619" -p "pSphere1Follicle619";
	rename -uid "B13479F4-413E-D930-2F39-F798F57D7179";
	setAttr -k off ".v";
	setAttr ".pu" 0.0625;
	setAttr ".pv" 0.1875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve2" -p "pSphere1Follicle619";
	rename -uid "4F0CCDDD-464F-7D10-C30D-229969E1758A";
createNode nurbsCurve -n "curveShape2" -p "curve2";
	rename -uid "36947821-40A7-29B6-6F01-EEA4EED9B386";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle631" -p "hairSystem1Follicles";
	rename -uid "CC2D934F-48C3-9A68-AB6E-B2ADDE1EC8FB";
createNode follicle -n "pSphere1FollicleShape631" -p "pSphere1Follicle631";
	rename -uid "64661387-4571-EFE8-3E32-E09E576A0F96";
	setAttr -k off ".v";
	setAttr ".pu" 0.0625;
	setAttr ".pv" 0.3125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve3" -p "pSphere1Follicle631";
	rename -uid "58D83A3C-419E-C36C-D1FF-FCA73F6128D6";
createNode nurbsCurve -n "curveShape3" -p "curve3";
	rename -uid "68EAAE8B-4F5E-66A8-732E-42A61859FFEE";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle643" -p "hairSystem1Follicles";
	rename -uid "8A5F5B28-427C-A46D-B5FC-A1BF7DAA14D2";
createNode follicle -n "pSphere1FollicleShape643" -p "pSphere1Follicle643";
	rename -uid "CBF30482-4398-B50E-162E-1F985D6C7FF4";
	setAttr -k off ".v";
	setAttr ".pu" 0.0625;
	setAttr ".pv" 0.4375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve4" -p "pSphere1Follicle643";
	rename -uid "C145E5BF-4BCA-91F3-B43B-169880F888E3";
createNode nurbsCurve -n "curveShape4" -p "curve4";
	rename -uid "DF02577A-42EA-61E8-5297-259298DADBF6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle656" -p "hairSystem1Follicles";
	rename -uid "D0E262BC-4C68-B649-665C-0D989B613E91";
createNode follicle -n "pSphere1FollicleShape656" -p "pSphere1Follicle656";
	rename -uid "5EA1A289-4D99-937E-F72A-3CAF0B9B90A1";
	setAttr -k off ".v";
	setAttr ".pu" 0.0625;
	setAttr ".pv" 0.5625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve5" -p "pSphere1Follicle656";
	rename -uid "F448D0C5-4F6D-96D9-5B53-3FB30E26D90E";
createNode nurbsCurve -n "curveShape5" -p "curve5";
	rename -uid "A670B8B9-41EC-B9FB-A0F2-17AE92FF90C2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle668" -p "hairSystem1Follicles";
	rename -uid "4076C695-4B4D-9BE2-BB89-1BAF59ACCB6D";
createNode follicle -n "pSphere1FollicleShape668" -p "pSphere1Follicle668";
	rename -uid "9810EC11-4395-6F4F-BF24-BDB64295A287";
	setAttr -k off ".v";
	setAttr ".pu" 0.0625;
	setAttr ".pv" 0.6875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve6" -p "pSphere1Follicle668";
	rename -uid "B8632461-4618-972B-F756-5B83E12ED77B";
createNode nurbsCurve -n "curveShape6" -p "curve6";
	rename -uid "AA7BC7F2-4047-9514-D296-6D82945D2927";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle680" -p "hairSystem1Follicles";
	rename -uid "2A2BD5F0-4F7B-92FA-D9FD-C9B86B533D8A";
createNode follicle -n "pSphere1FollicleShape680" -p "pSphere1Follicle680";
	rename -uid "B80AB2D8-456E-AD76-DB10-1187421049A5";
	setAttr -k off ".v";
	setAttr ".pu" 0.0625;
	setAttr ".pv" 0.8125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve7" -p "pSphere1Follicle680";
	rename -uid "6C5BA80F-4818-06AE-0F0E-E8B274778585";
createNode nurbsCurve -n "curveShape7" -p "curve7";
	rename -uid "CB114F0C-47FD-31D9-12AB-6AB296EA6EE9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle693" -p "hairSystem1Follicles";
	rename -uid "B6293BF0-406F-6928-55F7-6784A15C8483";
createNode follicle -n "pSphere1FollicleShape693" -p "pSphere1Follicle693";
	rename -uid "26544617-40A7-39C0-2B1D-008E2165D574";
	setAttr -k off ".v";
	setAttr ".pu" 0.0625;
	setAttr ".pv" 0.9375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve8" -p "pSphere1Follicle693";
	rename -uid "9297D4C9-494E-3E4F-8EF4-4BB5013EEFB2";
createNode nurbsCurve -n "curveShape8" -p "curve8";
	rename -uid "34D0CE07-4A0D-F81B-7F0B-70942DAD6AA4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle1906" -p "hairSystem1Follicles";
	rename -uid "D6F6EE87-430B-597B-7DA9-33957991C090";
createNode follicle -n "pSphere1FollicleShape1906" -p "pSphere1Follicle1906";
	rename -uid "F492457F-4021-9DD7-3DC0-F4BB4117FFED";
	setAttr -k off ".v";
	setAttr ".pu" 0.1875;
	setAttr ".pv" 0.0625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve9" -p "pSphere1Follicle1906";
	rename -uid "321187CB-4576-88F1-30F7-27BA9322BD09";
createNode nurbsCurve -n "curveShape9" -p "curve9";
	rename -uid "A0F67A18-4E8A-D4FF-1740-8D8DFD536AB1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle1919" -p "hairSystem1Follicles";
	rename -uid "9DC555D7-4FC1-56E8-EF1E-C2B4DEF461AB";
createNode follicle -n "pSphere1FollicleShape1919" -p "pSphere1Follicle1919";
	rename -uid "03128CC2-42DB-B773-AFDC-32998DB6F525";
	setAttr -k off ".v";
	setAttr ".pu" 0.1875;
	setAttr ".pv" 0.1875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve10" -p "pSphere1Follicle1919";
	rename -uid "A5352A7D-4ACF-19A4-0B4A-B1A0A0A851A1";
createNode nurbsCurve -n "curveShape10" -p "curve10";
	rename -uid "8C36BEE5-4477-59D5-94B5-379D2B431300";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle1931" -p "hairSystem1Follicles";
	rename -uid "C35FD348-40FC-58A4-AC4D-C090699C1795";
createNode follicle -n "pSphere1FollicleShape1931" -p "pSphere1Follicle1931";
	rename -uid "F88AF5FA-4AE6-02A7-3AC0-03ABB7AF6B6D";
	setAttr -k off ".v";
	setAttr ".pu" 0.1875;
	setAttr ".pv" 0.3125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve11" -p "pSphere1Follicle1931";
	rename -uid "4F890DF2-402F-AE3E-E520-BDB65D2B29FC";
createNode nurbsCurve -n "curveShape11" -p "curve11";
	rename -uid "24D63251-40C0-841F-8859-2DBA35871462";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle1943" -p "hairSystem1Follicles";
	rename -uid "B56940F2-4A3E-A1DB-82C1-FA8E3B457993";
createNode follicle -n "pSphere1FollicleShape1943" -p "pSphere1Follicle1943";
	rename -uid "E9E6DD63-4E65-F200-1AF6-F68F5B10F8DD";
	setAttr -k off ".v";
	setAttr ".pu" 0.1875;
	setAttr ".pv" 0.4375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve12" -p "pSphere1Follicle1943";
	rename -uid "131ABBC7-4BA2-4A0B-C0E0-8CBE62047160";
createNode nurbsCurve -n "curveShape12" -p "curve12";
	rename -uid "D2628349-47E7-9B74-4308-AA8CB3CA33B4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle1956" -p "hairSystem1Follicles";
	rename -uid "267CFBA6-405F-90A9-34F2-8FB872E545B2";
createNode follicle -n "pSphere1FollicleShape1956" -p "pSphere1Follicle1956";
	rename -uid "8AF36F90-4E8A-D584-E5E6-97BEDE49DED6";
	setAttr -k off ".v";
	setAttr ".pu" 0.1875;
	setAttr ".pv" 0.5625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve13" -p "pSphere1Follicle1956";
	rename -uid "DCE48A5C-4C73-B3D0-9215-BAAEAD2B7FBE";
createNode nurbsCurve -n "curveShape13" -p "curve13";
	rename -uid "3EE1BDC0-4F18-A1AC-69B3-67A8AC883D9C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle1968" -p "hairSystem1Follicles";
	rename -uid "7C2402A4-46E6-3DCA-643E-809FA0DDD8D8";
createNode follicle -n "pSphere1FollicleShape1968" -p "pSphere1Follicle1968";
	rename -uid "D32AF99A-4287-C05B-2F12-0AAF67EA999A";
	setAttr -k off ".v";
	setAttr ".pu" 0.1875;
	setAttr ".pv" 0.6875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve14" -p "pSphere1Follicle1968";
	rename -uid "A83CEA17-4DD5-DEE5-4226-B8A7ABD35D01";
createNode nurbsCurve -n "curveShape14" -p "curve14";
	rename -uid "1C64C39A-4D1D-4ACC-6B77-89BD1A3BFA56";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle1980" -p "hairSystem1Follicles";
	rename -uid "8E9B37EC-4866-ECFD-C6FA-CEBCE50DC785";
createNode follicle -n "pSphere1FollicleShape1980" -p "pSphere1Follicle1980";
	rename -uid "83326A20-4AD0-50C2-7A68-1CB0D241C1CF";
	setAttr -k off ".v";
	setAttr ".pu" 0.1875;
	setAttr ".pv" 0.8125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve15" -p "pSphere1Follicle1980";
	rename -uid "78E76B87-4334-ECB0-7CA3-4FBD77A3057B";
createNode nurbsCurve -n "curveShape15" -p "curve15";
	rename -uid "7D82B971-4123-463C-D986-8EA0FED140F1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle1993" -p "hairSystem1Follicles";
	rename -uid "FB762087-4437-5767-28DD-C39431BA5266";
createNode follicle -n "pSphere1FollicleShape1993" -p "pSphere1Follicle1993";
	rename -uid "40855204-4393-07AE-B1B3-1E9B37A3D6F0";
	setAttr -k off ".v";
	setAttr ".pu" 0.1875;
	setAttr ".pv" 0.9375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve16" -p "pSphere1Follicle1993";
	rename -uid "16B1568E-4439-1F37-D3B6-128D460DAF69";
createNode nurbsCurve -n "curveShape16" -p "curve16";
	rename -uid "AB798F59-4CC1-A566-4D24-969BB5999D41";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle3106" -p "hairSystem1Follicles";
	rename -uid "C1187CF3-4526-CDEB-0BD4-749ABA8EE3B2";
createNode follicle -n "pSphere1FollicleShape3106" -p "pSphere1Follicle3106";
	rename -uid "BCB53E86-41A7-CACC-73BA-1A83410CA3ED";
	setAttr -k off ".v";
	setAttr ".pu" 0.3125;
	setAttr ".pv" 0.0625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve17" -p "pSphere1Follicle3106";
	rename -uid "3AE297A3-4C2D-77CD-5126-849FE9F1631E";
createNode nurbsCurve -n "curveShape17" -p "curve17";
	rename -uid "A242FAA7-458B-BD13-7E74-2F83590E58AF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle3119" -p "hairSystem1Follicles";
	rename -uid "5CC7E0E5-42BA-09E8-6E64-8A9A7CB28A8B";
createNode follicle -n "pSphere1FollicleShape3119" -p "pSphere1Follicle3119";
	rename -uid "00074B19-411B-216D-1676-7DBE42C08657";
	setAttr -k off ".v";
	setAttr ".pu" 0.3125;
	setAttr ".pv" 0.1875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve18" -p "pSphere1Follicle3119";
	rename -uid "2A5C8567-4D8C-3EA1-CD4F-BFA7248CE7C1";
createNode nurbsCurve -n "curveShape18" -p "curve18";
	rename -uid "CB4D2F0F-43FD-AE53-1176-89B4D2D0E0D4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle3131" -p "hairSystem1Follicles";
	rename -uid "108324CA-4975-CB17-B394-00AFB9074C9F";
createNode follicle -n "pSphere1FollicleShape3131" -p "pSphere1Follicle3131";
	rename -uid "81351E9A-4D86-6EF0-A68D-C496AA0AD79D";
	setAttr -k off ".v";
	setAttr ".pu" 0.3125;
	setAttr ".pv" 0.3125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve19" -p "pSphere1Follicle3131";
	rename -uid "420B92A3-4504-0D40-F6A5-1B854FFF1F53";
createNode nurbsCurve -n "curveShape19" -p "curve19";
	rename -uid "3F57D0B8-426E-4259-8A6F-E99DDA4C7AE9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle3143" -p "hairSystem1Follicles";
	rename -uid "F910851B-40D5-4799-7A3B-8A97C278EF5B";
createNode follicle -n "pSphere1FollicleShape3143" -p "pSphere1Follicle3143";
	rename -uid "4A29FFC2-4CB0-4818-23EC-9EB9EF698495";
	setAttr -k off ".v";
	setAttr ".pu" 0.3125;
	setAttr ".pv" 0.4375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve20" -p "pSphere1Follicle3143";
	rename -uid "456A79F3-492D-5812-FC24-3190DA8D88C6";
createNode nurbsCurve -n "curveShape20" -p "curve20";
	rename -uid "EC0D3695-4B55-1FA1-067F-AD984BD55C44";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle3156" -p "hairSystem1Follicles";
	rename -uid "30AD5EFF-4826-1AE4-AB1F-B695580C00BD";
createNode follicle -n "pSphere1FollicleShape3156" -p "pSphere1Follicle3156";
	rename -uid "6CE6A5A9-45EC-139D-535A-218C0B8CEF56";
	setAttr -k off ".v";
	setAttr ".pu" 0.3125;
	setAttr ".pv" 0.5625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve21" -p "pSphere1Follicle3156";
	rename -uid "A77CC957-4F33-9FF9-654A-5AA1F64D1AE1";
createNode nurbsCurve -n "curveShape21" -p "curve21";
	rename -uid "0E10DCC5-4F79-6390-D1F6-FE875B4D6134";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle3168" -p "hairSystem1Follicles";
	rename -uid "7D6D8257-4361-154E-5293-F0B24E134F2B";
createNode follicle -n "pSphere1FollicleShape3168" -p "pSphere1Follicle3168";
	rename -uid "D7ED9A02-4DDB-322D-70E4-DAA08D84B9E5";
	setAttr -k off ".v";
	setAttr ".pu" 0.3125;
	setAttr ".pv" 0.6875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve22" -p "pSphere1Follicle3168";
	rename -uid "175E72FD-4E0A-91F1-252D-F0846D10DF8A";
createNode nurbsCurve -n "curveShape22" -p "curve22";
	rename -uid "E70F2C13-4B8A-6E09-AE06-BF8EF87376BC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle3180" -p "hairSystem1Follicles";
	rename -uid "3C145549-4BB3-BEDC-8616-81928ABA5678";
createNode follicle -n "pSphere1FollicleShape3180" -p "pSphere1Follicle3180";
	rename -uid "433822D1-4892-37FC-E378-2F97423366F6";
	setAttr -k off ".v";
	setAttr ".pu" 0.3125;
	setAttr ".pv" 0.8125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve23" -p "pSphere1Follicle3180";
	rename -uid "A58B7DD5-4FC4-6285-EAB3-C8A7A3765A84";
createNode nurbsCurve -n "curveShape23" -p "curve23";
	rename -uid "A61B96B0-4EF7-9FC6-C520-218C1EF159B3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle3193" -p "hairSystem1Follicles";
	rename -uid "66E5E18A-4AE2-D8C6-6F99-64B1E073A63F";
createNode follicle -n "pSphere1FollicleShape3193" -p "pSphere1Follicle3193";
	rename -uid "A31254DC-4F70-5C3B-F866-C69395539AE6";
	setAttr -k off ".v";
	setAttr ".pu" 0.3125;
	setAttr ".pv" 0.9375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve24" -p "pSphere1Follicle3193";
	rename -uid "39AC632E-44E9-DF43-58A5-6081EEF94279";
createNode nurbsCurve -n "curveShape24" -p "curve24";
	rename -uid "24E22435-4BC0-D7D5-19DA-4DA561FB840C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle4306" -p "hairSystem1Follicles";
	rename -uid "0A3AFA11-4282-7BC0-4335-87824D128CD1";
createNode follicle -n "pSphere1FollicleShape4306" -p "pSphere1Follicle4306";
	rename -uid "BD41648F-4C4A-3FC9-73B3-EFA0971785F7";
	setAttr -k off ".v";
	setAttr ".pu" 0.4375;
	setAttr ".pv" 0.0625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve25" -p "pSphere1Follicle4306";
	rename -uid "9D56240D-491F-D46D-D4F0-32BDB3067725";
createNode nurbsCurve -n "curveShape25" -p "curve25";
	rename -uid "45A20D4B-4ED0-71B1-406A-0F98E56F2C40";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle4319" -p "hairSystem1Follicles";
	rename -uid "5082472C-413A-FBFD-AB2F-30B31491B0C2";
createNode follicle -n "pSphere1FollicleShape4319" -p "pSphere1Follicle4319";
	rename -uid "F2235C24-4023-5E5D-7F74-9A8E3F0375F5";
	setAttr -k off ".v";
	setAttr ".pu" 0.4375;
	setAttr ".pv" 0.1875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve26" -p "pSphere1Follicle4319";
	rename -uid "20EA6A91-4557-13C6-FAF7-ABAE7337D54D";
createNode nurbsCurve -n "curveShape26" -p "curve26";
	rename -uid "1DB115E2-44C8-5E71-C083-94995C3243E9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle4331" -p "hairSystem1Follicles";
	rename -uid "C740E178-4144-AB12-6C0D-0AAF76FD5490";
createNode follicle -n "pSphere1FollicleShape4331" -p "pSphere1Follicle4331";
	rename -uid "77A8151E-4F83-C9D2-7169-6184316F1612";
	setAttr -k off ".v";
	setAttr ".pu" 0.4375;
	setAttr ".pv" 0.3125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve27" -p "pSphere1Follicle4331";
	rename -uid "48565EDC-4340-C08A-2109-E8BAF346C3D5";
createNode nurbsCurve -n "curveShape27" -p "curve27";
	rename -uid "68667C01-4BD2-0C05-B65D-C3A7E6B2037E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle4343" -p "hairSystem1Follicles";
	rename -uid "7B5256E0-46F7-E493-7DED-3291708AE3A7";
createNode follicle -n "pSphere1FollicleShape4343" -p "pSphere1Follicle4343";
	rename -uid "27209E3C-414D-E307-ECFE-EDA562D8780F";
	setAttr -k off ".v";
	setAttr ".pu" 0.4375;
	setAttr ".pv" 0.4375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve28" -p "pSphere1Follicle4343";
	rename -uid "0A952BA7-42D2-ED79-A24C-E79A9F710CED";
createNode nurbsCurve -n "curveShape28" -p "curve28";
	rename -uid "0E2CF563-425A-B822-1CA6-BB87365BEE16";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle4356" -p "hairSystem1Follicles";
	rename -uid "FC1F75FE-4768-A8A0-EA37-A2B3F6CE83C0";
createNode follicle -n "pSphere1FollicleShape4356" -p "pSphere1Follicle4356";
	rename -uid "CC9AC8C7-4D11-BF0D-1984-AD969ABA9580";
	setAttr -k off ".v";
	setAttr ".pu" 0.4375;
	setAttr ".pv" 0.5625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve29" -p "pSphere1Follicle4356";
	rename -uid "21B541EA-4FF4-87CC-A4EC-0090D9841838";
createNode nurbsCurve -n "curveShape29" -p "curve29";
	rename -uid "2A164368-4A5D-55C7-342D-42871FB52FEE";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle4368" -p "hairSystem1Follicles";
	rename -uid "BD45BD82-4C6D-6553-9D4B-4B8639F409F3";
createNode follicle -n "pSphere1FollicleShape4368" -p "pSphere1Follicle4368";
	rename -uid "440FEF39-4C28-74BF-7B6D-43B7C7E352C4";
	setAttr -k off ".v";
	setAttr ".pu" 0.4375;
	setAttr ".pv" 0.6875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve30" -p "pSphere1Follicle4368";
	rename -uid "CBA857FA-438A-DCC8-BC7A-89B4DEE48F6C";
createNode nurbsCurve -n "curveShape30" -p "curve30";
	rename -uid "53936AF8-4AF8-0991-A333-4582CC1B45BF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle4380" -p "hairSystem1Follicles";
	rename -uid "CFF55278-41B9-5631-78A5-6C8DF1A720BC";
createNode follicle -n "pSphere1FollicleShape4380" -p "pSphere1Follicle4380";
	rename -uid "4C9B5985-4D49-DEE3-D2FE-FBAEE936A611";
	setAttr -k off ".v";
	setAttr ".pu" 0.4375;
	setAttr ".pv" 0.8125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve31" -p "pSphere1Follicle4380";
	rename -uid "20CF3314-49A6-EEE8-7F08-479CFCAE95A8";
createNode nurbsCurve -n "curveShape31" -p "curve31";
	rename -uid "D580D17D-460E-5997-0132-1AA55FCFF007";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle4393" -p "hairSystem1Follicles";
	rename -uid "32D50756-4B73-CD84-F723-DB82B26A10A2";
createNode follicle -n "pSphere1FollicleShape4393" -p "pSphere1Follicle4393";
	rename -uid "888FAC8B-47B2-206A-C694-F1ADE21D2C61";
	setAttr -k off ".v";
	setAttr ".pu" 0.4375;
	setAttr ".pv" 0.9375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve32" -p "pSphere1Follicle4393";
	rename -uid "7518E119-40AF-FDA5-56C6-5DB03BDC0A91";
createNode nurbsCurve -n "curveShape32" -p "curve32";
	rename -uid "FF71E3F8-4323-4E36-D26A-92827F6D4A18";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle5606" -p "hairSystem1Follicles";
	rename -uid "838C3E16-4BCB-919A-7AA7-E386FC9DFBBA";
createNode follicle -n "pSphere1FollicleShape5606" -p "pSphere1Follicle5606";
	rename -uid "5F5D7165-451C-9F36-A119-2FAADC39E164";
	setAttr -k off ".v";
	setAttr ".pu" 0.5625;
	setAttr ".pv" 0.0625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve33" -p "pSphere1Follicle5606";
	rename -uid "607FC133-4555-629A-3659-C9B7CC67CAFC";
createNode nurbsCurve -n "curveShape33" -p "curve33";
	rename -uid "B7E8BA54-47E8-8615-AB86-F28750D0E8A5";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle5619" -p "hairSystem1Follicles";
	rename -uid "32740027-4AA6-BB48-EF34-9EB4C2E0A401";
createNode follicle -n "pSphere1FollicleShape5619" -p "pSphere1Follicle5619";
	rename -uid "B766B642-4AB8-8156-61F3-D98931987565";
	setAttr -k off ".v";
	setAttr ".pu" 0.5625;
	setAttr ".pv" 0.1875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve34" -p "pSphere1Follicle5619";
	rename -uid "B1A49D1A-4527-73B5-152A-7B871798AC29";
createNode nurbsCurve -n "curveShape34" -p "curve34";
	rename -uid "D1C4F363-4993-9B64-B827-C48F81676EC5";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle5631" -p "hairSystem1Follicles";
	rename -uid "F4470B5F-410E-50D0-8485-23A11BD653EE";
createNode follicle -n "pSphere1FollicleShape5631" -p "pSphere1Follicle5631";
	rename -uid "15A4FBC6-4B6D-277E-6913-66AD23AB6BA2";
	setAttr -k off ".v";
	setAttr ".pu" 0.5625;
	setAttr ".pv" 0.3125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve35" -p "pSphere1Follicle5631";
	rename -uid "F3E0369C-47DF-6A69-09ED-DA93FE910052";
createNode nurbsCurve -n "curveShape35" -p "curve35";
	rename -uid "EE3F42C6-4411-89F5-F829-FBAD7E409C7E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle5643" -p "hairSystem1Follicles";
	rename -uid "3169F7A3-4ACC-F801-DC1B-BC9A662A5058";
createNode follicle -n "pSphere1FollicleShape5643" -p "pSphere1Follicle5643";
	rename -uid "43BD8B67-48DE-1532-F1E8-F9A23B7FD1A7";
	setAttr -k off ".v";
	setAttr ".pu" 0.5625;
	setAttr ".pv" 0.4375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve36" -p "pSphere1Follicle5643";
	rename -uid "7B1820CE-4BB0-179D-9669-A1AFB8EE6936";
createNode nurbsCurve -n "curveShape36" -p "curve36";
	rename -uid "289753A4-4C39-42C0-14CA-33B36F4D44A0";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle5656" -p "hairSystem1Follicles";
	rename -uid "F058F01D-4FA7-E7CA-169E-CD895419CCBE";
createNode follicle -n "pSphere1FollicleShape5656" -p "pSphere1Follicle5656";
	rename -uid "9FD14E92-41B4-96C4-D371-95B49DEE0A9D";
	setAttr -k off ".v";
	setAttr ".pu" 0.5625;
	setAttr ".pv" 0.5625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve37" -p "pSphere1Follicle5656";
	rename -uid "CC2D5EEC-408E-5BDF-3B23-B485A39572E5";
createNode nurbsCurve -n "curveShape37" -p "curve37";
	rename -uid "33187A14-4228-B330-19F4-50A1FEDAB8F4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle5668" -p "hairSystem1Follicles";
	rename -uid "9E52D1C0-4AC3-BA5C-67F4-6F809AD0DB2D";
createNode follicle -n "pSphere1FollicleShape5668" -p "pSphere1Follicle5668";
	rename -uid "AE2133D0-4D30-8F24-18E8-D0B1346B9ECB";
	setAttr -k off ".v";
	setAttr ".pu" 0.5625;
	setAttr ".pv" 0.6875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve38" -p "pSphere1Follicle5668";
	rename -uid "A3E0F098-4D80-D270-E9E0-F4B3D4359736";
createNode nurbsCurve -n "curveShape38" -p "curve38";
	rename -uid "89802024-47C3-4CDF-A9C7-6E917D372928";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle5680" -p "hairSystem1Follicles";
	rename -uid "D6544F99-4608-66D5-47F4-20A84EB8DF9D";
createNode follicle -n "pSphere1FollicleShape5680" -p "pSphere1Follicle5680";
	rename -uid "E5949148-4261-6DFB-C28A-15A79628DF53";
	setAttr -k off ".v";
	setAttr ".pu" 0.5625;
	setAttr ".pv" 0.8125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve39" -p "pSphere1Follicle5680";
	rename -uid "FF01CE83-495D-A1B8-1233-D78D5925F598";
createNode nurbsCurve -n "curveShape39" -p "curve39";
	rename -uid "F6FC4DE1-4372-626F-CDA5-248B392BD973";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle5693" -p "hairSystem1Follicles";
	rename -uid "02A42E0B-489F-3BF2-795C-539DAFC8E5FA";
createNode follicle -n "pSphere1FollicleShape5693" -p "pSphere1Follicle5693";
	rename -uid "A7F115A7-4426-B19A-667F-A4862BD7CC3A";
	setAttr -k off ".v";
	setAttr ".pu" 0.5625;
	setAttr ".pv" 0.9375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve40" -p "pSphere1Follicle5693";
	rename -uid "43227F6F-4BBC-29C7-1BBB-88B9C7C1FEBB";
createNode nurbsCurve -n "curveShape40" -p "curve40";
	rename -uid "238D0ECE-489D-6954-7686-4CBE7CC31659";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle6806" -p "hairSystem1Follicles";
	rename -uid "76C64295-4EEC-1297-DA4B-45BA0402C51E";
createNode follicle -n "pSphere1FollicleShape6806" -p "pSphere1Follicle6806";
	rename -uid "4FE68D4C-4FDA-3C6A-C007-CCAFE77C4DA3";
	setAttr -k off ".v";
	setAttr ".pu" 0.6875;
	setAttr ".pv" 0.0625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve41" -p "pSphere1Follicle6806";
	rename -uid "90E0A679-436C-66D4-AD85-6C9DC7FD0BC8";
createNode nurbsCurve -n "curveShape41" -p "curve41";
	rename -uid "148822C8-4F85-0073-E3D0-DF9477F630CC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle6819" -p "hairSystem1Follicles";
	rename -uid "C56991DE-496D-B530-9DB0-7AA6D04FE4EE";
createNode follicle -n "pSphere1FollicleShape6819" -p "pSphere1Follicle6819";
	rename -uid "4CB50E08-4CC8-4DD8-BC1C-A3A8E20EDF3F";
	setAttr -k off ".v";
	setAttr ".pu" 0.6875;
	setAttr ".pv" 0.1875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve42" -p "pSphere1Follicle6819";
	rename -uid "CB46E27D-4E6C-D932-FCFF-59A090FA335D";
createNode nurbsCurve -n "curveShape42" -p "curve42";
	rename -uid "AEE1BA9E-4F14-23E9-DC8B-88AE5579F1B7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle6831" -p "hairSystem1Follicles";
	rename -uid "89FC29F0-4871-B061-7089-19894FA2FF5B";
createNode follicle -n "pSphere1FollicleShape6831" -p "pSphere1Follicle6831";
	rename -uid "F9C8A11E-4B13-EC40-505D-909F80A549CC";
	setAttr -k off ".v";
	setAttr ".pu" 0.6875;
	setAttr ".pv" 0.3125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve43" -p "pSphere1Follicle6831";
	rename -uid "E8F6D811-4ECD-EEA8-C5C4-2ABB6E27488A";
createNode nurbsCurve -n "curveShape43" -p "curve43";
	rename -uid "D4E59862-4AA7-F339-087E-93AB6D44DA57";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle6843" -p "hairSystem1Follicles";
	rename -uid "551865D8-46AB-B6EB-0BA1-FA8663500CA2";
createNode follicle -n "pSphere1FollicleShape6843" -p "pSphere1Follicle6843";
	rename -uid "90340834-4CEB-FF40-F6EE-54BE5B109F6D";
	setAttr -k off ".v";
	setAttr ".pu" 0.6875;
	setAttr ".pv" 0.4375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve44" -p "pSphere1Follicle6843";
	rename -uid "7C74EDE7-46F7-E4A6-B4D1-2FB8FECBC55A";
createNode nurbsCurve -n "curveShape44" -p "curve44";
	rename -uid "1EF5DAE9-4EF3-12C0-488B-EAB0C5B94D5B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle6856" -p "hairSystem1Follicles";
	rename -uid "B61BF44E-4A86-0D95-A367-F8A7861E1245";
createNode follicle -n "pSphere1FollicleShape6856" -p "pSphere1Follicle6856";
	rename -uid "253E6521-4252-2A32-E4A5-68AD8690B685";
	setAttr -k off ".v";
	setAttr ".pu" 0.6875;
	setAttr ".pv" 0.5625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve45" -p "pSphere1Follicle6856";
	rename -uid "535FD9BC-41D9-7DA7-9EA3-06A00338539D";
createNode nurbsCurve -n "curveShape45" -p "curve45";
	rename -uid "D8E0AC48-487C-7605-C636-78B9194D4969";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle6868" -p "hairSystem1Follicles";
	rename -uid "E90ACA9C-495C-94F4-AEF4-58AD35E629F8";
createNode follicle -n "pSphere1FollicleShape6868" -p "pSphere1Follicle6868";
	rename -uid "75F28E17-4364-7FC8-9EA6-C7A4A40644EE";
	setAttr -k off ".v";
	setAttr ".pu" 0.6875;
	setAttr ".pv" 0.6875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve46" -p "pSphere1Follicle6868";
	rename -uid "A89CDA94-485E-BE87-D047-D68C2EB95105";
createNode nurbsCurve -n "curveShape46" -p "curve46";
	rename -uid "7074A267-40D9-8A64-33D4-9A898F9BE8DC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle6880" -p "hairSystem1Follicles";
	rename -uid "0F93EBFE-4C08-596A-CB62-9193FF35BCE6";
createNode follicle -n "pSphere1FollicleShape6880" -p "pSphere1Follicle6880";
	rename -uid "C1F5EB20-4715-1DAC-6049-EA8A0EC8A1AD";
	setAttr -k off ".v";
	setAttr ".pu" 0.6875;
	setAttr ".pv" 0.8125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve47" -p "pSphere1Follicle6880";
	rename -uid "4AFBA5A8-4F14-1653-5606-20BE2CA8F94C";
createNode nurbsCurve -n "curveShape47" -p "curve47";
	rename -uid "4F4358C7-489A-79B7-556C-3D9507816C49";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle6893" -p "hairSystem1Follicles";
	rename -uid "F7BD22B8-4C3D-595F-AE73-7A996306ECAB";
createNode follicle -n "pSphere1FollicleShape6893" -p "pSphere1Follicle6893";
	rename -uid "946D9A24-4AF4-38A4-27E7-029112CC2C07";
	setAttr -k off ".v";
	setAttr ".pu" 0.6875;
	setAttr ".pv" 0.9375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve48" -p "pSphere1Follicle6893";
	rename -uid "7A21CAD7-436F-9CE0-4EBD-33B1D23F9E08";
createNode nurbsCurve -n "curveShape48" -p "curve48";
	rename -uid "C09BBA13-47C0-1620-F487-329A22A71031";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle8006" -p "hairSystem1Follicles";
	rename -uid "F5264B00-4DED-78FB-BF00-079630B5F7AC";
createNode follicle -n "pSphere1FollicleShape8006" -p "pSphere1Follicle8006";
	rename -uid "6E8F6A09-4455-A606-7F1F-BD96BAA3C393";
	setAttr -k off ".v";
	setAttr ".pu" 0.8125;
	setAttr ".pv" 0.0625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve49" -p "pSphere1Follicle8006";
	rename -uid "C54CDEB1-4C53-BE36-1D6B-AB88031A05F2";
createNode nurbsCurve -n "curveShape49" -p "curve49";
	rename -uid "C5D36280-413E-722A-9D38-69AF83D586A6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle8019" -p "hairSystem1Follicles";
	rename -uid "1ED162E3-4E7A-ECA5-1C76-5EA88667CDA0";
createNode follicle -n "pSphere1FollicleShape8019" -p "pSphere1Follicle8019";
	rename -uid "866D8975-46BD-AF6F-C880-69AE06DBF91A";
	setAttr -k off ".v";
	setAttr ".pu" 0.8125;
	setAttr ".pv" 0.1875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve50" -p "pSphere1Follicle8019";
	rename -uid "95536CA5-440F-4244-8636-F8897FE056E7";
createNode nurbsCurve -n "curveShape50" -p "curve50";
	rename -uid "36105711-4AD8-C189-F938-16ADBC1AF27D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle8031" -p "hairSystem1Follicles";
	rename -uid "0EB8F4C4-45F4-88AA-FCDA-8390B4138C21";
createNode follicle -n "pSphere1FollicleShape8031" -p "pSphere1Follicle8031";
	rename -uid "EF7ABE79-4395-B36B-113B-039A08EDBA49";
	setAttr -k off ".v";
	setAttr ".pu" 0.8125;
	setAttr ".pv" 0.3125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve51" -p "pSphere1Follicle8031";
	rename -uid "B4F24F34-4554-C76B-BBB0-E1A792BF0DF9";
createNode nurbsCurve -n "curveShape51" -p "curve51";
	rename -uid "021ED110-4FC9-2A98-D30E-B290009501DF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle8043" -p "hairSystem1Follicles";
	rename -uid "B2ADA7D7-47D5-1E97-977C-D3ADF7C41B38";
createNode follicle -n "pSphere1FollicleShape8043" -p "pSphere1Follicle8043";
	rename -uid "D74C98A0-4245-DEDE-3201-CD9925B1B953";
	setAttr -k off ".v";
	setAttr ".pu" 0.8125;
	setAttr ".pv" 0.4375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve52" -p "pSphere1Follicle8043";
	rename -uid "CEB4B8B1-4CF3-D769-4441-5A80246F0001";
createNode nurbsCurve -n "curveShape52" -p "curve52";
	rename -uid "62034131-429F-FEDB-EAFD-DD87F32F9DFD";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle8056" -p "hairSystem1Follicles";
	rename -uid "F5091D03-4724-8E39-25CC-EABAF5A794D9";
createNode follicle -n "pSphere1FollicleShape8056" -p "pSphere1Follicle8056";
	rename -uid "8C56019B-4709-A1C8-9384-75BF8A641C2D";
	setAttr -k off ".v";
	setAttr ".pu" 0.8125;
	setAttr ".pv" 0.5625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve53" -p "pSphere1Follicle8056";
	rename -uid "4F1E254D-4CB0-69D6-10A4-B2A4F8A19A1D";
createNode nurbsCurve -n "curveShape53" -p "curve53";
	rename -uid "79E051C2-4A4F-E57A-BFBE-C798ED2BF098";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle8068" -p "hairSystem1Follicles";
	rename -uid "BCF3022B-4027-7BEF-95F4-468C97DBDD69";
createNode follicle -n "pSphere1FollicleShape8068" -p "pSphere1Follicle8068";
	rename -uid "2EE28830-4A92-1E59-BD31-A4857570F9B5";
	setAttr -k off ".v";
	setAttr ".pu" 0.8125;
	setAttr ".pv" 0.6875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve54" -p "pSphere1Follicle8068";
	rename -uid "648CA060-4BCB-AA89-E409-5186CB5ABC2B";
createNode nurbsCurve -n "curveShape54" -p "curve54";
	rename -uid "0B920CD4-4A66-F81F-1BC0-E99BB961F68D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle8080" -p "hairSystem1Follicles";
	rename -uid "696A6624-4BF6-4661-DAD0-74BE367FEE19";
createNode follicle -n "pSphere1FollicleShape8080" -p "pSphere1Follicle8080";
	rename -uid "6E411D20-472A-3BD4-5443-79A7A93CF85D";
	setAttr -k off ".v";
	setAttr ".pu" 0.8125;
	setAttr ".pv" 0.8125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve55" -p "pSphere1Follicle8080";
	rename -uid "FFB54CFB-4397-4A0D-6F01-CE9F0D09C9CA";
createNode nurbsCurve -n "curveShape55" -p "curve55";
	rename -uid "B0F3290C-489A-5065-565B-EB8C38F3A369";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle8093" -p "hairSystem1Follicles";
	rename -uid "03403298-443B-6581-29E7-15BF36B37E37";
createNode follicle -n "pSphere1FollicleShape8093" -p "pSphere1Follicle8093";
	rename -uid "B744114F-4EC0-44AB-F262-87954678691C";
	setAttr -k off ".v";
	setAttr ".pu" 0.8125;
	setAttr ".pv" 0.9375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve56" -p "pSphere1Follicle8093";
	rename -uid "E8EF7F50-4DBC-9E9C-623C-1EBC9B3217FD";
createNode nurbsCurve -n "curveShape56" -p "curve56";
	rename -uid "B4564E18-4B82-76CB-2C4D-249FB5B61B5C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle9306" -p "hairSystem1Follicles";
	rename -uid "40CC5D03-4D3E-9123-5A5A-57A6F223C590";
createNode follicle -n "pSphere1FollicleShape9306" -p "pSphere1Follicle9306";
	rename -uid "1E735CD3-417D-1CCD-B8F1-368D835FD175";
	setAttr -k off ".v";
	setAttr ".pu" 0.9375;
	setAttr ".pv" 0.0625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve57" -p "pSphere1Follicle9306";
	rename -uid "CF221D28-4217-6E19-4CCC-E18A2A53F1D1";
createNode nurbsCurve -n "curveShape57" -p "curve57";
	rename -uid "A07A3F8D-423D-672F-9B5F-858E9B0B9952";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle9319" -p "hairSystem1Follicles";
	rename -uid "99309FD1-4BBB-1EBA-1D23-2C86B23034FA";
createNode follicle -n "pSphere1FollicleShape9319" -p "pSphere1Follicle9319";
	rename -uid "91CF3428-4540-A6D2-FA96-3388A90F5A07";
	setAttr -k off ".v";
	setAttr ".pu" 0.9375;
	setAttr ".pv" 0.1875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve58" -p "pSphere1Follicle9319";
	rename -uid "1BC4BE26-4345-1DA6-EFBC-FF90C2B515DC";
createNode nurbsCurve -n "curveShape58" -p "curve58";
	rename -uid "45C54984-47E3-611B-C0D4-1691916BBF68";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle9331" -p "hairSystem1Follicles";
	rename -uid "064FDB25-45EF-1CF5-A401-DC8924E0D416";
createNode follicle -n "pSphere1FollicleShape9331" -p "pSphere1Follicle9331";
	rename -uid "6317970E-4B1B-0452-95D4-F28B016D7F11";
	setAttr -k off ".v";
	setAttr ".pu" 0.9375;
	setAttr ".pv" 0.3125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve59" -p "pSphere1Follicle9331";
	rename -uid "5927DD38-4C35-EB74-10F9-7CAC8B6D107D";
createNode nurbsCurve -n "curveShape59" -p "curve59";
	rename -uid "44D92221-48D4-1201-BD0F-429068176844";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle9343" -p "hairSystem1Follicles";
	rename -uid "6048F419-4AE3-7480-EB4C-7D909F5A2E02";
createNode follicle -n "pSphere1FollicleShape9343" -p "pSphere1Follicle9343";
	rename -uid "155AC17D-44D9-AF61-28A7-B38C16694D70";
	setAttr -k off ".v";
	setAttr ".pu" 0.9375;
	setAttr ".pv" 0.4375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve60" -p "pSphere1Follicle9343";
	rename -uid "E8BF02CE-4087-9F5F-6C53-5AAF805F313D";
createNode nurbsCurve -n "curveShape60" -p "curve60";
	rename -uid "EDD1DF98-487E-2F2F-EB2F-55B8F2FF469C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle9356" -p "hairSystem1Follicles";
	rename -uid "A16AC997-41B8-1F10-88F7-A9930D2FE9DA";
createNode follicle -n "pSphere1FollicleShape9356" -p "pSphere1Follicle9356";
	rename -uid "51004AFB-434C-3042-58BA-3AA0B09EB1DD";
	setAttr -k off ".v";
	setAttr ".pu" 0.9375;
	setAttr ".pv" 0.5625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve61" -p "pSphere1Follicle9356";
	rename -uid "DC3CC69B-46A1-2F3B-E831-30812A7FBA7C";
createNode nurbsCurve -n "curveShape61" -p "curve61";
	rename -uid "7218C14D-4B81-79D9-288F-038E5DBB3073";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle9368" -p "hairSystem1Follicles";
	rename -uid "D8FA1486-4F75-4170-95AD-45B89DD8B3E5";
createNode follicle -n "pSphere1FollicleShape9368" -p "pSphere1Follicle9368";
	rename -uid "740422A9-4D01-ACDA-F2A1-33B5ABBC6099";
	setAttr -k off ".v";
	setAttr ".pu" 0.9375;
	setAttr ".pv" 0.6875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve62" -p "pSphere1Follicle9368";
	rename -uid "71381538-45A8-5D32-C358-288BC7740406";
createNode nurbsCurve -n "curveShape62" -p "curve62";
	rename -uid "78515041-4F14-CF58-5E02-00A285C7406B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle9380" -p "hairSystem1Follicles";
	rename -uid "24120ACF-437C-6E7B-F48A-58A667A514E2";
createNode follicle -n "pSphere1FollicleShape9380" -p "pSphere1Follicle9380";
	rename -uid "8A3B26B2-477C-4ED3-2EE1-43A173288DA1";
	setAttr -k off ".v";
	setAttr ".pu" 0.9375;
	setAttr ".pv" 0.8125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve63" -p "pSphere1Follicle9380";
	rename -uid "788120BC-49EA-1D58-6BBF-F4834797B4FC";
createNode nurbsCurve -n "curveShape63" -p "curve63";
	rename -uid "5F73B114-4764-A076-F39F-7DAE51A1A5EE";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pSphere1Follicle9393" -p "hairSystem1Follicles";
	rename -uid "D921C5F6-4F71-6966-E568-DD9FDA46B9A7";
createNode follicle -n "pSphere1FollicleShape9393" -p "pSphere1Follicle9393";
	rename -uid "CE86F9E3-4167-146E-500A-109CEC1A541C";
	setAttr -k off ".v";
	setAttr ".pu" 0.9375;
	setAttr ".pv" 0.9375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve64" -p "pSphere1Follicle9393";
	rename -uid "39C5F634-4AD1-4793-7716-D496FBB22170";
createNode nurbsCurve -n "curveShape64" -p "curve64";
	rename -uid "4AEE1578-4707-0ED4-9397-CA83A124BC23";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
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
createNode transform -n "pfxHair1";
	rename -uid "19101A8E-4403-83A8-D121-E688BBF16D54";
createNode pfxHair -n "pfxHairShape1" -p "pfxHair1";
	rename -uid "EBFA98D5-4990-17AC-5ED6-8C99B13EED8D";
	setAttr -k off ".v";
	setAttr ".dpc" 100;
	setAttr ".dam" no;
createNode expression -n "expression1";
	rename -uid "7D23E366-410E-5604-100F-03A6BBFF69F3";
	setAttr -k on ".nds";
	setAttr ".ixp" -type "string" ".O[0]=.I[0]*2;";
	setAttr ".ani" 0;
createNode lightLinker -s -n "lightLinker1";
	rename -uid "64289A4A-42ED-C176-97D9-F8AA01D25013";
	setAttr -s 5 ".lnk";
	setAttr -s 5 ".slnk";
createNode displayLayerManager -n "layerManager";
	rename -uid "882DCCF1-4197-8AD1-6D1A-11B887D40537";
createNode displayLayer -n "defaultLayer";
	rename -uid "4FB131AB-4F5E-0C6C-9622-1FA83885571F";
createNode renderLayerManager -n "renderLayerManager";
	rename -uid "909FAE3E-4CF9-1222-EB16-9F9A4156F5E3";
createNode renderLayer -n "defaultRenderLayer";
	rename -uid "42826214-4A2C-15B2-3EB5-798FAFC62E6D";
	setAttr ".g" yes;
createNode polySphere -n "polySphere1";
	rename -uid "84471A5E-44FD-AE66-C3C0-BC80698F50A8";
	setAttr ".sa" 40;
	setAttr ".sh" 40;
createNode aiOptions -s -n "defaultArnoldRenderOptions";
	rename -uid "0271EA1C-45FC-F59E-0208-B981FDA9018D";
	addAttr -ci true -k true -sn "driver_exr_compression" -ln "driverExrCompression" 
		-dv 2 -min 0 -max 4 -en "none:rle:zip:piz:pxr24" -at "enum";
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
	addAttr -ci true -k true -sn "driver_png_format" -ln "driverPngFormat" -min 0 -max 
		1 -en "int8:int16" -at "enum";
	addAttr -ci true -k true -sn "driver_tiff_compression" -ln "driverTiffCompression" 
		-dv 1 -min 0 -max 4 -en "none:lzw:ccittrle:zip:packbits" -at "enum";
	addAttr -ci true -k true -sn "driver_tiff_format" -ln "driverTiffFormat" -min 0 
		-max 3 -en "int8:int16:int32:float32" -at "enum";
	addAttr -ci true -k true -sn "driver_unpremult_alpha" -ln "driverUnpremultAlpha" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "filter_width" -ln "filterWidth" -dv 2 -at "float";
	addAttr -ci true -k true -sn "filter_domain" -ln "filterDomain" -min 0 -max 1 -en 
		"first_hit:all_hits" -at "enum";
	addAttr -ci true -k true -sn "filter_minimum" -ln "filterMinimum" -at "float";
	addAttr -ci true -k true -sn "filter_maximum" -ln "filterMaximum" -dv 1 -at "float";
	addAttr -ci true -k true -sn "filter_scalar_mode" -ln "filterScalarMode" -min 0 
		-max 1 -at "bool";
	addAttr -ci true -sn "ARV_options" -ln "ARV_options" -dt "string";
	setAttr ".img" -type "string" "tif";
	setAttr ".fltr" -type "string" "gaussian";
	setAttr -k on ".driver_tiled" no;
	setAttr -k on ".driver_gamma" 1;
createNode shadingEngine -n "aiStandard1SG";
	rename -uid "4BCCE438-48C7-6EA1-2AE8-B4949AEC1DF0";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
	rename -uid "125272D0-4FF2-C42E-23F9-459E404AEFCB";
createNode script -n "sceneConfigurationScriptNode";
	rename -uid "9AAAD341-4250-4732-C8D3-D58B6A4B420D";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".st" 6;
createNode shadingEngine -n "surfaceShader1SG";
	rename -uid "B3F8DAEF-42C6-4642-340F-DC804DEF6C52";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo2";
	rename -uid "F8CFFF61-4646-A021-AD61-8E9059BB4D21";
createNode aiStandard -n "aiStandard2";
	rename -uid "E0AB7E61-44A5-9CD7-E6EF-889C6B8268ED";
createNode shadingEngine -n "aiStandard2SG";
	rename -uid "3A468C05-48CD-79E9-EDA0-3CBA33B3D52F";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo3";
	rename -uid "CB775E7F-4A07-9739-AADE-68A9D51E1116";
createNode script -n "uiConfigurationScriptNode";
	rename -uid "C773F309-4223-BBBA-7C16-64A2DCB7972C";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"top\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n"
		+ "                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n"
		+ "                -rendererName \"base_OpenGL_Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n"
		+ "                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 1\n                -height 1\n                -sceneRenderFilter 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n"
		+ "                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n"
		+ "            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n"
		+ "            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n"
		+ "        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"side\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n"
		+ "                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n"
		+ "                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n"
		+ "                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 1\n                -height 1\n                -sceneRenderFilter 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"side\" \n            -useInteractiveMode 0\n"
		+ "            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n"
		+ "            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n"
		+ "            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" == $panelName) {\n"
		+ "\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"front\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n"
		+ "                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n"
		+ "                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n"
		+ "                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 1\n                -height 1\n                -sceneRenderFilter 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n"
		+ "            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n"
		+ "            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n"
		+ "            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n"
		+ "                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n"
		+ "                -rendererName \"base_OpenGL_Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n"
		+ "                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 462\n                -height 548\n                -sceneRenderFilter 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n"
		+ "                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n"
		+ "            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n"
		+ "            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 462\n            -height 548\n            -sceneRenderFilter 0\n            $editorName;\n"
		+ "        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n                -docTag \"isolOutln_fromSeln\" \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 0\n                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n"
		+ "                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 1\n                -showPublishedAsConnected 0\n                -showContainerContents 1\n                -ignoreDagHierarchy 0\n                -expandConnections 0\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"defaultSetFilter\" \n                -showSetMembers 1\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n"
		+ "                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 0\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -docTag \"isolOutln_fromSeln\" \n            -showShapes 1\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 0\n            -showReferenceMembers 0\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n"
		+ "            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 0\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n"
		+ "            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"graphEditor\" -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n"
		+ "                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n"
		+ "                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n"
		+ "                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 1\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -showCurveNames 0\n                -showActiveCurveNames 0\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                -valueLinesToggle 1\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n"
		+ "                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n"
		+ "\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 1\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -showCurveNames 0\n                -showActiveCurveNames 0\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                -valueLinesToggle 1\n"
		+ "                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dopeSheetPanel\" -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n"
		+ "                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n"
		+ "                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n"
		+ "                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n"
		+ "                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"clipEditorPanel\" -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels `;\n"
		+ "\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n"
		+ "                -initialized 0\n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"sequenceEditorPanel\" -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperGraphPanel\" -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n"
		+ "            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n"
		+ "                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n"
		+ "                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"visorPanel\" -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Editor\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"polyTexturePlacementPanel\" -l (localizedPanelLabel(\"UV Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"renderWindowPanel\" -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynRelEdPanel\" -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"relationshipPanel\" -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"referenceEditorPanel\" -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"componentEditorPanel\" -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynPaintScriptedPanelType\" -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"scriptEditorPanel\" -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"contentBrowserPanel\" (localizedPanelLabel(\"Content Browser\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"contentBrowserPanel\" -l (localizedPanelLabel(\"Content Browser\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Content Browser\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"Stereo\" (localizedPanelLabel(\"Stereo\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"Stereo\" -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels `;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n"
		+ "                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 4 4 \n"
		+ "                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n"
		+ "                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 0\n                -height 0\n                -sceneRenderFilter 0\n                -displayMode \"centerEye\" \n                -viewColor 0 0 0 1 \n                -useCustomBackground 1\n                $editorName;\n            stereoCameraView -e -viewSelected 0 $editorName;\n            stereoCameraView -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n"
		+ "                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels  $panelName;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n"
		+ "                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 4 4 \n                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n"
		+ "                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n"
		+ "                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 0\n                -height 0\n                -sceneRenderFilter 0\n                -displayMode \"centerEye\" \n                -viewColor 0 0 0 1 \n                -useCustomBackground 1\n                $editorName;\n            stereoCameraView -e -viewSelected 0 $editorName;\n            stereoCameraView -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperShadePanel\" -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"blendShapePanel\" (localizedPanelLabel(\"Blend Shape\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tblendShapePanel -unParent -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tblendShapePanel -edit -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"nodeEditorPanel\" (localizedPanelLabel(\"Node Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"nodeEditorPanel\" -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -consistentNameSize 1\n"
		+ "                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -activeTab -1\n                -editorMode \"default\" \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n"
		+ "                -allNodes 0\n                -autoSizeNodes 1\n                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -activeTab -1\n                -editorMode \"default\" \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"createNodePanel\" (localizedPanelLabel(\"Create Node\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"createNodePanel\" -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"shapePanel\" (localizedPanelLabel(\"Shape Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tshapePanel -unParent -l (localizedPanelLabel(\"Shape Editor\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tshapePanel -edit -l (localizedPanelLabel(\"Shape Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"posePanel\" (localizedPanelLabel(\"Pose Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tposePanel -unParent -l (localizedPanelLabel(\"Pose Editor\")) -mbv $menusOkayInPanels ;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tposePanel -edit -l (localizedPanelLabel(\"Pose Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"profilerPanel\" (localizedPanelLabel(\"Profiler Tool\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"profilerPanel\" -l (localizedPanelLabel(\"Profiler Tool\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Profiler Tool\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"timeEditorPanel\" (localizedPanelLabel(\"Time Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"timeEditorPanel\" -l (localizedPanelLabel(\"Time Editor\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Time Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"ToggledOutliner\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\"ToggledOutliner\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n                -showShapes 0\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 0\n                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 1\n"
		+ "                -showAssets 1\n                -showContainedOnly 1\n                -showPublishedAsConnected 0\n                -showContainerContents 1\n                -ignoreDagHierarchy 0\n                -expandConnections 0\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 1\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -isSet 0\n                -isSetMember 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n"
		+ "                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 0\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                -renderFilterIndex 0\n                -selectionOrder \"chronological\" \n                -expandAttribute 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"ToggledOutliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 0\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 0\n            -showReferenceMembers 0\n"
		+ "            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"0\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -isSet 0\n            -isSetMember 0\n            -displayMode \"DAG\" \n"
		+ "            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            -renderFilterIndex 0\n            -selectionOrder \"chronological\" \n            -expandAttribute 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n"
		+ "\t\t\t\t-userCreated false\n\t\t\t\t-defaultImage \"vacantCell.xP:/\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 462\\n    -height 548\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 462\\n    -height 548\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        setFocus `paneLayout -q -p1 $gMainPane`;\n        sceneUIReplacement -deleteRemaining;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode aiAOVFilter -s -n "defaultArnoldFilter";
	rename -uid "80804570-4D10-A9BD-3CDF-CDACF62422E7";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVDriver -s -n "defaultArnoldDriver";
	rename -uid "F2BC4EC2-471E-D4CC-96D2-A18252838879";
	addAttr -ci true -sn "gamma" -ln "gamma" -dv 2.2000000476837158 -min 9.9999997473787516e-005 
		-smx 5 -at "float";
	addAttr -ci true -sn "dither_amplitude" -ln "ditherAmplitude" -dv 1 -at "float";
	setAttr ".ai_translator" -type "string" "tif";
	setAttr ".gamma" 1;
createNode shapeEditorManager -n "shapeEditorManager";
	rename -uid "51F83663-43E4-CD26-FB7F-9B89C4DB7869";
createNode poseInterpolatorManager -n "poseInterpolatorManager";
	rename -uid "35C8EA30-4810-023F-0D65-338F3EFDC6AF";
createNode aiAOVDriver -s -n "defaultArnoldDisplayDriver";
	rename -uid "5C87D3DF-4C68-A452-9EDC-F8836CDCFFC2";
	setAttr ".output_mode" 0;
	setAttr ".ai_translator" -type "string" "maya";
select -ne :time1;
	setAttr ".o" 1;
	setAttr ".unw" 1;
select -ne :hardwareRenderingGlobals;
	setAttr ".vac" 2;
select -ne :renderPartition;
	setAttr -s 5 ".st";
select -ne :renderGlobalsList1;
select -ne :defaultShaderList1;
	setAttr -s 5 ".s";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderingList1;
select -ne :lightList1;
select -ne :initialShadingGroup;
	setAttr ".ro" yes;
select -ne :initialParticleSE;
	setAttr ".ro" yes;
select -ne :defaultRenderGlobals;
	setAttr ".ren" -type "string" "arnold";
	setAttr ".outf" 51;
	setAttr ".imfkey" -type "string" "tif";
	setAttr ".ep" 1;
	setAttr ".ifp" -type "string" "testrender";
select -ne :defaultResolution;
	setAttr ".w" 160;
	setAttr ".h" 120;
	setAttr ".pa" 1;
	setAttr ".dar" 1.3333333730697632;
select -ne :defaultLightSet;
select -ne :defaultColorMgtGlobals;
	setAttr ".cme" no;
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
connectAttr "polySphere1.out" "pSphereShape1.i";
connectAttr "expression1.out[0]" "pointLightShape1.de";
connectAttr ":time1.o" "hairSystemShape1.cti";
connectAttr "pSphere1FollicleShape606.oha" "hairSystemShape1.ih[0]";
connectAttr "pSphere1FollicleShape619.oha" "hairSystemShape1.ih[1]";
connectAttr "pSphere1FollicleShape631.oha" "hairSystemShape1.ih[2]";
connectAttr "pSphere1FollicleShape643.oha" "hairSystemShape1.ih[3]";
connectAttr "pSphere1FollicleShape656.oha" "hairSystemShape1.ih[4]";
connectAttr "pSphere1FollicleShape668.oha" "hairSystemShape1.ih[5]";
connectAttr "pSphere1FollicleShape680.oha" "hairSystemShape1.ih[6]";
connectAttr "pSphere1FollicleShape693.oha" "hairSystemShape1.ih[7]";
connectAttr "pSphere1FollicleShape1906.oha" "hairSystemShape1.ih[8]";
connectAttr "pSphere1FollicleShape1919.oha" "hairSystemShape1.ih[9]";
connectAttr "pSphere1FollicleShape1931.oha" "hairSystemShape1.ih[10]";
connectAttr "pSphere1FollicleShape1943.oha" "hairSystemShape1.ih[11]";
connectAttr "pSphere1FollicleShape1956.oha" "hairSystemShape1.ih[12]";
connectAttr "pSphere1FollicleShape1968.oha" "hairSystemShape1.ih[13]";
connectAttr "pSphere1FollicleShape1980.oha" "hairSystemShape1.ih[14]";
connectAttr "pSphere1FollicleShape1993.oha" "hairSystemShape1.ih[15]";
connectAttr "pSphere1FollicleShape3106.oha" "hairSystemShape1.ih[16]";
connectAttr "pSphere1FollicleShape3119.oha" "hairSystemShape1.ih[17]";
connectAttr "pSphere1FollicleShape3131.oha" "hairSystemShape1.ih[18]";
connectAttr "pSphere1FollicleShape3143.oha" "hairSystemShape1.ih[19]";
connectAttr "pSphere1FollicleShape3156.oha" "hairSystemShape1.ih[20]";
connectAttr "pSphere1FollicleShape3168.oha" "hairSystemShape1.ih[21]";
connectAttr "pSphere1FollicleShape3180.oha" "hairSystemShape1.ih[22]";
connectAttr "pSphere1FollicleShape3193.oha" "hairSystemShape1.ih[23]";
connectAttr "pSphere1FollicleShape4306.oha" "hairSystemShape1.ih[24]";
connectAttr "pSphere1FollicleShape4319.oha" "hairSystemShape1.ih[25]";
connectAttr "pSphere1FollicleShape4331.oha" "hairSystemShape1.ih[26]";
connectAttr "pSphere1FollicleShape4343.oha" "hairSystemShape1.ih[27]";
connectAttr "pSphere1FollicleShape4356.oha" "hairSystemShape1.ih[28]";
connectAttr "pSphere1FollicleShape4368.oha" "hairSystemShape1.ih[29]";
connectAttr "pSphere1FollicleShape4380.oha" "hairSystemShape1.ih[30]";
connectAttr "pSphere1FollicleShape4393.oha" "hairSystemShape1.ih[31]";
connectAttr "pSphere1FollicleShape5606.oha" "hairSystemShape1.ih[32]";
connectAttr "pSphere1FollicleShape5619.oha" "hairSystemShape1.ih[33]";
connectAttr "pSphere1FollicleShape5631.oha" "hairSystemShape1.ih[34]";
connectAttr "pSphere1FollicleShape5643.oha" "hairSystemShape1.ih[35]";
connectAttr "pSphere1FollicleShape5656.oha" "hairSystemShape1.ih[36]";
connectAttr "pSphere1FollicleShape5668.oha" "hairSystemShape1.ih[37]";
connectAttr "pSphere1FollicleShape5680.oha" "hairSystemShape1.ih[38]";
connectAttr "pSphere1FollicleShape5693.oha" "hairSystemShape1.ih[39]";
connectAttr "pSphere1FollicleShape6806.oha" "hairSystemShape1.ih[40]";
connectAttr "pSphere1FollicleShape6819.oha" "hairSystemShape1.ih[41]";
connectAttr "pSphere1FollicleShape6831.oha" "hairSystemShape1.ih[42]";
connectAttr "pSphere1FollicleShape6843.oha" "hairSystemShape1.ih[43]";
connectAttr "pSphere1FollicleShape6856.oha" "hairSystemShape1.ih[44]";
connectAttr "pSphere1FollicleShape6868.oha" "hairSystemShape1.ih[45]";
connectAttr "pSphere1FollicleShape6880.oha" "hairSystemShape1.ih[46]";
connectAttr "pSphere1FollicleShape6893.oha" "hairSystemShape1.ih[47]";
connectAttr "pSphere1FollicleShape8006.oha" "hairSystemShape1.ih[48]";
connectAttr "pSphere1FollicleShape8019.oha" "hairSystemShape1.ih[49]";
connectAttr "pSphere1FollicleShape8031.oha" "hairSystemShape1.ih[50]";
connectAttr "pSphere1FollicleShape8043.oha" "hairSystemShape1.ih[51]";
connectAttr "pSphere1FollicleShape8056.oha" "hairSystemShape1.ih[52]";
connectAttr "pSphere1FollicleShape8068.oha" "hairSystemShape1.ih[53]";
connectAttr "pSphere1FollicleShape8080.oha" "hairSystemShape1.ih[54]";
connectAttr "pSphere1FollicleShape8093.oha" "hairSystemShape1.ih[55]";
connectAttr "pSphere1FollicleShape9306.oha" "hairSystemShape1.ih[56]";
connectAttr "pSphere1FollicleShape9319.oha" "hairSystemShape1.ih[57]";
connectAttr "pSphere1FollicleShape9331.oha" "hairSystemShape1.ih[58]";
connectAttr "pSphere1FollicleShape9343.oha" "hairSystemShape1.ih[59]";
connectAttr "pSphere1FollicleShape9356.oha" "hairSystemShape1.ih[60]";
connectAttr "pSphere1FollicleShape9368.oha" "hairSystemShape1.ih[61]";
connectAttr "pSphere1FollicleShape9380.oha" "hairSystemShape1.ih[62]";
connectAttr "pSphere1FollicleShape9393.oha" "hairSystemShape1.ih[63]";
connectAttr "pSphere1FollicleShape606.ot" "pSphere1Follicle606.t" -l on;
connectAttr "pSphere1FollicleShape606.or" "pSphere1Follicle606.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape606.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape606.inm";
connectAttr "curveShape1.ws" "pSphere1FollicleShape606.sp";
connectAttr "pSphere1FollicleShape619.ot" "pSphere1Follicle619.t" -l on;
connectAttr "pSphere1FollicleShape619.or" "pSphere1Follicle619.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape619.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape619.inm";
connectAttr "curveShape2.ws" "pSphere1FollicleShape619.sp";
connectAttr "pSphere1FollicleShape631.ot" "pSphere1Follicle631.t" -l on;
connectAttr "pSphere1FollicleShape631.or" "pSphere1Follicle631.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape631.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape631.inm";
connectAttr "curveShape3.ws" "pSphere1FollicleShape631.sp";
connectAttr "pSphere1FollicleShape643.ot" "pSphere1Follicle643.t" -l on;
connectAttr "pSphere1FollicleShape643.or" "pSphere1Follicle643.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape643.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape643.inm";
connectAttr "curveShape4.ws" "pSphere1FollicleShape643.sp";
connectAttr "pSphere1FollicleShape656.ot" "pSphere1Follicle656.t" -l on;
connectAttr "pSphere1FollicleShape656.or" "pSphere1Follicle656.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape656.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape656.inm";
connectAttr "curveShape5.ws" "pSphere1FollicleShape656.sp";
connectAttr "pSphere1FollicleShape668.ot" "pSphere1Follicle668.t" -l on;
connectAttr "pSphere1FollicleShape668.or" "pSphere1Follicle668.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape668.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape668.inm";
connectAttr "curveShape6.ws" "pSphere1FollicleShape668.sp";
connectAttr "pSphere1FollicleShape680.ot" "pSphere1Follicle680.t" -l on;
connectAttr "pSphere1FollicleShape680.or" "pSphere1Follicle680.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape680.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape680.inm";
connectAttr "curveShape7.ws" "pSphere1FollicleShape680.sp";
connectAttr "pSphere1FollicleShape693.ot" "pSphere1Follicle693.t" -l on;
connectAttr "pSphere1FollicleShape693.or" "pSphere1Follicle693.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape693.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape693.inm";
connectAttr "curveShape8.ws" "pSphere1FollicleShape693.sp";
connectAttr "pSphere1FollicleShape1906.ot" "pSphere1Follicle1906.t" -l on;
connectAttr "pSphere1FollicleShape1906.or" "pSphere1Follicle1906.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1906.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1906.inm";
connectAttr "curveShape9.ws" "pSphere1FollicleShape1906.sp";
connectAttr "pSphere1FollicleShape1919.ot" "pSphere1Follicle1919.t" -l on;
connectAttr "pSphere1FollicleShape1919.or" "pSphere1Follicle1919.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1919.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1919.inm";
connectAttr "curveShape10.ws" "pSphere1FollicleShape1919.sp";
connectAttr "pSphere1FollicleShape1931.ot" "pSphere1Follicle1931.t" -l on;
connectAttr "pSphere1FollicleShape1931.or" "pSphere1Follicle1931.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1931.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1931.inm";
connectAttr "curveShape11.ws" "pSphere1FollicleShape1931.sp";
connectAttr "pSphere1FollicleShape1943.ot" "pSphere1Follicle1943.t" -l on;
connectAttr "pSphere1FollicleShape1943.or" "pSphere1Follicle1943.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1943.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1943.inm";
connectAttr "curveShape12.ws" "pSphere1FollicleShape1943.sp";
connectAttr "pSphere1FollicleShape1956.ot" "pSphere1Follicle1956.t" -l on;
connectAttr "pSphere1FollicleShape1956.or" "pSphere1Follicle1956.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1956.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1956.inm";
connectAttr "curveShape13.ws" "pSphere1FollicleShape1956.sp";
connectAttr "pSphere1FollicleShape1968.ot" "pSphere1Follicle1968.t" -l on;
connectAttr "pSphere1FollicleShape1968.or" "pSphere1Follicle1968.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1968.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1968.inm";
connectAttr "curveShape14.ws" "pSphere1FollicleShape1968.sp";
connectAttr "pSphere1FollicleShape1980.ot" "pSphere1Follicle1980.t" -l on;
connectAttr "pSphere1FollicleShape1980.or" "pSphere1Follicle1980.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1980.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1980.inm";
connectAttr "curveShape15.ws" "pSphere1FollicleShape1980.sp";
connectAttr "pSphere1FollicleShape1993.ot" "pSphere1Follicle1993.t" -l on;
connectAttr "pSphere1FollicleShape1993.or" "pSphere1Follicle1993.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1993.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1993.inm";
connectAttr "curveShape16.ws" "pSphere1FollicleShape1993.sp";
connectAttr "pSphere1FollicleShape3106.ot" "pSphere1Follicle3106.t" -l on;
connectAttr "pSphere1FollicleShape3106.or" "pSphere1Follicle3106.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3106.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3106.inm";
connectAttr "curveShape17.ws" "pSphere1FollicleShape3106.sp";
connectAttr "pSphere1FollicleShape3119.ot" "pSphere1Follicle3119.t" -l on;
connectAttr "pSphere1FollicleShape3119.or" "pSphere1Follicle3119.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3119.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3119.inm";
connectAttr "curveShape18.ws" "pSphere1FollicleShape3119.sp";
connectAttr "pSphere1FollicleShape3131.ot" "pSphere1Follicle3131.t" -l on;
connectAttr "pSphere1FollicleShape3131.or" "pSphere1Follicle3131.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3131.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3131.inm";
connectAttr "curveShape19.ws" "pSphere1FollicleShape3131.sp";
connectAttr "pSphere1FollicleShape3143.ot" "pSphere1Follicle3143.t" -l on;
connectAttr "pSphere1FollicleShape3143.or" "pSphere1Follicle3143.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3143.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3143.inm";
connectAttr "curveShape20.ws" "pSphere1FollicleShape3143.sp";
connectAttr "pSphere1FollicleShape3156.ot" "pSphere1Follicle3156.t" -l on;
connectAttr "pSphere1FollicleShape3156.or" "pSphere1Follicle3156.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3156.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3156.inm";
connectAttr "curveShape21.ws" "pSphere1FollicleShape3156.sp";
connectAttr "pSphere1FollicleShape3168.ot" "pSphere1Follicle3168.t" -l on;
connectAttr "pSphere1FollicleShape3168.or" "pSphere1Follicle3168.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3168.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3168.inm";
connectAttr "curveShape22.ws" "pSphere1FollicleShape3168.sp";
connectAttr "pSphere1FollicleShape3180.ot" "pSphere1Follicle3180.t" -l on;
connectAttr "pSphere1FollicleShape3180.or" "pSphere1Follicle3180.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3180.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3180.inm";
connectAttr "curveShape23.ws" "pSphere1FollicleShape3180.sp";
connectAttr "pSphere1FollicleShape3193.ot" "pSphere1Follicle3193.t" -l on;
connectAttr "pSphere1FollicleShape3193.or" "pSphere1Follicle3193.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3193.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3193.inm";
connectAttr "curveShape24.ws" "pSphere1FollicleShape3193.sp";
connectAttr "pSphere1FollicleShape4306.ot" "pSphere1Follicle4306.t" -l on;
connectAttr "pSphere1FollicleShape4306.or" "pSphere1Follicle4306.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4306.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4306.inm";
connectAttr "curveShape25.ws" "pSphere1FollicleShape4306.sp";
connectAttr "pSphere1FollicleShape4319.ot" "pSphere1Follicle4319.t" -l on;
connectAttr "pSphere1FollicleShape4319.or" "pSphere1Follicle4319.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4319.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4319.inm";
connectAttr "curveShape26.ws" "pSphere1FollicleShape4319.sp";
connectAttr "pSphere1FollicleShape4331.ot" "pSphere1Follicle4331.t" -l on;
connectAttr "pSphere1FollicleShape4331.or" "pSphere1Follicle4331.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4331.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4331.inm";
connectAttr "curveShape27.ws" "pSphere1FollicleShape4331.sp";
connectAttr "pSphere1FollicleShape4343.ot" "pSphere1Follicle4343.t" -l on;
connectAttr "pSphere1FollicleShape4343.or" "pSphere1Follicle4343.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4343.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4343.inm";
connectAttr "curveShape28.ws" "pSphere1FollicleShape4343.sp";
connectAttr "pSphere1FollicleShape4356.ot" "pSphere1Follicle4356.t" -l on;
connectAttr "pSphere1FollicleShape4356.or" "pSphere1Follicle4356.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4356.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4356.inm";
connectAttr "curveShape29.ws" "pSphere1FollicleShape4356.sp";
connectAttr "pSphere1FollicleShape4368.ot" "pSphere1Follicle4368.t" -l on;
connectAttr "pSphere1FollicleShape4368.or" "pSphere1Follicle4368.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4368.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4368.inm";
connectAttr "curveShape30.ws" "pSphere1FollicleShape4368.sp";
connectAttr "pSphere1FollicleShape4380.ot" "pSphere1Follicle4380.t" -l on;
connectAttr "pSphere1FollicleShape4380.or" "pSphere1Follicle4380.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4380.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4380.inm";
connectAttr "curveShape31.ws" "pSphere1FollicleShape4380.sp";
connectAttr "pSphere1FollicleShape4393.ot" "pSphere1Follicle4393.t" -l on;
connectAttr "pSphere1FollicleShape4393.or" "pSphere1Follicle4393.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4393.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4393.inm";
connectAttr "curveShape32.ws" "pSphere1FollicleShape4393.sp";
connectAttr "pSphere1FollicleShape5606.ot" "pSphere1Follicle5606.t" -l on;
connectAttr "pSphere1FollicleShape5606.or" "pSphere1Follicle5606.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5606.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5606.inm";
connectAttr "curveShape33.ws" "pSphere1FollicleShape5606.sp";
connectAttr "pSphere1FollicleShape5619.ot" "pSphere1Follicle5619.t" -l on;
connectAttr "pSphere1FollicleShape5619.or" "pSphere1Follicle5619.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5619.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5619.inm";
connectAttr "curveShape34.ws" "pSphere1FollicleShape5619.sp";
connectAttr "pSphere1FollicleShape5631.ot" "pSphere1Follicle5631.t" -l on;
connectAttr "pSphere1FollicleShape5631.or" "pSphere1Follicle5631.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5631.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5631.inm";
connectAttr "curveShape35.ws" "pSphere1FollicleShape5631.sp";
connectAttr "pSphere1FollicleShape5643.ot" "pSphere1Follicle5643.t" -l on;
connectAttr "pSphere1FollicleShape5643.or" "pSphere1Follicle5643.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5643.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5643.inm";
connectAttr "curveShape36.ws" "pSphere1FollicleShape5643.sp";
connectAttr "pSphere1FollicleShape5656.ot" "pSphere1Follicle5656.t" -l on;
connectAttr "pSphere1FollicleShape5656.or" "pSphere1Follicle5656.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5656.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5656.inm";
connectAttr "curveShape37.ws" "pSphere1FollicleShape5656.sp";
connectAttr "pSphere1FollicleShape5668.ot" "pSphere1Follicle5668.t" -l on;
connectAttr "pSphere1FollicleShape5668.or" "pSphere1Follicle5668.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5668.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5668.inm";
connectAttr "curveShape38.ws" "pSphere1FollicleShape5668.sp";
connectAttr "pSphere1FollicleShape5680.ot" "pSphere1Follicle5680.t" -l on;
connectAttr "pSphere1FollicleShape5680.or" "pSphere1Follicle5680.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5680.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5680.inm";
connectAttr "curveShape39.ws" "pSphere1FollicleShape5680.sp";
connectAttr "pSphere1FollicleShape5693.ot" "pSphere1Follicle5693.t" -l on;
connectAttr "pSphere1FollicleShape5693.or" "pSphere1Follicle5693.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5693.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5693.inm";
connectAttr "curveShape40.ws" "pSphere1FollicleShape5693.sp";
connectAttr "pSphere1FollicleShape6806.ot" "pSphere1Follicle6806.t" -l on;
connectAttr "pSphere1FollicleShape6806.or" "pSphere1Follicle6806.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6806.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6806.inm";
connectAttr "curveShape41.ws" "pSphere1FollicleShape6806.sp";
connectAttr "pSphere1FollicleShape6819.ot" "pSphere1Follicle6819.t" -l on;
connectAttr "pSphere1FollicleShape6819.or" "pSphere1Follicle6819.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6819.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6819.inm";
connectAttr "curveShape42.ws" "pSphere1FollicleShape6819.sp";
connectAttr "pSphere1FollicleShape6831.ot" "pSphere1Follicle6831.t" -l on;
connectAttr "pSphere1FollicleShape6831.or" "pSphere1Follicle6831.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6831.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6831.inm";
connectAttr "curveShape43.ws" "pSphere1FollicleShape6831.sp";
connectAttr "pSphere1FollicleShape6843.ot" "pSphere1Follicle6843.t" -l on;
connectAttr "pSphere1FollicleShape6843.or" "pSphere1Follicle6843.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6843.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6843.inm";
connectAttr "curveShape44.ws" "pSphere1FollicleShape6843.sp";
connectAttr "pSphere1FollicleShape6856.ot" "pSphere1Follicle6856.t" -l on;
connectAttr "pSphere1FollicleShape6856.or" "pSphere1Follicle6856.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6856.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6856.inm";
connectAttr "curveShape45.ws" "pSphere1FollicleShape6856.sp";
connectAttr "pSphere1FollicleShape6868.ot" "pSphere1Follicle6868.t" -l on;
connectAttr "pSphere1FollicleShape6868.or" "pSphere1Follicle6868.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6868.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6868.inm";
connectAttr "curveShape46.ws" "pSphere1FollicleShape6868.sp";
connectAttr "pSphere1FollicleShape6880.ot" "pSphere1Follicle6880.t" -l on;
connectAttr "pSphere1FollicleShape6880.or" "pSphere1Follicle6880.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6880.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6880.inm";
connectAttr "curveShape47.ws" "pSphere1FollicleShape6880.sp";
connectAttr "pSphere1FollicleShape6893.ot" "pSphere1Follicle6893.t" -l on;
connectAttr "pSphere1FollicleShape6893.or" "pSphere1Follicle6893.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6893.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6893.inm";
connectAttr "curveShape48.ws" "pSphere1FollicleShape6893.sp";
connectAttr "pSphere1FollicleShape8006.ot" "pSphere1Follicle8006.t" -l on;
connectAttr "pSphere1FollicleShape8006.or" "pSphere1Follicle8006.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8006.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8006.inm";
connectAttr "curveShape49.ws" "pSphere1FollicleShape8006.sp";
connectAttr "pSphere1FollicleShape8019.ot" "pSphere1Follicle8019.t" -l on;
connectAttr "pSphere1FollicleShape8019.or" "pSphere1Follicle8019.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8019.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8019.inm";
connectAttr "curveShape50.ws" "pSphere1FollicleShape8019.sp";
connectAttr "pSphere1FollicleShape8031.ot" "pSphere1Follicle8031.t" -l on;
connectAttr "pSphere1FollicleShape8031.or" "pSphere1Follicle8031.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8031.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8031.inm";
connectAttr "curveShape51.ws" "pSphere1FollicleShape8031.sp";
connectAttr "pSphere1FollicleShape8043.ot" "pSphere1Follicle8043.t" -l on;
connectAttr "pSphere1FollicleShape8043.or" "pSphere1Follicle8043.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8043.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8043.inm";
connectAttr "curveShape52.ws" "pSphere1FollicleShape8043.sp";
connectAttr "pSphere1FollicleShape8056.ot" "pSphere1Follicle8056.t" -l on;
connectAttr "pSphere1FollicleShape8056.or" "pSphere1Follicle8056.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8056.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8056.inm";
connectAttr "curveShape53.ws" "pSphere1FollicleShape8056.sp";
connectAttr "pSphere1FollicleShape8068.ot" "pSphere1Follicle8068.t" -l on;
connectAttr "pSphere1FollicleShape8068.or" "pSphere1Follicle8068.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8068.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8068.inm";
connectAttr "curveShape54.ws" "pSphere1FollicleShape8068.sp";
connectAttr "pSphere1FollicleShape8080.ot" "pSphere1Follicle8080.t" -l on;
connectAttr "pSphere1FollicleShape8080.or" "pSphere1Follicle8080.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8080.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8080.inm";
connectAttr "curveShape55.ws" "pSphere1FollicleShape8080.sp";
connectAttr "pSphere1FollicleShape8093.ot" "pSphere1Follicle8093.t" -l on;
connectAttr "pSphere1FollicleShape8093.or" "pSphere1Follicle8093.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8093.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8093.inm";
connectAttr "curveShape56.ws" "pSphere1FollicleShape8093.sp";
connectAttr "pSphere1FollicleShape9306.ot" "pSphere1Follicle9306.t" -l on;
connectAttr "pSphere1FollicleShape9306.or" "pSphere1Follicle9306.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9306.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9306.inm";
connectAttr "curveShape57.ws" "pSphere1FollicleShape9306.sp";
connectAttr "pSphere1FollicleShape9319.ot" "pSphere1Follicle9319.t" -l on;
connectAttr "pSphere1FollicleShape9319.or" "pSphere1Follicle9319.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9319.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9319.inm";
connectAttr "curveShape58.ws" "pSphere1FollicleShape9319.sp";
connectAttr "pSphere1FollicleShape9331.ot" "pSphere1Follicle9331.t" -l on;
connectAttr "pSphere1FollicleShape9331.or" "pSphere1Follicle9331.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9331.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9331.inm";
connectAttr "curveShape59.ws" "pSphere1FollicleShape9331.sp";
connectAttr "pSphere1FollicleShape9343.ot" "pSphere1Follicle9343.t" -l on;
connectAttr "pSphere1FollicleShape9343.or" "pSphere1Follicle9343.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9343.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9343.inm";
connectAttr "curveShape60.ws" "pSphere1FollicleShape9343.sp";
connectAttr "pSphere1FollicleShape9356.ot" "pSphere1Follicle9356.t" -l on;
connectAttr "pSphere1FollicleShape9356.or" "pSphere1Follicle9356.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9356.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9356.inm";
connectAttr "curveShape61.ws" "pSphere1FollicleShape9356.sp";
connectAttr "pSphere1FollicleShape9368.ot" "pSphere1Follicle9368.t" -l on;
connectAttr "pSphere1FollicleShape9368.or" "pSphere1Follicle9368.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9368.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9368.inm";
connectAttr "curveShape62.ws" "pSphere1FollicleShape9368.sp";
connectAttr "pSphere1FollicleShape9380.ot" "pSphere1Follicle9380.t" -l on;
connectAttr "pSphere1FollicleShape9380.or" "pSphere1Follicle9380.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9380.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9380.inm";
connectAttr "curveShape63.ws" "pSphere1FollicleShape9380.sp";
connectAttr "pSphere1FollicleShape9393.ot" "pSphere1Follicle9393.t" -l on;
connectAttr "pSphere1FollicleShape9393.or" "pSphere1Follicle9393.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9393.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9393.inm";
connectAttr "curveShape64.ws" "pSphere1FollicleShape9393.sp";
connectAttr "hairSystemShape1.orh" "pfxHairShape1.rhs";
connectAttr "pointLightShape1.ai_decay_type" "expression1.in[0]";
connectAttr "pointLightShape1.msg" "expression1.obm";
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
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr ":time1.o" ":defaultArnoldRenderOptions.aaseed";
connectAttr ":defaultArnoldFilter.msg" ":defaultArnoldRenderOptions.filt";
connectAttr ":defaultArnoldDriver.msg" ":defaultArnoldRenderOptions.drvr";
connectAttr ":defaultArnoldDisplayDriver.msg" ":defaultArnoldRenderOptions.drivers"
		 -na;
connectAttr "aiStandard1SG.msg" "materialInfo1.sg";
connectAttr "surfaceShader1SG.msg" "materialInfo2.sg";
connectAttr "aiStandard2.out" "aiStandard2SG.ss";
connectAttr "pSphereShape1.iog" "aiStandard2SG.dsm" -na;
connectAttr "aiStandard2SG.msg" "materialInfo3.sg";
connectAttr "aiStandard2.msg" "materialInfo3.m";
connectAttr "aiStandard2.msg" "materialInfo3.t" -na;
connectAttr "aiStandard1SG.pa" ":renderPartition.st" -na;
connectAttr "surfaceShader1SG.pa" ":renderPartition.st" -na;
connectAttr "aiStandard2SG.pa" ":renderPartition.st" -na;
connectAttr "aiStandard2.msg" ":defaultShaderList1.s" -na;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "pointLightShape1.ltd" ":lightList1.l" -na;
connectAttr "pointLight1.iog" ":defaultLightSet.dsm" -na;
// End of test.ma
