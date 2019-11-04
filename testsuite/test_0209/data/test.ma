//Maya ASCII 2017ff04 scene
//Name: test.ma
//Last modified: Fri, May 26, 2017 12:53:52 PM
//Codeset: 1252
requires maya "2017ff04";
requires -nodeType "hairPhysicalShader" "hairPhysicalShader" "1.0";
requires -nodeType "aiOptions" -nodeType "aiAOVDriver" -nodeType "aiAOVFilter" -nodeType "aiSkyDomeLight"
		 "mtoa" "2.0.1.0_wip";
requires -nodeType "xgmPalette" -nodeType "xgmDescription" -nodeType "xgmMakeGuide"
		 -nodeType "xgmSubdPatch" -nodeType "xgmSplineGuide" -dataType "xgmGuideData" -dataType "igmDescriptionData"
		 -dataType "xgmSplineData" -dataType "xgmMeshData" -dataType "xgmSplineTweakData"
		 -dataType "xgmSplineBoundInfoData" -dataType "xgmGuideRefData" "xgenToolkit" "1.0";
requires "stereoCamera" "10.0";
requires "stereoCamera" "10.0";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2017";
fileInfo "version" "2017";
fileInfo "cutIdentifier" "201701201345-1014054";
fileInfo "osv" "Microsoft Windows 7 Enterprise Edition, 64-bit Windows 7 Service Pack 1 (Build 7601)\n";
createNode transform -s -n "persp";
	rename -uid "473D8CC0-4CCF-AC0B-4137-709960174935";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -5.4903112020855991 6.8877757177012766 5.5478196414656944 ;
	setAttr -av ".tx";
	setAttr -av ".ty";
	setAttr -av ".tz";
	setAttr ".r" -type "double3" -47.254549267023201 1025.6841975479583 0 ;
	setAttr -av ".rx";
	setAttr -av ".ry";
	setAttr -av ".rz";
createNode camera -s -n "perspShape" -p "persp";
	rename -uid "60D4B1A0-45AC-692E-94F9-D6BE4DB09A23";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999993;
	setAttr ".coi" 9.5548210115839751;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".hc" -type "string" "viewSet -p %camera";
	setAttr ".ai_translator" -type "string" "perspective";
createNode transform -s -n "top";
	rename -uid "1A3495C8-4064-4D0F-DD00-72B35FA9A4E8";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 1000.1 0 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	rename -uid "2E87C53D-43B9-893B-99F1-0CBCEEFC8661";
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
	rename -uid "B09C05FA-4289-1529-3D19-7F8A080D0E23";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 1000.1 ;
createNode camera -s -n "frontShape" -p "front";
	rename -uid "13B88D34-4880-1DD6-A245-94B4D96D3CB2";
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
	rename -uid "1F58FF19-4602-5A05-8CEA-0FB09B0DC362";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 1000.1 0 0 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	rename -uid "D9DED69E-48B7-7800-8F6E-D28038FAA9F6";
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
	rename -uid "6FDCD42D-4F37-5CDD-1774-B1A81759530F";
	setAttr ".rp" -type "double3" 0 0.05335741613676924 0 ;
	setAttr ".sp" -type "double3" 0 0.05335741613676924 0 ;
createNode mesh -n "pPlaneShape1" -p "pPlane1";
	rename -uid "7FFBCC77-4EBA-3B04-41A7-588B1F6EA956";
	addAttr -ci true -sn "Git_Interptest_mask" -ln "Git_Interptest_mask" -at "float";
	addAttr -ci true -sn "xgen_Pref" -ln "xgen_Pref" -dt "vectorArray";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.25 0.75 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".ovb" 2;
	setAttr ".dr" 3;
	setAttr ".dsm" 2;
	setAttr ".ai_translator" -type "string" "polymesh";
	setAttr ".xgen_Pref" -type "vectorArray" 9 -1.7000910043716431 0.053357414901256561
		 1.7000910043716431 0 0.053357414901256561 1.7000910043716431 1.7000910043716431 0.053357414901256561
		 1.7000910043716431 -1.7000910043716431 0.053357414901256561 0 0 0.053357414901256561
		 0 1.7000910043716431 0.053357414901256561 0 -1.7000910043716431 0.053357414901256561
		 -1.7000910043716431 0 0.053357414901256561 -1.7000910043716431 1.7000910043716431
		 0.053357414901256561 -1.7000910043716431 ;
createNode xgmPalette -n "Git";
	rename -uid "5656D33D-4DB6-76EB-38BA-24A5CA850405";
	addAttr -ci true -sn "xgVersion" -ln "xgVersion" -dt "string";
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
	setAttr ".xbf" -type "string" "";
	setAttr ".xfn" -type "string" "test__Git.xgen";
	setAttr ".xgVersion" -type "string" "D:/DailyCut/2016-06-15-03-45.2017gold/runTime/plug-ins/xgen/";
createNode transform -n "Interptest" -p "Git";
	rename -uid "2F77D77C-4A9F-53AE-4231-27A681E2AFDC";
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
createNode xgmDescription -n "InterptestShape" -p "Interptest";
	rename -uid "8DE4F573-4805-D801-6F24-2DB8B9816B4E";
	setAttr -k off ".v";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".motion_blur_mode" 1;
	setAttr ".motion_blur_steps" 2;
	setAttr ".motion_blur_factor" 0.5;
createNode transform -n "pPlane1_Interptest" -p "Interptest";
	rename -uid "F2B467D0-48B4-2EA3-7A70-C9934E83BA1C";
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
createNode xgmSubdPatch -n "pPlane1_InterptestShape" -p "pPlane1_Interptest";
	rename -uid "ACCCB212-4FE3-3FCB-EF80-3AB884C068C7";
	setAttr -k off ".v";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".bb1" -type "float3" -1.700091 0.053357415 -1.700091 ;
	setAttr ".bb2" -type "float3" 1.700091 0.053357415 1.700091 ;
createNode transform -n "xgGuide25" -p "pPlane1_Interptest";
	rename -uid "563DAA0A-4889-E851-0F2B-739B9C6D9083";
	setAttr ".rp" -type "double3" -1.3268228769302368 0.053357414901256561 1.3268228769302368 ;
	setAttr ".sp" -type "double3" -1.3268228769302368 0.053357414901256561 1.3268228769302368 ;
	setAttr ".mntl" -type "double3" 0 0 0 ;
	setAttr ".mxtl" -type "double3" 0 0 0 ;
	setAttr ".mtxe" yes;
	setAttr ".mtye" yes;
	setAttr ".mtze" yes;
	setAttr ".xtxe" yes;
	setAttr ".xtye" yes;
	setAttr ".xtze" yes;
createNode xgmSplineGuide -n "xgGuide25Shape" -p "xgGuide25";
	rename -uid "7D29763A-4224-21D8-E9C4-229CDD0FB8D1";
	setAttr -k off ".v";
	setAttr ".tw" yes;
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -l on ".ul" 0.15264999866485596;
	setAttr -l on ".vl" 0.15264999866485596;
	setAttr -l on ".fid";
	setAttr ".bb1" -type "float3" -1.5112573 0.053257413 1.3267229 ;
	setAttr ".bb2" -type "float3" -1.2302698 0.84119177 1.7181826 ;
	setAttr ".xin" -type "string" "1.6249791972211929e+000:1.6249691972211928e+000:8.5755135138770344e-001:1.5868252528046707e+000:2.9884201348546853e+000:1.2143053050001313e+000:3.9706613444937822e+000";
	setAttr ".tpr" 0.89999997615814209;
	setAttr ".wid" 0.039999999105930328;
	setAttr ".dwi" yes;
	setAttr ".fcm" yes;
createNode transform -n "xgGuide26" -p "pPlane1_Interptest";
	rename -uid "D834CAA2-42EA-9DA1-1765-79A4F5E271F8";
	setAttr ".rp" -type "double3" 1.4402415752410889 0.053357414901256561 -1.2131211757659912 ;
	setAttr ".sp" -type "double3" 1.4402415752410889 0.053357414901256561 -1.2131211757659912 ;
	setAttr ".mntl" -type "double3" 0 0 0 ;
	setAttr ".mxtl" -type "double3" 0 0 0 ;
	setAttr ".mtxe" yes;
	setAttr ".mtye" yes;
	setAttr ".mtze" yes;
	setAttr ".xtxe" yes;
	setAttr ".xtye" yes;
	setAttr ".xtze" yes;
createNode xgmSplineGuide -n "xgGuide26Shape" -p "xgGuide26";
	rename -uid "D5D10E33-4816-8260-D766-A48A205F4012";
	setAttr -k off ".v";
	setAttr ".tw" yes;
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -l on ".ul" 0.90995299816131592;
	setAttr -l on ".vl" 0.77635997533798218;
	setAttr -l on ".fid" 3;
	setAttr ".bb1" -type "float3" 1.4007779 0.053257413 -1.5344924 ;
	setAttr ".bb2" -type "float3" 1.6173936 0.61066979 -1.2130212 ;
	setAttr ".xin" -type "string" "2.7020361833950832e+000:2.7020261833950832e+000:1.0218541881060459e+000:1.8746915919589620e+000:1.7124294662507022e+000:1.4679395225431384e+000:2.0466595530732219e+000";
	setAttr ".tpr" 0.89999997615814209;
	setAttr ".wid" 0.039999999105930328;
	setAttr ".dwi" yes;
	setAttr ".fcm" yes;
createNode transform -n "xgGuide27" -p "pPlane1_Interptest";
	rename -uid "A819C801-497D-F02C-3D04-1CBF104B2536";
	setAttr ".rp" -type "double3" -0.0070655629970133305 0.053357407450675964 -1.4584329128265381 ;
	setAttr ".sp" -type "double3" -0.0070655629970133305 0.053357407450675964 -1.4584329128265381 ;
	setAttr ".mntl" -type "double3" 0 0 0 ;
	setAttr ".mxtl" -type "double3" 0 0 0 ;
	setAttr ".mtxe" yes;
	setAttr ".mtye" yes;
	setAttr ".mtze" yes;
	setAttr ".xtxe" yes;
	setAttr ".xtye" yes;
	setAttr ".xtze" yes;
