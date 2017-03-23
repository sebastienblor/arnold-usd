//Maya ASCII 2017 scene
//Name: test.ma
//Last modified: Thu, Mar 23, 2017 01:32:15 PM
//Codeset: 1252
requires maya "2017";
requires -nodeType "hairPhysicalShader" "hairPhysicalShader" "1.0";
requires -nodeType "xgmSplineBase" -nodeType "xgmModifierSculpt" -nodeType "xgmModifierScale"
		 -nodeType "xgmSplineDescription" -dataType "xgmGuideData" -dataType "igmDescriptionData"
		 -dataType "xgmSplineData" -dataType "xgmMeshData" -dataType "xgmSplineTweakData"
		 -dataType "xgmSplineBoundInfoData" -dataType "xgmGuideRefData" "xgenToolkit" "1.0";
requires -nodeType "aiOptions" -nodeType "aiAOVDriver" -nodeType "aiAOVFilter" -nodeType "aiSkyDomeLight"
		 "mtoa" "2.0.0.0";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2017";
fileInfo "version" "2017";
fileInfo "cutIdentifier" "201606150345-997974";
fileInfo "osv" "Microsoft Windows 7 Enterprise Edition, 64-bit Windows 7 Service Pack 1 (Build 7601)\n";
createNode transform -s -n "persp";
	rename -uid "F8B02230-4CD2-F84D-3B40-A587B18E631B";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -7.2820941605995628 4.3022851785458336 1.8535876957123463 ;
	setAttr ".r" -type "double3" -29.138352729685415 300.19999999998555 0 ;
createNode camera -s -n "perspShape" -p "persp";
	rename -uid "392E93B6-4B51-007C-13B1-51B56EFAE4E5";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999993;
	setAttr ".coi" 9.3878426212645838;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".hc" -type "string" "viewSet -p %camera";
	setAttr ".ai_translator" -type "string" "perspective";
createNode transform -s -n "top";
	rename -uid "D9AC54E9-4014-DE86-E316-B09E68E9682F";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 1000.1 0 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	rename -uid "CE51A23D-4968-5C3B-7249-C4A3438427C5";
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
	rename -uid "FDDBB4A1-402F-19CB-C90E-CCA7B929D560";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 1000.1 ;
createNode camera -s -n "frontShape" -p "front";
	rename -uid "FFB027BA-42B8-E8D3-3000-76AAF25F360B";
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
	rename -uid "F2875EB3-48C5-FF45-4BAF-7F8CEF7754EF";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 1000.1 0 0 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	rename -uid "91CE2A43-4153-30FA-CE75-E8A9CC3AE864";
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
	rename -uid "891EBB18-4FB0-F218-AD75-AEB106363BC9";
	setAttr ".s" -type "double3" 4.1230457588849188 4.1230457588849188 4.1230457588849188 ;
createNode mesh -n "pPlaneShape1" -p "pPlane1";
	rename -uid "0C7EEBB8-48C4-0A1B-1A4B-C4AAD05C6323";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".ai_translator" -type "string" "polymesh";
createNode transform -n "description_hairPhysical";
	rename -uid "87C1BB8A-4359-3E0D-6829-0AAFFA18EFAB";
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
createNode xgmSplineDescription -n "description_hairPhysical_Shape" -p "description_hairPhysical";
	rename -uid "8B18AE04-4F18-E862-16C1-C4833F416B4D";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".w" 0.097744360566139221;
	setAttr -s 2 ".wdr[0:1]"  0 1 1 1 0.94 1;
createNode transform -n "aiSkyDomeLight1";
	rename -uid "7F07C0EE-43C9-9D2A-A2D8-72AE1858AE1D";
createNode aiSkyDomeLight -n "aiSkyDomeLightShape1" -p "aiSkyDomeLight1";
	rename -uid "EBB9C9EA-4B6A-297C-1FED-2C91A6560323";
	addAttr -ci true -h true -sn "aal" -ln "attributeAliasList" -dt "attributeAlias";
	setAttr -k off ".v";
	setAttr ".csh" no;
	setAttr ".rcsh" no;
	setAttr ".camera" 0;
	setAttr ".ai_exposure" 1;
	setAttr ".aal" -type "attributeAlias" {"exposure","aiExposure"} ;
createNode transform -n "test:pPlane1";
	rename -uid "18B514EE-4AA5-4605-81FA-B5AB5A9294F5";
	setAttr ".t" -type "double3" 0 0 -4.805175943127395 ;
	setAttr ".s" -type "double3" 4.1230457588849188 4.1230457588849188 4.1230457588849188 ;
createNode mesh -n "test:pPlaneShape1" -p "test:pPlane1";
	rename -uid "3BCC83AB-4BFD-78D8-0C54-138A1E7A9A78";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".ai_translator" -type "string" "polymesh";
createNode transform -n "test:description_hairPhysical";
	rename -uid "A91EF647-45A2-A097-468E-B29B72B9B296";
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
createNode xgmSplineDescription -n "test:description_hairPhysical_Shape" -p "test:description_hairPhysical";
	rename -uid "B136DC47-4BC6-F67B-CF89-48864935FF9D";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".w" 0.097744360566139221;
	setAttr -s 2 ".wdr[0:1]"  0 1 1 1 0.94 1;
createNode lightLinker -s -n "lightLinker1";
	rename -uid "2FC51FD0-4371-11D2-DB86-CB9D8C804F5D";
	setAttr -s 6 ".lnk";
	setAttr -s 6 ".slnk";
createNode shapeEditorManager -n "shapeEditorManager";
	rename -uid "560A607E-46E1-5E6B-4A4F-84BE0C5CB423";
createNode poseInterpolatorManager -n "poseInterpolatorManager";
	rename -uid "52B87662-414C-D520-DDF0-E188E36E1806";
createNode displayLayerManager -n "layerManager";
	rename -uid "B854A3A1-4544-9EF0-7EC3-7CA3051E61E7";
createNode displayLayer -n "defaultLayer";
	rename -uid "F7E425F4-4188-6365-EC31-8D8F1AC2A2B3";
createNode renderLayerManager -n "renderLayerManager";
	rename -uid "A8A2DC59-4931-881B-CB49-EE97A0BE4193";
createNode renderLayer -n "defaultRenderLayer";
	rename -uid "2E1365B0-48AC-6C66-AC43-16BEBC2FB318";
	setAttr ".g" yes;
createNode polyPlane -n "polyPlane1";
	rename -uid "BEAF2008-4B9F-7ACA-380F-AC957E9076A1";
	setAttr ".cuv" 2;
createNode xgmSplineBase -n "description1_base";
	rename -uid "CAD167C6-44F6-45C3-742D-699104404887";
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
	setAttr ".dm" 2;
	setAttr ".w" 1;
	setAttr ".csd" -type "xgmSplineData" "Header" "517" "7"
