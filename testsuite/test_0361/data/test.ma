//Maya ASCII 2018ff09 scene
//Name: test.ma
//Last modified: Wed, Jul 24, 2019 03:57:24 pm
//Codeset: UTF-8
requires maya "2018ff09";
requires -nodeType "aiOptions" -nodeType "aiAOVDriver" -nodeType "aiAOVFilter" -nodeType "aiAreaLight"
		 -nodeType "aiStandardSurface" "mtoa" "3.3.0.wip";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2018";
fileInfo "version" "2018";
fileInfo "cutIdentifier" "201903222215-65bada0e52";
fileInfo "osv" "Linux 3.10.0-957.21.3.el7.x86_64 #1 SMP Tue Jun 18 16:35:19 UTC 2019 x86_64";
createNode transform -s -n "persp";
	rename -uid "03CBB940-0000-6A04-5D38-4F630000034E";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -1.7860066842838775 1.9609375970445868 6.2048057375531922 ;
	setAttr ".r" -type "double3" -6.9383527296035101 -22.599999999999241 2.1531896409584659e-16 ;
	setAttr ".rp" -type "double3" 0 2.2204460492503131e-16 0 ;
	setAttr ".rpt" -type "double3" -7.3562173735672791e-17 -2.5878808296570588e-17 -7.3562173735672865e-17 ;
createNode camera -s -n "perspShape" -p "persp";
	rename -uid "03CBB940-0000-6A04-5D38-4F630000034F";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999979;
	setAttr ".coi" 7.816084235925203;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".tp" -type "double3" 0.23302471357949983 0.55557330360451473 -0.62288511693585491 ;
	setAttr ".hc" -type "string" "viewSet -p %camera";
	setAttr ".ai_translator" -type "string" "perspective";
createNode transform -s -n "top";
	rename -uid "03CBB940-0000-6A04-5D38-4F6300000350";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 1000.1 0 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	rename -uid "03CBB940-0000-6A04-5D38-4F6300000351";
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
	rename -uid "03CBB940-0000-6A04-5D38-4F6300000352";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 1000.1 ;
createNode camera -s -n "frontShape" -p "front";
	rename -uid "03CBB940-0000-6A04-5D38-4F6300000353";
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
	rename -uid "03CBB940-0000-6A04-5D38-4F6300000354";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 1000.1 0 0 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	rename -uid "03CBB940-0000-6A04-5D38-4F6300000355";
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
createNode transform -n "aiAreaLight1";
	rename -uid "03CBB940-0000-6A04-5D38-507A000003B2";
	setAttr ".t" -type "double3" 1.5 1.5 1.5 ;
	setAttr ".s" -type "double3" 2.5 3.5489490397912671 1 ;
createNode aiAreaLight -n "aiAreaLightShape1" -p "aiAreaLight1";
	rename -uid "03CBB940-0000-6A04-5D38-507A000003B1";
	addAttr -ci true -h true -sn "aal" -ln "attributeAliasList" -dt "attributeAlias";
	setAttr -k off ".v";
	setAttr ".csh" no;
	setAttr ".rcsh" no;
	setAttr ".sc" -type "float3" 0 1 1 ;
	setAttr ".ai_exposure" 3;
	setAttr ".ai_translator" -type "string" "quad";
	setAttr ".aal" -type "attributeAlias" {"exposure","aiExposure","normalize","aiNormalize"
		} ;
createNode transform -n "aiAreaLight2";
	rename -uid "A57E4940-0000-A964-5D38-62700000027C";
	setAttr ".t" -type "double3" 1.5 1.5 1.5 ;
	setAttr ".s" -type "double3" 2.5 3.5489490397912671 1 ;
createNode aiAreaLight -n "aiAreaLightShape2" -p "aiAreaLight2";
	rename -uid "A57E4940-0000-A964-5D38-62700000027D";
	addAttr -ci true -h true -sn "aal" -ln "attributeAliasList" -dt "attributeAlias";
	setAttr -k off ".v";
	setAttr ".csh" no;
	setAttr ".rcsh" no;
	setAttr ".sc" -type "float3" 0 1 0 ;
	setAttr ".ai_exposure" 3;
	setAttr ".ai_translator" -type "string" "quad";
	setAttr ".aal" -type "attributeAlias" {"exposure","aiExposure","normalize","aiNormalize"
		} ;
createNode transform -n "pCube1";
	rename -uid "03CBB940-0000-6A04-5D38-4F65000003AE";
	setAttr ".t" -type "double3" 0 0 -1.3619139398369748 ;
	setAttr ".s" -type "double3" 2.0036959926863704 2.0036959926863704 2.0036959926863704 ;
createNode mesh -n "pCubeShape1" -p "pCube1";
	rename -uid "03CBB940-0000-6A04-5D38-4F65000003AD";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".ai_translator" -type "string" "polymesh";