createNode xgmSplineGuide -n "xgGuide27Shape" -p "xgGuide27";
	rename -uid "5F222D94-4F99-1ED2-9187-D68F6797CD72";
	setAttr -k off ".v";
	setAttr ".tw" yes;
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -l on ".ul" 0.99584400653839111;
	setAttr -l on ".vl" 0.85785597562789917;
	setAttr -l on ".fid" 2;
	setAttr ".bb1" -type "float3" -0.0071655628 0.053257406 -1.8434926 ;
	setAttr ".bb2" -type "float3" 0.35145864 0.62416553 -1.3532653 ;
	setAttr ".xin" -type "string" "1.4679495225431385e+000:1.4679395225431384e+000:2.7927383942248585e-002:1.3307734305754353e+000:1.0038914394419585e+000:1.3044639247381742e+000:1.9882270122346841e+000";
	setAttr ".tpr" 0.89999997615814209;
	setAttr ".wid" 0.039999999105930328;
	setAttr ".dwi" yes;
	setAttr ".fcm" yes;
createNode transform -n "xgGuide28" -p "pPlane1_Interptest";
	rename -uid "B2A49093-4E99-7856-3D3E-D79720794144";
	setAttr ".rp" -type "double3" -0.090077593922615051 0.05335741862654686 -0.13024108111858368 ;
	setAttr ".sp" -type "double3" -0.090077593922615051 0.05335741862654686 -0.13024108111858368 ;
	setAttr ".mntl" -type "double3" 0 0 0 ;
	setAttr ".mxtl" -type "double3" 0 0 0 ;
	setAttr ".mtxe" yes;
	setAttr ".mtye" yes;
	setAttr ".mtze" yes;
	setAttr ".xtxe" yes;
	setAttr ".xtye" yes;
	setAttr ".xtze" yes;
createNode xgmSplineGuide -n "xgGuide28Shape" -p "xgGuide28";
	rename -uid "0729FD5F-446B-482F-A814-CEB50A5A1CD6";
	setAttr -k off ".v";
	setAttr ".tw" yes;
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -l on ".ul" 0.94701600074768066;
	setAttr -l on ".vl" 0.07660830020904541;
	setAttr -l on ".fid" 2;
	setAttr ".bb1" -type "float3" -0.090177596 0.053257417 -0.18540175 ;
	setAttr ".bb2" -type "float3" 0.27986056 0.76095116 -0.053639095 ;
	setAttr ".xin" -type "string" "2.0440760024340241e+000:2.0440660024340240e+000:6.1733597644235916e-001:1.2497120126267598e+000:1.0010438024229993e+000:1.4126478360868331e+000:2.0072830171941387e+000:1.6974429736301382e+000:2.4887068342239367e+000:1.3307734305754353e+000:3.0038910707775464e+000:1.8746915919589620e+000:3.6663460397484773e+000";
	setAttr ".tpr" 0.89999997615814209;
	setAttr ".wid" 0.039999999105930328;
	setAttr ".dwi" yes;
	setAttr ".fcm" yes;
createNode transform -n "xgGuide29" -p "pPlane1_Interptest";
	rename -uid "8556271A-4315-67E8-E23F-E88644150762";
	setAttr ".rp" -type "double3" 1.1743848323822021 0.053357414901256561 1.4758042097091675 ;
	setAttr ".sp" -type "double3" 1.1743848323822021 0.053357414901256561 1.4758042097091675 ;
	setAttr ".mntl" -type "double3" 0 0 0 ;
	setAttr ".mxtl" -type "double3" 0 0 0 ;
	setAttr ".mtxe" yes;
	setAttr ".mtye" yes;
	setAttr ".mtze" yes;
	setAttr ".xtxe" yes;
	setAttr ".xtye" yes;
	setAttr ".xtze" yes;
createNode xgmSplineGuide -n "xgGuide29Shape" -p "xgGuide29";
	rename -uid "187A3AAE-4109-04D7-3B87-A1A89BE20EAA";
	setAttr -k off ".v";
	setAttr ".tw" yes;
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -l on ".ul" 0.75026398897171021;
	setAttr -l on ".vl" 0.072440102696418762;
	setAttr -l on ".fid" 1;
	setAttr ".bb1" -type "float3" 1.1742848 0.053257413 0.99225724 ;
	setAttr ".bb2" -type "float3" 1.9071113 1.7195938 1.4759042 ;
	setAttr ".xin" -type "string" "2.7020361833950832e+000:1.1922692668957731e+000:1.2949869376818164e+000:1.3527776213819838e+000:2.1032005839652541e+000:2.0440660024340240e+000:2.6746798112824961e+000:2.7020261833950832e+000:3.0249294391047137e+000";
	setAttr ".tpr" 0.89999997615814209;
	setAttr ".wid" 0.039999999105930328;
	setAttr ".dwi" yes;
	setAttr ".fcm" yes;
createNode transform -n "xgGuide30" -p "pPlane1_Interptest";
	rename -uid "3116C4ED-40DC-8B41-5B84-53836A0AB343";
	setAttr ".rp" -type "double3" -1.3038375377655029 0.053357414901256561 -1.3168890476226807 ;
	setAttr ".sp" -type "double3" -1.3038375377655029 0.053357414901256561 -1.3168890476226807 ;
	setAttr ".mntl" -type "double3" 0 0 0 ;
	setAttr ".mxtl" -type "double3" 0 0 0 ;
	setAttr ".mtxe" yes;
	setAttr ".mtye" yes;
	setAttr ".mtze" yes;
	setAttr ".xtxe" yes;
	setAttr ".xtye" yes;
	setAttr ".xtze" yes;
createNode xgmSplineGuide -n "xgGuide30Shape" -p "xgGuide30";
	rename -uid "35898088-474C-E769-5357-F1A04806DF8A";
	setAttr -k off ".v";
	setAttr ".tw" yes;
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -l on ".ul" 0.17279599606990814;
	setAttr -l on ".vl" 0.8348810076713562;
	setAttr -l on ".fid" 2;
	setAttr ".bb1" -type "float3" -2.4111259 0.043368053 -1.8411381 ;
	setAttr ".bb2" -type "float3" -1.3037375 0.46111965 -1.316789 ;
	setAttr ".xin" -type "string" "1.6974529736301383e+000:1.6974429736301382e+000:4.9155419788715604e-001:1.0835429832894889e+000:1.0329807638010540e+000:1.3044639247381742e+000:3.9888328367917625e+000";
	setAttr ".tpr" 0.89999997615814209;
	setAttr ".wid" 0.039999999105930328;
	setAttr ".dwi" yes;
	setAttr ".fcm" yes;
createNode transform -n "xgGuide31" -p "pPlane1_Interptest";
	rename -uid "D15EE560-4A71-C3D2-AF68-DD8EDF4094AF";
	setAttr ".rp" -type "double3" -1.4975818395614624 0.053357422351837158 -0.25079795718193054 ;
	setAttr ".sp" -type "double3" -1.4975818395614624 0.053357422351837158 -0.25079795718193054 ;
	setAttr ".mntl" -type "double3" 0 0 0 ;
	setAttr ".mxtl" -type "double3" 0 0 0 ;
	setAttr ".mtxe" yes;
	setAttr ".mtye" yes;
	setAttr ".mtze" yes;
	setAttr ".xtxe" yes;
	setAttr ".xtye" yes;
	setAttr ".xtze" yes;
createNode xgmSplineGuide -n "xgGuide31Shape" -p "xgGuide31";
	rename -uid "74952C9D-4576-2132-A91A-2AA9DD6D0897";
	setAttr -k off ".v";
	setAttr ".tw" yes;
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -l on ".ul" 0.11870399862527847;
	setAttr -l on ".vl" 0.14793300628662109;
	setAttr -l on ".fid" 2;
	setAttr ".bb1" -type "float3" -1.9734426 0.053257421 -0.25089794 ;
	setAttr ".bb2" -type "float3" -1.4431887 0.67638803 0.015604462 ;
	setAttr ".xin" -type "string" "1.9351788029957873e+000:1.4126478360868331e+000:6.6681354421344041e-003:1.9351688029957872e+000:4.9721806227699750e-001:1.5868252528046707e+000:9.8761625386204177e-001:1.0835429832894889e+000:3.0306839168373965e+000";
	setAttr ".tpr" 0.89999997615814209;
	setAttr ".wid" 0.039999999105930328;
	setAttr ".dwi" yes;
	setAttr ".fcm" yes;
createNode transform -n "xgGuide36" -p "pPlane1_Interptest";
	rename -uid "4515FF08-43E6-B292-E3B9-9788795130BE";
	setAttr ".rp" -type "double3" -0.13045336306095123 0.05335741862654686 1.1188285350799561 ;
	setAttr ".sp" -type "double3" -0.13045336306095123 0.05335741862654686 1.1188285350799561 ;
	setAttr ".mntl" -type "double3" 0 0 0 ;
	setAttr ".mxtl" -type "double3" 0 0 0 ;
	setAttr ".mtxe" yes;
	setAttr ".mtye" yes;
	setAttr ".mtze" yes;
	setAttr ".xtxe" yes;
	setAttr ".xtye" yes;
	setAttr ".xtze" yes;
createNode xgmSplineGuide -n "xgGuide36Shape" -p "xgGuide36";
	rename -uid "EE78DEAE-43E7-7DA2-2B4E-BE941AF33B9C";
	setAttr -k off ".v";
	setAttr ".tw" yes;
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -l on ".ul" 0.92323899269104004;
	setAttr -l on ".vl" 0.34187299013137817;
	setAttr -l on ".fid";
	setAttr ".bb1" -type "float3" -0.13055336 0.053257417 1.0564338 ;
	setAttr ".bb2" -type "float3" 0.26938054 1.1474699 1.1698302 ;
	setAttr ".xin" -type "string" "1.9351788029957873e+000:1.3527776213819838e+000:6.9385660446914299e-002:1.5692339498381442e+000:7.8997697508515197e-001:1.8093040149530106e+000:1.1035524323816441e+000:1.2143053050001313e+000:1.9704966967227309e+000:1.9351688029957872e+000:2.5004255726049744e+000:1.2497120126267598e+000:3.0010125670086674e+000";
	setAttr ".tpr" 0.89999997615814209;
	setAttr ".dwi" yes;
	setAttr ".fcm" yes;