"{\"Header\":{\"Version\":1,\"Type\":\"XgSplineData\",\"GroupVersion\":1,\"GroupCount\":1"
",\"GroupBase64\":true,\"GroupDeflate\":true,\"GroupDeflateLevel\":9},\"Items\":[{\"Na"
"me\":\".boundMesh[0]\",\"Id\":\"{CE5FC223-E841-4DC2-AD29-622440BDED8E}\",\"Mode\":\"De"
"nsity\",\"PrimitiveInfos\":0,\"Positions\":1,\"PatchUVs\":2,\"MeshN\":3,\"MeshU\":4,\"Me"
"shV\":5,\"FaceUV\":6,\"FaceId\":7,\"FrozenSet\":8,\"WIDTH_CV\":9}],\"RefMeshArray\":[{\""
"Name\":\".boundMesh[0]\",\"Vertices\":10,\"UVs\":11,\"VertsPerFace\":12,\"FaceVertOffs"
"ets\":13,\"FaceUVOffsets\":14,\"TriIndices\":15}],\"CustomData\":{}}"
"Group" "24968" "329"
"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"
"u0yhHTD2mH3ylD3fBaN3Xo5iyFTLDe2Ascfsk88N3xlc7f5nFEOm2khoB4w9Zp88ct/yYNyZfLUw"
"24XpZyKhHTD2mH2y9+2cYFna51FMIEz3nNB4zwntMfvkNmsnBB125q+F2S5MtdzQDhh7zD75efe3"
"3DELjkQxgTD9bk9oB4w9Zp+8oPqK3AGtvoxiyPQ3F6EdMPaYffK08ltjS69diGLI9DsYoR0w9ph9"
"spgPf3PseBQTzp+N8b6hTDVksw559YhMyBj0XRRDpr/lCe2Ascfsk6v+PQ5e33s4iiGLvsYOGHvM"
"PvnQDQtgZ8mfoxiyuAaNHTD2mH3y7fN2Q26tw1EMmeb8oR0w9ph98qyhxyAH++Yoi/c1dkCb7ldm"
"HfJvKVeh43dHoxgyjaGhHTD2mH3y78vlcwPHNVXPQm50n1t5ZQbY2h1717tJCwta24dbb3GTGlW0"
"tn3vrwc31NicaWtPVzztvtq+Adjab0+lw5nzmzJs7ZqNt8EXzRvGbG3+zyZCgxP1wdaeanfULTa6"
"p7XdfPyCe+zxIdZ25L6+cLJIrZit7Xd/CTj2eHewtQvXlIMldYdY264pQ9y2larY22FP4flQz9qO"
"rpsCRbL6WduVGxrBDw/0traxK1Xc4fv7W9tX+qfi6462tiP21YUnBj3yH2iLw8cvLXFt7ZFbn3av"
"/7432NrmF/vD4oaFrO1PX+L35kWPWNt7CqdCp1+Xubb2yV/qQemv37W2hT9e4r7UoTPY2n2L33A7"
"1r/T2tbo1QvW+uOt7YJRg+HvdY9Y21HNx8B7aU+6trZ5m0fhTNmx1vbJk9Vh6OB7re0Dk9Lg0x9n"
"WNsiY1vCySt4blvaeS3awO/vp1nbg/UquK1PFLW2H/xQIHZs6Ruurf0qbUVs5FOrra14/71Jj1rb"
"5hVedR/yTlvbElXdmOOlgK0V502t5s/GbO3Y9+/D+34ZsLVLU6a7B+4uam1XlF3vVvg+3drmr5Pr"
"FqzcxtoWrXvM3b1nUMzWVm13xX1o4i3WtkylBZmfpxyztl0yxseeqZTm2tpPmt0CH720w9pWKdka"
"mpRYam334dj/w7oXMmxt87blYW68fszWjtxcwz2efqNra99ILggff9jd2rbtVAbSWj5nbffeWxxe"
"HFgUbG3vEhWh2OgK1vaewj2hz01Ra/uoPwTWNUuzthuuP+2O/ewRa9v1sV/ch5PutbatOpaAyitH"
"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"
"JUNdr5nqWoqx+wWo+4unzvVb1Pmsr/3W6p5zG7t22qi0VWnHzhl9fXZU6cTOzy4qXdX9qZu6L3Vn"
"96Ke6n52h7r29TXdh51v+hrT11X/f7lvDFL3ucHqvna3yl019tEd9q1K/+ZZA3nJS17ykpe85CUv"
"eclLXvKSl7zkJS95yUte8pKXvOQlL3nJy//T5P0cKy95yUte8pKXvOQlL3nJS17y8n+aa9cm/bvk"
"bfN//nv8V9rP/6+25T/6vv/Z6/1Puz7+b3y2/0rHKC9+3nH7b3r9/v8yluVd53n5/33elHfc/nN+"
"jvWa+rvXq+cKbMcsx3hzIwcC7UY9Xgk9J9KD+UCutupPjkcOuGx9YK8TOityILSK9nZ87wQGxDZo"
"i23QFttgfCBXW2yDttgGtj6w1wmdJd+DrD6z9nZcL6HWDbTFNmiLbTA+kKsttkFbbANbH9jrhM6S"
"25RQ+8Zj3o59EmrbA22xDdpiG4wP5GqLbdAW28DWB/Y6obPkPkqoY+Ux4zYcyJkj92WgLbdBWm6D"
"ttgGabkN0nIbwvWBvU5ouQ3S6tzS3q5/yiq2QVtsg7bYBuMDuc7/5n9xtU1qfWCvEzpLnUPqXPaY"
"t2P/1Lg81wJtsQ3aYhuMD+Rqq/cmx+W+0OsDe53QWfLcJatrSVucD1F17gfa6nyIqnOBmd4jqveN"
"tjof9PrAXie0Oh+i6lr3mMV1EVXXYqCtElXbwkyfLaqPlba6LvT6wF4ntLououre4zFvx3+LZsl7"
"Q6Ct+pDVZ1CmfUpWx5gcl+ecXh/Y64TOkvcIsrpnaYv7ZC11rwq01fbVUseKmY5fLX0ua6v7pF4f"
"2OuEVvfJWuo9PW19795V4N+e53v37InJtiOd7/PicVct1+c/yOURahs3bgxqfWqH33MPmN9pMK9p"
"0tEVrXhdtcxV7wlyeYRa8bpqfWrF68r1fODbaF5TRr4uLXfVZwC5PEKtfF1al1r5umI9H/hnNtso"
"X1NHvK7aJyCXRagVr6vWp1a8rlzPB74PzWfuGL6WWu6qfQxyOe0Htb171PbuUdu7R22vOSZmH3YM"
"t02/FnucAqjjBuq4gTpuoI6bGtvNMTbHpGP4WfW26deSy2XkfqD1qZX7IQJyP5hzxhzjjuG+059V"
"b5t+LdHqiNcVrXhduZ4P/Bw050zH8Fjofac/q942/VpqfYo6H0CdD+E5bc7BjuGx1cdC7zv9WfW2"
"6dcSrY54XX6NmHO6IztX/PAY62Oi96H+zHob9WvqnMvKR688QzbhMxzy8l8j+vh4hc09N596VkYJ"
"9hyOUuy5JgXUszNKsedylFHL9PM5yqpl+jkd5dQy/byO8mqZfnbHdWqZfoZHBbVMP8ujolqmn+Wh"
"n2ein+mRrJbpZ3tE1DL9jI9Kapl+1kdltUw/96OKWqaf/1FVLdPPAammlulngtyglulng1RXy/Qz"
"QmqoZ3voZ4XUVMv0M0NS1TL97JCoWqafIVJLLdPPEqmtlulnitRRy/SzRW5Uy/QzRuqqZfpZI/XU"
"Mv28kfrqWSH6uSNpapl+/kgDtUw/h6ShWqafSdJILdPPJrlZLdPPKGmslulnlTRRy/QzS5qqZfrZ"
"Jc3UMv38knT1/BH9LJMWapl+pklLtUw/2yRDLdPPOMlUy/SzTmJqmX7uiauW6eefgFqmn4XiqWX6"
"mSi3qGX6mSi3quea6OejtFbL9HNSblPL9PNSblfL9LNT2qhl+hkqbdUy/SyVdmqZHizbq2X6+Sod"
"1DL9nJWOapl+1kpn9awU/cyVLmqZfv5KV7VMP4elm1qmn8fSXS3Tz2XpoZbp57P0VMv0s1p6qWX6"
"mS13qGX62S13qmX6+S191HNY9HNc+qpl+nku/dQy/WyXu9Qy/YyX/mqZftbLALVMP/NloFqmn/8y"
"SC3Tz4IZrJbp58IMUcv0s2GGqme86OfEDFPL9PNi7lHL9HNjhqtl+vkxI9Qy/SyZkWqZfq7MKLVM"
"P19mtFqmnzVzr1qmnzlzn1p2t9qOsZgxymI7xqllw9R23K+W3aO2Y7xaNlxtxwS1bITajgfUspFq"
"Ox5Uy0ap7XhILRuttmOiWnav2raH1bK88SRvPMkbT/LGk7zxJG88+c8YT/SzLAcV+7fjSQk2npRk"
"4wp//mIpNr6UYuNLaTbOlGbjTBk23pRh401ZNu6UZeNOOTb+lGPjT3k2DpVn49B1bDy6jo1HFdi4"
"pJ83qMcoPT5dz8anJDZOJbFxKpmNV8lsvIqwcSvCxq1KbPyqxMavymwcq8zGsSpsPKvCxrOqbFyr"
"ysa1amx8q8bGtxvYOKefh6jHPD3epbDxrgYb92qwca8mG/9qsvEvlY2DqWwcjLLxMMrGw1psXKzF"
"xsXabHyszcbHOmycrMPGyRvZeHkjGy/rsnFTP69Rj6F6/LyJjZ/12Than42jaWw8TWPjaQM2rjZg"
"42pDNr42ZONrIzbONmLj7M1svL2ZjbeN2bjbmI27Tdj424SNv03ZOKyfK6nHZD0eN2fjcTobl9PZ"
"uNyCjc8t2Pjcko3TLdk4ncHG6ww2XmeycTuTjdsxNn7H2PjtsnHcZeM4sPEc2HjusXFdP/tSj/F6"
"fG/Fxvdb2Th/KxvnW7PxvjUb729j4/5tbNy/nY3/t7Pxvw2bB7Rh84C2bD7Qls0H2rF5QTs2L2jP"
"5gft2fygA5sn6Odz6jmDni90YvOFzmze0JnNG7qw+UMXNn/oyuYRXdk8ohubT3Rj84nubF7Rnc0r"
"erD5RQ82v+jJ5hk92TyjF5tv9GLzjTvYvEM/D1nPQfT8ozebf/Rh85A+bB7Sl81H+rL5SD82L+nH"
"5iV3sfnJXWx+0p/NU/qzecoANl8ZwOYrA9m8ZSCbtwxi85dBbP4ymM1j9HNP9ZxGz2fuZvOZoWxe"
"M5TNa4ax+c0wNr+5h81z7mHznOFsvjOczXdGsHnPCDbvGcnmPyPZ/GcUmweNYvOg0Ww+NJrNh+5l"
"86J7VfQcSc+PxrD50Vg2TxrL5knj2HxpHJsv3c/mTfezedN4Nn8az+ZPE9g8agKbRz3A5lMPsPnU"
"g2xe9SCbVz3E5lcPsfnVRDbPmqii51z/C7UAbq0="
;
createNode xgmModifierScale -n "scale";
	rename -uid "AA2A2014-4B57-E231-A2DE-5A9CF27B5E9F";
