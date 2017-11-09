//Maya ASCII 2018 scene
//Name: test.ma
//Last modified: Mon, Oct 23, 2017 01:43:19 PM
//Codeset: 1252
requires maya "2018";
requires -nodeType "hairPhysicalShader" "hairPhysicalShader" "1.0";
requires -nodeType "xgmSplineBase" -nodeType "xgmModifierSculpt" -nodeType "xgmModifierScale"
		 -nodeType "xgmSplineDescription" -dataType "xgmGuideData" -dataType "igmDescriptionData"
		 -dataType "xgmSplineData" -dataType "xgmMeshData" -dataType "xgmSplineTweakData"
		 -dataType "xgmSplineBoundInfoData" -dataType "xgmGuideRefData" "xgenToolkit" "1.0";
requires -nodeType "aiOptions" -nodeType "aiAOVDriver" -nodeType "aiAOVFilter" -nodeType "aiSkyDomeLight"
		 "mtoa" "2.0.3.wip";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2018";
fileInfo "version" "2018";
fileInfo "cutIdentifier" "201708092215-22fcc444d4";
fileInfo "osv" "Microsoft Windows 7 Enterprise Edition, 64-bit Windows 7 Service Pack 1 (Build 7601)\n";
createNode transform -s -n "persp";
	rename -uid "07557DD1-4BE2-6D53-6267-829B8F4920F4";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 7.37351132826476 13.446706168749465 6.8759105514138472 ;
	setAttr ".r" -type "double3" -53.138352729602502 47.000000000000071 4.663581128218628e-15 ;
createNode camera -s -n "perspShape" -p "persp";
	rename -uid "3C60EFB3-4C19-3A1D-7EE8-E1B557BA0548";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999993;
	setAttr ".coi" 16.806567823555703;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".hc" -type "string" "viewSet -p %camera";
	setAttr ".ai_translator" -type "string" "perspective";
createNode transform -s -n "top";
	rename -uid "E46CB17D-4A67-1A2D-42E9-BB934DD89DFE";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 1000.1 0 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	rename -uid "1FBDEE38-4DE7-1C4E-7C47-26B53EA18579";
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
	rename -uid "1DA22878-4A9F-4BD5-175B-EF8CD5A6F0E5";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 1000.1 ;
createNode camera -s -n "frontShape" -p "front";
	rename -uid "BCF7640D-4DCA-0099-EF90-FA848FA130CD";
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
	rename -uid "23FA6736-40A8-6FA2-E1F1-008A1C5BB252";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 1000.1 0 0 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	rename -uid "F6DA1093-4D82-48CF-E655-439E1C4301C5";
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
createNode transform -n "pPlane1";
	rename -uid "22382427-4C17-9484-E917-EFBE8A3F61B3";
	setAttr ".s" -type "double3" 8.6375822713260693 8.6375822713260693 8.6375822713260693 ;
createNode mesh -n "pPlaneShape1" -p "pPlane1";
	rename -uid "41E7FCAD-4DEC-878E-4C7D-DFAEBBD7E999";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".ai_translator" -type "string" "polymesh";
createNode transform -n "description1";
	rename -uid "22E6B7A8-4D09-FF58-4102-4AB84F298E8D";
	setAttr ".mntl" -type "double3" 0 0 0 ;
	setAttr ".mxtl" -type "double3" 0 0 0 ;
	setAttr ".mtxe" yes;
	setAttr ".mtye" yes;
	setAttr ".mtze" yes;
	setAttr ".xtxe" yes;
	setAttr ".xtye" yes;
	setAttr ".xtze" yes;
	setAttr ".mnrl" -type "double3" 0 0 0 ;
	setAttr ".mxrl" -type "double3" 0 0 0 ;
	setAttr ".mrxe" yes;
	setAttr ".mrye" yes;
	setAttr ".mrze" yes;
	setAttr ".xrxe" yes;
	setAttr ".xrye" yes;
	setAttr ".xrze" yes;
	setAttr ".mnsl" -type "double3" 1 1 1 ;
	setAttr ".msxe" yes;
	setAttr ".msye" yes;
	setAttr ".msze" yes;
	setAttr ".xsxe" yes;
	setAttr ".xsye" yes;
	setAttr ".xsze" yes;