createNode transform -n "pPlane2_Interptest" -p "Interptest";
	rename -uid "4C19E86F-4255-8FAA-3F1D-0CBD6278349F";
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
createNode xgmSubdPatch -n "pPlane2_InterptestShape" -p "pPlane2_Interptest";
	rename -uid "6AB405C0-4588-812A-1F06-C188373A4304";
	setAttr -k off ".v";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".bb1" -type "float3" -1.700091 0.053357415 1.913002 ;
	setAttr ".bb2" -type "float3" 1.700091 0.053357415 5.3131838 ;
createNode transform -n "xgGuide23" -p "pPlane2_Interptest";
	rename -uid "CAE8A11A-4B68-C23B-01C3-64B28B681680";
	setAttr ".rp" -type "double3" 1.2159451246261597 0.053357414901256561 4.8290386199951172 ;
	setAttr ".sp" -type "double3" 1.2159451246261597 0.053357414901256561 4.8290386199951172 ;
	setAttr ".mntl" -type "double3" 0 0 0 ;
	setAttr ".mxtl" -type "double3" 0 0 0 ;
	setAttr ".mtxe" yes;
	setAttr ".mtye" yes;
	setAttr ".mtze" yes;
	setAttr ".xtxe" yes;
	setAttr ".xtye" yes;
	setAttr ".xtze" yes;
createNode xgmSplineGuide -n "xgGuide23Shape" -p "xgGuide23";
	rename -uid "2ADD5E44-4020-E193-4163-ECA4357BCF8D";
	setAttr -k off ".v";
	setAttr ".tw" yes;
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -l on ".ul" 0.75;
	setAttr -l on ".vl" 0.25;
	setAttr -l on ".fid" 1;
	setAttr ".bb1" -type "float3" 1.2158451 0.053257413 4.7530708 ;
	setAttr ".bb2" -type "float3" 1.4643916 0.38572249 4.8291388 ;
	setAttr ".xin" -type "string" "2.3170220735975433e+000:2.3170120735975432e+000:2.0159237092248574e+000:1.3545669294618163e+000:3.0139585921650287e+000";
	setAttr ".tpr" 0.89999997615814209;
	setAttr ".wid" 0.039999999105930328;
	setAttr ".dwi" yes;
	setAttr ".fcm" yes;
createNode transform -n "xgGuide24" -p "pPlane2_Interptest";
	rename -uid "91546777-4E4A-9E6A-BE79-9E8468721E2B";
	setAttr ".rp" -type "double3" -1.0825550556182861 0.053357414901256561 4.5366549491882324 ;
	setAttr ".sp" -type "double3" -1.0825550556182861 0.053357414901256561 4.5366549491882324 ;
	setAttr ".mntl" -type "double3" 0 0 0 ;
	setAttr ".mxtl" -type "double3" 0 0 0 ;
	setAttr ".mtxe" yes;
	setAttr ".mtye" yes;
	setAttr ".mtze" yes;
	setAttr ".xtxe" yes;
	setAttr ".xtye" yes;
	setAttr ".xtze" yes;
createNode xgmSplineGuide -n "xgGuide24Shape" -p "xgGuide24";
	rename -uid "DD6A29CC-409E-7C51-40E4-8F901E1706B6";
	setAttr -k off ".v";
	setAttr ".tw" yes;
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -l on ".ul" 0.35331800580024719;
	setAttr -l on ".vl" 0.44683799147605896;
	setAttr -l on ".fid";
	setAttr ".bb1" -type "float3" -1.1945795 0.053257413 4.3777037 ;
	setAttr ".bb2" -type "float3" -1.082455 0.79337221 4.8755589 ;
	setAttr ".xin" -type "string" "2.6744352653202172e+000:2.3170120735975432e+000:1.7428579095126628e-002:9.3250687792012243e-001:2.8962132416555653e+000:1.7445053966509450e+000:3.0472115358764720e+000:2.6744252653202172e+000:3.8492940793213148e+000";
	setAttr ".tpr" 0.89999997615814209;
	setAttr ".wid" 0.039999999105930328;
	setAttr ".dwi" yes;
	setAttr ".fcm" yes;
createNode transform -n "xgGuide32" -p "pPlane2_Interptest";
	rename -uid "53685726-4C5C-603D-90DC-9199D79C2C96";
	setAttr ".rp" -type "double3" 0.58802437782287598 0.05335741862654686 2.5139329433441162 ;
	setAttr ".sp" -type "double3" 0.58802437782287598 0.05335741862654686 2.5139329433441162 ;
	setAttr ".mntl" -type "double3" 0 0 0 ;
	setAttr ".mxtl" -type "double3" 0 0 0 ;
	setAttr ".mtxe" yes;
	setAttr ".mtye" yes;
	setAttr ".mtze" yes;
	setAttr ".xtxe" yes;
	setAttr ".xtye" yes;
	setAttr ".xtze" yes;
createNode xgmSplineGuide -n "xgGuide32Shape" -p "xgGuide32";
	rename -uid "D25DB077-489A-534B-B846-B0A380E4AA8D";
	setAttr -k off ".v";
	setAttr ".tw" yes;
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -l on ".ul" 0.34838500618934631;
	setAttr -l on ".vl" 0.64903700351715088;
	setAttr -l on ".fid" 3;
	setAttr ".bb1" -type "float3" 0.58792436 0.053257417 2.0886886 ;
	setAttr ".bb2" -type "float3" 1.2249184 1.7569838 2.5140328 ;
	setAttr ".xin" -type "string" "1.5692439498381443e+000:1.2497145049490779e+000:5.9691344735574225e-001:1.3397420314832076e+000:1.9411355700856832e+000:1.5692339498381442e+000:2.7857461516635489e+000:1.1922692668957731e+000:3.2370330511529599e+000";
	setAttr ".tpr" 0.89999997615814209;
	setAttr ".wid" 0.039999999105930328;
	setAttr ".dwi" yes;
	setAttr ".fcm" yes;
createNode transform -n "xgGuide33" -p "pPlane2_Interptest";
	rename -uid "D89E0EE8-477E-A2C4-26D0-25AA62BA628D";
	setAttr ".rp" -type "double3" -0.71351724863052368 0.053357411175966263 2.8316197395324707 ;
	setAttr ".sp" -type "double3" -0.71351724863052368 0.053357411175966263 2.8316197395324707 ;
	setAttr ".mntl" -type "double3" 0 0 0 ;
	setAttr ".mxtl" -type "double3" 0 0 0 ;
	setAttr ".mtxe" yes;
	setAttr ".mtye" yes;
	setAttr ".mtze" yes;
	setAttr ".xtxe" yes;
	setAttr ".xtye" yes;
	setAttr ".xtze" yes;
createNode xgmSplineGuide -n "xgGuide33Shape" -p "xgGuide33";
	rename -uid "995EE018-401F-79C5-76AB-648E9A01BF91";
	setAttr -k off ".v";
	setAttr ".tw" yes;
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -l on ".ul" 0.57867497205734253;
	setAttr -l on ".vl" 0.46129700541496277;
	setAttr -l on ".fid" 2;
	setAttr ".bb1" -type "float3" -0.71361727 0.053257409 2.8315198 ;
	setAttr ".bb2" -type "float3" -0.49511722 1.1240858 2.93943 ;
	setAttr ".xin" -type "string" "2.1889606362869682e+000:2.1889506362869682e+000:8.9102137317367602e-002:1.7445053966509450e+000:1.0449629285796052e+000:1.0649511626391763e+000:1.4018112737318507e+000:1.6249691972211928e+000:2.8579108782544402e+000:1.8093040149530106e+000:3.1041636663340553e+000:1.3397420314832076e+000:3.9440092516719147e+000";
	setAttr ".tpr" 0.89999997615814209;
	setAttr ".dwi" yes;
	setAttr ".fcm" yes;
createNode transform -n "xgGuide34" -p "pPlane2_Interptest";
	rename -uid "1F3FAE45-47AE-22D5-BBD8-37A912353B7E";
	setAttr ".rp" -type "double3" -1.3881571292877197 0.053357411175966263 3.6556363105773926 ;
	setAttr ".sp" -type "double3" -1.3881571292877197 0.053357411175966263 3.6556363105773926 ;
	setAttr ".mntl" -type "double3" 0 0 0 ;
	setAttr ".mxtl" -type "double3" 0 0 0 ;
	setAttr ".mtxe" yes;
	setAttr ".mtye" yes;
	setAttr ".mtze" yes;
	setAttr ".xtxe" yes;
	setAttr ".xtye" yes;
	setAttr ".xtze" yes;
createNode xgmSplineGuide -n "xgGuide34Shape" -p "xgGuide34";
	rename -uid "C22F976E-4E71-7AC6-26BC-7DBF2929C257";
	setAttr -k off ".v";
	setAttr ".tw" yes;
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -l on ".ul" 0.18347899615764618;
	setAttr -l on ".vl" 0.97497397661209106;
	setAttr -l on ".fid";
	setAttr ".bb1" -type "float3" -1.4224718 0.053257409 3.6389425 ;
	setAttr ".bb2" -type "float3" -1.3286862 0.94077319 3.8915403 ;
	setAttr ".xin" -type "string" "1.0649611626391764e+000:9.3250687792012243e-001:8.9266175324486885e-001:1.0649511626391763e+000:3.4014027946172929e+000";
	setAttr ".tpr" 0.89999997615814209;
	setAttr ".dwi" yes;
	setAttr ".fcm" yes;
createNode transform -n "xgGuide35" -p "pPlane2_Interptest";
	rename -uid "793F3E6E-4FF7-7138-28EC-A39EAE1F1DF7";
	setAttr ".rp" -type "double3" 1.3759839534759521 0.05335741862654686 3.4839489459991455 ;
	setAttr ".sp" -type "double3" 1.3759839534759521 0.05335741862654686 3.4839489459991455 ;
	setAttr ".mntl" -type "double3" 0 0 0 ;
	setAttr ".mxtl" -type "double3" 0 0 0 ;
	setAttr ".mtxe" yes;
	setAttr ".mtye" yes;
	setAttr ".mtze" yes;
	setAttr ".xtxe" yes;
	setAttr ".xtye" yes;
	setAttr ".xtze" yes;