createNode xgmModifierSculpt -n "sculpt";
	rename -uid "B1F10D97-4A51-2840-E9DE-B585668603FD";
	setAttr ".ts[0].uio" 1;
createNode hairPhysicalShader -n "hairPhysicalShader1";
	rename -uid "0C35701E-436F-952D-2A51-7BB42413C7CA";
createNode shadingEngine -n "hairPhysicalShader1SG";
	rename -uid "542CA85C-4F66-584F-01F4-F49BF1E78A32";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
	rename -uid "B8EEC245-40FE-7C95-12A6-B5ADC6E3B7D6";
createNode lambert -n "lambert2";
	rename -uid "B239E8B7-4E43-7ABD-6C81-869DEC661AB0";
createNode shadingEngine -n "lambert2SG";
	rename -uid "DF8498AD-4B3E-5E3B-8BD3-B78C5858A5A3";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo2";
	rename -uid "400C909F-40F9-8E02-D3C9-14A9BB655DD6";
createNode file -n "file1";
	rename -uid "9A1F8B69-435A-54D7-89C0-2CB7706B3CA3";
	setAttr ".ftn" -type "string" "Jupiter_Map.jpg";
	setAttr ".cs" -type "string" "sRGB";
createNode place2dTexture -n "place2dTexture1";
	rename -uid "5BB326F6-47A6-6F5C-EA1F-5D9513746F62";
