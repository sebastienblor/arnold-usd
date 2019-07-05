//Maya ASCII 2018ff09 scene
//Name: test.ma
//Last modified: Fri, Jun 28, 2019 05:16:01 pm
//Codeset: UTF-8
requires maya "2018ff09";
requires -nodeType "aiOptions" -nodeType "aiAOVDriver" -nodeType "aiAOVFilter" -nodeType "aiSkyDomeLight"
		 -nodeType "aiSwitch" -nodeType "aiClipGeo" -nodeType "aiStandardSurface" "mtoa" "3.3.0.wip";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2018";
fileInfo "version" "2018";
fileInfo "cutIdentifier" "201903222215-65bada0e52";
fileInfo "osv" "Linux 3.10.0-957.21.3.el7.x86_64 #1 SMP Tue Jun 18 16:35:19 UTC 2019 x86_64";
createNode transform -s -n "persp";
	rename -uid "C7278940-0000-4213-5C7F-EA3900000231";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 10.0988824382988 5.6723772257804947 -6.6488255066398381 ;
	setAttr ".r" -type "double3" -25.538352729635463 121.79999999999166 0 ;
createNode camera -s -n "perspShape" -p "persp";
	rename -uid "C7278940-0000-4213-5C7F-EA3900000232";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".fl" 34.999999999999993;
	setAttr ".coi" 12.985405751825615;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".hc" -type "string" "viewSet -p %camera";
	setAttr ".ai_translator" -type "string" "perspective";
createNode transform -s -n "top";
	rename -uid "C7278940-0000-4213-5C7F-EA3900000233";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 1000.1 0 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	rename -uid "C7278940-0000-4213-5C7F-EA3900000234";
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
createNode transform -s -n "front";
	rename -uid "C7278940-0000-4213-5C7F-EA3900000235";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 1000.1 ;
createNode camera -s -n "frontShape" -p "front";
	rename -uid "C7278940-0000-4213-5C7F-EA3900000236";
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
createNode transform -s -n "side";
	rename -uid "C7278940-0000-4213-5C7F-EA3900000237";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 1000.1 0 0 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	rename -uid "C7278940-0000-4213-5C7F-EA3900000238";
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
createNode transform -n "aiSkyDomeLight1";
	rename -uid "C7278940-0000-4213-5C7F-EC0B00000311";
createNode aiSkyDomeLight -n "aiSkyDomeLightShape1" -p "aiSkyDomeLight1";
	rename -uid "C7278940-0000-4213-5C7F-EC0B00000310";
	addAttr -ci true -h true -sn "aal" -ln "attributeAliasList" -dt "attributeAlias";
	setAttr -k off ".v";
	setAttr ".csh" no;
	setAttr ".rcsh" no;
	setAttr ".camera" 0;
	setAttr ".aal" -type "attributeAlias" {"exposure","aiExposure"} ;
createNode transform -n "test_geo";
	rename -uid "261DE940-0000-3E51-5D16-376D00000381";
createNode transform -n "pSphere3" -p "test_geo";
	rename -uid "261DE940-0000-3E51-5D16-35540000030A";
createNode mesh -n "pSphereShape3" -p "pSphere3";
	rename -uid "261DE940-0000-3E51-5D16-355400000309";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".dn" yes;
	setAttr ".ai_subdiv_type" 1;
	setAttr ".ai_subdiv_iterations" 2;
	setAttr ".ai_translator" -type "string" "polymesh";
createNode transform -n "pCube5" -p "test_geo";
	rename -uid "261DE940-0000-3E51-5D16-357900000345";
	setAttr ".t" -type "double3" 2.5 2.5 -2.5 ;
createNode mesh -n "pCubeShape5" -p "pCube5";
	rename -uid "261DE940-0000-3E51-5D16-357900000344";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".ai_translator" -type "string" "polymesh";
createNode transform -n "pCube6" -p "test_geo";
	rename -uid "261DE940-0000-3E51-5D16-35EE00000351";
	setAttr ".t" -type "double3" 2.5 2.5 2.5 ;