createNode xgmSplineGuide -n "xgGuide35Shape" -p "xgGuide35";
	rename -uid "4B8B95CF-401D-A2DA-AB7F-1F93A4394923";
	setAttr -k off ".v";
	setAttr ".tw" yes;
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -l on ".ul" 0.80940502882003784;
	setAttr -l on ".vl" 0.076008997857570648;
	setAttr -l on ".fid" 3;
	setAttr ".bb1" -type "float3" 1.3758839 0.053257417 3.2359467 ;
	setAttr ".bb2" -type "float3" 1.8157583 1.0611305 3.4840488 ;
	setAttr ".xin" -type "string" "2.6744352653202172e+000:1.3545669294618163e+000:1.0141566280094960e+000:2.6744252653202172e+000:1.8456728394986317e+000:2.1889506362869682e+000:2.0892881182809160e+000:1.2497145049490779e+000:2.6032842448465017e+000";
	setAttr ".tpr" 0.89999997615814209;
	setAttr ".dwi" yes;
	setAttr ".fcm" yes;
createNode transform -n "pPlane2";
	rename -uid "A579FB7E-4DF9-1925-F776-89B3672BAD39";
	setAttr ".rp" -type "double3" 0 0.05335741613676924 3.6130929349052483 ;
	setAttr ".sp" -type "double3" 0 0.05335741613676924 3.6130929349052483 ;
createNode mesh -n "pPlaneShape2" -p "pPlane2";
	rename -uid "1C292310-4636-B91F-6055-F6829D21BECA";
	addAttr -ci true -sn "Git_Interptest_mask" -ln "Git_Interptest_mask" -at "float";
	addAttr -ci true -sn "xgen_Pref" -ln "xgen_Pref" -dt "vectorArray";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:3]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.25 0.75 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 9 ".uvst[0].uvsp[0:8]" -type "float2" 0 0 0.5 0 1 0 0 0.5
		 0.5 0.5 1 0.5 0 1 0.5 1 1 1;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".ovb" 2;
	setAttr -s 9 ".pt[0:8]" -type "float3"  -1.200091 0.053357415 4.8131838 
		0 0.053357415 4.8131838 1.200091 0.053357415 4.8131838 -1.200091 0.053357415 3.6130929 
		0 0.053357415 3.6130929 1.200091 0.053357415 3.6130929 -1.200091 0.053357415 2.413002 
		0 0.053357415 2.413002 1.200091 0.053357415 2.413002;
	setAttr -s 9 ".vt[0:8]"  -0.5 -1.110223e-016 0.5 0 -1.110223e-016 0.5
		 0.5 -1.110223e-016 0.5 -0.5 0 0 0 0 0 0.5 0 0 -0.5 1.110223e-016 -0.5 0 1.110223e-016 -0.5
		 0.5 1.110223e-016 -0.5;
	setAttr -s 12 ".ed[0:11]"  0 1 0 0 3 0 1 2 0 1 4 1 2 5 0 3 4 1 3 6 0
		 4 5 1 4 7 1 5 8 0 6 7 0 7 8 0;
	setAttr -s 4 -ch 16 ".fc[0:3]" -type "polyFaces" 
		f 4 0 3 -6 -2
		mu 0 4 0 1 4 3
		f 4 2 4 -8 -4
		mu 0 4 1 2 5 4
		f 4 5 8 -11 -7
		mu 0 4 3 4 7 6
		f 4 7 9 -12 -9
		mu 0 4 4 5 8 7;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr ".dr" 3;
	setAttr ".dsm" 2;
	setAttr ".ai_translator" -type "string" "polymesh";
	setAttr ".xgen_Pref" -type "vectorArray" 9 -1.7000910043716431 0.053357414901256561
		 5.3131837844848633 0 0.053357414901256561 5.3131837844848633 1.7000910043716431 0.053357414901256561
		 5.3131837844848633 -1.7000910043716431 0.053357414901256561 3.6130928993225098 0
		 0.053357414901256561 3.6130928993225098 1.7000910043716431 0.053357414901256561 3.6130928993225098 -1.7000910043716431
		 0.053357414901256561 1.9130020141601562 0 0.053357414901256561 1.9130020141601562 1.7000910043716431
		 0.053357414901256561 1.9130020141601562 ;
createNode transform -n "aiSkyDomeLight1";
	rename -uid "1A36278D-49AE-C7A7-853C-C18248023E14";
createNode aiSkyDomeLight -n "aiSkyDomeLightShape1" -p "aiSkyDomeLight1";
	rename -uid "CD825DFE-4F2E-D54F-4E1B-F59544F1D3E3";
	addAttr -ci true -h true -sn "aal" -ln "attributeAliasList" -dt "attributeAlias";
	setAttr -k off ".v";
	setAttr ".csh" no;
	setAttr ".rcsh" no;
	setAttr ".aal" -type "attributeAlias" {"exposure","aiExposure"} ;
createNode lightLinker -s -n "lightLinker1";
	rename -uid "95703FD4-414B-AD91-D327-86ACB0102A1A";
	setAttr -s 3 ".lnk";
	setAttr -s 3 ".slnk";
createNode shapeEditorManager -n "shapeEditorManager";
	rename -uid "C80F6273-4EEF-8CA5-DE63-26BE37490D1D";
createNode poseInterpolatorManager -n "poseInterpolatorManager";
	rename -uid "6AC003DE-4CA1-7490-EFE3-6EB7BEEDCFB0";
createNode displayLayerManager -n "layerManager";
	rename -uid "5B4A6B45-43A4-62FD-7EEA-CFBD7BBCDDE2";
	setAttr -s 3 ".dli[1:2]"  1 2;
	setAttr -s 2 ".dli";
createNode displayLayer -n "defaultLayer";
	rename -uid "8182CE84-4DBC-8824-286F-1AA669E820D8";
createNode renderLayerManager -n "renderLayerManager";
	rename -uid "FD87E38E-4A84-1417-5F10-1CB13BDAC3CF";
createNode renderLayer -n "defaultRenderLayer";
	rename -uid "AD5E5894-404B-090C-0EFF-EAB45C64A80B";
	setAttr ".g" yes;
createNode polyPlane -n "polyPlane1";
	rename -uid "2F1D8EE8-4ED9-C9CB-AB64-2CA0569C36A2";
	setAttr ".sw" 2;
	setAttr ".sh" 2;
	setAttr ".cuv" 2;
createNode hairPhysicalShader -n "hairPhysicalShader1";
	rename -uid "BDB85633-4927-84DD-8B5E-B7B6A0CB2558";
createNode shadingEngine -n "hairPhysicalShader1SG";
	rename -uid "7BCDFA44-4F28-0C34-07CC-C380A912320C";
	setAttr ".ihi" 0;
	setAttr -s 13 ".dsm";
	setAttr ".ro" yes;
	setAttr ".ai_surface_shader" -type "float3" -1.9198344e-029 0 0 ;
	setAttr ".ai_volume_shader" -type "float3" -1.9198344e-029 0 0 ;
createNode materialInfo -n "materialInfo1";
	rename -uid "2754D7AD-4F5F-43ED-44C5-E58DAEDA21D2";
createNode expression -n "xgmRefreshPreview";
	rename -uid "5F98A1C6-4BE4-90DE-C26B-619A6FC94AF3";
	setAttr -k on ".nds";
	setAttr ".ixp" -type "string" "xgmPreview -r";
	setAttr ".uno" 1;
createNode script -n "uiConfigurationScriptNode";
	rename -uid "F9F6A966-4500-2ED5-8F6E-B7BEA6A3ABE9";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n"
		+ "            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n"
		+ "            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n"
		+ "            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"side\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n"
		+ "            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n"
		+ "            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n"
		+ "            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n"
		+ "            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n"
		+ "        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n"
		+ "            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n"
		+ "            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n"
		+ "            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1087\n            -height 699\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"ToggledOutliner\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"ToggledOutliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 0\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 0\n            -showReferenceMembers 0\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n"
		+ "            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -isSet 0\n            -isSetMember 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n"
		+ "            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            -renderFilterIndex 0\n            -selectionOrder \"chronological\" \n            -expandAttribute 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n"
		+ "            -showShapes 0\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 0\n            -showReferenceMembers 0\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n"
		+ "            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n"
		+ "                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n"
		+ "                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 1\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -showCurveNames 0\n                -showActiveCurveNames 0\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n"
		+ "                -constrainDrag 0\n                -classicMode 1\n                -valueLinesToggle 1\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n"
		+ "                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n"
		+ "                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n"
		+ "                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"timeEditorPanel\" (localizedPanelLabel(\"Time Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Time Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n"
		+ "                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 1 \n"
		+ "                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n                -showUnderworld 0\n                -showInvisible 0\n"
		+ "                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"createNodePanel\" (localizedPanelLabel(\"Create Node\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"shapePanel\" (localizedPanelLabel(\"Shape Editor\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tshapePanel -edit -l (localizedPanelLabel(\"Shape Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"posePanel\" (localizedPanelLabel(\"Pose Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tposePanel -edit -l (localizedPanelLabel(\"Pose Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"profilerPanel\" (localizedPanelLabel(\"Profiler Tool\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Profiler Tool\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"contentBrowserPanel\" (localizedPanelLabel(\"Content Browser\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Content Browser\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"Stereo\" (localizedPanelLabel(\"Stereo\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels  $panelName;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n"
		+ "                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 4 4 \n                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n"
		+ "                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n"
		+ "                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 0\n                -height 0\n                -sceneRenderFilter 0\n                -displayMode \"centerEye\" \n                -viewColor 0 0 0 1 \n                -useCustomBackground 1\n                $editorName;\n            stereoCameraView -e -viewSelected 0 $editorName;\n            stereoCameraView -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"nodeEditorPanel\" (localizedPanelLabel(\"Node Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -nodeTitleMode \"name\" \n"
		+ "                -gridSnap 0\n                -gridVisibility 1\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -activeTab -1\n                -editorMode \"default\" \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-userCreated false\n\t\t\t\t-defaultImage \"vacantCell.xP:/\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 1087\\n    -height 699\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 1087\\n    -height 699\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	rename -uid "29CA2FBE-4E7A-B8FD-5142-6EA2F30AC6E2";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 120 -ast 1 -aet 200 ";
	setAttr ".st" 6;
createNode file -n "file1";
	rename -uid "F0C054E0-4A1C-FD72-AC25-239091E0549F";