createNode xgmSplineDescription -n "description1_Shape" -p "description1";
	rename -uid "1E791370-4B9B-5FB5-D230-4181C81DE5BD";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".w" 0.5;
	setAttr -s 2 ".wdr[0:1]"  0 1 1 1 1 1;
	setAttr ".fca" no;
createNode transform -n "aiSkyDomeLight1";
	rename -uid "DBC39656-4702-CA87-F228-3396A4753215";
createNode aiSkyDomeLight -n "aiSkyDomeLightShape1" -p "aiSkyDomeLight1";
	rename -uid "3BF4F826-4E76-9569-F858-CAA1619502F2";
	addAttr -ci true -h true -sn "aal" -ln "attributeAliasList" -dt "attributeAlias";
	setAttr -k off ".v";
	setAttr ".csh" no;
	setAttr ".rcsh" no;
	setAttr ".camera" 0;
	setAttr ".ai_samples" 3;
	setAttr ".aal" -type "attributeAlias" {"exposure","aiExposure"} ;
createNode lightLinker -s -n "lightLinker1";
	rename -uid "14DEFA87-4A52-C505-510F-229AD927D6A2";
	setAttr -s 3 ".lnk";
	setAttr -s 3 ".slnk";
createNode shapeEditorManager -n "shapeEditorManager";
	rename -uid "0B2FB82C-44FE-C70E-D2C7-4189E7F5F4F2";
createNode poseInterpolatorManager -n "poseInterpolatorManager";
	rename -uid "FE422865-4E7E-16A0-1045-91BA7036A3C8";
createNode displayLayerManager -n "layerManager";
	rename -uid "AD7990FB-4615-733E-B53A-FE8E429042A4";
createNode displayLayer -n "defaultLayer";
	rename -uid "EAFF92C9-4F45-7FD4-7FA6-8D90CA90BECF";
createNode renderLayerManager -n "renderLayerManager";
	rename -uid "04CE945F-4AD3-4DC1-75F8-CEA347ED1C1D";
createNode renderLayer -n "defaultRenderLayer";
	rename -uid "FC3D8C77-43A6-EC84-F275-F7A6D11F78B4";
	setAttr ".g" yes;
createNode aiOptions -s -n "defaultArnoldRenderOptions";
	rename -uid "13F394B8-4B8D-3305-54F4-F8BC7D2D16D4";
	addAttr -ci true -sn "ARV_options" -ln "ARV_options" -dt "string";
	setAttr ".GI_diffuse_samples" 3;
	setAttr ".GI_specular_samples" 0;
	setAttr ".GI_transmission_samples" 0;
	setAttr ".GI_sss_samples" 0;
	setAttr ".version" -type "string" "2.0.3.wip";
createNode aiAOVFilter -s -n "defaultArnoldFilter";
	rename -uid "91560CCD-45AA-95EE-8007-19A24D2ACE98";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVDriver -s -n "defaultArnoldDriver";
	rename -uid "322782DA-469B-D46E-2915-AF994961C809";
	setAttr ".color_management" 1;
	setAttr ".ai_translator" -type "string" "tif";
createNode aiAOVDriver -s -n "defaultArnoldDisplayDriver";
	rename -uid "50D844E6-49EE-8F79-C717-C3BE9A02560E";
	setAttr ".output_mode" 0;
	setAttr ".ai_translator" -type "string" "maya";
createNode polyPlane -n "polyPlane1";
	rename -uid "A6B97334-4AC3-F7CE-819D-E2B0BA1D4857";
	setAttr ".cuv" 2;