createNode mesh -n "pCubeShape6" -p "pCube6";
	rename -uid "261DE940-0000-3E51-5D16-35EE00000352";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.375 0 0.625 0 0.375
		 0.25 0.625 0.25 0.375 0.5 0.625 0.5 0.375 0.75 0.625 0.75 0.375 1 0.625 1 0.875 0
		 0.875 0.25 0.125 0 0.125 0.25;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".vt[0:7]"  -2 -2 2 2 -2 2 -2 2 2 2 2 2 -2 2 -2 2 2 -2
		 -2 -2 -2 2 -2 -2;
	setAttr -s 12 ".ed[0:11]"  0 1 0 2 3 0 4 5 0 6 7 0 0 2 0 1 3 0 2 4 0
		 3 5 0 4 6 0 5 7 0 6 0 0 7 1 0;
	setAttr -s 6 -ch 24 ".fc[0:5]" -type "polyFaces" 
		f 4 0 5 -2 -5
		mu 0 4 0 1 3 2
		f 4 1 7 -3 -7
		mu 0 4 2 3 5 4
		f 4 2 9 -4 -9
		mu 0 4 4 5 7 6
		f 4 3 11 -1 -11
		mu 0 4 6 7 9 8
		f 4 -12 -10 -8 -6
		mu 0 4 1 10 11 3
		f 4 10 4 6 8
		mu 0 4 12 0 2 13;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr ".ai_translator" -type "string" "polymesh";
createNode transform -n "pCube7" -p "test_geo";
	rename -uid "261DE940-0000-3E51-5D16-35FB00000357";
	setAttr ".t" -type "double3" 2.5 -2.5 2.5 ;
createNode mesh -n "pCubeShape7" -p "pCube7";
	rename -uid "261DE940-0000-3E51-5D16-35FB00000358";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.375 0 0.625 0 0.375
		 0.25 0.625 0.25 0.375 0.5 0.625 0.5 0.375 0.75 0.625 0.75 0.375 1 0.625 1 0.875 0
		 0.875 0.25 0.125 0 0.125 0.25;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".vt[0:7]"  -2 -2 2 2 -2 2 -2 2 2 2 2 2 -2 2 -2 2 2 -2
		 -2 -2 -2 2 -2 -2;
	setAttr -s 12 ".ed[0:11]"  0 1 0 2 3 0 4 5 0 6 7 0 0 2 0 1 3 0 2 4 0
		 3 5 0 4 6 0 5 7 0 6 0 0 7 1 0;
	setAttr -s 6 -ch 24 ".fc[0:5]" -type "polyFaces" 
		f 4 0 5 -2 -5
		mu 0 4 0 1 3 2
		f 4 1 7 -3 -7
		mu 0 4 2 3 5 4
		f 4 2 9 -4 -9
		mu 0 4 4 5 7 6
		f 4 3 11 -1 -11
		mu 0 4 6 7 9 8
		f 4 -12 -10 -8 -6
		mu 0 4 1 10 11 3
		f 4 10 4 6 8
		mu 0 4 12 0 2 13;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr ".ai_translator" -type "string" "polymesh";
createNode transform -n "pCube8" -p "test_geo";
	rename -uid "261DE940-0000-3E51-5D16-36000000035D";
	setAttr ".t" -type "double3" 2.5 -2.5 -2.5 ;