createNode script -n "uiConfigurationScriptNode";
	rename -uid "7B4CC29B-4EF8-3BC6-8111-50B5785F913C";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"top\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"smoothShaded\" \n"
		+ "                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 0\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n"
		+ "                -rendererName \"vp2Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n"
		+ "                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 1\n                -height 1\n                -sceneRenderFilter 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n"
		+ "                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n"
		+ "            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n"
		+ "            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n"
		+ "        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"side\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"smoothShaded\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 0\n                -backfaceCulling 0\n"
		+ "                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n                -rendererName \"vp2Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n"
		+ "                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n"
		+ "                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 1\n                -height 1\n                -sceneRenderFilter 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"side\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n"
		+ "            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n"
		+ "            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n"
		+ "            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"front\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"smoothShaded\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 0\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n"
		+ "                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n                -rendererName \"vp2Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n"
		+ "                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n"
		+ "                -width 1\n                -height 1\n                -sceneRenderFilter 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n"
		+ "            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n"
		+ "            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n"
		+ "            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"smoothShaded\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n"
		+ "                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 0\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 1\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n                -rendererName \"vp2Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n"
		+ "                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n"
		+ "                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 1091\n                -height 523\n                -sceneRenderFilter 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n"
		+ "\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 1\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n"
		+ "            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n"
		+ "            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1091\n            -height 523\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n"
		+ "            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"ToggledOutliner\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\"ToggledOutliner\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n                -showShapes 0\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 0\n                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 1\n                -showAssets 1\n                -showContainedOnly 1\n                -showPublishedAsConnected 0\n"
		+ "                -showContainerContents 1\n                -ignoreDagHierarchy 0\n                -expandConnections 0\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"defaultSetFilter\" \n                -showSetMembers 1\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -isSet 0\n                -isSetMember 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n"
		+ "                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 0\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                -renderFilterIndex 0\n                -selectionOrder \"chronological\" \n                -expandAttribute 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"ToggledOutliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 0\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 0\n            -showReferenceMembers 0\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n"
		+ "            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -isSet 0\n            -isSetMember 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n"
		+ "            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            -renderFilterIndex 0\n            -selectionOrder \"chronological\" \n            -expandAttribute 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n"
		+ "                -showShapes 0\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 0\n                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 1\n                -showAssets 1\n                -showContainedOnly 1\n                -showPublishedAsConnected 0\n                -showContainerContents 1\n                -ignoreDagHierarchy 0\n                -expandConnections 0\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n"
		+ "                -transmitFilters 0\n                -setFilter \"defaultSetFilter\" \n                -showSetMembers 1\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 0\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 0\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 0\n            -showReferenceMembers 0\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n"
		+ "            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"graphEditor\" -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n"
		+ "                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n"
		+ "                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 1\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -showCurveNames 0\n                -showActiveCurveNames 0\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n"
		+ "                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                -valueLinesToggle 1\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n"
		+ "                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n"
		+ "                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 1\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -showCurveNames 0\n"
		+ "                -showActiveCurveNames 0\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                -valueLinesToggle 1\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dopeSheetPanel\" -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n"
		+ "                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n"
		+ "                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n"
		+ "                -displayValues 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n"
		+ "                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n"
		+ "                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n"
		+ "                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"timeEditorPanel\" (localizedPanelLabel(\"Time Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"timeEditorPanel\" -l (localizedPanelLabel(\"Time Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Time Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"clipEditorPanel\" -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels `;\n"
		+ "\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n"
		+ "                -initialized 0\n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"sequenceEditorPanel\" -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperGraphPanel\" -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n"
		+ "            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n"
		+ "                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n"
		+ "                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"visorPanel\" -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"createNodePanel\" (localizedPanelLabel(\"Create Node\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"createNodePanel\" -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"polyTexturePlacementPanel\" -l (localizedPanelLabel(\"UV Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"renderWindowPanel\" -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"shapePanel\" (localizedPanelLabel(\"Shape Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tshapePanel -unParent -l (localizedPanelLabel(\"Shape Editor\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tshapePanel -edit -l (localizedPanelLabel(\"Shape Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"posePanel\" (localizedPanelLabel(\"Pose Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tposePanel -unParent -l (localizedPanelLabel(\"Pose Editor\")) -mbv $menusOkayInPanels ;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tposePanel -edit -l (localizedPanelLabel(\"Pose Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynRelEdPanel\" -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"relationshipPanel\" -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"referenceEditorPanel\" -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"componentEditorPanel\" -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynPaintScriptedPanelType\" -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"scriptEditorPanel\" -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"profilerPanel\" (localizedPanelLabel(\"Profiler Tool\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"profilerPanel\" -l (localizedPanelLabel(\"Profiler Tool\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Profiler Tool\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"contentBrowserPanel\" (localizedPanelLabel(\"Content Browser\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"contentBrowserPanel\" -l (localizedPanelLabel(\"Content Browser\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Content Browser\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperShadePanel\" -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"nodeEditorPanel\" (localizedPanelLabel(\"Node Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"nodeEditorPanel\" -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels `;\n"
		+ "\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -activeTab -1\n                -editorMode \"default\" \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -activeTab -1\n                -editorMode \"default\" \n"
		+ "                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-userCreated false\n\t\t\t\t-defaultImage \"vacantCell.xP:/\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 1\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 1091\\n    -height 523\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 1\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 1091\\n    -height 523\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        setFocus `paneLayout -q -p1 $gMainPane`;\n        sceneUIReplacement -deleteRemaining;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	rename -uid "1E1B28AC-45E0-A8EC-83B7-9C92AA080042";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 120 -ast 1 -aet 200 ";
	setAttr ".st" 6;
createNode file -n "file2";
	rename -uid "D7DE665D-4AAE-5C63-6FD5-6ABC66B69F41";
	setAttr ".ftn" -type "string" "Jupiter_Map.jpg";
	setAttr ".exp" 1.5;
	setAttr ".cs" -type "string" "sRGB";
createNode place2dTexture -n "place2dTexture2";
	rename -uid "8FFE3205-4118-F739-00DD-E28344A7129F";
createNode aiOptions -s -n "defaultArnoldRenderOptions";
	rename -uid "DB892BF7-4B2E-F553-C54D-A6A1E96D1AC9";
	addAttr -ci true -sn "ARV_options" -ln "ARV_options" -dt "string";
	setAttr ".AA_samples" 5;
	setAttr ".GI_diffuse_samples" 3;
	setAttr ".version" -type "string" "1.4.2.0";
	setAttr ".ARV_options" -type "string" "Display Settings=0;Show Status Bar=1;Display Pixel Information=0;3D Manipulation=0;Show AOVs list=1;Show Cameras list=1;Show RGBA icon=1;Show Crop Region icon=1;Show 3D Manipulation icon=0;Show Debug Shading icon=0;Show Exposure icon=1;Show Gamma icon=0;Darken Out-Of-Region=0;Show Render Tiles=0;AOVs=Beauty;Test Resolution=100%;Log=Last Progressive Step;Camera=perspShape;Save UI Threads=1;Debug Shading=Disabled;Color Management.Gamma=1;Color Management.Exposure=0;Background.BG=BG Color;Background.Color=0 0 0;Background.Image=;Background.Scale=1 1;Background.Offset=0 0;Background.Apply Color Management=1;Foreground.Enable FG=0;Foreground.Image=;Foreground.Scale=1 1;Foreground.Offset=0 0;Foreground.Apply Color Management=1;";
createNode aiAOVFilter -s -n "defaultArnoldFilter";
	rename -uid "CDDBEC03-4DDC-6C9B-EFF6-B492245E0D05";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVDriver -s -n "defaultArnoldDriver";
	rename -uid "3905103B-47AF-CB8E-33DD-349EC824F884";
	setAttr ".color_management" 1;
	setAttr ".ai_translator" -type "string" "tif";