createNode transform -n "pCube2";
	rename -uid "A57E4940-0000-A964-5D38-626800000277";
	setAttr ".t" -type "double3" 2.3380311937052118 0 -1.3694504198003685 ;
	setAttr ".s" -type "double3" 2.0036959926863704 2.0036959926863704 2.0036959926863704 ;
createNode mesh -n "pCubeShape2" -p "pCube2";
	rename -uid "A57E4940-0000-A964-5D38-626800000276";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".ai_translator" -type "string" "polymesh";
createNode transform -n "pCube3";
	rename -uid "67CF7940-0000-2419-5D38-707D000002B9";
	setAttr ".t" -type "double3" 2.3380311937052118 2.1115849451902968 -1.3694504198003685 ;
	setAttr ".s" -type "double3" 2.0036959926863704 2.0036959926863704 2.0036959926863704 ;
createNode mesh -n "pCubeShape3" -p "pCube3";
	rename -uid "67CF7940-0000-2419-5D38-707D000002BA";
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
	setAttr -s 8 ".vt[0:7]"  -0.5 -0.5 0.5 0.5 -0.5 0.5 -0.5 0.5 0.5 0.5 0.5 0.5
		 -0.5 0.5 -0.5 0.5 0.5 -0.5 -0.5 -0.5 -0.5 0.5 -0.5 -0.5;
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
createNode transform -n "pCube4";
	rename -uid "67CF7940-0000-2419-5D38-707D000002BB";
	setAttr ".t" -type "double3" 0 2.1115849451902968 -1.3619139398369748 ;
	setAttr ".s" -type "double3" 2.0036959926863704 2.0036959926863704 2.0036959926863704 ;
createNode mesh -n "pCubeShape4" -p "pCube4";
	rename -uid "67CF7940-0000-2419-5D38-707D000002BC";
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
	setAttr -s 8 ".vt[0:7]"  -0.5 -0.5 0.5 0.5 -0.5 0.5 -0.5 0.5 0.5 0.5 0.5 0.5
		 -0.5 0.5 -0.5 0.5 0.5 -0.5 -0.5 -0.5 -0.5 0.5 -0.5 -0.5;
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
createNode lightLinker -s -n "lightLinker1";
	rename -uid "67CF7940-0000-2419-5D38-701100000268";
	setAttr -s 10 ".lnk";
	setAttr -s 4 ".ign";
	setAttr -s 3 ".slnk";
createNode shapeEditorManager -n "shapeEditorManager";
	rename -uid "67CF7940-0000-2419-5D38-701100000269";
createNode poseInterpolatorManager -n "poseInterpolatorManager";
	rename -uid "67CF7940-0000-2419-5D38-70110000026A";
createNode displayLayerManager -n "layerManager";
	rename -uid "67CF7940-0000-2419-5D38-70110000026B";
createNode displayLayer -n "defaultLayer";
	rename -uid "03CBB940-0000-6A04-5D38-4F63000003A5";
createNode renderLayerManager -n "renderLayerManager";
	rename -uid "67CF7940-0000-2419-5D38-70110000026D";
createNode renderLayer -n "defaultRenderLayer";
	rename -uid "03CBB940-0000-6A04-5D38-4F63000003A7";
	setAttr ".g" yes;
createNode aiOptions -s -n "defaultArnoldRenderOptions";
	rename -uid "03CBB940-0000-6A04-5D38-4F63000003A8";
	addAttr -ci true -sn "ARV_options" -ln "ARV_options" -dt "string";
	setAttr ".arnrt" 2;
	setAttr ".mtrinf" yes;
	setAttr ".sspath" -type "string" "/software/solidangle/mtoa/3.3.0.wip-dev/2018/plugins";
	setAttr ".export_full_paths" yes;
	setAttr ".export_separator" 1;
	setAttr ".version" -type "string" "3.3.0.wip";
	setAttr ".ARV_options" -type "string" "Test Resolution=100%;Camera=perspShape;Color Management.Gamma=1;Color Management.Exposure=0;Background.BG=BG Color;Background.Color=0 0 0;Background.Image=;Background.Scale=1         1;Background.Offset=0         0;Background.Apply Color Management=1;Foreground.Enable FG=0;Foreground.Image=;Foreground.Scale=1         1;Foreground.Offset=0         0;Foreground.Apply Color Management=1;";
createNode aiAOVFilter -s -n "defaultArnoldFilter";
	rename -uid "03CBB940-0000-6A04-5D38-4F63000003A9";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVDriver -s -n "defaultArnoldDriver";
	rename -uid "03CBB940-0000-6A04-5D38-4F63000003AA";
	setAttr ".color_management" 1;
	setAttr ".ai_translator" -type "string" "tif";