createNode mesh -n "pCubeShape8" -p "pCube8";
	rename -uid "261DE940-0000-3E51-5D16-36000000035E";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.375 0 0.625 0 0.375
		 0.25 0.625 0.25 0.375 0.5 0.625 0.5 0.375 0.75 0.625 0.75 0.375 1 0.625 1 0.875 0
		 0.875 0.25 0.125 0 0.125 0.25;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".vt[0:7]"  -2 -2 2 2 -2 2 -2 2 2 2 2 2 -2 2 -2 2 2 -2
		 -2 -2 -2 2 -2 -2;
	setAttr -s 12 ".ed[0:11]"  0 1 0 2 3 0 4 5 0 6 7 0 0 2 0 1 3 0 2 4 0
		 3 5 0 4 6 0 5 7 0 6 0 0 7 1 0;
	setAttr -s 6 -ch 24 ".fc[0:5]" -type "polyFaces" 
		f 4 0 5 -2 -5
		mu 0 4 0 1 3 2
		f 4 1 7 -3 -7
		mu 0 4 2 3 5 4
		f 4 2 9 -4 -9
		mu 0 4 4 5 7 6
		f 4 3 11 -1 -11
		mu 0 4 6 7 9 8
		f 4 -12 -10 -8 -6
		mu 0 4 1 10 11 3
		f 4 10 4 6 8
		mu 0 4 12 0 2 13;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr ".ai_translator" -type "string" "polymesh";
createNode transform -n "pSphere4" -p "test_geo";
	rename -uid "261DE940-0000-3E51-5D16-369800000371";
createNode mesh -n "pSphereShape4" -p "pSphere4";
	rename -uid "261DE940-0000-3E51-5D16-369800000370";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".ai_subdiv_type" 1;
	setAttr ".ai_subdiv_iterations" 2;
	setAttr ".ai_translator" -type "string" "polymesh";
createNode transform -n "renderCam";
	rename -uid "261DE940-0000-3E51-5D16-3803000003A6";
	setAttr ".t" -type "double3" 12.397821267310084 6.9648076150495681 -8.0742279714874439 ;
	setAttr ".r" -type "double3" -25.538352729635463 121.79999999999166 0 ;
createNode camera -n "renderCamShape" -p "renderCam";
	rename -uid "261DE940-0000-3E51-5D16-3803000003A7";
	setAttr -k off ".v";
	setAttr ".fl" 45;
	setAttr ".coi" 15.983283040414271;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".hc" -type "string" "viewSet -p %camera";
	setAttr ".ai_translator" -type "string" "perspective";
createNode lightLinker -s -n "lightLinker1";
	rename -uid "47D6A940-0000-5B9B-5D16-3A7B0000026F";
	setAttr -s 12 ".lnk";
	setAttr -s 12 ".slnk";
createNode shapeEditorManager -n "shapeEditorManager";
	rename -uid "47D6A940-0000-5B9B-5D16-3A7B00000270";
createNode poseInterpolatorManager -n "poseInterpolatorManager";
	rename -uid "47D6A940-0000-5B9B-5D16-3A7B00000271";
createNode displayLayerManager -n "layerManager";
	rename -uid "47D6A940-0000-5B9B-5D16-3A7B00000272";
createNode displayLayer -n "defaultLayer";
	rename -uid "C7278940-0000-4213-5C7F-EA3900000251";
createNode renderLayerManager -n "renderLayerManager";
	rename -uid "47D6A940-0000-5B9B-5D16-3A7B00000274";
createNode renderLayer -n "defaultRenderLayer";
	rename -uid "C7278940-0000-4213-5C7F-EA3900000253";
	setAttr ".g" yes;
createNode aiOptions -s -n "defaultArnoldRenderOptions";
	rename -uid "C7278940-0000-4213-5C7F-EA83000002F5";
	addAttr -ci true -sn "ARV_options" -ln "ARV_options" -dt "string";
	setAttr ".logv" 2;
	setAttr ".export_separator" 1;
	setAttr ".version" -type "string" "3.2.0.wip";
	setAttr ".ARV_options" -type "string" "Test Resolution=100%;Camera=renderCamShape;Color Management.Gamma=1;Color Management.Exposure=0;Background.BG=BG Color;Background.Color=0 0 0;Background.Image=;Background.Scale=1       1;Background.Offset=0       0;Background.Apply Color Management=1;Foreground.Enable FG=0;Foreground.Image=;Foreground.Scale=1       1;Foreground.Offset=0       0;Foreground.Apply Color Management=1;";