createNode place2dTexture -n "place2dTexture1";
	rename -uid "0FCDABCC-4421-95CE-FF98-56B7A71D42E9";
createNode groupId -n "groupId1";
	rename -uid "3C0059FC-495C-1D8B-06D2-FE9D371AD107";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts1";
	rename -uid "D82C2BC0-4065-E72A-4872-BB8008FD173B";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:3]";
createNode groupId -n "groupId2";
	rename -uid "0FACB323-4DB0-6DC0-6F03-919C45B239EF";
	setAttr ".ihi" 0;
createNode brush -n "art3dPaintLastPaintBrush";
	rename -uid "21E5ACE3-4E1E-0874-CD94-148C6E615AB5";
	setAttr ".lcl[0]"  0 0.5 1;
	setAttr ".pcl[0]"  0 0.5 1;
	setAttr ".wsc[0]"  0 1 1;
	setAttr ".lws[0]"  0 1 1;
	setAttr ".pws[0]"  0 1 1;
	setAttr ".tls[0]"  0 1 1;
	setAttr -s 3 ".env";
	setAttr ".env[0].envp" 0.20000000298023224;
	setAttr ".env[0].envc" -type "float3" 0 0 0.15000001 ;
	setAttr ".env[0].envi" 2;
	setAttr ".env[1].envp" 0.5;
	setAttr ".env[1].envc" -type "float3" 0.47999999 0.55000001 0.69999999 ;
	setAttr ".env[1].envi" 2;
	setAttr ".env[2].envp" 1;
	setAttr ".env[2].envc" -type "float3" 0 0.1 0.44999999 ;
	setAttr ".env[2].envi" 2;
	setAttr ".rro[0]"  0 1 1;
createNode brush -n "art3dPaintLastBlurBrush";
	rename -uid "1B0F7301-4116-A580-4FED-EEBF06D0F00C";
	setAttr ".gsc" 3.547252434492111;
	setAttr ".brt" 2;
	setAttr ".bwd" 0.050000000750000002;
	setAttr ".sft" 0.5;
	setAttr ".bin" 8;
	setAttr ".lcl[0]"  0 0.5 1;
	setAttr ".pcl[0]"  0 0.5 1;
	setAttr ".wsc[0]"  0 1 1;
	setAttr ".lws[0]"  0 1 1;
	setAttr ".pws[0]"  0 1 1;
	setAttr ".tls[0]"  0 1 1;
	setAttr -s 3 ".env";
	setAttr ".env[0].envp" 0.20000000298023224;
	setAttr ".env[0].envc" -type "float3" 0 0 0.15000001 ;
	setAttr ".env[0].envi" 1;
	setAttr ".env[1].envp" 0.5;
	setAttr ".env[1].envc" -type "float3" 0.47999999 0.55000001 0.69999999 ;
	setAttr ".env[1].envi" 1;
	setAttr ".env[2].envp" 1;
	setAttr ".env[2].envc" -type "float3" 0 0.1 0.44999999 ;
	setAttr ".env[2].envi" 1;
	setAttr ".rro[0]"  0 1 1;
createNode groupId -n "groupId3";
	rename -uid "D24486A0-4712-EDB6-3569-73B6DCF23298";
	setAttr ".ihi" 0;
createNode groupId -n "groupId4";
	rename -uid "DECF8B8B-4788-2268-D91D-BFB38FAD8021";
	setAttr ".ihi" 0;
createNode file -n "file2";
	rename -uid "558AB0F0-4CC7-CE47-8666-759EA75C8CE7";
createNode place2dTexture -n "place2dTexture2";
	rename -uid "C3C795AC-46C1-890D-B224-38AEE6ADC768";
createNode transformGeometry -n "transformGeometry1";
	rename -uid "284AAA1B-445C-28C6-8412-7188BD60B507";
	setAttr ".txf" -type "matrix" 3.4001820245099532 0 0 0 0 3.4001820245099532 0 0
		 0 0 3.4001820245099532 0 0 0.05335741613676924 0 1;
createNode animCurveTL -n "persp_translateX";
	rename -uid "BBB054EE-4B6F-1573-59E5-B9996EC72441";
	setAttr ".tan" 18;
	setAttr -s 3 ".ktv[0:2]"  1 -6.8942691379872434 60 0.30564995869909378
		 120 7.667977098221229;
createNode animCurveTL -n "persp_translateY";
	rename -uid "EE678562-4EC9-022B-BD7F-8C89593EA1F8";
	setAttr ".tan" 18;
	setAttr -s 3 ".ktv[0:2]"  1 3.6932587262835121 60 2.8755376648883155
		 120 4.1179695165162213;
createNode animCurveTL -n "persp_translateZ";
	rename -uid "CDC5DCF2-4A7E-ACC6-08EE-CFA655181258";
	setAttr ".tan" 18;
	setAttr -s 3 ".ktv[0:2]"  1 5.9712483077492919 60 8.9031139310284413
		 120 4.3566812851732664;
createNode animCurveTA -n "persp_rotateX";
	rename -uid "16E69130-4A98-7ACD-F283-769CD3E768C0";
	setAttr ".tan" 18;
	setAttr -s 3 ".ktv[0:2]"  1 -23.138352729576905 60 -24.619445611613287
		 120 -26.138352729576368;
createNode animCurveTA -n "persp_rotateY";
	rename -uid "724AF63E-4A67-AAF9-BC86-A881AD9B25D3";
	setAttr ".tan" 18;
	setAttr -s 3 ".ktv[0:2]"  1 661.39999999957024 60 725.18573345260711
		 120 790.59999999955346;
createNode animCurveTA -n "persp_rotateZ";
	rename -uid "9ADE6CC6-4EDE-CACE-D538-54AA188C3182";
	setAttr ".tan" 18;
	setAttr -s 3 ".ktv[0:2]"  1 -3.0522993118827937e-015 60 -3.9090468922514742e-015
		 120 -4.7876683574565612e-015;
createNode animCurveTU -n "persp_scaleX";
	rename -uid "CE544907-4F96-9C8C-24FF-13B5329FBE14";
	setAttr ".tan" 18;
	setAttr -s 3 ".ktv[0:2]"  1 1 60 1 120 1;
createNode animCurveTU -n "persp_scaleY";
	rename -uid "559DE2FA-42DA-AF18-FC89-9FA0CA198B4A";
	setAttr ".tan" 18;
	setAttr -s 3 ".ktv[0:2]"  1 1 60 1 120 1;
createNode animCurveTU -n "persp_scaleZ";
	rename -uid "6467379E-45BE-B67B-C569-5FBCD3A7BDB6";
	setAttr ".tan" 18;
	setAttr -s 3 ".ktv[0:2]"  1 1 60 1 120 1;
createNode displayLayer -n "xgGroomLayer";
	rename -uid "899BADF9-4989-45EF-E3B1-A6A7475732E7";
	setAttr ".do" 1;
createNode xgmMakeGuide -n "xgmMakeGuide1";
	rename -uid "BE2E005B-41EE-2DE1-C0CE-63BD62B52973";
	setAttr ".cgm" -type "pointArray" 4 1.2159451246261597 0.053357414901256561
		 4.8290386199951172 1 1.2604684818902996 0.19712684880083772 4.8076605511053057 1 1.3557756949451498
		 0.31396706720041884 4.8266802560526534 1 1.464291622 0.38562248989999964 4.7531707340000002
		 1 ;
	setAttr ".mct" 3;
	setAttr ".frm" -type "vectorArray" 4 1.2159451246261597 0.053357414901256561
		 4.8290386199951172 0.99999999999996592 3.0623215741795215e-009 2.6091210327627624e-007 -3.0623209402238767e-009
		 1 -2.4297675002693271e-009 2.6091210328371695e-007 -2.4297667012726465e-009 -0.99999999999996592 ;
createNode xgmMakeGuide -n "xgmMakeGuide2";
	rename -uid "4A068C77-4E37-A00C-F326-508D19E69255";
	setAttr ".cgm" -type "pointArray" 4 -1.0825550556182861 0.053357414901256561
		 4.5366549491882324 1 -1.1708858916912435 0.29359030570083777 4.3778037846835929 1 -1.1944794873456217
		 0.54752604790041892 4.7930927783417969 1 -1.15097855 0.79327222220000071 4.8754587020000004
		 1 ;
	setAttr ".mct" 3;
	setAttr ".frm" -type "vectorArray" 4 -1.0825550556182861 0.053357414901256561
		 4.5366549491882324 0.99998272955102019 -6.4252323870477852e-009 -0.0058771251212752802 6.3817331984578374e-009
		 1 -7.4201932216292667e-009 -0.005877125121275327 -7.3825588270628795e-009 -0.99998272955102019 ;
createNode xgmMakeGuide -n "xgmMakeGuide3";
	rename -uid "06FC708A-4FA1-1ACF-4519-B2B1C50C239A";
	setAttr ".cgm" -type "pointArray" 4 -1.3268228769302368 0.053357414901256561
		 1.3268228769302368 1 -1.5111572787596843 0.28239119100083776 1.5428208667596843 1 -1.3920730363798421
		 0.57188265250041825 1.647175341379842 1 -1.2303697870000001 0.84109176259999996 1.7180826010000001
		 1 ;
	setAttr ".mct" 3;
	setAttr ".frm" -type "vectorArray" 4 -1.3268228769302368 0.053357414901256561
		 1.3268228769302368 0.99999999999999978 -1.3501156256561891e-009 2.074127786403217e-008 1.3501156585508617e-009
		 1 -1.5859520556671952e-009 2.074127786189095e-008 -1.5859520836703191e-009 -0.99999999999999978 ;
createNode xgmMakeGuide -n "xgmMakeGuide4";
	rename -uid "88BD79A5-4DFB-0355-D4F2-BCBA94E5A9E4";
	setAttr ".cgm" -type "pointArray" 4 1.4402415752410889 0.053357414901256561
		 -1.2131211757659912 1 1.6172935682847698 0.12095969480083771 -1.5343923875494587
		 1 1.4008779067470296 0.33438488860041882 -1.4655537141700847 1 1.4312471672092895
		 0.61056979300000069 -1.3166776957907105 1 ;
	setAttr ".mct" 3;
	setAttr ".frm" -type "vectorArray" 4 1.4402415752410889 0.053357414901256561
		 -1.2131211757659912 0.99875900459552525 1.8481210877913274e-009 -0.049804123718376919 -1.7783686527982003e-009
		 1 1.4448478749593922e-009 -0.049804123718376919 1.3544847329855577e-009 -0.99875900459552525 ;
