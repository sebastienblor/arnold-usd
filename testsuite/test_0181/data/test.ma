//Maya ASCII 2017 scene
//Name: test.ma
//Last modified: Wed, Mar 22, 2017 02:45:25 PM
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
	setAttr ".t" -type "double3" 5.4291247529456585 4.3790234560731172 0.06155960210632927 ;
	setAttr ".r" -type "double3" -38.738352729605232 90.599999999992448 0 ;
createNode camera -s -n "perspShape" -p "persp";
	rename -uid "392E93B6-4B51-007C-13B1-51B56EFAE4E5";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999993;
	setAttr ".coi" 7.7582072296005595;
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
	setAttr ".aal" -type "attributeAlias" {"exposure","aiExposure"} ;
createNode lightLinker -s -n "lightLinker1";
	rename -uid "64AEF749-4A9B-0058-9D14-2DA4084D0A07";
	setAttr -s 4 ".lnk";
	setAttr -s 4 ".slnk";
createNode shapeEditorManager -n "shapeEditorManager";
	rename -uid "6685BBBD-4ABA-54CB-5BD5-1AA0405CD57B";
createNode poseInterpolatorManager -n "poseInterpolatorManager";
	rename -uid "D0D01D8A-46CF-4DCA-77A9-B09559D7F9FD";
createNode displayLayerManager -n "layerManager";
	rename -uid "21944396-4434-66AF-8B4A-EBA885ABAF50";
createNode displayLayer -n "defaultLayer";
	rename -uid "F7E425F4-4188-6365-EC31-8D8F1AC2A2B3";