createNode aiAOVFilter -s -n "defaultArnoldFilter";
	rename -uid "C7278940-0000-4213-5C7F-EA83000002F6";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVDriver -s -n "defaultArnoldDriver";
	rename -uid "C7278940-0000-4213-5C7F-EA83000002F7";
	setAttr ".color_management" 1;
	setAttr ".ai_translator" -type "string" "tif";
createNode aiAOVDriver -s -n "defaultArnoldDisplayDriver";
	rename -uid "C7278940-0000-4213-5C7F-EA83000002F8";
	setAttr ".output_mode" 0;
	setAttr ".ai_translator" -type "string" "maya";
createNode aiStandardSurface -n "aiStandardSurface1";
	rename -uid "C7278940-0000-4213-5C7F-EB72000002FA";
	setAttr ".base_color" -type "float3" 0 0 1 ;
createNode aiStandardSurface -n "aiStandardSurface2";
	rename -uid "C7278940-0000-4213-5C7F-EB86000002FD";
	setAttr ".base_color" -type "float3" 0 1 0 ;
createNode shadingEngine -n "aiSubtractGeo1SG";
	rename -uid "C7278940-0000-4213-5C7F-EB9500000302";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".ai_surface_shader" -type "float3" 0.5 0.5 0.5 ;
createNode materialInfo -n "materialInfo1";
	rename -uid "C7278940-0000-4213-5C7F-EB9500000303";
createNode aiStandardSurface -n "aiStandardSurface3";
	rename -uid "C7278940-0000-4213-5C7F-EF7B000003CC";
	setAttr ".base_color" -type "float3" 1 0 0 ;
createNode shadingEngine -n "aiStandardSurface3SG";
	rename -uid "C7278940-0000-4213-5C7F-EF7B000003CD";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo3";
	rename -uid "C7278940-0000-4213-5C7F-EF7B000003CE";
createNode script -n "sceneConfigurationScriptNode";
	rename -uid "C7278940-0000-4213-5C7F-F150000003F0";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 120 -ast 1 -aet 200 ";
	setAttr ".st" 6;
createNode shadingEngine -n "aiSubtractGeo1SG1";
	rename -uid "3664C940-0000-5182-5C7F-F4E200000314";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo4";
	rename -uid "3664C940-0000-5182-5C7F-F4E200000315";
createNode shadingEngine -n "aiSubtractGeo1SG2";
	rename -uid "3664C940-0000-5182-5C7F-F4FC00000317";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo5";
	rename -uid "3664C940-0000-5182-5C7F-F4FC00000318";
createNode aiStandardSurface -n "aiStandardSurface4";
	rename -uid "3664C940-0000-5182-5C7F-F5150000031A";
	setAttr ".base_color" -type "float3" 1 0 1 ;
createNode aiStandardSurface -n "aiStandardSurface5";
	rename -uid "3664C940-0000-5182-5C7F-F5340000031B";
	setAttr ".base_color" -type "float3" 1 1 0 ;
createNode shadingEngine -n "aiSubtractGeo5SG";
	rename -uid "7F0649C0-0000-9D22-5C81-072000000328";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo6";
	rename -uid "7F0649C0-0000-9D22-5C81-072000000329";
createNode shadingEngine -n "aiSubtractGeo6SG";
	rename -uid "7F0649C0-0000-9D22-5C81-07E700000337";
	setAttr ".ihi" 0;
	setAttr -s 5 ".dsm";
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo7";
	rename -uid "7F0649C0-0000-9D22-5C81-07E700000338";
createNode aiStandardSurface -n "aiStandardSurface6";
	rename -uid "7F0649C0-0000-9D22-5C81-08120000033D";
	setAttr ".base_color" -type "float3" 0 0.77593362 0 ;
createNode shadingEngine -n "aiStandardSurface6SG";
	rename -uid "7F0649C0-0000-9D22-5C81-08120000033E";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo8";
	rename -uid "7F0649C0-0000-9D22-5C81-08120000033F";
createNode aiSwitch -n "aiSwitch1";
	rename -uid "7F0649C0-0000-9D22-5C81-0DD4000003AB";