createNode aiAOVDriver -s -n "defaultArnoldDisplayDriver";
	rename -uid "721DB22F-46CD-A8D8-2214-31B615515300";
	setAttr ".output_mode" 0;
	setAttr ".ai_translator" -type "string" "maya";
createNode ramp -n "ramp1";
	rename -uid "E2472249-4501-4F54-2FDA-FBA6FE9BAA18";
	setAttr -s 2 ".cel";
	setAttr ".cel[0].ep" 0.38023951649665833;
	setAttr ".cel[0].ec" -type "float3" 1 0 0 ;
	setAttr ".cel[1].ep" 1;
	setAttr ".cel[1].ec" -type "float3" 0 1 0 ;
createNode place2dTexture -n "place2dTexture3";
	rename -uid "E2A4DF0B-434E-9E89-002E-D98F173B8074";
createNode renderLayerManager -n "test:renderLayerManager";
	rename -uid "6A350DA1-408F-1048-15E8-6F95E256B949";
createNode renderLayer -n "test:defaultRenderLayer";
	rename -uid "FBD7A78E-4997-4369-24BB-7CB580C9F99E";
	setAttr ".g" yes;
createNode polyPlane -n "test:polyPlane1";
	rename -uid "D1EDE1B4-4041-CB1C-AFE1-A5A02727E08F";
	setAttr ".cuv" 2;
createNode xgmSplineBase -n "test:description1_base";
	rename -uid "B8B875BA-48CE-1BE1-738A-C583E13A247B";
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
	setAttr ".dm" 2;
	setAttr ".w" 1;
	setAttr ".csd" -type "xgmSplineData" "Header" "517" "7"
"{\"Header\":{\"Version\":1,\"Type\":\"XgSplineData\",\"GroupVersion\":1,\"GroupCount\":1"
",\"GroupBase64\":true,\"GroupDeflate\":true,\"GroupDeflateLevel\":9},\"Items\":[{\"Na"
"me\":\".boundMesh[0]\",\"Id\":\"{CE5FC223-E841-4DC2-AD29-622440BDED8E}\",\"Mode\":\"De"
"nsity\",\"PrimitiveInfos\":0,\"Positions\":1,\"PatchUVs\":2,\"MeshN\":3,\"MeshU\":4,\"Me"
"shV\":5,\"FaceUV\":6,\"FaceId\":7,\"FrozenSet\":8,\"WIDTH_CV\":9}],\"RefMeshArray\":[{\""
"Name\":\".boundMesh[0]\",\"Vertices\":10,\"UVs\":11,\"VertsPerFace\":12,\"FaceVertOffs"
"ets\":13,\"FaceUVOffsets\":14,\"TriIndices\":15}],\"CustomData\":{}}"
"Group" "29280" "386"
"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"
"nYpzJLdaiucKmLPcschff6d9d9XDOeD2PSzzdNQmy9v04Vj3/oFRVp9Cf6MtUzc6b0RP1p81bofj"
"aZsYLJ/vRU2ncj8v7t/Tfap1dvZuWP747GLOS5P1/Z02zXTGz5mur0XfUy3OQ/o7W6asHxw+1jDc"
"P032s0TtqYF1veWd1gmR2avgv/PjGWP9e+4C3J+pa22Hd9fa+hyZ6rLG/xOL+8scDHNWOKgfJzZt"
"ibPEotv6/Do30pG3dHls5/ihPSz7h67B+1gytbmlWzUVj5PknOTc5tTH8668K51JrY3nYOAli8f8"
"31im9NMhzvFBxXH/mZw5nIcTgvE4XyaFOB8v+oDHf+Td19Jzckv9vnfa7vjQSn/Pb+L0szZz+qFX"
"tjsan2vEMqx7P8VieZhi0d/zs9+P79ayF17LDf8djoD1n/A+1qmc1Xl08n0sUy2gkfN4tWQ8TseP"
"9xy77+nPxCfddloOp+r9GZ0hryWpagLO00pXn8T3+XcLlrl0pLlz8Rv9nbN81ms+v1r4Y/mPJ/+2"
"bm90UH8PaXTY0SxbC31ez9jnaPFcfydcGxtnrXG/C7atSOm6loTmMvrFkNOOBuHHcI6XylrK52+f"
"KXicfO/f+5SqXhev8dOKOc5j81bjMbOVP+v4Ym2DftQ0i9X303W8xv6pE5xj/fbh/mNTq1r9j1XB"
"49co3dO6rp3+DhD+eaTl1sHa6JGFtjleNdLf23e16WuxbkrAdi746G+tGqA/Hxd8f+KYvOQOHj+m"
"Sm3npoiG+uejebmtFfOv1D/fpW509p64Vb/Xdepaf4/yR4fkz2tNdZ7Ac50ZVtG6ecQvvMbk70ss"
"a3PP0Mf2vLnOo3H/w/J5F+V3Oo5cxjJDqqiWjOfK4P62JUY5385ur4//v7Jbc+X6F9vWuqK3teS/"
"2fE4Uo7tlmeP9XfX7ws2O9bUnoP7Q+ZanC8ah2PdVz2rO3NEZtDbNj2ftfyi4lg+Zt655QPKnA8j"
"v2LIkk7lA650LY//XoR/9BZnzmL68ytlZa/4Aec/Yl+dKVPBZ2bQCLx2myw7xuRfg2WSD4dYVvvo"
"z4sLXws6oza/wDINou87KiYcwbq+A0dY++bWn5X/ez3SejlFXx+aRX90Zrt1EI+TfibeEls6FY/z"
"bHtTZ9V8C/E4bXfu8yu5aO9uD3hnKL+zqD1o5/7/mxeAfIAn5stUt5EPQEM+gO6HfMAPzLWpbsgH"
"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"
"1RPxWU9Q9Fy/A4SCyWI="
;
createNode xgmModifierScale -n "test:scale";
	rename -uid "01CBD72A-4678-B4BE-A257-D5A896D8FF70";
createNode xgmModifierSculpt -n "test:sculpt";
	rename -uid "ECF778DD-4BD8-BFE3-682B-06AFF257A711";
	setAttr ".ts[0].uio" 1;
createNode hairPhysicalShader -n "test:hairPhysicalShader1";
	rename -uid "CD36475B-429A-D64C-181C-68BD23744464";
createNode shadingEngine -n "test:hairPhysicalShader1SG";
	rename -uid "757AE03B-4BC2-9518-40D3-388B49C8D666";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "test:materialInfo1";
	rename -uid "99C287D0-4B9F-2DB7-BC07-79B2E9B8BF8F";
createNode lambert -n "test:lambert2";
	rename -uid "F7DC29C4-4691-D910-2C63-219AFA7075D0";
createNode shadingEngine -n "test:lambert2SG";
	rename -uid "34FE0657-4631-4332-288E-A39BBB719AA6";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "test:materialInfo2";
	rename -uid "B22DAA5D-4918-3426-9FBF-EDA0B403A459";
createNode file -n "test:file1";
	rename -uid "1D9FA8B4-4D0C-C704-F780-449DC029A844";
	setAttr ".ftn" -type "string" "Jupiter_Map.jpg";
	setAttr ".cs" -type "string" "sRGB";