createNode renderLayerManager -n "renderLayerManager";
	rename -uid "E45E046E-4FCB-80BE-5D1F-00B20874468C";
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
"trbN8LjEv3rE2hbD+9sTOK7b2jpuWxDzHVtbeGcDd2yPOta29MQv3KlNvnRt7V3Df3LnLD5nbcX5"
"Oqd9E7C1m2qWgYObXWvbZGERGFL7sGtrNyVXgA0NL1jbpm1qAIz81tqez0mDo0v/sLYHMyZCu6Hp"
"YGs/+6YntHyvn7Wdj+P13PY9rW3BCr3hwSlfuLY2PXmF+0l6KtjahfftdNfNqm9t393wYuZ9V/JZ"
"2/YXh8eeLF3e2jZv2x3i8QGutS3YD+rOmWJtv//x+0zn3uZgaze2GBRrX7+NtX2ycP3Y1e0HXVs7"
"psyM2PQmf1rbLiPGuf0fKG9tkx+f7k4pn25t7yv9sDt05clMW7uy7Ay3+KLVMVsr5h1vd0pxbe3T"
"b7zrTrxYzNp2gebwSXoTsLULR92C94XW1nb5+nKw8ZaotR3d6Ae3UVoL19auLHve/WL0pZitFd8H"
"+txUCGyt+P6TfOk6a1uwQlGo2rqGa2vT29SDIjPSwdZ2W38f3t82uba26/qq8HKHamBrq06qC7VO"
"1ra2b9YEyKheyNqOevQ2uHlCBWtbwDkT2z5ztLVddjTdHZx0v7X97NsMGHrPLzFb+/RJD8T5YGu7"
"4vWZ1vJHa5uydaSb1OhBsLXFpsxx77tyv7WdsmU0vOF3t7bDCz+A338GWNuDD16MiXHd1hZuVN8d"
"+fRpa9vEezkz7c27rK34npR+36PWtuuw79xXDgy2tqsmnnVn75pubT9/YIB7d+351rbj2qfd24pt"
"tbbrr1/lbrtttLXtsPZd9+HMadZWvP/gH7pY21arXnB/avq4tR139rBbKu0ta3sjjkkfHthpbfd/"
"OxoaTygItraO+zCI74+2dvCWBZmdD3e2tptvmRm7Z9md1rb6ij9jA+6ZmmlrD1xp4a56smXM1lab"
"1Bfvb23A1r5zdgCI77e29pk3XnV7zXnFtbXV2m1zJ+9aZ22LfLzE7Zc8Gmzt28c3ubefeNjaVsXP"
"03H3G66t3T/miHt5bXGwtYm/f4sVqJrj2tr7Sqe7c/uusbY7v7gfzqV9aW0LVngYJk65aG0/bdYN"
"fi9wIGZrn/qlH5z2K7i2dn6L5lAp2Gdtz6a0gjqfnLe2y9enwLivL8RsbTeoDfPbNXdt7VuTi9J9"
"wda2nNnSnbJrhrUV9flDzW8BW1vw5O9u40Utre1d94+GXr0uxWxtv/sfhdveu961teLzrPFbga09"
"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"
"Gep6zVTXUozdL0DdXzx1rt+izmd97bdW95zb2LXTRqWtSjt2zujrs6NKJ3Z+dlHpqu5P3dR9qTu7"
"F/VU97M71LWvr+k+7HzT15i+rvr/y31jkLrPDVb3tbtV7qqxj+6wb1f6N88ayEte8pKXvOQlL3nJ"
"S17ykpe85CUveclLXvKSl7zkJS95yUte/p8m7+dYeclLXvKSl7zkJS95yUte8pKX/9Ncuzbp3yVv"
"m//z3+O/0n7+f7Ut/9H3/c9e73/a9fF/47P9VzpGefHzjtt/0+v3/5exLO86z8v/7/OmvOP2n/Nz"
"rNfV371ePVdgO2YZxpsbORBoN+rxSug5kR7MB3K1VX9yPHLAZesDe53QWZEDoVW0t+N7JzAgtkFb"
"bIO22AbjA7naYhu0xTaw9YG9Tugs+R5k9Zm1t+N6CbVuoC22QVtsg/GBXG2xDdpiG9j6wF4ndJbc"
"poTaNx7zduyTUNseaItt0BbbYHwgV1tsg7bYBrY+sNcJnSX3UUIdK48Zt+FAzhy5LwNtuQ3Schu0"
"xTZIy22QltsQrg/sdULLbZBW55b2dv1TVrEN2mIbtMU2GB/Idf43/4urbVLrA3ud0FnqHFLnsse8"
"HfunxuW5FmiLbdAW22B8IFdbvTc5LveFXh/Y64TOkucuWV1L2uJ8iKpzP9BW50NUnQvM9B5RvW+0"
"1fmg1wf2OqHV+RBV17rHLK6LqLoWA22VqNoWZvpsUX2stNV1odcH9jqh1XURVfcej3k7/ls0S94b"
"Am3Vh6w+gzLtU7I6xuS4POf0+sBeJ3SWvEeQ1T1LW9wna6l7VaCttq+WOlbMdPxq6XNZW90n9frA"
"Xie0uk/WUu/paet7964C//Y837tnT0y2Hel8nxePu2q5Pv9BLo9Q27hxY1DrUzv8nnvA/E6DeU2T"
"jq5oxeuqZa56T5DLI9SK11XrUyteV67nA99G85oy8nVpuas+A8jlEWrl69K61MrXFev5wD+z2Ub5"
"mjriddU+AbksQq14XbU+teJ15Xo+8H1oPnPH8LXUclftY5DLaT+o7d2jtneP2t49anvNMTH7sGO4"
"bfq12OMUQB03UMcN1HEDddzU2G6OsTkmHcPPqrdNv5ZcLiP3A61PrdwPEZD7wZwz5hh3DPed/qx6"
"2/RriVZHvK5oxevK9Xzg56A5ZzqGx0LvO/1Z9bbp11LrU9T5AOp8CM9pcw52DI+tPhZ63+nPqrdN"
"v5ZodcTr8mvEnNMd2bnih8dYHxO9D/Vn1tuoX1PnXFY+euXpsgmf4ZCX/xrRx8crbO65+dSzMkqw"
"53CUYs81KaCenVGKPZejjFqmn89RVi3Tz+kop5bp53WUV8v0szuuU8v0MzwqqGX6WR4V1TL9LA/9"
"PBP9TI9ktUw/2yOilulnfFRSy/SzPiqrZfq5H1XUMv38j6pqmX4OSDW1TD8T5Aa1TD8bpLpapp8R"
"UkM920M/K6SmWqafGZKqlulnh0TVMv0MkVpqmX6WSG21TD9TpI5app8tcqNapp8xUlct088aqaeW"
"6eeN1FfPCtHPHUlTy/TzRxqoZfo5JA3VMv1MkkZqmX42yc1qmX5GSWO1TD+rpIlapp9Z0lQt088u"
"aaaW6eeXpKvnj+hnmbRQy/QzTVqqZfrZJhlqmX7GSaZapp91ElPL9HNPXLVMP/8E1DL9LBRPLdPP"
"RLlFLdPPRLlVPddEPx+ltVqmn5Nym1qmn5dyu1qmn53SRi3Tz1Bpq5bpZ6m0U8v0YNleLdPPV+mg"
"lunnrHRUy/SzVjqrZ6XoZ650Ucv081e6qmX6OSzd1DL9PJbuapl+LksPtUw/n6WnWqaf1dJLLdPP"
"bLlDLdPPbrlTLdPPb+mjnsOin+PSVy3Tz3Ppp5bpZ7vcpZbpZ7z0V8v0s14GqGX6mS8D1TL9/JdB"
"apl+FsxgtUw/F2aIWqafDTNUPeNFPydmmFqmnxdzj1qmnxszXC3Tz48ZoZbpZ8mMVMv0c2VGqWX6"
"+TKj1TL9rJl71TL9zJn71LK71XaMxYxRFtsxTi0bprbjfrXsHrUd49Wy4Wo7JqhlI9R2PKCWjVTb"
"8aBaNkptx0Nq2Wi1HRPVsnvVtj2sluWNJ3njSd54kjee5I0neePJf8Z4op9lOajYvx1PSrDxpCQb"
"V/jzF0ux8aUUG19Ks3GmNBtnyrDxpgwbb8qycacsG3fKsfGnHBt/yrNxqDwbh65j49F1bDyqwMYl"
"/bxBPUbp8el6Nj4lsXEqiY1TyWy8SmbjVYSNWxE2blVi41clNn5VZuNYZTaOVWHjWRU2nlVl41pV"
"Nq5VY+NbNTa+3cDGOf08RD3m6fEuhY13Ndi4V4ONezXZ+FeTjX+pbBxMZeNglI2HUTYe1mLjYi02"
"LtZm42NtNj7WYeNkHTZO3sjGyxvZeFmXjZv6eY16DNXj501s/KzPxtH6bBxNY+NpGhtPG7BxtQEb"
"Vxuy8bUhG18bsXG2ERtnb2bj7c1svG3Mxt3GbNxtwsbfJmz8bcrGYf1cST0m6/G4ORuP09m4nM7G"
"5RZsfG7BxueWbJxuycbpDDZeZ7DxOpON25ls3I6x8TvGxm+XjeMuG8eBjefAxnOPjev62Zd6jNfj"
"eys2vt/Kxvlb2Tjfmo33rdl4fxsb929j4/7tbPy/nY3/bdg8oA2bB7Rl84G2bD7Qjs0L2rF5QXs2"
"P2jP5gcd2DxBP59Tzxn0fKETmy90ZvOGzmze0IXNH7qw+UNXNo/oyuYR3dh8ohubT3Rn84rubF7R"
"g80verD5RU82z+jJ5hm92HyjF5tv3MHmHfp5yHoOoucfvdn8ow+bh/Rh85C+bD7Sl81H+rF5ST82"
"L7mLzU/uYvOT/mye0p/NUwaw+coANl8ZyOYtA9m8ZRCbvwxi85fBbB6jn3uq5zR6PnM3m88MZfOa"
"oWxeM4zNb4ax+c09bJ5zD5vnDGfzneFsvjOCzXtGsHnPSDb/GcnmP6PYPGgUmweNZvOh0Ww+dC+b"
"F92roudIen40hs2PxrJ50lg2TxrH5kvj2HzpfjZvup/Nm8az+dN4Nn+awOZRE9g86gE2n3qAzace"
"ZPOqB9m86iE2v3qIza8msnnWRBU95/pfHBFuDg=="
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
		+ "                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 1006\n                -height 537\n                -sceneRenderFilter 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n"
		+ "\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 1\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n"
		+ "            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n"
		+ "            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1006\n            -height 537\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n"
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
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 1\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 1006\\n    -height 537\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 1\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 1006\\n    -height 537\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
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
createNode nodeGraphEditorInfo -n "hyperShadePrimaryNodeEditorSavedTabsInfo";
	rename -uid "82B5B2A9-458D-27B5-11C2-11AFB607042F";
	setAttr ".tgi[0].tn" -type "string" "Untitled_1";
	setAttr ".tgi[0].vl" -type "double2" -258.83416178332197 -187.04444795308666 ;
	setAttr ".tgi[0].vh" -type "double2" 458.22880669248156 544.49135107910922 ;
	setAttr -s 8 ".tgi[0].ni";
	setAttr ".tgi[0].ni[0].x" 247.65650939941406;
	setAttr ".tgi[0].ni[0].y" 515.29608154296875;
	setAttr ".tgi[0].ni[0].nvs" 1923;
	setAttr ".tgi[0].ni[1].x" 284.34307861328125;
	setAttr ".tgi[0].ni[1].y" 1045.8389892578125;
	setAttr ".tgi[0].ni[1].nvs" 1954;
	setAttr ".tgi[0].ni[2].x" 347.22225952148437;
	setAttr ".tgi[0].ni[2].y" 20.197969436645508;
	setAttr ".tgi[0].ni[2].nvs" 1923;
	setAttr ".tgi[0].ni[3].x" 353.63821411132812;
	setAttr ".tgi[0].ni[3].y" 222.7132568359375;
	setAttr ".tgi[0].ni[3].nvs" 1923;
	setAttr ".tgi[0].ni[4].x" -441.42855834960937;
	setAttr ".tgi[0].ni[4].y" 348.57144165039063;
	setAttr ".tgi[0].ni[4].nvs" 1923;
	setAttr ".tgi[0].ni[5].x" -176.65150451660156;
	setAttr ".tgi[0].ni[5].y" 446.07305908203125;
	setAttr ".tgi[0].ni[5].nvs" 1923;
	setAttr ".tgi[0].ni[6].x" -248.57142639160156;
	setAttr ".tgi[0].ni[6].y" -192.85714721679687;
	setAttr ".tgi[0].ni[6].nvs" 1923;
	setAttr ".tgi[0].ni[7].x" 52.094856262207031;
	setAttr ".tgi[0].ni[7].y" -103.24290466308594;
	setAttr ".tgi[0].ni[7].nvs" 1923;
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
	setAttr -s 4 ".st";