createNode shadingEngine -n "aiSwitch1SG";
	rename -uid "7F0649C0-0000-9D22-5C81-0DD4000003AC";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo9";
	rename -uid "7F0649C0-0000-9D22-5C81-0DD4000003AD";
createNode aiClipGeo -n "aiClipGeo3";
	rename -uid "34EA5940-0000-3022-5D16-2FF700000308";
createNode polySphere -n "polySphere2";
	rename -uid "261DE940-0000-3E51-5D16-355400000308";
	setAttr ".r" 4;
createNode polyExtrudeFace -n "polyExtrudeFace1";
	rename -uid "261DE940-0000-3E51-5D16-35700000030C";
	setAttr ".ics" -type "componentList" 1 "f[0:399]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0
		 0 0 1 0 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" -4.7683716e-07 0 -7.1525574e-07 ;
	setAttr ".rs" 1195280354;
	setAttr ".lt" -type "double3" -3.2813378750273792e-16 9.0205620750793969e-17 0.17114076010144802 ;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" -4.0000009536743164 -4 -4.0000019073486328 ;
	setAttr ".cbx" -type "double3" 4 4 4.0000004768371582 ;
createNode polyCube -n "polyCube3";
	rename -uid "261DE940-0000-3E51-5D16-357900000343";
	setAttr ".w" 4;
	setAttr ".h" 4;
	setAttr ".d" 4;
	setAttr ".cuv" 4;
createNode lambert -n "lambert2";
	rename -uid "261DE940-0000-3E51-5D16-35D70000034A";
	setAttr ".c" -type "float3" 1 0 1 ;
createNode shadingEngine -n "lambert2SG";
	rename -uid "261DE940-0000-3E51-5D16-35D70000034B";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo10";
	rename -uid "261DE940-0000-3E51-5D16-35D70000034C";
createNode polyNormal -n "polyNormal1";
	rename -uid "261DE940-0000-3E51-5D16-362D00000363";
	setAttr ".ics" -type "componentList" 1 "f[*]";
	setAttr ".nm" 2;
	setAttr ".unm" no;
createNode polySphere -n "polySphere3";
	rename -uid "261DE940-0000-3E51-5D16-36980000036F";
	setAttr ".r" 3;
createNode lambert -n "lambert3";
	rename -uid "261DE940-0000-3E51-5D16-36B300000374";
	setAttr ".c" -type "float3" 1 1 0 ;
createNode shadingEngine -n "lambert3SG";
	rename -uid "261DE940-0000-3E51-5D16-36B300000375";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo11";
	rename -uid "261DE940-0000-3E51-5D16-36B300000376";
createNode nodeGraphEditorInfo -n "hyperShadePrimaryNodeEditorSavedTabsInfo";
	rename -uid "47D6A940-0000-5B9B-5D16-3D4100000301";
	setAttr ".tgi[0].tn" -type "string" "Untitled_1";
	setAttr ".tgi[0].vl" -type "double2" 169.37405561340378 2224.6030862053913 ;
	setAttr ".tgi[0].vh" -type "double2" 1266.3401730507246 3353.1744699314222 ;
	setAttr -s 4 ".tgi[0].ni";
	setAttr ".tgi[0].ni[0].x" 810;
	setAttr ".tgi[0].ni[0].y" 2842.857177734375;
	setAttr ".tgi[0].ni[0].nvs" 1922;
	setAttr ".tgi[0].ni[1].x" 284.28570556640625;
	setAttr ".tgi[0].ni[1].y" 3074.28564453125;
	setAttr ".tgi[0].ni[1].nvs" 2579;
	setAttr ".tgi[0].ni[2].x" 650;
	setAttr ".tgi[0].ni[2].y" 3140;
	setAttr ".tgi[0].ni[2].nvs" 1922;
	setAttr ".tgi[0].ni[3].x" 957.14288330078125;
	setAttr ".tgi[0].ni[3].y" 3118.571533203125;
	setAttr ".tgi[0].ni[3].nvs" 1923;