createNode place2dTexture -n "test:place2dTexture1";
	rename -uid "BC820E15-4701-D49A-B4FF-4DBD18D9F3B5";
createNode file -n "test:file2";
	rename -uid "B208A240-4A67-15FA-CE01-A795810DA356";
	setAttr ".ftn" -type "string" "Jupiter_Map.jpg";
	setAttr ".exp" 1.5;
	setAttr ".cs" -type "string" "sRGB";
createNode place2dTexture -n "test:place2dTexture2";
	rename -uid "C94E17B4-400C-6388-89CC-24974DEE412E";
createNode ramp -n "test:ramp1";
	rename -uid "120344D5-4332-00A9-F252-1A8BC91215F8";
	setAttr -s 2 ".cel";
	setAttr ".cel[0].ep" 0.38023951649665833;
	setAttr ".cel[0].ec" -type "float3" 1 0 0 ;
	setAttr ".cel[1].ep" 1;
	setAttr ".cel[1].ec" -type "float3" 0 1 0 ;
	setAttr ".ai_curve_implicit_uvs" no;
createNode place2dTexture -n "test:place2dTexture3";
	rename -uid "3EE86A5D-4EA3-B9D6-20C7-A68485149371";
createNode nodeGraphEditorInfo -n "test:hyperShadePrimaryNodeEditorSavedTabsInfo";
	rename -uid "1BBF7EFE-460B-AFF2-60BF-289D98896408";
	setAttr ".def" no;
	setAttr ".tgi[0].tn" -type "string" "Untitled_1";
	setAttr ".tgi[0].vl" -type "double2" -434.88907698893144 -280.48256081131836 ;
	setAttr ".tgi[0].vh" -type "double2" 943.11456511179483 926.91110655312752 ;
	setAttr -s 10 ".tgi[0].ni";
	setAttr ".tgi[0].ni[0].x" 677.42974853515625;
	setAttr ".tgi[0].ni[0].y" 799.15802001953125;
	setAttr ".tgi[0].ni[0].nvs" 1954;
	setAttr ".tgi[0].ni[1].x" -33.596965789794922;
	setAttr ".tgi[0].ni[1].y" 369.505126953125;
	setAttr ".tgi[0].ni[1].nvs" 1923;
	setAttr ".tgi[0].ni[2].x" 52.094856262207031;
	setAttr ".tgi[0].ni[2].y" -103.24290466308594;
	setAttr ".tgi[0].ni[2].nvs" 1923;
	setAttr ".tgi[0].ni[3].x" -200;
	setAttr ".tgi[0].ni[3].y" 187.14285278320312;
	setAttr ".tgi[0].ni[3].nvs" 1923;
	setAttr ".tgi[0].ni[4].x" 347.22225952148437;
	setAttr ".tgi[0].ni[4].y" 20.197969436645508;
	setAttr ".tgi[0].ni[4].nvs" 1923;
	setAttr ".tgi[0].ni[5].x" 247.65650939941406;
	setAttr ".tgi[0].ni[5].y" 515.29608154296875;
	setAttr ".tgi[0].ni[5].nvs" 1923;
	setAttr ".tgi[0].ni[6].x" 353.63821411132812;
	setAttr ".tgi[0].ni[6].y" 222.7132568359375;
	setAttr ".tgi[0].ni[6].nvs" 1923;
	setAttr ".tgi[0].ni[7].x" -122.23210906982422;
	setAttr ".tgi[0].ni[7].y" 603.70166015625;
	setAttr ".tgi[0].ni[7].nvs" 1923;
	setAttr ".tgi[0].ni[8].x" -248.57142639160156;
	setAttr ".tgi[0].ni[8].y" -192.85714721679687;
	setAttr ".tgi[0].ni[8].nvs" 1923;
	setAttr ".tgi[0].ni[9].x" -441.42855834960937;
	setAttr ".tgi[0].ni[9].y" 348.57144165039063;
	setAttr ".tgi[0].ni[9].nvs" 1923;
createNode nodeGraphEditorInfo -n "hyperShadePrimaryNodeEditorSavedTabsInfo";
	rename -uid "C39431F1-42C7-A22E-86AB-9F8041D37A86";
	setAttr ".tgi[0].tn" -type "string" "Untitled_1";
	setAttr ".tgi[0].vl" -type "double2" -524.37481289559275 -380.7860981969327 ;
	setAttr ".tgi[0].vh" -type "double2" 1082.6748705192015 1026.0241677955707 ;
	setAttr -s 10 ".tgi[0].ni";
	setAttr ".tgi[0].ni[0].x" -200;
	setAttr ".tgi[0].ni[0].y" 187.14285278320312;
	setAttr ".tgi[0].ni[0].nvs" 1923;
	setAttr ".tgi[0].ni[1].x" -33.596965789794922;
	setAttr ".tgi[0].ni[1].y" 369.505126953125;
	setAttr ".tgi[0].ni[1].nvs" 1923;
	setAttr ".tgi[0].ni[2].x" -122.23210906982422;
	setAttr ".tgi[0].ni[2].y" 603.70166015625;
	setAttr ".tgi[0].ni[2].nvs" 1923;
	setAttr ".tgi[0].ni[3].x" 677.42974853515625;
	setAttr ".tgi[0].ni[3].y" 799.15802001953125;
	setAttr ".tgi[0].ni[3].nvs" 1954;
	setAttr ".tgi[0].ni[4].x" 347.22225952148437;
	setAttr ".tgi[0].ni[4].y" 20.197969436645508;
	setAttr ".tgi[0].ni[4].nvs" 1923;
	setAttr ".tgi[0].ni[5].x" 353.63821411132812;
	setAttr ".tgi[0].ni[5].y" 222.7132568359375;
	setAttr ".tgi[0].ni[5].nvs" 1923;
	setAttr ".tgi[0].ni[6].x" 52.094856262207031;
	setAttr ".tgi[0].ni[6].y" -103.24290466308594;
	setAttr ".tgi[0].ni[6].nvs" 1923;
	setAttr ".tgi[0].ni[7].x" 247.65650939941406;
	setAttr ".tgi[0].ni[7].y" 515.29608154296875;
	setAttr ".tgi[0].ni[7].nvs" 1923;
	setAttr ".tgi[0].ni[8].x" -248.57142639160156;
	setAttr ".tgi[0].ni[8].y" -192.85714721679687;
	setAttr ".tgi[0].ni[8].nvs" 1923;
	setAttr ".tgi[0].ni[9].x" -441.42855834960937;
	setAttr ".tgi[0].ni[9].y" 348.57144165039063;
	setAttr ".tgi[0].ni[9].nvs" 1923;
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
	setAttr -s 6 ".st";
select -ne :renderGlobalsList1;
select -ne :defaultShaderList1;
	setAttr -s 8 ".s";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
	setAttr -s 6 ".u";
select -ne :defaultRenderingList1;
	setAttr -s 2 ".r";