createNode aiAOVDriver -s -n "defaultArnoldDisplayDriver";
	rename -uid "03CBB940-0000-6A04-5D38-4F63000003AB";
	setAttr ".output_mode" 0;
	setAttr ".ai_translator" -type "string" "maya";
createNode polyCube -n "polyCube1";
	rename -uid "03CBB940-0000-6A04-5D38-4F65000003AC";
	setAttr ".cuv" 4;
createNode script -n "sceneConfigurationScriptNode";
	rename -uid "03CBB940-0000-6A04-5D38-56BC000003E1";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 120 -ast 1 -aet 200 ";
	setAttr ".st" 6;
createNode aiStandardSurface -n "aiStandardSurface1";
	rename -uid "A57E4940-0000-A964-5D38-61C200000272";
	setAttr ".diffuse_roughness" 1;
	setAttr ".specular" 0;
	setAttr ".metalness" 1;
createNode shadingEngine -n "aiStandardSurface1SG";
	rename -uid "A57E4940-0000-A964-5D38-61C200000273";
	setAttr ".ihi" 0;
	setAttr -s 4 ".dsm";
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
	rename -uid "A57E4940-0000-A964-5D38-61C200000274";
createNode polyCube -n "polyCube2";
	rename -uid "A57E4940-0000-A964-5D38-626800000275";
	setAttr ".cuv" 4;
createNode nodeGraphEditorInfo -n "hyperShadePrimaryNodeEditorSavedTabsInfo";
	rename -uid "F6269940-0000-1CD6-5D38-6F4B00000303";
	setAttr ".tgi[0].tn" -type "string" "Untitled_1";
	setAttr ".tgi[0].vl" -type "double2" -329.7618916583445 -340.87300232793973 ;
	setAttr ".tgi[0].vh" -type "double2" 277.38094135882403 339.2857008037119 ;
	setAttr -s 2 ".tgi[0].ni";
	setAttr ".tgi[0].ni[0].x" -91.428573608398438;
	setAttr ".tgi[0].ni[0].y" 71.428573608398438;
	setAttr ".tgi[0].ni[0].nvs" 1923;
	setAttr ".tgi[0].ni[1].x" -91.428573608398438;
	setAttr ".tgi[0].ni[1].y" -115.71428680419922;
	setAttr ".tgi[0].ni[1].nvs" 1923;
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
	setAttr -s 3 ".st";
select -ne :renderGlobalsList1;
select -ne :defaultShaderList1;
	setAttr -s 5 ".s";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderingList1;
select -ne :lightList1;
	setAttr -s 2 ".l";
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
	setAttr -s 2 ".dsm";
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
connectAttr "polyCube1.out" "pCubeShape1.i";
connectAttr "polyCube2.out" "pCubeShape2.i";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "pCubeShape2.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "pCubeShape1.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "pCubeShape3.message" "aiAreaLightShape2.message";
relationship "link" ":lightLinker1" "pCubeShape4.message" "aiAreaLightShape1.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandardSurface1SG.message" ":defaultLightSet.message";
relationship "ignore" ":lightLinker1" "pCubeShape1.message" "aiAreaLightShape1.message";
relationship "ignore" ":lightLinker1" "pCubeShape2.message" "aiAreaLightShape2.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr ":defaultArnoldDisplayDriver.msg" ":defaultArnoldRenderOptions.drivers"
		 -na;
connectAttr ":defaultArnoldFilter.msg" ":defaultArnoldRenderOptions.filt";
connectAttr ":defaultArnoldDriver.msg" ":defaultArnoldRenderOptions.drvr";
connectAttr "aiStandardSurface1.out" "aiStandardSurface1SG.ss";
connectAttr "pCubeShape1.iog" "aiStandardSurface1SG.dsm" -na;
connectAttr "pCubeShape2.iog" "aiStandardSurface1SG.dsm" -na;
connectAttr "pCubeShape3.iog" "aiStandardSurface1SG.dsm" -na;
connectAttr "pCubeShape4.iog" "aiStandardSurface1SG.dsm" -na;
connectAttr "aiStandardSurface1SG.msg" "materialInfo1.sg";
connectAttr "aiStandardSurface1.msg" "materialInfo1.m";
connectAttr "aiStandardSurface1.msg" "materialInfo1.t" -na;
connectAttr "aiAreaLightShape1.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[1].dn"
		;
connectAttr "aiStandardSurface1SG.pa" ":renderPartition.st" -na;
connectAttr "aiStandardSurface1.msg" ":defaultShaderList1.s" -na;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "aiAreaLightShape1.ltd" ":lightList1.l" -na;
connectAttr "aiAreaLightShape2.ltd" ":lightList1.l" -na;
connectAttr "aiAreaLight1.iog" ":defaultLightSet.dsm" -na;
connectAttr "aiAreaLight2.iog" ":defaultLightSet.dsm" -na;
// End of test.ma