createNode xgmMakeGuide -n "xgmMakeGuide5";
	rename -uid "E6E3D046-424A-8CDA-F4E6-F2BCE1686015";
	setAttr ".cgm" -type "pointArray" 4 -0.0070655629970133305 0.053357407450675964
		 -1.4584329128265381 1 0.052667668133657801 0.35807692694221438 -1.3533653475200602
		 1 0.20744978861182903 0.56720973285846188 -1.5543963590507406 1 0.35135865570000002
		 0.62406551287470968 -1.8433925705814209 1 ;
	setAttr ".mct" 3;
	setAttr ".frm" -type "vectorArray" 4 -0.0070655629970133305 0.053357407450675964
		 -1.4584329128265381 0.99999999999522027 9.5305351778175957e-010 -3.0918587109213236e-006 -9.530552115297208e-010
		 1 -5.4780750491252249e-010 -3.0918587109208014e-006 -5.4781045162196183e-010 -0.99999999999522027 ;
createNode xgmMakeGuide -n "xgmMakeGuide6";
	rename -uid "EACC7801-477D-1851-C353-99BD4D4E82BB";
	setAttr ".cgm" -type "pointArray" 4 -0.090077593922615051 0.05335741862654686
		 -0.13024108111858368 1 0.14479424039941752 0.28204049209769788 -0.13083547364417422
		 1 0.034887335429708663 0.58710009789884887 -0.18530175337324811 1 0.27976057089999967
		 0.76085113540000004 -0.053739098332322557 1 ;
	setAttr ".mct" 3;
	setAttr ".frm" -type "vectorArray" 4 -0.090077593922615051 0.05335741862654686
		 -0.13024108111858368 0.99999999999999123 -5.1014608984081796e-009 -1.311001580580857e-007 5.1014607906820719e-009
		 1 -8.2170879253528111e-010 -1.3110015806227761e-007 -8.217081237329579e-010 -0.99999999999999123 ;
createNode xgmMakeGuide -n "xgmMakeGuide7";
	rename -uid "63954C4D-4C9D-44A9-9857-C6B91425B23A";
	setAttr ".cgm" -type "pointArray" 4 1.1743848323822021 0.053357414901256561
		 1.4758042097091675 1 1.2114753223633625 0.53876003160083763 0.99235724847277962 1 1.3224757561816813
		 1.2809904260004161 1.1407750102363892 1 1.9070112229999974 1.7194937199999987 1.1929392390000002
		 1 ;
	setAttr ".mct" 3;
	setAttr ".frm" -type "vectorArray" 4 1.1743848323822021 0.053357414901256561
		 1.4758042097091675 0.99820033744657477 1.2594597932494357e-009 -0.059967377144108179 -1.37839070998105e-009
		 0.99999999999999989 -1.9418938118142056e-009 -0.059967377144108186 -2.0210575337957246e-009
		 -0.99820033744657488 ;
createNode xgmMakeGuide -n "xgmMakeGuide8";
	rename -uid "680B196C-499D-D0EC-0496-CBA3E1B9F9EB";
	setAttr ".cgm" -type "pointArray" 4 -1.3038375377655029 0.053357414901256561
		 -1.3168890476226807 1 -1.5217893781460368 0.043468054260837712 -1.801437851391805
		 1 -1.9632538645730184 0.31651244080041885 -1.8410381296959026 1 -2.4110260380000001
		 0.46101965519999999 -1.6540199310000001 1 ;
	setAttr ".mct" 3;
	setAttr ".frm" -type "vectorArray" 4 -1.3038375377655029 0.053357414901256561
		 -1.3168890476226807 0.99999594429841632 -1.7636257018281079e-009 -0.0028480496341681009 1.7679410503345074e-009
		 1 1.5126766799975971e-009 -0.0028480496341680983 1.5177057288940264e-009 -0.99999594429841632 ;
createNode xgmMakeGuide -n "xgmMakeGuide9";
	rename -uid "DA594FE1-4226-8605-0B8F-96BDE4B3C1AA";
	setAttr ".cgm" -type "pointArray" 4 -1.4975818395614624 0.053357422351837158
		 -0.25079795718193054 1 -1.4432886720719402 0.37139529041378333 -0.19370145602675173
		 1 -1.68015169153597 0.57590819855688991 -0.092189747213375861 1 -1.9733425059999998
		 0.67628800999999872 0.015504462909999683 1 ;
	setAttr ".mct" 3;
	setAttr ".frm" -type "vectorArray" 4 -1.4975818395614624 0.053357422351837158
		 -0.25079795718193054 0.99999317453938663 4.0999013254116544e-010 0.0036947090060685081 -4.1100065743019631e-010
		 0.99999999999999989 2.7274668142372648e-010 0.0036947090060685086 2.7426334763250251e-010
		 -0.99999317453938674 ;
createNode xgmMakeGuide -n "xgmMakeGuide10";
	rename -uid "7F01AC9B-4347-9272-602B-1B95B8AD1B96";
	setAttr ".cgm" -type "pointArray" 4 0.58802437782287598 0.05335741862654686
		 2.5139329433441162 1 0.88870896946953137 0.59433117828436521 2.3770176965627443 1 0.95037687283476502
		 1.2117137612421838 2.2507075387813722 1 1.2248183880000001 1.7568836830000001 2.0887885119999998
		 1 ;
	setAttr ".mct" 3;
	setAttr ".frm" -type "vectorArray" 4 0.58802437782287598 0.05335741862654686
		 2.5139329433441162 0.99998397288376317 -1.8166444809553028e-011 0.0056616230539570846 2.44378794288981e-011
		 1 -1.1076404929904077e-009 0.0056616230539570846 -1.1077610987690426e-009 -0.99998397288376317 ;
createNode aiOptions -s -n "defaultArnoldRenderOptions";
	rename -uid "71B059DE-41FF-F85E-829E-35A15DE7C72E";
	addAttr -ci true -sn "ARV_options" -ln "ARV_options" -dt "string";
	setAttr ".version" -type "string" "2.0.0.1";
	setAttr ".ARV_options" -type "string" "Display Settings=0;Show Status Bar=1;Display Pixel Information=0;3D Manipulation=0;Show AOVs list=1;Show Cameras list=1;Show RGBA icon=1;Show Crop Region icon=1;Show 3D Manipulation icon=0;Show Debug Shading icon=0;Show Exposure icon=1;Show Gamma icon=0;Darken Out-Of-Region=0;Show Render Tiles=0;AOVs=Beauty;Test Resolution=100%;Log=Last Progressive Step;Camera=perspShape;Save UI Threads=1;Debug Shading=Disabled;Color Management.Gamma=1;Color Management.Exposure=0;Background.BG=BG Color;Background.Color=0 0 0;Background.Image=;Background.Scale=1   1;Background.Offset=0   0;Background.Apply Color Management=1;Foreground.Enable FG=0;Foreground.Image=;Foreground.Scale=1   1;Foreground.Offset=0   0;Foreground.Apply Color Management=1;";
createNode aiAOVFilter -s -n "defaultArnoldFilter";
	rename -uid "1D31D787-4B5F-74A6-6ACF-A5A4949E5E7A";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVDriver -s -n "defaultArnoldDriver";
	rename -uid "63478406-4B2D-5343-8D04-CA9CB631687B";
	setAttr ".color_management" 1;
	setAttr ".ai_translator" -type "string" "tif";
createNode aiAOVDriver -s -n "defaultArnoldDisplayDriver";
	rename -uid "0D1E79E3-48C1-F230-3231-729A0466CEC9";
	setAttr ".output_mode" 0;
	setAttr ".ai_translator" -type "string" "maya";
createNode xgmMakeGuide -n "xgmMakeGuide11";
	rename -uid "32AD3AEC-4BBD-F944-67E1-25B7F04FF73C";
	setAttr ".cgm" -type "pointArray" 4 -0.71351724863052368 0.053357411175966263
		 2.8316197395324707 1 -0.69805400800836914 0.42274449769531963 2.8406274350897407
		 1 -0.62821861931980194 0.7804970784144889 2.9053816976243234 1 -0.49521719955332466
		 1.1239857193010587 2.9393299893553455 1 ;
	setAttr ".mct" 3;
	setAttr ".frm" -type "vectorArray" 4 -0.71351724863052368 0.053357411175966263
		 2.8316197395324707 0.9999998676034223 2.2172510059200103e-009 -0.0005145805456841791 -2.2172635350455313e-009
		 1 -2.3777749702466789e-011 -0.00051458054568417899 -2.4918707234163464e-011 -0.9999998676034223 ;
createNode xgmMakeGuide -n "xgmMakeGuide12";
	rename -uid "198AA16F-46F7-0788-61F1-278F719CDC1C";
	setAttr ".cgm" -type "pointArray" 4 -1.3881571292877197 0.053357411175966263
		 3.6556363105773926 1 -1.4223717391420749 0.36426477877697439 3.6390423555274269 1 -1.3966573897527443
		 0.64953943635375788 3.7828726825498675 1 -1.328786274599804 0.94067314710719918 3.8914403465454752
		 1 ;
	setAttr ".mct" 3;
	setAttr ".frm" -type "vectorArray" 4 -1.3881571292877197 0.053357411175966263
		 3.6556363105773926 0.99999999520069738 4.382675252985496e-009 -9.7972472204035723e-005 -4.3824906599057294e-009
		 1 1.8843467814215721e-009 -9.7972472204043977e-005 1.8839174089336592e-009 -0.99999999520069738 ;
createNode xgmMakeGuide -n "xgmMakeGuide13";
	rename -uid "202632F7-4507-F7A7-83DD-B3980A2FE093";
	setAttr ".cgm" -type "pointArray" 4 1.3759839534759521 0.05335741862654686
		 3.4839489459991455 1 1.5280495138278225 0.39074547269251464 3.4113445215154385 1 1.6438863643499302
		 0.74305146461420624 3.3423139363813652 1 1.8156583691379589 1.0610304706234133 3.2360466097220715
		 1 ;
	setAttr ".mct" 3;
	setAttr ".frm" -type "vectorArray" 4 1.3759839534759521 0.05335741862654686
		 3.4839489459991455 0.99999964649897 5.5103146038063657e-010 -0.00084083407105948137 -5.483197832788214e-010
		 1 3.2252153269744085e-009 -0.00084083407105948311 3.2247531409018518e-009 -0.99999964649897 ;