select -ne :time1;
	setAttr ".o" 1;
	setAttr ".unw" 1;
select -ne :hardwareRenderingGlobals;
	setAttr ".otfna" -type "stringArray" 22 "NURBS Curves" "NURBS Surfaces" "Polygons" "Subdiv Surface" "Particles" "Particle Instance" "Fluids" "Strokes" "Image Planes" "UI" "Lights" "Cameras" "Locators" "Joints" "IK Handles" "Deformers" "Motion Trails" "Components" "Hair Systems" "Follicles" "Misc. UI" "Ornaments"  ;
	setAttr ".otfva" -type "Int32Array" 22 0 1 1 1 1 1
		 1 1 1 0 0 0 0 0 0 0 0 0
		 0 0 0 0 ;
	setAttr ".fprt" yes;
select -ne :renderPartition;
	setAttr -s 12 ".st";
select -ne :renderGlobalsList1;
select -ne :defaultShaderList1;
	setAttr -s 14 ".s";
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
	setAttr ".ifp" -type "string" "testrender";
select -ne :defaultResolution;
	setAttr ".w" 160;
	setAttr ".h" 120;
	setAttr ".pa" 1;
	setAttr ".dar" 1.3333333730697632;
select -ne :defaultLightSet;
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
connectAttr "polyNormal1.out" "pSphereShape3.i";
connectAttr "polyCube3.out" "pCubeShape5.i";
connectAttr "polySphere3.out" "pSphereShape4.i";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiSubtractGeo1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "lambert2SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandardSurface3SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiSubtractGeo1SG1.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiSubtractGeo1SG2.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiSubtractGeo5SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiSubtractGeo6SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandardSurface6SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiSwitch1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "lambert3SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiSubtractGeo1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "lambert2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandardSurface3SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiSubtractGeo1SG1.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiSubtractGeo1SG2.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiSubtractGeo5SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiSubtractGeo6SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandardSurface6SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiSwitch1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "lambert3SG.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr ":defaultArnoldDisplayDriver.msg" ":defaultArnoldRenderOptions.drivers"
		 -na;
connectAttr ":defaultArnoldFilter.msg" ":defaultArnoldRenderOptions.filt";
connectAttr ":defaultArnoldDriver.msg" ":defaultArnoldRenderOptions.drvr";
connectAttr "aiSubtractGeo1SG.msg" "materialInfo1.sg";
connectAttr "aiStandardSurface3.out" "aiStandardSurface3SG.ss";
connectAttr "aiStandardSurface3SG.msg" "materialInfo3.sg";
connectAttr "aiStandardSurface3.msg" "materialInfo3.m";
connectAttr "aiStandardSurface3.msg" "materialInfo3.t" -na;
connectAttr "aiStandardSurface2.out" "aiSubtractGeo1SG1.ss";
connectAttr "aiSubtractGeo1SG1.msg" "materialInfo4.sg";
connectAttr "aiStandardSurface2.msg" "materialInfo4.m";
connectAttr "aiStandardSurface2.msg" "materialInfo4.t" -na;
connectAttr "aiStandardSurface4.out" "aiSubtractGeo1SG2.ss";
connectAttr "aiSubtractGeo1SG2.msg" "materialInfo5.sg";
connectAttr "aiStandardSurface4.msg" "materialInfo5.m";
connectAttr "aiStandardSurface4.msg" "materialInfo5.t" -na;
connectAttr "aiSubtractGeo5SG.msg" "materialInfo6.sg";
connectAttr "aiClipGeo3.out" "aiSubtractGeo6SG.ss";
connectAttr "pCubeShape5.iog" "aiSubtractGeo6SG.dsm" -na;
connectAttr "pCubeShape6.iog" "aiSubtractGeo6SG.dsm" -na;
connectAttr "pCubeShape7.iog" "aiSubtractGeo6SG.dsm" -na;
connectAttr "pCubeShape8.iog" "aiSubtractGeo6SG.dsm" -na;
connectAttr "aiSubtractGeo6SG.msg" "materialInfo7.sg";
connectAttr "aiClipGeo3.msg" "materialInfo7.m";
connectAttr "aiClipGeo3.msg" "materialInfo7.t" -na;
connectAttr "aiStandardSurface6.out" "aiStandardSurface6SG.ss";
connectAttr "aiStandardSurface6SG.msg" "materialInfo8.sg";
connectAttr "aiStandardSurface6.msg" "materialInfo8.m";
connectAttr "aiStandardSurface6.msg" "materialInfo8.t" -na;
connectAttr "aiSwitch1.out" "aiSwitch1SG.ss";
connectAttr "aiSwitch1SG.msg" "materialInfo9.sg";
connectAttr "aiSwitch1.msg" "materialInfo9.m";
connectAttr "aiSwitch1.msg" "materialInfo9.t" -na;
connectAttr "aiStandardSurface6.out" "aiClipGeo3.intersection";
connectAttr "polySphere2.out" "polyExtrudeFace1.ip";
connectAttr "pSphereShape3.wm" "polyExtrudeFace1.mp";
connectAttr "lambert2.oc" "lambert2SG.ss";
connectAttr "pSphereShape3.iog" "lambert2SG.dsm" -na;
connectAttr "lambert2SG.msg" "materialInfo10.sg";
connectAttr "lambert2.msg" "materialInfo10.m";
connectAttr "polyExtrudeFace1.out" "polyNormal1.ip";
connectAttr "lambert3.oc" "lambert3SG.ss";
connectAttr "pSphereShape4.iog" "lambert3SG.dsm" -na;
connectAttr "lambert3SG.msg" "materialInfo11.sg";
connectAttr "lambert3.msg" "materialInfo11.m";
connectAttr "renderCamShape.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[0].dn"
		;