select -ne :lightList1;
select -ne :defaultTextureList1;
	setAttr -s 6 ".tx";
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
connectAttr "sculpt.osd" "description_hairPhysical_Shape.isd";
connectAttr "sculpt.msg" "description_hairPhysical_Shape.as";
connectAttr "test:polyPlane1.out" "test:pPlaneShape1.i";
connectAttr "test:sculpt.osd" "test:description_hairPhysical_Shape.isd";
connectAttr "test:sculpt.msg" "test:description_hairPhysical_Shape.as";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "hairPhysicalShader1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "lambert2SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "test:hairPhysicalShader1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "test:lambert2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "hairPhysicalShader1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "lambert2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "test:hairPhysicalShader1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "test:lambert2SG.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr "pPlaneShape1.w" "description1_base.bm[0]";
connectAttr "description1_base.osd" "scale.isd";
connectAttr "scale.osd" "sculpt.isd";
connectAttr "ramp1.oa" "hairPhysicalShader1.iD";
connectAttr "ramp1.oc" "hairPhysicalShader1.cG";
connectAttr "ramp1.oc" "hairPhysicalShader1.cR";
connectAttr "ramp1.oc" "hairPhysicalShader1.cTRT";
connectAttr "ramp1.oc" "hairPhysicalShader1.cTT";
connectAttr "ramp1.oc" "hairPhysicalShader1.rcD";
connectAttr "ramp1.oc" "hairPhysicalShader1.tcD";
connectAttr "hairPhysicalShader1.oc" "hairPhysicalShader1SG.ss";
connectAttr "description_hairPhysical_Shape.iog" "hairPhysicalShader1SG.dsm" -na
		;
connectAttr "hairPhysicalShader1SG.msg" "materialInfo1.sg";
connectAttr "hairPhysicalShader1.msg" "materialInfo1.m";
connectAttr "hairPhysicalShader1.msg" "materialInfo1.t" -na;
connectAttr "file1.oc" "lambert2.c";
connectAttr "lambert2.oc" "lambert2SG.ss";
connectAttr "pPlaneShape1.iog" "lambert2SG.dsm" -na;
connectAttr "lambert2SG.msg" "materialInfo2.sg";
connectAttr "lambert2.msg" "materialInfo2.m";
connectAttr "file1.msg" "materialInfo2.t" -na;
connectAttr ":defaultColorMgtGlobals.cme" "file1.cme";
connectAttr ":defaultColorMgtGlobals.cfe" "file1.cmcf";
connectAttr ":defaultColorMgtGlobals.cfp" "file1.cmcp";
connectAttr ":defaultColorMgtGlobals.wsn" "file1.ws";
connectAttr "place2dTexture1.c" "file1.c";
connectAttr "place2dTexture1.tf" "file1.tf";
connectAttr "place2dTexture1.rf" "file1.rf";
connectAttr "place2dTexture1.mu" "file1.mu";
connectAttr "place2dTexture1.mv" "file1.mv";
connectAttr "place2dTexture1.s" "file1.s";
connectAttr "place2dTexture1.wu" "file1.wu";
connectAttr "place2dTexture1.wv" "file1.wv";
connectAttr "place2dTexture1.re" "file1.re";
connectAttr "place2dTexture1.of" "file1.of";
connectAttr "place2dTexture1.r" "file1.ro";
connectAttr "place2dTexture1.n" "file1.n";
connectAttr "place2dTexture1.vt1" "file1.vt1";
connectAttr "place2dTexture1.vt2" "file1.vt2";
connectAttr "place2dTexture1.vt3" "file1.vt3";
connectAttr "place2dTexture1.vc1" "file1.vc1";
connectAttr "place2dTexture1.o" "file1.uv";
connectAttr "place2dTexture1.ofs" "file1.fs";
connectAttr ":defaultColorMgtGlobals.cme" "file2.cme";
connectAttr ":defaultColorMgtGlobals.cfe" "file2.cmcf";
connectAttr ":defaultColorMgtGlobals.cfp" "file2.cmcp";
connectAttr ":defaultColorMgtGlobals.wsn" "file2.ws";
connectAttr "place2dTexture2.c" "file2.c";
connectAttr "place2dTexture2.tf" "file2.tf";
connectAttr "place2dTexture2.rf" "file2.rf";
connectAttr "place2dTexture2.mu" "file2.mu";
connectAttr "place2dTexture2.mv" "file2.mv";
connectAttr "place2dTexture2.s" "file2.s";
connectAttr "place2dTexture2.wu" "file2.wu";
connectAttr "place2dTexture2.wv" "file2.wv";
connectAttr "place2dTexture2.re" "file2.re";
connectAttr "place2dTexture2.of" "file2.of";
connectAttr "place2dTexture2.r" "file2.ro";
connectAttr "place2dTexture2.n" "file2.n";
connectAttr "place2dTexture2.vt1" "file2.vt1";
connectAttr "place2dTexture2.vt2" "file2.vt2";
connectAttr "place2dTexture2.vt3" "file2.vt3";
connectAttr "place2dTexture2.vc1" "file2.vc1";
connectAttr "place2dTexture2.o" "file2.uv";
connectAttr "place2dTexture2.ofs" "file2.fs";
connectAttr ":defaultArnoldDisplayDriver.msg" ":defaultArnoldRenderOptions.drivers"
		 -na;
connectAttr ":defaultArnoldFilter.msg" ":defaultArnoldRenderOptions.filt";
connectAttr ":defaultArnoldDriver.msg" ":defaultArnoldRenderOptions.drvr";
connectAttr "place2dTexture3.o" "ramp1.uv";
connectAttr "place2dTexture3.ofs" "ramp1.fs";
connectAttr "test:renderLayerManager.rlmi[0]" "test:defaultRenderLayer.rlid";
connectAttr "test:pPlaneShape1.w" "test:description1_base.bm[0]";
connectAttr "test:description1_base.osd" "test:scale.isd";
connectAttr "test:scale.osd" "test:sculpt.isd";
connectAttr "test:ramp1.oa" "test:hairPhysicalShader1.iD";
connectAttr "test:ramp1.oc" "test:hairPhysicalShader1.cG";
connectAttr "test:ramp1.oc" "test:hairPhysicalShader1.cR";
connectAttr "test:ramp1.oc" "test:hairPhysicalShader1.cTRT";
connectAttr "test:ramp1.oc" "test:hairPhysicalShader1.cTT";
connectAttr "test:ramp1.oc" "test:hairPhysicalShader1.rcD";
connectAttr "test:ramp1.oc" "test:hairPhysicalShader1.tcD";
connectAttr "test:hairPhysicalShader1.oc" "test:hairPhysicalShader1SG.ss";
connectAttr "test:description_hairPhysical_Shape.iog" "test:hairPhysicalShader1SG.dsm"
		 -na;