createNode xgmSplineBase -n "description1_base";
	rename -uid "D4639BE4-43BF-DFCF-2EFA-CA9D085A0866";
	setAttr ".bid" -type "xgmSplineBoundInfoData" "Header" "284" "4"
"{\"Header\":{\"Version\":1,\"Type\":\"XgSplineBoundInfoData\",\"GroupVersion\":1,\"Grou"
"pCount\":1,\"GroupBase64\":true,\"GroupDeflate\":true,\"GroupDeflateLevel\":9},\"Ite"
"ms\":[{\"Name\":\".boundMesh[0]\",\"GlobalDensity\":0.0,\"Resolution\":3,\"FaceIds\":0,"
"\"SubdivDensity\":1,\"RowNumOfFaces\":2,\"ColNumOfFaces\":3}]}"
"Group" "392" "6"
"AAAAAAAAAAAEAAAAAAAAALBoAAAAAAAAAAAAAAAAAAB42u3dxU5DARCFYaxwcXcrxd1dS4u7u/NA"
"7Hgt3oQda/5bQsKaVRf/Sb7MJLM8DzCf75kZYd5+RirhmoVs5CCCXOQhQD4KUIgiFKMEpShDOSpQ"
"iSpUowa1qEM9GtCIJjSjBa2Iog0xtKMDnehCN3rQiz70YwCDGMIwRjCKMYxjApOYwjRmMIs5zGMB"
"i1jCMlYQxyoSSGIN69jAJrawjR3sYg/7OMAhjnCME5ziDOe4wCWucI0b3OIO93jAI57wjJfwHvuI"
"pEp5/W0nGpckSZIkSZIkSZIkSZIkSZIkSZIkSZIkSZIkSZIkSZIkSZIkSZL0P8ngK/H3X0ugtGI/"
"6e0bM+mbGQ=="
;
	setAttr ".dm" 0.10000000149011612;
	setAttr ".w" 1;
	setAttr ".csd" -type "xgmSplineData" "Header" "535" "8"