select -ne :renderGlobalsList1;
select -ne :defaultShaderList1;
	setAttr -s 6 ".s";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
	setAttr -s 2 ".u";
select -ne :defaultRenderingList1;
select -ne :lightList1;
select -ne :defaultTextureList1;
	setAttr -s 2 ".tx";
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
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "hairPhysicalShader1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "lambert2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "hairPhysicalShader1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "lambert2SG.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr "pPlaneShape1.w" "description1_base.bm[0]";
connectAttr "description1_base.osd" "scale.isd";
connectAttr "scale.osd" "sculpt.isd";
connectAttr "file2.oa" "hairPhysicalShader1.iD";
connectAttr "file2.oc" "hairPhysicalShader1.cG";
connectAttr "file2.oc" "hairPhysicalShader1.cR";
connectAttr "file2.oc" "hairPhysicalShader1.cTRT";
connectAttr "file2.oc" "hairPhysicalShader1.cTT";
connectAttr "file2.oc" "hairPhysicalShader1.rcD";
connectAttr "file2.oc" "hairPhysicalShader1.tcD";
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
connectAttr "hairPhysicalShader1SG.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[0].dn"
		;
connectAttr "hairPhysicalShader1.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[1].dn"
		;
connectAttr "lambert2.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[2].dn"
		;