connectAttr "test:hairPhysicalShader1SG.msg" "test:materialInfo1.sg";
connectAttr "test:hairPhysicalShader1.msg" "test:materialInfo1.m";
connectAttr "test:hairPhysicalShader1.msg" "test:materialInfo1.t" -na;
connectAttr "test:file1.oc" "test:lambert2.c";
connectAttr "test:lambert2.oc" "test:lambert2SG.ss";
connectAttr "test:pPlaneShape1.iog" "test:lambert2SG.dsm" -na;
connectAttr "test:lambert2SG.msg" "test:materialInfo2.sg";
connectAttr "test:lambert2.msg" "test:materialInfo2.m";
connectAttr "test:file1.msg" "test:materialInfo2.t" -na;
connectAttr ":defaultColorMgtGlobals.cme" "test:file1.cme";
connectAttr ":defaultColorMgtGlobals.cfe" "test:file1.cmcf";
connectAttr ":defaultColorMgtGlobals.cfp" "test:file1.cmcp";
connectAttr ":defaultColorMgtGlobals.wsn" "test:file1.ws";
connectAttr "test:place2dTexture1.c" "test:file1.c";
connectAttr "test:place2dTexture1.tf" "test:file1.tf";
connectAttr "test:place2dTexture1.rf" "test:file1.rf";
connectAttr "test:place2dTexture1.mu" "test:file1.mu";
connectAttr "test:place2dTexture1.mv" "test:file1.mv";
connectAttr "test:place2dTexture1.s" "test:file1.s";
connectAttr "test:place2dTexture1.wu" "test:file1.wu";
connectAttr "test:place2dTexture1.wv" "test:file1.wv";
connectAttr "test:place2dTexture1.re" "test:file1.re";
connectAttr "test:place2dTexture1.of" "test:file1.of";
connectAttr "test:place2dTexture1.r" "test:file1.ro";
connectAttr "test:place2dTexture1.n" "test:file1.n";
connectAttr "test:place2dTexture1.vt1" "test:file1.vt1";
connectAttr "test:place2dTexture1.vt2" "test:file1.vt2";
connectAttr "test:place2dTexture1.vt3" "test:file1.vt3";
connectAttr "test:place2dTexture1.vc1" "test:file1.vc1";
connectAttr "test:place2dTexture1.o" "test:file1.uv";
connectAttr "test:place2dTexture1.ofs" "test:file1.fs";
connectAttr ":defaultColorMgtGlobals.cme" "test:file2.cme";
connectAttr ":defaultColorMgtGlobals.cfe" "test:file2.cmcf";
connectAttr ":defaultColorMgtGlobals.cfp" "test:file2.cmcp";
connectAttr ":defaultColorMgtGlobals.wsn" "test:file2.ws";
connectAttr "test:place2dTexture2.c" "test:file2.c";
connectAttr "test:place2dTexture2.tf" "test:file2.tf";
connectAttr "test:place2dTexture2.rf" "test:file2.rf";
connectAttr "test:place2dTexture2.mu" "test:file2.mu";
connectAttr "test:place2dTexture2.mv" "test:file2.mv";
connectAttr "test:place2dTexture2.s" "test:file2.s";
connectAttr "test:place2dTexture2.wu" "test:file2.wu";
connectAttr "test:place2dTexture2.wv" "test:file2.wv";
connectAttr "test:place2dTexture2.re" "test:file2.re";
connectAttr "test:place2dTexture2.of" "test:file2.of";
connectAttr "test:place2dTexture2.r" "test:file2.ro";
connectAttr "test:place2dTexture2.n" "test:file2.n";
connectAttr "test:place2dTexture2.vt1" "test:file2.vt1";
connectAttr "test:place2dTexture2.vt2" "test:file2.vt2";
connectAttr "test:place2dTexture2.vt3" "test:file2.vt3";
connectAttr "test:place2dTexture2.vc1" "test:file2.vc1";
connectAttr "test:place2dTexture2.o" "test:file2.uv";
connectAttr "test:place2dTexture2.ofs" "test:file2.fs";
connectAttr "test:place2dTexture3.o" "test:ramp1.uv";
connectAttr "test:place2dTexture3.ofs" "test:ramp1.fs";
connectAttr "test:hairPhysicalShader1.msg" "test:hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[0].dn"
		;
connectAttr "test:ramp1.msg" "test:hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[1].dn"
		;
connectAttr "test:file1.msg" "test:hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[2].dn"
		;
connectAttr "test:place2dTexture3.msg" "test:hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[3].dn"
		;
connectAttr "test:lambert2.msg" "test:hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[4].dn"
		;
connectAttr "test:hairPhysicalShader1SG.msg" "test:hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[5].dn"
		;
connectAttr "test:lambert2SG.msg" "test:hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[6].dn"
		;
connectAttr "test:file2.msg" "test:hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[7].dn"
		;
connectAttr "test:place2dTexture1.msg" "test:hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[8].dn"
		;
connectAttr "test:place2dTexture2.msg" "test:hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[9].dn"
		;
connectAttr "place2dTexture3.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[0].dn"
		;
connectAttr "ramp1.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[1].dn"
		;
connectAttr "file2.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[2].dn"
		;
connectAttr "hairPhysicalShader1.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[3].dn"
		;
connectAttr "lambert2.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[4].dn"
		;
connectAttr "lambert2SG.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[5].dn"
		;
connectAttr "file1.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[6].dn"
		;
connectAttr "hairPhysicalShader1SG.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[7].dn"
		;
connectAttr "place2dTexture1.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[8].dn"
		;
connectAttr "place2dTexture2.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[9].dn"
		;
connectAttr "hairPhysicalShader1SG.pa" ":renderPartition.st" -na;
connectAttr "lambert2SG.pa" ":renderPartition.st" -na;
connectAttr "test:hairPhysicalShader1SG.pa" ":renderPartition.st" -na;
connectAttr "test:lambert2SG.pa" ":renderPartition.st" -na;
connectAttr "hairPhysicalShader1.msg" ":defaultShaderList1.s" -na;
connectAttr "lambert2.msg" ":defaultShaderList1.s" -na;
connectAttr "test:hairPhysicalShader1.msg" ":defaultShaderList1.s" -na;
connectAttr "test:lambert2.msg" ":defaultShaderList1.s" -na;
connectAttr "place2dTexture1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture2.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture3.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "test:place2dTexture1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "test:place2dTexture2.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "test:place2dTexture3.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "test:defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "aiSkyDomeLightShape1.ltd" ":lightList1.l" -na;
connectAttr "file1.msg" ":defaultTextureList1.tx" -na;
connectAttr "file2.msg" ":defaultTextureList1.tx" -na;
connectAttr "ramp1.msg" ":defaultTextureList1.tx" -na;
connectAttr "test:file1.msg" ":defaultTextureList1.tx" -na;
connectAttr "test:file2.msg" ":defaultTextureList1.tx" -na;
connectAttr "test:ramp1.msg" ":defaultTextureList1.tx" -na;
connectAttr "aiSkyDomeLight1.iog" ":defaultLightSet.dsm" -na;
// End of test.ma