"{\"Header\":{\"Version\":1,\"Type\":\"XgSplineData\",\"GroupVersion\":1,\"GroupCount\":1"
",\"GroupBase64\":true,\"GroupDeflate\":true,\"GroupDeflateLevel\":9},\"Items\":[{\"Na"
"me\":\".boundMesh[0]\",\"Id\":\"{FC103D71-1AA1-4F49-A320-476745569D4D}\",\"Mode\":\"De"
"nsity\",\"PrimitiveInfos\":0,\"Positions\":1,\"PatchUVs\":2,\"MeshN\":3,\"MeshU\":4,\"Me"
"shV\":5,\"FaceUV\":6,\"FaceId\":7,\"FrozenSet\":8,\"WIDTH_CV\":9,\"DIRECTION_CV\":10}],"
"\"RefMeshArray\":[{\"Name\":\".boundMesh[0]\",\"Vertices\":11,\"UVs\":12,\"VertsPerFace"
"\":13,\"FaceVertOffsets\":14,\"FaceUVOffsets\":15,\"TriIndices\":16}],\"CustomData\":"
"{}}"
"Group" "8732" "115"
"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"
"+e7H2uSinqXfF9VelUDROrNumZSDepp+v9F0dkfUc5qvK9wfQdHc4+dPy0HRmMxfb06gaO6TTa7p"
"gHrq23Mb4bN4uwtQe7S//cgXEygac+6v3yVQxHevejOCojXLO/erQw3U3GT147koiq247qUEisYf"
"n3h2G4rey7x36iIomnvnn3dHUMQZzf+eQLmvVVGWQBGP3rM9gqIx902fE0ERz+8TS6BKavC6qaMf"
"PoSi63bP/g73owZpvjp2Tx2KYrv63ffrUMQVizYmULT+L97+ug5F/qkdRydQ5L/ppg0JFPHsy16J"
"oOh108+Oq0PR+Oy79+Si6P1mq965KOK5N3SIoGj8oZINCRSt0/bGM3Uomntix/xcFI3v99sBdSjy"
"t238cR2KuNGUJbkoGvPWqptzUcQlC/8RQdH6vTdAAkXrf3G6KBdFY27Z3DmBIv+6ORfkosh/rCxa"
"h6L1J006E0HROi37T02gaPxtD+YlUMRnez0YQdGYaed9FEERdzj9xDaUe0/jI7ehiPcu3J5A0dz6"
"7P4RFI1P3h2JoIjfuWnjNhSNH7/m3yMo8p95/2c5KHo+967N7oiiZ9h++P0IisZ8edXN21A095v7"
"jyRQ7jOz/w91KHpfds3xHBStc+ixsgSKxvzlxVQuiq7DdZ23JVDkf/D1DxIo4ldLP8lF0ZjTf0ts"
"QxX/MIc5dzHPnZTOXY917mIeF6BSnLvk59wl5twl5twl5twl5twl5twl5twl5twl1rnrsc5dzOMC"
"VIpzl/ycu24Mbu4Sc+4Sc+4Sc+66r+vmLjHnLjHnLjHnrvtabu4Sc+4Sc+4S69z1WOcu5nEBKsW5"
"S37OXZfd3CXm3CXm3CXm3CXm3CXm3CXm3CXm3HXZzV1izl1izl1izl1izl1izl13jJu7xJy7xJy7"
"xJy7xJy7xJy7xJy7xJy7xJy7xJy7xJy7xJy7xJy7xJy7xJy77j11c5eYc5eYc5eYc5eYc5eYc5eY"
"c9ddx81dYs5dYs5dYs5d95lxc9cd7+aue/0fM88S566bF27uEnPuEnPuEuvc9dj7nf36Rm4Ob03u"
"thMtv7QvKBwHO19/097c7Bn7RPcs2Pn4Fvv4ybYwY8Jb9td14+1OZ5vC1X8bB3d81Rpa3DUIOp3t"
"hX/PAm9+m4U5cM+NJ+wBX26xG694w/5VxhA4XPEHXDcLjn1aCBtWZsO0omqIXNUUzi8rhPE7n7C/"
"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"
;
createNode xgmModifierScale -n "scale";
	rename -uid "5CDB928A-4558-994F-FA10-EF9E91A249E8";
	setAttr ".s" 3;
createNode xgmModifierSculpt -n "sculpt";
	rename -uid "9D8C3A2E-49C4-A514-FF4A-FF943FEE4995";
	setAttr ".ts[0].uio" 1;
createNode hairPhysicalShader -n "hairPhysicalShader1";
	rename -uid "E2D9D93C-4A4B-A445-F66E-34A00AAADDFE";
createNode shadingEngine -n "hairPhysicalShader1SG";
	rename -uid "EBBBDEE9-493C-7408-368B-508F8EC4C728";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
	rename -uid "D180C7C8-4D96-5D34-E13B-3781467384A3";