createNode xgmMakeGuide -n "xgmMakeGuide14";
	rename -uid "CFE3B793-453A-E6CB-CFF7-FBA35E23C185";
	setAttr ".cgm" -type "pointArray" 4 -0.13045336306095123 0.05335741862654686
		 1.1188285350799561 1 -0.096031150398662224 0.45407492977075448 1.056533842293448
		 1 0.009596960498255841 0.8408121651301963 1.1062530543572777 1 0.26928056097444514
		 1.147369869921657 1.1697301380627543 1 ;
	setAttr ".mct" 3;
	setAttr ".frm" -type "vectorArray" 4 -0.13045336306095123 0.05335741862654686
		 1.1188285350799561 0.99999988132164908 1.2345705376785827e-009 0.00048719266001373123 -1.2351045910393901e-009
		 1 1.0958842865493923e-009 0.00048719266001372987 1.0964858903827562e-009 -0.99999988132164908 ;
createNode nodeGraphEditorInfo -n "hyperShadePrimaryNodeEditorSavedTabsInfo";
	rename -uid "36E9EF0D-464C-3D64-2EF9-A5A83BBCDA10";
	setAttr ".tgi[0].tn" -type "string" "Untitled_1";
	setAttr ".tgi[0].vl" -type "double2" -619.58959984739465 -4032.3507882782601 ;
	setAttr ".tgi[0].vh" -type "double2" 663.53075179858126 4028.7793598487478 ;
	setAttr -s 3 ".tgi[0].ni";
	setAttr ".tgi[0].ni[0].x" -597.14288330078125;
	setAttr ".tgi[0].ni[0].y" 330;
	setAttr ".tgi[0].ni[0].nvs" 1954;
	setAttr ".tgi[0].ni[1].x" -287.14285278320312;
	setAttr ".tgi[0].ni[1].y" 307.14285278320312;
	setAttr ".tgi[0].ni[1].nvs" 1923;
	setAttr ".tgi[0].ni[2].x" -91.428573608398437;
	setAttr ".tgi[0].ni[2].y" 92.857139587402344;
	setAttr ".tgi[0].ni[2].nvs" 1922;
createNode script -n "xgenGlobals";
	rename -uid "66A5D0E1-4384-16A9-7606-6F88CAC5D504";
	setAttr ".a" -type "string" "import maya.cmds as cmds\nif cmds.about(batch=True):\n\txgg.Playblast=False\nelse:\n\txgui.createDescriptionEditor(False).setGlobals( previewSel=0, previewMode=0, clearSel=0, clearMode=0, playblast=1, clearCache=0, autoCreateMR=1 )";
	setAttr ".stp" 1;
	setAttr ".ire" 1;
select -ne :time1;
	setAttr -av -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -av -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr ".o" 54;
	setAttr -av ".unw" 54;
	setAttr -k on ".etw";
	setAttr -k on ".tps";
	setAttr -av -k on ".tms";
select -ne :hardwareRenderingGlobals;
	setAttr ".otfna" -type "stringArray" 22 "NURBS Curves" "NURBS Surfaces" "Polygons" "Subdiv Surface" "Particles" "Particle Instance" "Fluids" "Strokes" "Image Planes" "UI" "Lights" "Cameras" "Locators" "Joints" "IK Handles" "Deformers" "Motion Trails" "Components" "Hair Systems" "Follicles" "Misc. UI" "Ornaments"  ;
	setAttr ".otfva" -type "Int32Array" 22 0 1 1 1 1 1
		 1 1 1 0 0 0 0 0 0 0 0 0
		 0 0 0 0 ;
	setAttr ".aoon" yes;
	setAttr -av ".aoam";
	setAttr -k on ".mbsof";
	setAttr ".msaa" yes;
	setAttr ".fprt" yes;
select -ne :renderPartition;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 3 ".st";
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
	setAttr -s 5 ".s";
select -ne :postProcessList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -av -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 2 ".u";
select -ne :defaultRenderingList1;
select -ne :lightList1;
	setAttr -k on ".cch";
	setAttr -k on ".nds";
select -ne :defaultTextureList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 2 ".tx";
select -ne :initialShadingGroup;
	setAttr -av -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -av -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 8 ".dsm";
	setAttr -k on ".mwc";
	setAttr -cb on ".an";
	setAttr -cb on ".il";
	setAttr -cb on ".vo";
	setAttr -cb on ".eo";
	setAttr -cb on ".fo";
	setAttr -cb on ".epo";
	setAttr ".ro" yes;
	setAttr -s 4 ".gn";
	setAttr ".ai_surface_shader" -type "float3" -1.9198344e-029 0 0 ;
	setAttr ".ai_volume_shader" -type "float3" -1.9198344e-029 0 0 ;
select -ne :initialParticleSE;
	setAttr -av -k on ".cch";
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
	setAttr ".ai_surface_shader" -type "float3" -1.9198344e-029 0 0 ;
	setAttr ".ai_volume_shader" -type "float3" -1.9198344e-029 0 0 ;
select -ne :defaultRenderGlobals;
	setAttr -k on ".cch";
	setAttr -k on ".ihi";
	setAttr -k on ".nds";
	setAttr -k on ".bnm";
	setAttr -k on ".macc";
	setAttr -k on ".macd";
	setAttr -k on ".macq";
	setAttr -k on ".mcfr";
	setAttr -k on ".ifg";
	setAttr -k on ".clip";
	setAttr -k on ".edm";
	setAttr -k on ".edl";
	setAttr ".ren" -type "string" "arnold";
	setAttr -av -k on ".esr";
	setAttr -k on ".ors";
	setAttr -k on ".sdf";
	setAttr -av -k on ".outf" 51;
	setAttr -cb on ".imfkey" -type "string" "tif";
	setAttr -k on ".gama";
	setAttr -k on ".ar";
	setAttr -k on ".fs";
	setAttr ".ef" 120;
	setAttr -av -k on ".bfs";
	setAttr -k on ".me";
	setAttr -k on ".se";
	setAttr -k on ".be";
	setAttr -k on ".ep";
	setAttr -k on ".fec";
	setAttr -av -k on ".ofc";
	setAttr -k on ".ofe";
	setAttr -k on ".efe";
	setAttr -k on ".oft";
	setAttr -k on ".umfn";
	setAttr -k on ".ufe";
	setAttr ".pff" yes;
	setAttr -k on ".peie";
	setAttr -k on ".ifp" -type "string" "testrender";
	setAttr -k on ".rv";
	setAttr -k on ".comp";
	setAttr -k on ".cth";
	setAttr -k on ".soll";
	setAttr -cb on ".sosl";
	setAttr -k on ".rd";
	setAttr -k on ".lp";
	setAttr -av -k on ".sp";
	setAttr -k on ".shs";
	setAttr -av -k on ".lpr";
	setAttr -k on ".gv";
	setAttr -k on ".sv";
	setAttr -k on ".mm";
	setAttr -k on ".npu";
	setAttr -k on ".itf";
	setAttr -k on ".shp";
	setAttr -k on ".isp";
	setAttr -k on ".uf";
	setAttr -k on ".oi";
	setAttr -k on ".rut";
	setAttr -k on ".mot";
	setAttr -av -k on ".mb";
	setAttr -av -k on ".mbf";
	setAttr -k on ".mbso";
	setAttr -k on ".mbsc";
	setAttr -av -k on ".afp";
	setAttr -k on ".pfb";
	setAttr -k on ".pram";
	setAttr -k on ".poam";
	setAttr -k on ".prlm";
	setAttr -k on ".polm";
	setAttr -k on ".prm";
	setAttr -k on ".pom";
	setAttr -k on ".pfrm";
	setAttr -k on ".pfom";
	setAttr -av -k on ".bll";
	setAttr -av -k on ".bls";
	setAttr -av -k on ".smv";
	setAttr -k on ".ubc";
	setAttr -k on ".mbc";
	setAttr -k on ".mbt";
	setAttr -k on ".udbx";
	setAttr -k on ".smc";
	setAttr -k on ".kmv";
	setAttr -k on ".isl";
	setAttr -k on ".ism";
	setAttr -k on ".imb";
	setAttr -k on ".rlen";
	setAttr -av -k on ".frts";
	setAttr -k on ".tlwd";
	setAttr -k on ".tlht";
	setAttr -k on ".jfc";
	setAttr -k on ".rsb";
	setAttr -k on ".ope";
	setAttr -k on ".oppf";
	setAttr -k on ".rcp";
	setAttr -k on ".icp";
	setAttr -k on ".ocp";
	setAttr -k on ".hbl";
select -ne :defaultResolution;
	setAttr -av -k on ".cch";
	setAttr -k on ".ihi";
	setAttr -av -k on ".nds";
	setAttr -k on ".bnm";
	setAttr -av ".w" 160;
	setAttr -av ".h" 120;
	setAttr -av ".pa" 1.3329999446868896;
	setAttr -av -k on ".al";
	setAttr -av ".dar" 1.7777777910232544;
	setAttr -av -k on ".ldar";
	setAttr -k on ".dpi";
	setAttr -av -k on ".off";
	setAttr -av -k on ".fld";
	setAttr -av -k on ".zsl";
	setAttr -k on ".isu";
	setAttr -k on ".pdu";
select -ne :defaultLightSet;
	setAttr -k on ".cch";
	setAttr -k on ".ihi";
	setAttr -av -k on ".nds";
	setAttr -k on ".bnm";
	setAttr -k on ".mwc";
	setAttr -k on ".an";
	setAttr -k on ".il";
	setAttr -k on ".vo";
	setAttr -k on ".eo";
	setAttr -k on ".fo";
	setAttr -k on ".epo";
	setAttr -k on ".ro";