connectAttr "aiStandardSurface6.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[1].dn"
		;
connectAttr "aiClipGeo3.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[2].dn"
		;
connectAttr "aiSubtractGeo6SG.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[3].dn"
		;
connectAttr "aiSubtractGeo1SG.pa" ":renderPartition.st" -na;
connectAttr "aiStandardSurface3SG.pa" ":renderPartition.st" -na;
connectAttr "aiSubtractGeo1SG1.pa" ":renderPartition.st" -na;
connectAttr "aiSubtractGeo1SG2.pa" ":renderPartition.st" -na;
connectAttr "aiSubtractGeo5SG.pa" ":renderPartition.st" -na;
connectAttr "aiSubtractGeo6SG.pa" ":renderPartition.st" -na;
connectAttr "aiStandardSurface6SG.pa" ":renderPartition.st" -na;
connectAttr "aiSwitch1SG.pa" ":renderPartition.st" -na;
connectAttr "lambert2SG.pa" ":renderPartition.st" -na;
connectAttr "lambert3SG.pa" ":renderPartition.st" -na;
connectAttr "aiStandardSurface1.msg" ":defaultShaderList1.s" -na;
connectAttr "aiStandardSurface2.msg" ":defaultShaderList1.s" -na;
connectAttr "aiStandardSurface3.msg" ":defaultShaderList1.s" -na;
connectAttr "aiStandardSurface4.msg" ":defaultShaderList1.s" -na;
connectAttr "aiStandardSurface5.msg" ":defaultShaderList1.s" -na;
connectAttr "aiStandardSurface6.msg" ":defaultShaderList1.s" -na;
connectAttr "aiSwitch1.msg" ":defaultShaderList1.s" -na;
connectAttr "aiClipGeo3.msg" ":defaultShaderList1.s" -na;
connectAttr "lambert2.msg" ":defaultShaderList1.s" -na;
connectAttr "lambert3.msg" ":defaultShaderList1.s" -na;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "aiSkyDomeLightShape1.ltd" ":lightList1.l" -na;
connectAttr "aiSkyDomeLight1.iog" ":defaultLightSet.dsm" -na;
// End of test.ma