createNode script -n "uiConfigurationScriptNode";
	rename -uid "D82A2E79-4790-1541-8829-78834A4F1ECB";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n"
		+ "            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n"
		+ "            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n"
		+ "            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"side\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n"
		+ "            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n"
		+ "            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n"
		+ "\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n"
		+ "            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n"
		+ "            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n"
		+ "            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n"
		+ "            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n"
		+ "            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n"
		+ "            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 734\n            -height 748\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"ToggledOutliner\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"ToggledOutliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 0\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 1\n            -showReferenceMembers 1\n            -showAttributes 0\n"
		+ "            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -organizeByClip 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showParentContainers 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -isSet 0\n            -isSetMember 0\n"
		+ "            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            -renderFilterIndex 0\n            -selectionOrder \"chronological\" \n            -expandAttribute 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 0\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 0\n            -showReferenceMembers 0\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -organizeByClip 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showParentContainers 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n"
		+ "            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n"
		+ "\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -organizeByClip 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showParentContainers 1\n"
		+ "                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n"
		+ "                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 1\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -showCurveNames 0\n"
		+ "                -showActiveCurveNames 0\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                -valueLinesToggle 1\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n"
		+ "                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -organizeByClip 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showParentContainers 1\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n"
		+ "                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n"
		+ "                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"timeEditorPanel\" (localizedPanelLabel(\"Time Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Time Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n"
		+ "                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n"
		+ "                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" != $panelName) {\n"
		+ "\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"createNodePanel\" (localizedPanelLabel(\"Create Node\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Editor\")) `;\n\tif (\"\" != $panelName) {\n"
		+ "\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"shapePanel\" (localizedPanelLabel(\"Shape Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tshapePanel -edit -l (localizedPanelLabel(\"Shape Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"posePanel\" (localizedPanelLabel(\"Pose Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tposePanel -edit -l (localizedPanelLabel(\"Pose Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n\tif (\"\" != $panelName) {\n"
		+ "\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"profilerPanel\" (localizedPanelLabel(\"Profiler Tool\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Profiler Tool\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"contentBrowserPanel\" (localizedPanelLabel(\"Content Browser\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Content Browser\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"nodeEditorPanel\" (localizedPanelLabel(\"Node Editor\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -connectNodeOnCreation 0\n                -connectOnDrop 0\n                -highlightConnections 0\n                -copyConnectionsOnPaste 0\n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -crosshairOnEdgeDragging 0\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n"
		+ "                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -activeTab -1\n                -editorMode \"default\" \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-userCreated false\n\t\t\t\t-defaultImage \"vacantCell.xP:/\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -controllers 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 734\\n    -height 748\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -controllers 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 734\\n    -height 748\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	rename -uid "A86732D8-4FAA-56C8-97C6-9B800CB4D9E5";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 120 -ast 1 -aet 200 ";
	setAttr ".st" 6;
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
	setAttr ".pa" 1.3329999446868896;
select -ne :defaultLightSet;
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
connectAttr "polyPlane1.out" "pPlaneShape1.i";
connectAttr "sculpt.osd" "description1_Shape.isd";
connectAttr "sculpt.msg" "description1_Shape.as";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "hairPhysicalShader1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "hairPhysicalShader1SG.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr ":defaultArnoldDisplayDriver.msg" ":defaultArnoldRenderOptions.drivers"
		 -na;
connectAttr ":defaultArnoldFilter.msg" ":defaultArnoldRenderOptions.filt";
connectAttr ":defaultArnoldDriver.msg" ":defaultArnoldRenderOptions.drvr";
connectAttr "pPlaneShape1.w" "description1_base.bm[0]";
connectAttr "description1_base.osd" "scale.isd";
connectAttr "scale.osd" "sculpt.isd";
connectAttr "hairPhysicalShader1.oc" "hairPhysicalShader1SG.ss";
connectAttr "description1_Shape.iog" "hairPhysicalShader1SG.dsm" -na;
connectAttr "hairPhysicalShader1SG.msg" "materialInfo1.sg";
connectAttr "hairPhysicalShader1.msg" "materialInfo1.m";
connectAttr "hairPhysicalShader1.msg" "materialInfo1.t" -na;
connectAttr "hairPhysicalShader1SG.pa" ":renderPartition.st" -na;
connectAttr "hairPhysicalShader1.msg" ":defaultShaderList1.s" -na;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "aiSkyDomeLightShape1.ltd" ":lightList1.l" -na;
connectAttr "pPlaneShape1.iog" ":initialShadingGroup.dsm" -na;
connectAttr "aiSkyDomeLight1.iog" ":defaultLightSet.dsm" -na;
// End of test.ma