select -ne :hardwareRenderGlobals;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr ".ctrs" 256;
	setAttr -av ".btrs" 512;
	setAttr -k off ".fbfm";
	setAttr -k off -cb on ".ehql";
	setAttr -k off -cb on ".eams";
	setAttr -k off -cb on ".eeaa";
	setAttr -k off -cb on ".engm";
	setAttr -k off -cb on ".mes";
	setAttr -k off -cb on ".emb";
	setAttr -av -k off -cb on ".mbbf";
	setAttr -k off -cb on ".mbs";
	setAttr -k off -cb on ".trm";
	setAttr -k off -cb on ".tshc";
	setAttr -k off ".enpt";
	setAttr -k off -cb on ".clmt";
	setAttr -k off -cb on ".tcov";
	setAttr -k off -cb on ".lith";
	setAttr -k off -cb on ".sobc";
	setAttr -k off -cb on ".cuth";
	setAttr -k off -cb on ".hgcd";
	setAttr -k off -cb on ".hgci";
	setAttr -k off -cb on ".mgcs";
	setAttr -k off -cb on ".twa";
	setAttr -k off -cb on ".twz";
	setAttr -k on ".hwcc";
	setAttr -k on ".hwdp";
	setAttr -k on ".hwql";
	setAttr -k on ".hwfr";
	setAttr -k on ".soll";
	setAttr -k on ".sosl";
	setAttr -k on ".bswa";
	setAttr -k on ".shml";
	setAttr -k on ".hwel";
connectAttr "persp_translateX.o" ":persp.tx";
connectAttr "persp_translateY.o" ":persp.ty";
connectAttr "persp_translateZ.o" ":persp.tz";
connectAttr "persp_rotateX.o" ":persp.rx";
connectAttr "persp_rotateY.o" ":persp.ry";
connectAttr "persp_rotateZ.o" ":persp.rz";
connectAttr "persp_scaleX.o" ":persp.sx";
connectAttr "persp_scaleY.o" ":persp.sy";
connectAttr "persp_scaleZ.o" ":persp.sz";
connectAttr "transformGeometry1.og" "pPlaneShape1.i";
connectAttr "groupId1.id" "pPlaneShape1.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "pPlaneShape1.iog.og[0].gco";
connectAttr "file1.oa" "pPlaneShape1.Git_Interptest_mask";
connectAttr "groupId2.id" "pPlaneShape1.ciog.cog[0].cgid";
connectAttr "pPlaneShape1.w" "pPlane1_InterptestShape.geo";
connectAttr "pPlane1.m" "pPlane1_InterptestShape.t";
connectAttr "xgmMakeGuide3.os" "xgGuide25Shape.is";
connectAttr "xgmMakeGuide3.tg" "xgGuide25Shape.tmg";
connectAttr "xgmMakeGuide4.os" "xgGuide26Shape.is";
connectAttr "xgmMakeGuide4.tg" "xgGuide26Shape.tmg";
connectAttr "xgmMakeGuide5.os" "xgGuide27Shape.is";
connectAttr "xgmMakeGuide5.tg" "xgGuide27Shape.tmg";
connectAttr "xgmMakeGuide6.os" "xgGuide28Shape.is";
connectAttr "xgmMakeGuide6.tg" "xgGuide28Shape.tmg";
connectAttr "xgmMakeGuide7.os" "xgGuide29Shape.is";
connectAttr "xgmMakeGuide7.tg" "xgGuide29Shape.tmg";
connectAttr "xgmMakeGuide8.os" "xgGuide30Shape.is";
connectAttr "xgmMakeGuide8.tg" "xgGuide30Shape.tmg";
connectAttr "xgmMakeGuide9.os" "xgGuide31Shape.is";
connectAttr "xgmMakeGuide9.tg" "xgGuide31Shape.tmg";
connectAttr "xgmMakeGuide14.os" "xgGuide36Shape.is";
connectAttr "xgmMakeGuide14.tg" "xgGuide36Shape.tmg";
connectAttr "pPlaneShape2.w" "pPlane2_InterptestShape.geo";
connectAttr "pPlane2.m" "pPlane2_InterptestShape.t";
connectAttr "xgmMakeGuide1.os" "xgGuide23Shape.is";
connectAttr "xgmMakeGuide1.tg" "xgGuide23Shape.tmg";
connectAttr "xgmMakeGuide2.os" "xgGuide24Shape.is";
connectAttr "xgmMakeGuide2.tg" "xgGuide24Shape.tmg";
connectAttr "xgmMakeGuide10.os" "xgGuide32Shape.is";
connectAttr "xgmMakeGuide10.tg" "xgGuide32Shape.tmg";
connectAttr "xgmMakeGuide11.os" "xgGuide33Shape.is";
connectAttr "xgmMakeGuide11.tg" "xgGuide33Shape.tmg";
connectAttr "xgmMakeGuide12.os" "xgGuide34Shape.is";
connectAttr "xgmMakeGuide12.tg" "xgGuide34Shape.tmg";
connectAttr "xgmMakeGuide13.os" "xgGuide35Shape.is";
connectAttr "xgmMakeGuide13.tg" "xgGuide35Shape.tmg";
connectAttr "groupId3.id" "pPlaneShape2.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "pPlaneShape2.iog.og[0].gco";
connectAttr "file2.oa" "pPlaneShape2.Git_Interptest_mask";
connectAttr "groupId4.id" "pPlaneShape2.ciog.cog[1].cgid";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "hairPhysicalShader1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "hairPhysicalShader1SG.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr "hairPhysicalShader1.oc" "hairPhysicalShader1SG.ss";
connectAttr "InterptestShape.iog" "hairPhysicalShader1SG.dsm" -na;
connectAttr "xgGuide32Shape.iog" "hairPhysicalShader1SG.dsm" -na;
connectAttr "xgGuide24Shape.iog" "hairPhysicalShader1SG.dsm" -na;
connectAttr "xgGuide23Shape.iog" "hairPhysicalShader1SG.dsm" -na;
connectAttr "pPlane2_InterptestShape.iog" "hairPhysicalShader1SG.dsm" -na;
connectAttr "xgGuide31Shape.iog" "hairPhysicalShader1SG.dsm" -na;
connectAttr "xgGuide30Shape.iog" "hairPhysicalShader1SG.dsm" -na;
connectAttr "xgGuide29Shape.iog" "hairPhysicalShader1SG.dsm" -na;
connectAttr "xgGuide28Shape.iog" "hairPhysicalShader1SG.dsm" -na;
connectAttr "xgGuide27Shape.iog" "hairPhysicalShader1SG.dsm" -na;
connectAttr "xgGuide26Shape.iog" "hairPhysicalShader1SG.dsm" -na;
connectAttr "xgGuide25Shape.iog" "hairPhysicalShader1SG.dsm" -na;
connectAttr "pPlane1_InterptestShape.iog" "hairPhysicalShader1SG.dsm" -na;
connectAttr "hairPhysicalShader1SG.msg" "materialInfo1.sg";
connectAttr "hairPhysicalShader1.msg" "materialInfo1.m";
connectAttr "hairPhysicalShader1.msg" "materialInfo1.t" -na;
connectAttr ":time1.o" "xgmRefreshPreview.tim";
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
connectAttr "polyPlane1.out" "groupParts1.ig";
connectAttr "groupId1.id" "groupParts1.gi";
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
connectAttr "groupParts1.og" "transformGeometry1.ig";
connectAttr "layerManager.dli[2]" "xgGroomLayer.id";
connectAttr "pPlane2_InterptestShape.go" "xgmMakeGuide1.gi";
connectAttr "pPlane2_InterptestShape.go" "xgmMakeGuide2.gi";
connectAttr "pPlane1_InterptestShape.go" "xgmMakeGuide3.gi";
connectAttr "pPlane1_InterptestShape.go" "xgmMakeGuide4.gi";
connectAttr "pPlane1_InterptestShape.go" "xgmMakeGuide5.gi";
connectAttr "pPlane1_InterptestShape.go" "xgmMakeGuide6.gi";
connectAttr "pPlane1_InterptestShape.go" "xgmMakeGuide7.gi";
connectAttr "pPlane1_InterptestShape.go" "xgmMakeGuide8.gi";
connectAttr "pPlane1_InterptestShape.go" "xgmMakeGuide9.gi";
connectAttr "pPlane2_InterptestShape.go" "xgmMakeGuide10.gi";
connectAttr ":defaultArnoldDisplayDriver.msg" ":defaultArnoldRenderOptions.drivers"
		 -na;
connectAttr ":defaultArnoldFilter.msg" ":defaultArnoldRenderOptions.filt";
connectAttr ":defaultArnoldDriver.msg" ":defaultArnoldRenderOptions.drvr";
connectAttr "pPlane2_InterptestShape.go" "xgmMakeGuide11.gi";
connectAttr "pPlane2_InterptestShape.go" "xgmMakeGuide12.gi";
connectAttr "pPlane2_InterptestShape.go" "xgmMakeGuide13.gi";
connectAttr "pPlane1_InterptestShape.go" "xgmMakeGuide14.gi";
connectAttr "hairPhysicalShader1.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[0].dn"
		;
connectAttr "hairPhysicalShader1SG.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[1].dn"
		;
connectAttr "defaultRenderLayer.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[2].dn"
		;
connectAttr "hairPhysicalShader1SG.pa" ":renderPartition.st" -na;
connectAttr "hairPhysicalShader1.msg" ":defaultShaderList1.s" -na;
connectAttr "place2dTexture1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture2.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "aiSkyDomeLightShape1.ltd" ":lightList1.l" -na;
connectAttr "file1.msg" ":defaultTextureList1.tx" -na;
connectAttr "file2.msg" ":defaultTextureList1.tx" -na;
connectAttr "pPlaneShape1.ciog.cog[0]" ":initialShadingGroup.dsm" -na;
connectAttr "pPlaneShape2.ciog.cog[1]" ":initialShadingGroup.dsm" -na;
connectAttr "pPlaneShape1.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "pPlaneShape2.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "xgGuide33Shape.iog" ":initialShadingGroup.dsm" -na;
connectAttr "xgGuide34Shape.iog" ":initialShadingGroup.dsm" -na;
connectAttr "xgGuide35Shape.iog" ":initialShadingGroup.dsm" -na;
connectAttr "xgGuide36Shape.iog" ":initialShadingGroup.dsm" -na;
connectAttr "groupId1.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId2.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId3.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId4.msg" ":initialShadingGroup.gn" -na;
connectAttr "aiSkyDomeLight1.iog" ":defaultLightSet.dsm" -na;
// End of test.ma