connectAttr "lambert2SG.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[3].dn"
		;
connectAttr "place2dTexture2.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[4].dn"
		;
connectAttr "file2.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[5].dn"
		;
connectAttr "place2dTexture1.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[6].dn"
		;
connectAttr "file1.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[7].dn"
		;
connectAttr ":defaultArnoldDisplayDriver.msg" ":defaultArnoldRenderOptions.drivers"
		 -na;
connectAttr ":defaultArnoldFilter.msg" ":defaultArnoldRenderOptions.filt";
connectAttr ":defaultArnoldDriver.msg" ":defaultArnoldRenderOptions.drvr";
connectAttr "hairPhysicalShader1SG.pa" ":renderPartition.st" -na;
connectAttr "lambert2SG.pa" ":renderPartition.st" -na;
connectAttr "hairPhysicalShader1.msg" ":defaultShaderList1.s" -na;
connectAttr "lambert2.msg" ":defaultShaderList1.s" -na;
connectAttr "place2dTexture1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture2.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "aiSkyDomeLightShape1.ltd" ":lightList1.l" -na;
connectAttr "file1.msg" ":defaultTextureList1.tx" -na;
connectAttr "file2.msg" ":defaultTextureList1.tx" -na;
connectAttr "aiSkyDomeLight1.iog" ":defaultLightSet.dsm" -na;
// End of test.ma
