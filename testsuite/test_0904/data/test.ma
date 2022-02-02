//Maya ASCII 2022 scene
//Name: test.ma
//Last modified: Mon, Jan 31, 2022 10:56:54 AM
//Codeset: UTF-8
requires maya "2022";
requires -nodeType "aiOptions" -nodeType "aiAOVDriver" -nodeType "aiAOVFilter" -nodeType "aiSkyDomeLight"
		 -nodeType "aiStandardSurface" -nodeType "aiOslShader" "mtoa" "5.1.0";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2022";
fileInfo "version" "2022";
fileInfo "cutIdentifier" "202108111415-612a77abf4";
fileInfo "osv" "Linux 3.10.0-1062.1.2.el7.x86_64 #1 SMP Mon Sep 30 14:19:46 UTC 2019 x86_64";
fileInfo "UUID" "0A405D80-0000-5859-61F7-C076000002B3";
createNode transform -s -n "persp";
	rename -uid "FB399D80-0000-D412-61E1-41ED0000023B";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0.59964733093085743 -1.5601012162877337 -0.115362850113141 ;
	setAttr ".r" -type "double3" 93.806499759467386 0.38043328234785928 17.995324253362675 ;
createNode camera -s -n "perspShape" -p "persp";
	rename -uid "FB399D80-0000-D412-61E1-41ED0000023C";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999993;
	setAttr ".coi" 1.6706925502172232;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".hc" -type "string" "viewSet -p %camera";
createNode transform -s -n "top";
	rename -uid "FB399D80-0000-D412-61E1-41ED0000023D";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 1000.1 ;
createNode camera -s -n "topShape" -p "top";
	rename -uid "FB399D80-0000-D412-61E1-41ED0000023E";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 1000.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
createNode transform -s -n "front";
	rename -uid "FB399D80-0000-D412-61E1-41ED0000023F";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 -1000.1 0 ;
	setAttr ".r" -type "double3" 89.999999999999986 0 0 ;
createNode camera -s -n "frontShape" -p "front";
	rename -uid "FB399D80-0000-D412-61E1-41ED00000240";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 1000.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
createNode transform -s -n "side";
	rename -uid "FB399D80-0000-D412-61E1-41ED00000241";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 1000.1 0 0 ;
	setAttr ".r" -type "double3" 90 1.2722218725854067e-14 89.999999999999986 ;
createNode camera -s -n "sideShape" -p "side";
	rename -uid "FB399D80-0000-D412-61E1-41ED00000242";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 1000.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
createNode transform -n "pPlane1";
	rename -uid "FB399D80-0000-D412-61E1-4251000002CA";
createNode mesh -n "pPlaneShape1" -p "pPlane1";
	rename -uid "FB399D80-0000-D412-61E1-4251000002C9";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "aiSkyDomeLight1";
	rename -uid "FB399D80-0000-D412-61E1-425B000002CC";
createNode aiSkyDomeLight -n "aiSkyDomeLightShape1" -p "aiSkyDomeLight1";
	rename -uid "FB399D80-0000-D412-61E1-425B000002CB";
	addAttr -ci true -h true -sn "aal" -ln "attributeAliasList" -dt "attributeAlias";
	setAttr -k off ".v";
	setAttr ".csh" no;
	setAttr ".rcsh" no;
	setAttr ".aal" -type "attributeAlias" {"exposure","aiExposure"} ;
createNode lightLinker -s -n "lightLinker1";
	rename -uid "0A405D80-0000-5859-61F7-C03E000002A1";
	setAttr -s 3 ".lnk";
	setAttr -s 3 ".slnk";
createNode displayLayerManager -n "layerManager";
	rename -uid "0A405D80-0000-5859-61F7-C03E000002A2";
createNode displayLayer -n "defaultLayer";
	rename -uid "FB399D80-0000-D412-61E1-41EE00000259";
createNode renderLayerManager -n "renderLayerManager";
	rename -uid "0A405D80-0000-5859-61F7-C03E000002A4";
createNode renderLayer -n "defaultRenderLayer";
	rename -uid "FB399D80-0000-D412-61E1-41EE0000025B";
	setAttr ".g" yes;
createNode shapeEditorManager -n "shapeEditorManager";
	rename -uid "0A405D80-0000-5859-61F7-C03E000002A6";
createNode poseInterpolatorManager -n "poseInterpolatorManager";
	rename -uid "0A405D80-0000-5859-61F7-C03E000002A7";
createNode aiOslShader -n "aiOslShader1";
	rename -uid "FB399D80-0000-D412-61E1-41FA000002BF";
	addAttr -ci true -h true -sn "cmpSts" -ln "compileStatus" -min 0 -max 3 -en "Needs (Re)Compile:Compile Success:Compile Warnings:Compile Failure" 
		-at "enum";
	addAttr -r false -ci true -sn "paramUVW" -ln "param_UVW" -ct "oslAttribute" -at "float3" 
		-nc 3;
	addAttr -r false -ci true -sn "paramUVWX" -ln "param_UVWX" -at "float" -p "param_UVW";
	addAttr -r false -ci true -sn "paramUVWY" -ln "param_UVWY" -at "float" -p "param_UVW";
	addAttr -r false -ci true -sn "paramUVWZ" -ln "param_UVWZ" -at "float" -p "param_UVW";
	addAttr -r false -ci true -sn "paramSeed" -ln "param_Seed" -ct "oslAttribute" -at "long";
	addAttr -r false -ci true -sn "paramScale" -ln "param_scale" -ct "oslAttribute" 
		-dv 1 -at "float";
	addAttr -r false -ci true -sn "paramUnitDependent" -ln "param_unit_dependent" -ct "oslAttribute" 
		-dv 1 -at "long";
	addAttr -r false -ci true -sn "paramAxis" -ln "param_axis" -ct "oslAttribute" -dv 
		2 -at "long";
	addAttr -r false -ci true -sn "paramRingThickness" -ln "param_ring_thickness" -ct "oslAttribute" 
		-dv 0.75 -at "float";
	addAttr -r false -ci true -sn "paramLateWoodRatio" -ln "param_late_wood_ratio" -ct "oslAttribute" 
		-dv 0.23800000548362732 -at "float";
	addAttr -r false -ci true -sn "paramEarlyWoodSharpness" -ln "param_early_wood_sharpness" 
		-ct "oslAttribute" -dv 0.39500001072883606 -at "float";
	addAttr -r false -ci true -sn "paramLateWoodSharpness" -ln "param_late_wood_sharpness" 
		-ct "oslAttribute" -dv 0.10899999737739563 -at "float";
	addAttr -r false -ci true -uac -sn "paramEarlyWoodColor" -ln "param_early_wood_color" 
		-ct "oslAttribute" -at "float3" -nc 3;
	addAttr -r false -ci true -sn "paramEarlyWoodColorR" -ln "param_early_wood_colorR" 
		-dv 0.20000000298023224 -at "float" -p "param_early_wood_color";
	addAttr -r false -ci true -sn "paramEarlyWoodColorG" -ln "param_early_wood_colorG" 
		-dv 0.079999998211860657 -at "float" -p "param_early_wood_color";
	addAttr -r false -ci true -sn "paramEarlyWoodColorB" -ln "param_early_wood_colorB" 
		-dv 0.024000000208616257 -at "float" -p "param_early_wood_color";
	addAttr -r false -ci true -sn "paramUseManualLateWoodColor" -ln "param_use_manual_late_wood_color" 
		-ct "oslAttribute" -at "long";
	addAttr -r false -ci true -uac -sn "paramManualLateWoodColor" -ln "param_manual_late_wood_color" 
		-ct "oslAttribute" -at "float3" -nc 3;
	addAttr -r false -ci true -sn "paramManualLateWoodColorR" -ln "param_manual_late_wood_colorR" 
		-dv 0.21699999272823334 -at "float" -p "param_manual_late_wood_color";
	addAttr -r false -ci true -sn "paramManualLateWoodColorG" -ln "param_manual_late_wood_colorG" 
		-dv 0.071000002324581146 -at "float" -p "param_manual_late_wood_color";
	addAttr -r false -ci true -sn "paramManualLateWoodColorB" -ln "param_manual_late_wood_colorB" 
		-dv 0.0099999997764825821 -at "float" -p "param_manual_late_wood_color";
	addAttr -r false -ci true -sn "paramLateWoodColorPower" -ln "param_late_wood_color_power" 
		-ct "oslAttribute" -dv 1.0700000524520874 -at "float";
	addAttr -r false -ci true -sn "paramUsePores" -ln "param_use_pores" -ct "oslAttribute" 
		-dv 1 -at "long";
	addAttr -r false -ci true -sn "paramPoreType" -ln "param_pore_type" -ct "oslAttribute" 
		-at "long";
	addAttr -r false -ci true -sn "paramPoreCellDim" -ln "param_pore_cell_dim" -ct "oslAttribute" 
		-dv 0.11999999731779099 -at "float";
	addAttr -r false -ci true -sn "paramPoreRadius" -ln "param_pore_radius" -ct "oslAttribute" 
		-dv 0.039999999105930328 -at "float";
	addAttr -r false -ci true -sn "paramPoreColorPower" -ln "param_pore_color_power" 
		-ct "oslAttribute" -dv 2.130000114440918 -at "float";
	addAttr -r false -ci true -sn "paramPoreDepth" -ln "param_pore_depth" -ct "oslAttribute" 
		-dv 0.0099999997764825821 -at "float";
	addAttr -r false -ci true -sn "paramUseRays" -ln "param_use_rays" -ct "oslAttribute" 
		-dv 1 -at "long";
	addAttr -r false -ci true -sn "paramRayColorPower" -ln "param_ray_color_power" -ct "oslAttribute" 
		-dv 1.1000000238418579 -at "float";
	addAttr -r false -ci true -sn "paramRaySegLengthZ" -ln "param_ray_seg_length_z" 
		-ct "oslAttribute" -dv 5 -at "float";
	addAttr -r false -ci true -sn "paramRayNumSlices" -ln "param_ray_num_slices" -ct "oslAttribute" 
		-dv 160 -at "long";
	addAttr -r false -ci true -sn "paramRayEllipseDepth" -ln "param_ray_ellipse_depth" 
		-ct "oslAttribute" -dv 2 -at "float";
	addAttr -r false -ci true -sn "paramRayEllipseScaleX" -ln "param_ray_ellipse_scale_x" 
		-ct "oslAttribute" -dv 0.20000000298023224 -at "float";
	addAttr -r false -ci true -sn "paramRayEllipseZ2x" -ln "param_ray_ellipse_z2x" -ct "oslAttribute" 
		-dv 10 -at "float";
	addAttr -r false -ci true -sn "paramRayRoughness" -ln "param_ray_roughness" -ct "oslAttribute" 
		-dv 0.10000000149011612 -at "float";
	addAttr -r false -ci true -sn "paramRayBumpDepth" -ln "param_ray_bump_depth" -ct "oslAttribute" 
		-dv 0.019999999552965164 -at "float";
	addAttr -r false -ci true -sn "paramRoughness" -ln "param_roughness" -ct "oslAttribute" 
		-dv 0.25 -at "float";
	addAttr -r false -ci true -sn "paramDiffuseLobeWeight" -ln "param_diffuse_lobe_weight" 
		-ct "oslAttribute" -dv 0.85000002384185791 -at "float";
	addAttr -r false -ci true -sn "paramUseLateWoodBump" -ln "param_use_late_wood_bump" 
		-ct "oslAttribute" -dv 1 -at "long";
	addAttr -r false -ci true -sn "paramLateWoodBumpDepth" -ln "param_late_wood_bump_depth" 
		-ct "oslAttribute" -dv 0.0020000000949949026 -at "float";
	addAttr -r false -ci true -sn "paramUseGrooveRoughness" -ln "param_use_groove_roughness" 
		-ct "oslAttribute" -dv 1 -at "long";
	addAttr -r false -ci true -sn "paramGrooveRoughness" -ln "param_groove_roughness" 
		-ct "oslAttribute" -dv 0.18999999761581421 -at "float";
	addAttr -r false -ci true -sn "paramUseFiberPerlin" -ln "param_use_fiber_perlin" 
		-ct "oslAttribute" -dv 1 -at "long";
	addAttr -r false -ci true -sn "paramFiberPerlinBandCount" -ln "param_fiber_perlin_band_count" 
		-ct "oslAttribute" -dv 3 -at "long";
	addAttr -r false -ci true -uac -sn "paramFiberPerlinFrequencies" -ln "param_fiber_perlin_frequencies" 
		-ct "oslAttribute" -at "float3" -nc 3;
	addAttr -r false -ci true -sn "paramFiberPerlinFrequenciesR" -ln "param_fiber_perlin_frequenciesR" 
		-dv 0.02500000037252903 -at "float" -p "param_fiber_perlin_frequencies";
	addAttr -r false -ci true -sn "paramFiberPerlinFrequenciesG" -ln "param_fiber_perlin_frequenciesG" 
		-dv 0.05000000074505806 -at "float" -p "param_fiber_perlin_frequencies";
	addAttr -r false -ci true -sn "paramFiberPerlinFrequenciesB" -ln "param_fiber_perlin_frequenciesB" 
		-dv 0.28600001335144043 -at "float" -p "param_fiber_perlin_frequencies";
	addAttr -r false -ci true -uac -sn "paramFiberPerlinWeights" -ln "param_fiber_perlin_weights" 
		-ct "oslAttribute" -at "float3" -nc 3;
	addAttr -r false -ci true -sn "paramFiberPerlinWeightsR" -ln "param_fiber_perlin_weightsR" 
		-dv 3 -at "float" -p "param_fiber_perlin_weights";
	addAttr -r false -ci true -sn "paramFiberPerlinWeightsG" -ln "param_fiber_perlin_weightsG" 
		-dv 1 -at "float" -p "param_fiber_perlin_weights";
	addAttr -r false -ci true -sn "paramFiberPerlinWeightsB" -ln "param_fiber_perlin_weightsB" 
		-dv 0.20000000298023224 -at "float" -p "param_fiber_perlin_weights";
	addAttr -r false -ci true -sn "paramFiberPerlinScaleZ" -ln "param_fiber_perlin_scale_z" 
		-ct "oslAttribute" -dv 0.30000001192092896 -at "float";
	addAttr -r false -ci true -sn "paramUseFiberCosine" -ln "param_use_fiber_cosine" 
		-ct "oslAttribute" -dv 1 -at "long";
	addAttr -r false -ci true -sn "paramFiberCosineBandCount" -ln "param_fiber_cosine_band_count" 
		-ct "oslAttribute" -dv 2 -at "long";
	addAttr -r false -ci true -uac -sn "paramFiberCosineFrequencies" -ln "param_fiber_cosine_frequencies" 
		-ct "oslAttribute" -at "float3" -nc 3;
	addAttr -r false -ci true -sn "paramFiberCosineFrequenciesR" -ln "param_fiber_cosine_frequenciesR" 
		-dv 0.067000001668930054 -at "float" -p "param_fiber_cosine_frequencies";
	addAttr -r false -ci true -sn "paramFiberCosineFrequenciesG" -ln "param_fiber_cosine_frequenciesG" 
		-dv 0.25 -at "float" -p "param_fiber_cosine_frequencies";
	addAttr -r false -ci true -sn "paramFiberCosineFrequenciesB" -ln "param_fiber_cosine_frequenciesB" 
		-at "float" -p "param_fiber_cosine_frequencies";
	addAttr -r false -ci true -uac -sn "paramFiberCosineWeights" -ln "param_fiber_cosine_weights" 
		-ct "oslAttribute" -at "float3" -nc 3;
	addAttr -r false -ci true -sn "paramFiberCosineWeightsR" -ln "param_fiber_cosine_weightsR" 
		-dv 2.5 -at "float" -p "param_fiber_cosine_weights";
	addAttr -r false -ci true -sn "paramFiberCosineWeightsG" -ln "param_fiber_cosine_weightsG" 
		-dv 0.5 -at "float" -p "param_fiber_cosine_weights";
	addAttr -r false -ci true -sn "paramFiberCosineWeightsB" -ln "param_fiber_cosine_weightsB" 
		-at "float" -p "param_fiber_cosine_weights";
	addAttr -r false -ci true -sn "paramUseGrowthPerlin" -ln "param_use_growth_perlin" 
		-ct "oslAttribute" -dv 1 -at "long";
	addAttr -r false -ci true -sn "paramGrowthPerlinBandCount" -ln "param_growth_perlin_band_count" 
		-ct "oslAttribute" -dv 3 -at "long";
	addAttr -r false -ci true -uac -sn "paramGrowthPerlinFrequencies" -ln "param_growth_perlin_frequencies" 
		-ct "oslAttribute" -at "float3" -nc 3;
	addAttr -r false -ci true -sn "paramGrowthPerlinFrequenciesR" -ln "param_growth_perlin_frequenciesR" 
		-dv 1 -at "float" -p "param_growth_perlin_frequencies";
	addAttr -r false -ci true -sn "paramGrowthPerlinFrequenciesG" -ln "param_growth_perlin_frequenciesG" 
		-dv 0.20000000298023224 -at "float" -p "param_growth_perlin_frequencies";
	addAttr -r false -ci true -sn "paramGrowthPerlinFrequenciesB" -ln "param_growth_perlin_frequenciesB" 
		-dv 0.076999999582767487 -at "float" -p "param_growth_perlin_frequencies";
	addAttr -r false -ci true -uac -sn "paramGrowthPerlinWeights" -ln "param_growth_perlin_weights" 
		-ct "oslAttribute" -at "float3" -nc 3;
	addAttr -r false -ci true -sn "paramGrowthPerlinWeightsR" -ln "param_growth_perlin_weightsR" 
		-dv 1 -at "float" -p "param_growth_perlin_weights";
	addAttr -r false -ci true -sn "paramGrowthPerlinWeightsG" -ln "param_growth_perlin_weightsG" 
		-dv 2 -at "float" -p "param_growth_perlin_weights";
	addAttr -r false -ci true -sn "paramGrowthPerlinWeightsB" -ln "param_growth_perlin_weightsB" 
		-dv 1 -at "float" -p "param_growth_perlin_weights";
	addAttr -r false -ci true -sn "paramUseDiffusePerlin" -ln "param_use_diffuse_perlin" 
		-ct "oslAttribute" -dv 1 -at "long";
	addAttr -r false -ci true -sn "paramDiffusePerlinBandCount" -ln "param_diffuse_perlin_band_count" 
		-ct "oslAttribute" -dv 3 -at "long";
	addAttr -r false -ci true -uac -sn "paramDiffusePerlinFrequencies" -ln "param_diffuse_perlin_frequencies" 
		-ct "oslAttribute" -at "float3" -nc 3;
	addAttr -r false -ci true -sn "paramDiffusePerlinFrequenciesR" -ln "param_diffuse_perlin_frequenciesR" 
		-dv 50 -at "float" -p "param_diffuse_perlin_frequencies";
	addAttr -r false -ci true -sn "paramDiffusePerlinFrequenciesG" -ln "param_diffuse_perlin_frequenciesG" 
		-dv 10 -at "float" -p "param_diffuse_perlin_frequencies";
	addAttr -r false -ci true -sn "paramDiffusePerlinFrequenciesB" -ln "param_diffuse_perlin_frequenciesB" 
		-dv 0.33000001311302185 -at "float" -p "param_diffuse_perlin_frequencies";
	addAttr -r false -ci true -uac -sn "paramDiffusePerlinWeights" -ln "param_diffuse_perlin_weights" 
		-ct "oslAttribute" -at "float3" -nc 3;
	addAttr -r false -ci true -sn "paramDiffusePerlinWeightsR" -ln "param_diffuse_perlin_weightsR" 
		-dv 0.25 -at "float" -p "param_diffuse_perlin_weights";
	addAttr -r false -ci true -sn "paramDiffusePerlinWeightsG" -ln "param_diffuse_perlin_weightsG" 
		-dv 0.20000000298023224 -at "float" -p "param_diffuse_perlin_weights";
	addAttr -r false -ci true -sn "paramDiffusePerlinWeightsB" -ln "param_diffuse_perlin_weightsB" 
		-dv 0.05000000074505806 -at "float" -p "param_diffuse_perlin_weights";
	addAttr -r false -ci true -sn "paramDiffusePerlinScaleZ" -ln "param_diffuse_perlin_scale_z" 
		-ct "oslAttribute" -dv 0.15000000596046448 -at "float";
	addAttr -r false -ci true -sn "paramUseEarlyWoodColorPerlin" -ln "param_use_early_wood_color_perlin" 
		-ct "oslAttribute" -dv 1 -at "long";
	addAttr -r false -ci true -sn "paramEarlycolorPerlinBandCount" -ln "param_earlycolor_perlin_band_count" 
		-ct "oslAttribute" -dv 2 -at "long";
	addAttr -r false -ci true -uac -sn "paramEarlycolorPerlinFrequencies" -ln "param_earlycolor_perlin_frequencies" 
		-ct "oslAttribute" -at "float3" -nc 3;
	addAttr -r false -ci true -sn "paramEarlycolorPerlinFrequenciesR" -ln "param_earlycolor_perlin_frequenciesR" 
		-dv 0.12300000339746475 -at "float" -p "param_earlycolor_perlin_frequencies";
	addAttr -r false -ci true -sn "paramEarlycolorPerlinFrequenciesG" -ln "param_earlycolor_perlin_frequenciesG" 
		-dv 0.33300000429153442 -at "float" -p "param_earlycolor_perlin_frequencies";
	addAttr -r false -ci true -sn "paramEarlycolorPerlinFrequenciesB" -ln "param_earlycolor_perlin_frequenciesB" 
		-at "float" -p "param_earlycolor_perlin_frequencies";
	addAttr -r false -ci true -uac -sn "paramEarlycolorPerlinWeights" -ln "param_earlycolor_perlin_weights" 
		-ct "oslAttribute" -at "float3" -nc 3;
	addAttr -r false -ci true -sn "paramEarlycolorPerlinWeightsR" -ln "param_earlycolor_perlin_weightsR" 
		-dv 0.30000001192092896 -at "float" -p "param_earlycolor_perlin_weights";
	addAttr -r false -ci true -sn "paramEarlycolorPerlinWeightsG" -ln "param_earlycolor_perlin_weightsG" 
		-dv 0.5 -at "float" -p "param_earlycolor_perlin_weights";
	addAttr -r false -ci true -sn "paramEarlycolorPerlinWeightsB" -ln "param_earlycolor_perlin_weightsB" 
		-at "float" -p "param_earlycolor_perlin_weights";
	addAttr -r false -ci true -sn "paramUseLateWoodColorPerlin" -ln "param_use_late_wood_color_perlin" 
		-ct "oslAttribute" -dv 1 -at "long";
	addAttr -r false -ci true -sn "paramLatecolorPerlinBandCount" -ln "param_latecolor_perlin_band_count" 
		-ct "oslAttribute" -dv 1 -at "long";
	addAttr -r false -ci true -uac -sn "paramLatecolorPerlinFrequencies" -ln "param_latecolor_perlin_frequencies" 
		-ct "oslAttribute" -at "float3" -nc 3;
	addAttr -r false -ci true -sn "paramLatecolorPerlinFrequenciesR" -ln "param_latecolor_perlin_frequenciesR" 
		-dv 0.22200000286102295 -at "float" -p "param_latecolor_perlin_frequencies";
	addAttr -r false -ci true -sn "paramLatecolorPerlinFrequenciesG" -ln "param_latecolor_perlin_frequenciesG" 
		-at "float" -p "param_latecolor_perlin_frequencies";
	addAttr -r false -ci true -sn "paramLatecolorPerlinFrequenciesB" -ln "param_latecolor_perlin_frequenciesB" 
		-at "float" -p "param_latecolor_perlin_frequencies";
	addAttr -r false -ci true -uac -sn "paramLatecolorPerlinWeights" -ln "param_latecolor_perlin_weights" 
		-ct "oslAttribute" -at "float3" -nc 3;
	addAttr -r false -ci true -sn "paramLatecolorPerlinWeightsR" -ln "param_latecolor_perlin_weightsR" 
		-dv 0.75 -at "float" -p "param_latecolor_perlin_weights";
	addAttr -r false -ci true -sn "paramLatecolorPerlinWeightsG" -ln "param_latecolor_perlin_weightsG" 
		-at "float" -p "param_latecolor_perlin_weights";
	addAttr -r false -ci true -sn "paramLatecolorPerlinWeightsB" -ln "param_latecolor_perlin_weightsB" 
		-at "float" -p "param_latecolor_perlin_weights";
	addAttr -r false -ci true -sn "paramCmScale" -ln "param_cm_scale" -ct "oslAttribute" 
		-dv 0.0099999997764825821 -at "float";
	addAttr -w false -uac -sn "paramCol" -ln "param_Col" -ct "oslAttribute" -at "float3" 
		-nc 3;
	addAttr -w false -sn "paramColR" -ln "param_ColR" -at "float" -p "param_Col";
	addAttr -w false -sn "paramColG" -ln "param_ColG" -at "float" -p "param_Col";
	addAttr -w false -sn "paramColB" -ln "param_ColB" -at "float" -p "param_Col";
	setAttr ".code" -type "string" (
		"//\n// PRISM Wood material shader, based on the RTSL implementation by Karl Schmidt\n// which in turn was based on work by Zhao Dong and Milos Hasan\n// OSL port by Zap Andersson\n//\n// Modified: 2021-09-23\n// Copyright 2021 Autodesk Inc, All rights reserved. This file is licensed under Apache 2.0 license\n//    https://github.com/ADN-DevTech/3dsMax-OSL-Shaders/blob/license/LICENSE.txt\n//\n\n///////////////////////////////////////////////////////////////////////////\n// Constants\n///////////////////////////////////////////////////////////////////////////\n\n#include <vector4.h>\n\n#define float4 vector4\n\n#define PI 3.14159265\n#define TWO_PI (2.0 * PI)\n#define HALF_PI (0.5 * PI)\n#define bool int\n#define true 1\n#define false 0\n\n#define PERFECT_SPECULAR_ROUGNESS 0.001\n\n#define WOOD_PORE_BOTH  0\n#define WOOD_PORE_EARLY 1\n#define WOOD_PORE_LATE  2\n\n///////////////////////////////////////////////////////////////////////////\n// Utility functions for RapidRT Prism shaders\n///////////////////////////////////////////////////////////////////////////\n"
		+ "\nfloat frac(float f)\n{\n\treturn f - floor(f);\n}\n\nfloat saturate(float x) \n{\n\treturn clamp(x,0.0,1.0);\n}\n\n\nfloat partialSmoothstep(float min, float max, float x){\n  float tmp = saturate((x - min) / (max - min));\n  if (tmp < 0.5) {\n    return tmp;\n  }\n  else {\n    return tmp*tmp*(3.0 - 2.0*tmp);\n  }\n}\n\n\n///////////////////////////////////////////////////////////////\n// Shader /////////////////////////////////////////////////////\nshader prism_wood \n[[ string label = \"Advanced Wood\",\n   string version = \"1.0.0\",\n   string help  = \"<h3>Advanced Wood</h3>\"\n                  \"This OSL shader is similar to the classic 'Advanced Wood', except the UVW \"\n\t\t\t\t  \"can be modified with external shaders, and a random seed can be driven \"\n\t\t\t\t  \"to have per-object variations.<br>\" \n\t\t\t\t  \"It is also fully supported in a 'High Quality' viewport and will<br>\"\n\t\t\t\t  \"render <i>exactly the same</i> on any OSL capable renderer.\"\n   ]]\n(\n  vector UVW = transform(\"object\", P),\n  int    Seed = 0,\n  //////////\n  // Wood //\n  //////////\n"
		+ "\n  float scale = 1.0        [[ string label = \"Scale\" ]],\n  int   unit_dependent = 1 [[ string label = \"Unit Dependent\", string widget = \"checkBox\", int connectable = 0]],\n  int   axis           = 2 [[ string label = \"Axis\", string widget = \"mapper\", string options = \"X:0|Y:1|Z:2\", int connectable = 0]],\n\n  // Wood Seasonal Growth Params\n  float     ring_thickness      =  0.75  [[ string label = \"Ring Thickness\",       float min = 0.01, float max = 1.0 ]],\n  float     late_wood_ratio      = 0.238 [[ string label = \"Late Wood Ratio\",      float min = 0.0,  float max = 1.0 ]],\n  float     early_wood_sharpness = 0.395 [[ string label = \"Early Wood Sharpness\", int connectable = 0, float min = 0.0,  float max = 1.0 ]],\n  float     late_wood_sharpness  = 0.109 [[ string label = \"Late Wood Sharpness\",  int connectable = 0, float min = 0.0,  float max = 1.0 ]],\n\n  // Wood Grain Color Params\n  color     early_wood_color           = color(0.2, 0.08, 0.024) [[ string label = \"Early Wood Color\" ]],\n  bool      use_manual_late_wood_color = false [[ string label = \"Use Manual Late Wood Color\", string widget = \"checkBox\", int connectable = 0  ]],\n"
		+ "  color     manual_late_wood_color     = color(0.217, 0.071, 0.01) [[ string label = \"Late Wood Color\" ]],\n  float     late_wood_color_power      = 1.07 [[ string label = \"Late Wood Color Power\", float min = 0.01,  float max = 5.0 ]],\n\n  // Wood Pores Params\n  bool      use_pores        = true  [[ string label = \"Use Pores\", string widget = \"checkBox\", int connectable = 0  ]],\n  int       pore_type        = 0     [[ string label = \"Pore Type\", int connectable = 0, string widget=\"mapper\", string options = \"Both:0|Early Only:1|Late Only:2\" ]],\n  float     pore_cell_dim    = 0.12  [[ string label = \"Pore Cell Dim\", int connectable = 0, float min = 0.01, float max = 3.0 ]],\n  float     pore_radius      = 0.04  [[ string label = \"Pore Radius\", int connectable = 0, float min = 0.01, float max = 2.0 ]],\n  float     pore_color_power = 2.13  [[ string label = \"Pore Color Power\", float min = 0.01, float max = 5.0 ]],\n  float     pore_depth       = 0.01  [[ string label = \"Pore Depth\", int connectable = 0, float min = 0.0,  float max = 1.0 ]],\n"
		+ "\n  // Wood Rays Params\n  bool      use_rays             = true  [[ string label = \"Use Rays\", string widget = \"checkBox\", int connectable = 0  ]],\n  float     ray_color_power      = 1.1   [[ string label = \"Ray Color Power\", float min = 0.01, float max = 5.0 ]],\n  float     ray_seg_length_z     = 5.0   [[ string label = \"Ray Seg Length Z\", int connectable = 0, float min = 0.01, float max = 10.0 ]],\n  int       ray_num_slices       = 160   [[ string label = \"Ray Num Slices\", int connectable = 0, int   min = 1,    int   max = 400  ]],\n  float     ray_ellipse_depth    = 2.0   [[ string label = \"Ray Ellipse Depth\", int connectable = 0, float min = 0.01, float max = 10.0 ]],\n  float     ray_ellipse_scale_x  = 0.2   [[ string label = \"Ray Ellipse Scale Z\", int connectable = 0, float min = 0.01, float max = 1.0 ]],\n  float     ray_ellipse_z2x      = 10.0  [[ string label = \"Ray Ellipse Z2X\", int connectable = 0, float min = 0.0,  float max = 40.0 ]],\n  float     ray_roughness        = 0.1   [[ string label = \"Ray Ellipse Roughness\", int connectable = 0, float min = 0.0,  float max = 1.0 ]],\n"
		+ "  float     ray_bump_depth       = 0.02  [[ string label = \"Ray Ellipse Bump Depth\", int connectable = 0, float min = 0.0,  float max = 1.0 ]],\n\n\n  // Wood Shading Params\n  float     roughness            = 0.25   [[ string label = \"Roughness\", float min = 0.0,  float max = 1.0 ]],\n  float     diffuse_lobe_weight  = 0.85   [[ string label = \"Diffuse Lobe Weight\" ]],\n  bool      use_late_wood_bump   = true   [[ string widget = \"checkBox\", int connectable = 0  ]],\n  float     late_wood_bump_depth = 0.002  [[ int connectable = 0, float min = 0.0,  float max = 1.0 ]],\n  bool      use_groove_roughness = true   [[ string widget = \"checkBox\", int connectable = 0  ]],\n  float     groove_roughness     = 0.19   [[ string label = \"Groove Roughness\" ]],\n\n\n  // Wood Distortion Noise Params. The actual noise profiles are hidden from the user and only come from the presets,\n  // just like the C++ Advanced Wood\n  bool      use_fiber_perlin         = true [[ string widget = \"checkBox\", int connectable = 0 ]],\n  int       fiber_perlin_band_count  = 3    [[ int connectable = 0, string widget=\"null\" ]],\n"
		+ "  float4     fiber_perlin_frequencies = {0.025, 0.05, 0.286, 0.0 } [[ int connectable = 0, string widget=\"null\" ]],\n  float4     fiber_perlin_weights     = {3.0, 1.0, 0.2, 0.0 }    [[ int connectable = 0, string widget=\"null\" ]],\n  \n  float     fiber_perlin_scale_z     = 0.3  [[ int connectable = 0, float min = 0.05, float max = 1.0 ]],\n  bool      use_fiber_cosine         = true [[ string widget = \"checkBox\", int connectable = 0  ]],\n  int       fiber_cosine_band_count  = 2    [[ int connectable = 0, string widget=\"null\" ]],\n  float4     fiber_cosine_frequencies = {0.067, 0.25, 0.0, 0.0} [[ int connectable = 0, string widget=\"null\" ]],\n  float4     fiber_cosine_weights     = {2.5, 0.5, 0.0, 0.0} [[ int connectable = 0, string widget=\"null\" ]],\n\n  // Wood Color Noise Params\n  bool      use_growth_perlin         = true[[ string widget = \"checkBox\", int connectable = 0  ]],\n  int       growth_perlin_band_count  = 3 [[ int connectable = 0, string widget=\"null\" ]],\n  float4     growth_perlin_frequencies = {1.0,  0.2, 0.077, 0.0} [[ int connectable = 0, string widget=\"null\" ]],\n"
		+ "  float4     growth_perlin_weights     = {1.0, 2.0, 1.0, 0.0} [[ int connectable = 0, string widget=\"null\" ]],\n\n  bool      use_diffuse_perlin      = true [[ string widget = \"checkBox\", int connectable = 0  ]],\n  int       diffuse_perlin_band_count  = 3 [[ int connectable = 0, string widget=\"null\" ]],\n  float4     diffuse_perlin_frequencies = {50.0, 10.0, 0.33, 0.0} [[ int connectable = 0, string widget=\"null\" ]],\n  float4     diffuse_perlin_weights     = {0.25, 0.2, 0.05, 0.0} [[ int connectable = 0, string widget=\"null\" ]],\n  float     diffuse_perlin_scale_z     = 0.15 [[ int connectable = 0, float min = 0.01,  float max = 1.0 ]],\n\n  bool      use_early_wood_color_perlin      = true [[ string widget = \"checkBox\", int connectable = 0  ]],\n  int       earlycolor_perlin_band_count  = 2 [[ int connectable = 0, string widget=\"null\" ]],\n  float4     earlycolor_perlin_frequencies = {0.123, 0.333, 0.0, 0.0} [[ int connectable = 0, string widget=\"null\" ]],\n  float4     earlycolor_perlin_weights     = {0.3, 0.5, 0.0, 0.0} [[ int connectable = 0, string widget=\"null\" ]],\n"
		+ "\n  bool      use_late_wood_color_perlin      = true [[ string widget = \"checkBox\", int connectable = 0  ]],\n  int       latecolor_perlin_band_count    = 1 [[ int connectable = 0, string widget=\"null\" ]],\n  float4     latecolor_perlin_frequencies  = {0.222, 0.0, 0.0, 0.0} [[ int connectable = 0, string widget=\"null\" ]],\n  float4     latecolor_perlin_weights      = {0.75, 0.0, 0.0, 0.0} [[ int connectable = 0, string widget=\"null\" ]],\n\n  // This parameter should not be set by the user and is just used as information to the shader about scene\n  // units, since it's defined in worldunits and should equal 1 cm.\n  float cm_scale = 0.01    [[ int worldunits = 1, int connectable=0, string widget=\"null\" ]],\n\n   \n  output color Col = 1 [[ string label = \"Diffuse Color\" ]],\n  output float Roughness = 0.2,\n  output float Bump = 0.0\n)\n{ \n\t// Needs to be limited for numerical stability\n\tint adjustedSeed = Seed & 0xfff;\n\n\t// NOTE: This is not the pixel-identical noise to the C++ implementation, it WILL be slightly different\n"
		+ "\tfloat woodnoise1(float x)\n\t{\n\t\treturn noise(\"perlin\", x)/2.0;\n\t}\n\t\n\tfloat woodnoise1(vector x)\n\t{\n\t\treturn noise(\"perlin\", x)/2.0;\n\t}\n\t\n\t\n\t// Evaluate a 1D -> 1D wood noise profile\n\t// The profile is treated as set of pairs (weight, wavelength), each representing a single band of\n\t// Perlin noise. All the bands are evaluated as \"weight * noise(frequency * x)\" and summed.\n\tfloat woodNoise1(float4 profile_frequencies, float4 profile_weights, int band_count, float x)\n\t{\n\t  float offset = 0.0;\n\t  if (band_count > 0) {\n\t    float frequency = profile_frequencies.x;\n\t    float weight = profile_weights.x;\n\t    offset += weight * woodnoise1(frequency * x);\n\t  }\n\t  if (band_count > 1) {\n\t    float frequency = profile_frequencies.y;\n\t    float weight = profile_weights.y;\n\t    offset += weight * woodnoise1(frequency * x);\n\t  }\n\t  if (band_count > 2) {\n\t    float frequency = profile_frequencies.z;\n\t    float weight = profile_weights.z;\n\t    offset += weight * woodnoise1(frequency * x);\n\t  }\n\t  if (band_count > 3) {\n\t    float frequency = profile_frequencies.w;\n"
		+ "\t    float weight = profile_weights.w;\n\t    offset += weight * woodnoise1(frequency * x);\n\t  }\n\t  return offset;\n\t}\n\t\n\t\n\t// Evaluate a 3D -> 1D wood noise profile.\n\t// The profile is treated as set of pairs (weight, wavelength), each representing a single band of\n\t// Perlin noise. All the bands are evaluated as \"weight * noise(frequency * x)\" and summed.\n\tfloat woodNoise1(float4 profile_frequencies, float4 profile_weights, int band_count, vector p)\n\t{\n\t  float offset = 0.0;\n\t  if (band_count > 0) {\n\t    float frequency = profile_frequencies.x;\n\t    float weight = profile_weights.x;\n\t    offset += weight * woodnoise1(frequency * p);\n\t  }\n\t  if (band_count > 1) {\n\t    float frequency = profile_frequencies.y;\n\t    float weight = profile_weights.y;\n\t    offset += weight * woodnoise1(frequency * p);\n\t  }\n\t  if (band_count > 2) {\n\t    float frequency = profile_frequencies.z;\n\t    float weight = profile_weights.z;\n\t    offset += weight * woodnoise1(frequency * p);\n\t  }\n\t  if (band_count > 3) {\n\t    float frequency = profile_frequencies.w;\n"
		+ "\t    float weight = profile_weights.w;\n\t    offset += weight * woodnoise1(frequency * p);\n\t  }\n\t  return offset;\n\t}\n\t\n\t\n\t// Evaluate a cosine noise profile\n\tvector cosineNoiseRadialDir(float4 profile_frequencies, float4 profile_weights, int band_count,\n\t                            vector p)\n\t{\n\t  float radius = length(vector(p[0],p[1],0.0));\n\t  if ( radius < 0.00001 ) {\n\t    return p;\n\t  }\n\t\n\t  float cos_theta = p[0] / radius;\n\t  float sin_theta = p[1] / radius;\n\t\n\t  float radius_shift = 0.0;\n\t  float angdist = p[2] / TWO_PI;\n\t  if (band_count > 0) {\n\t    float frequency = profile_frequencies.x;\n\t    float weight = profile_weights.x;\n\t    radius_shift += weight * cos(frequency * angdist);\n\t  }\n\t  if (band_count > 1) {\n\t    float frequency = profile_frequencies.y;\n\t    float weight = profile_weights.y;\n\t    radius_shift += weight * cos(frequency * angdist);\n\t  }\n\t  if (band_count > 2) {\n\t    float frequency = profile_frequencies.z;\n\t    float weight = profile_weights.z;\n\t    radius_shift += weight * cos(frequency * angdist);\n"
		+ "\t  }\n\t  if (band_count > 3) {\n\t    float frequency = profile_frequencies.w;\n\t    float weight = profile_weights.w;\n\t    radius_shift += weight * cos(frequency * angdist);\n\t  }\n\t\n\t  // Radius at which the distortion reaches full power.\n\t  float min_radius = 1.5;\n\t  float shift_weight = partialSmoothstep(0.0, min_radius, radius) * radius_shift;\n\t\n\t    vector p2 = p;\n\t  p2[0] += cos_theta * shift_weight;\n\t  p2[1] += sin_theta * shift_weight;\n\t\n\t  return p2;\n\t}\n\t\n\t\n\t// Wyvill kernel\n\tfloat wyvill(float r)\n\t{\n\t  float tmp = 1.0 - r*r;\n\t  return r >= 1.0 ? 0.0 : tmp * tmp * tmp;\n\t}\n\t\n\t\n\t// Wyvill kernel with pre-squared input.\n\tfloat wyvillsq(float rsq)\n\t{\n\t  float tmp = 1.0 - rsq;\n\t  return rsq >= 1.0 ? 0.0 : tmp * tmp * tmp;\n\t}\n\t\n  // The weight ratio of earlywood to latewood. 1.0 indicates all earlywood (lighter). 0.0\n  // indicates latewood (darker). Computes the radial position as a side-effect.\n  float computeEarlywoodRatio(vector position, output float radius)\n  {\n    // Seed is applied to radius\n    radius = length(vector(position[0],position[1],0.0))  + abs(adjustedSeed) * 20;\n"
		+ "\n    // Growth rate variation\n    if (use_growth_perlin) {    \n      radius += woodNoise1(growth_perlin_frequencies, growth_perlin_weights,\n          growth_perlin_band_count, radius);\n      if (radius < 0.0) {\n        radius = 0.0;\n      }\n    }\n\n    // ring_thickness will never be 0, this is worked around in PrismWood rtsi class.\n    float fraction = frac(radius/ring_thickness);\n\n    // As fraction goes from 0 to 1 the wood coloration sectors are\n    // Earlywood -> Fall -> Latewood -> Rise\n    // We find the transition points and handle each interval separately.\n    float fall_to_late_wood =  1.0 - late_wood_ratio;\n    float early_wood_to_fall = fall_to_late_wood * early_wood_sharpness;\n    float late_wood_to_rise = fall_to_late_wood + late_wood_ratio * late_wood_sharpness;\n\n    // Earlywood\n    if (fraction <= early_wood_to_fall) {\n      return 1.0;\n    }\n\n    // Fall\n    if (fraction <= fall_to_late_wood) {\n      float fall_width = fall_to_late_wood - early_wood_to_fall;\n      float t = 1.0 - (fraction - early_wood_to_fall) / fall_width;\n"
		+ "      return t;\n    }\n\n    // Latewood\n    if (fraction <= late_wood_to_rise) {\n      return 0.0;\n    }\n\n    // Rise\n    float rise_width = 1.0 - late_wood_to_rise;\n    float t = (fraction - late_wood_to_rise) / rise_width;\n    return t;\n  }\n\n\n  // The total wyvill impulse from the radial fibers (rays) at position P.\n  float weight3DRayImpulses(vector pin)\n  {\n    // The segment is the longitudinal subdivision. As rays can have a significant longitudinal\n    // extent we sum the effect of the ray in both the segment P is in (id_0) and the nearest other\n    // segment (id_1)\n\n\tvector p = pin;\n\tp[2] /= ray_ellipse_depth;\n\n    int segment_id_0 = int(floor(p[2] / ray_seg_length_z));\n    float factor = p[2] / ray_seg_length_z - float(segment_id_0);\n    int segment_id_1 = factor > 0.5 ? segment_id_0 + 1 : segment_id_0 - 1;\n    int segments[2] = { segment_id_0, segment_id_1 };\n\n    // The slice is the circumferential subidivison. Rays have very little circumferential extent so\n    // we simplify and only use the current slice. This will cause some clipping for rays near the\n"
		+ "    // edge of a slice.\n    float theta = atan2(p[1], p[0]);\n    int slice_id = int(floor((theta + PI) / TWO_PI * float(ray_num_slices)));\n    if (slice_id == ray_num_slices) {\n      slice_id--;\n    }\n\n    // Sum the impulses for the two segments affecting P.\n    float weight = 0.0;\n    for (int i = 0; i < 2; i++) {\n      int current_segment = segments[i];\n      vector hash = noise(\"hash\", slice_id + adjustedSeed * 39, current_segment);\n\n      // Each ray starts at a position randomly offset from the center of the tree, at most 5 cm.\n      float base_radial_offset = 5.0;\n      if (length(vector(p[0],p[1],0.0)) >= base_radial_offset * hash[0]) {\n\t      // The direction of the ray\n\t      float ray_theta = (float(slice_id) + hash[0]) / float(ray_num_slices) * TWO_PI - PI;\n\t      float ray_pos_z = (float(current_segment) + hash[1]) * ray_seg_length_z;\n\t      vector ray_direction = vector(cos(ray_theta), sin(ray_theta), 0.0);\n\t\n\t      // Scale down the Z component so the ray cross-section is circular.\n\t      vector p_normalized = p;\n"
		+ "\t      p_normalized[2] = (p_normalized[2] - ray_pos_z) / ray_ellipse_z2x;\n\t\n\t      // Compute the point to line distance and use it for the wyvill impulse.\n\t      float dist = length(cross(ray_direction, p_normalized)) / length(ray_direction);\n\t      weight += wyvill(dist / ray_ellipse_scale_x);\n\t  }\n    }\n\n    return weight;\n  }\n\n\n  // The total wyvil impulse of the longitudinal pores at position P.\n  float weight2DNeighborImpulses(vector p, float wood_weight)\n  {\n    if (wood_weight <= 0.0) {\n      return 0.0;\n    }\n    float pore_radius = pore_radius * wood_weight;\n\n    // Determine the boundaries of the pore cells that may affect us, given the pore radius and\n    // cell dimensions.\n    float x_left  = floor((p[0] - pore_radius) / pore_cell_dim);\n    float x_right = floor((p[0] + pore_radius) / pore_cell_dim);\n    float y_left  = floor((p[1] - pore_radius) / pore_cell_dim);\n    float y_right = floor((p[1] + pore_radius) / pore_cell_dim);\n\n    // Sum the wyvill impulse from all potentially contributing cells.\n"
		+ "    float weight = 0.0;\n    float inverse_radius_sq = 1.0 / (pore_radius * pore_radius);\n    for (int j = int(y_left); j <= int(y_right); j++) {\n      for (int i = int(x_left); i <= int(x_right); i++) {\n        // Determine where the pore is in the cell\n        vector offset =  noise(\"cell\", i + adjustedSeed * 37, j);\n        offset[2] = 0.0;\n        vector pore_position = (offset + vector(float(i), float(j), 0.0)) * pore_cell_dim;\n\n        // Compute the distance to the pore and use it for the wyvill impulse.\n        vector diff = pore_position - vector(p[0],p[1],0.0);\n        float diff_sq = dot(diff, diff);\n        weight += wyvillsq(diff_sq * inverse_radius_sq);\n      }\n    }\n\n    return weight;\n  }\n\n\n  // Darken a color based on whether or not that color is on a pore.\n  color darkenColorWithPores(vector position, color col, float wood_weight,\n  \tfloat pore_color_power,\n  \tfloat pore_radius,\n  \tfloat pore_cell_dim )\n  {\n    float pores_weight = weight2DNeighborImpulses(position, wood_weight);\n    float exponent = (pore_color_power - 1.0) * pores_weight + 1.0;\n"
		+ "    return pow(col, exponent);\n  }\n\n\n  // Darken a color based on whether or not that color is on a ray.\n  color darkenColorWithRays(vector position, color col, output float weight)\n  {\n    weight = weight3DRayImpulses(position);\n    float exponent = (ray_color_power - 1.0) * weight + 1.0;\n    return pow(col, exponent);\n  }\n\n\n  // Compute the diffuse color.\n  color computeDiffuseColor(vector position, float early_wood_ratio, float radius, output float rayweight)\n  {\n    // Earlywood color\n    color early_color = early_wood_color;\n    if (use_early_wood_color_perlin) {\n      float exponent = 1.0 + woodNoise1(earlycolor_perlin_frequencies, earlycolor_perlin_weights, earlycolor_perlin_band_count, radius);\n      early_color = pow(early_color, exponent);\n    }\n\n    // Latewood color\n    color late_wood_color = pow(early_color, late_wood_color_power);\n    if (use_manual_late_wood_color) {\n      late_wood_color = manual_late_wood_color;\n    }\n    if (use_late_wood_color_perlin) {\n      float exponent = 1.0 + woodNoise1(latecolor_perlin_frequencies, latecolor_perlin_weights, latecolor_perlin_band_count, radius);\n"
		+ "      late_wood_color = pow(late_wood_color, exponent);\n    }\n\n    // Overall diffuse albedo\n    color diffuse_albedo =\n        early_wood_ratio * early_color + (1.0 - early_wood_ratio) * late_wood_color;\n    if (use_diffuse_perlin) {\n      vector diffuse_position = position;\n      diffuse_position[2] = diffuse_position[2] * diffuse_perlin_scale_z;\n      float exponent = 1.0 + woodNoise1(diffuse_perlin_frequencies, diffuse_perlin_weights, diffuse_perlin_band_count, diffuse_position);\n      diffuse_albedo = pow(diffuse_albedo, exponent);\n    }\n\n    // Darkening from pores\n    if (use_pores) {\n      if (pore_type == WOOD_PORE_EARLY) {\n        diffuse_albedo = darkenColorWithPores(position, diffuse_albedo, early_wood_ratio,\n\t\t\tpore_color_power,\n\t\t\tpore_radius,\n\t\t\tpore_cell_dim);\n      }\n      else if (pore_type == WOOD_PORE_LATE) {\n        diffuse_albedo = darkenColorWithPores(position, diffuse_albedo, 1.0 - early_wood_ratio,\n\t\t\tpore_color_power,\n\t\t\tpore_radius,\n\t\t\tpore_cell_dim);\n      }\n      else {\n        diffuse_albedo = darkenColorWithPores(position, diffuse_albedo, 1.0,\n"
		+ "\t\t\tpore_color_power,\n\t\t\tpore_radius,\n\t\t\tpore_cell_dim);\n      }\n    }\n\n    // Darkening from rays\n    if (use_rays) {\n      diffuse_albedo = darkenColorWithRays(position, diffuse_albedo, rayweight);\n    }\n    return diffuse_albedo;\n  }\n\n\n  // Apply radial cosine distortion to P.\n  vector distort3DCosineRadialDir(vector p)\n  {\n    return cosineNoiseRadialDir(fiber_cosine_frequencies, fiber_cosine_weights,\n        fiber_cosine_band_count, p);\n  }\n\n\n  // Apply Perlin distortion to P.\n  vector distort3DPerlin(vector pp)\n  {\n  \tvector p = pp;\n    vector p_aniso = p;\n    p_aniso[2] *= fiber_perlin_scale_z;\n\n    float radial_shift = woodNoise1(fiber_perlin_frequencies, fiber_perlin_weights,\n        fiber_perlin_band_count, p_aniso);\n    p[0] += radial_shift;\n    p[1] += radial_shift;\n\n    return p;\n  }\n\n    // Apply distortion to P.\n  vector distort(vector pi)\n  {\n  \tvector p = pi;\n    // Cosine distortion\n    if (use_fiber_cosine) {\n      p = distort3DCosineRadialDir(p);\n    }\n\n    // Perlin distortion\n    if (use_fiber_perlin) {\n"
		+ "      p = distort3DPerlin(p);\n    }\n\n    return p;\n  }\n\n  // Compute the height differential from pores\n  float heightFrom2DNeighborPores(vector position, float wood_weight)\n  {\n    // Assume pores are always negative depth\n    return -weight2DNeighborImpulses(position, wood_weight) * pore_depth;\n  }\n\n\n  // Compute the total bump displacement\n  float heightVarForBump(vector position)\n  {\n    float height = 0.0;\n\n    // compute the weight from the early wood ratio\n    float radius;\n    float early_wood_ratio = computeEarlywoodRatio(position, radius);\n\n    // consider pores case\n    if (use_pores) {\n      if (pore_type == WOOD_PORE_EARLY) {\n        height += heightFrom2DNeighborPores(position, early_wood_ratio);\n      }\n      else if (pore_type == WOOD_PORE_LATE) {\n        height += heightFrom2DNeighborPores(position, 1.0 - early_wood_ratio);\n      }\n      else {\n        height += heightFrom2DNeighborPores(position, 1.0);\n      }\n    }\n\n    if (use_late_wood_bump) {\n      height += (1.0 - early_wood_ratio) * late_wood_bump_depth;\n"
		+ "    }\n\n    return height;\n  }\n\n  // Compute treespace coordinates, pick the right axis\n  vector tree_position = UVW;\n  if (axis == 0)  // Adjust for X axis\n  {\n\ttree_position[0] = UVW[1];\n\ttree_position[1] = UVW[2];\n\ttree_position[2] = UVW[0];\n  }\n  else if (axis == 1) // Adjust for Y axis\n  {\n\ttree_position[0] = UVW[2];\n\ttree_position[1] = UVW[0];\n\ttree_position[2] = UVW[1];\n  }\n\n  // Apply the scale\n  if (scale != 0.0)\n  {\n    float scaling = scale;\n\tif (unit_dependent)\n\t{\n\t\t// Scale by the unit-dependent parameter. This should not be set by the user\n\t\t// and always be kept at whatever value matches 1 centimeter in the current\n\t\t// scene units\n\t\tscaling *= cm_scale;\n\t}\n\ttree_position /= scaling;\n  }\n \n    // Apply random seed simply as a Z shift, simplifies all the noise\n    // calls and is just as \"random\". Seed still applies to the cellnoise \n    // functions that are not position driven.\n    tree_position[2] += adjustedSeed * 50;\n    \n    vector tree_distorted_position = distort(tree_position);\n\n\tBump = heightVarForBump(tree_distorted_position);\n"
		+ "    \n    float surface_roughness = roughness;\n\n    // Wood lobe\n    float radius;\n\n    float early_wood_ratio = computeEarlywoodRatio(tree_distorted_position, radius);\n\n    // Compute the surface roughness\n    if (use_groove_roughness) {\n      surface_roughness =\n          early_wood_ratio * groove_roughness + (1.0 - early_wood_ratio) * surface_roughness;\n    }\n    float lobe_roughness = surface_roughness;\n\n    // Wood diffuse lobe.\n\tfloat rayweight = 0.0;\n    vector wood_f0 = vector(0.04);\n    vector diffuse_color =\n        computeDiffuseColor(tree_distorted_position, early_wood_ratio, radius, rayweight) * diffuse_lobe_weight;\n\n\t// Output\n\tCol       = diffuse_color;\n\tRoughness = clamp(lobe_roughness + ray_roughness * rayweight, 0.0, 1.0);\n\tBump      -= rayweight * ray_bump_depth;\n}\n");
	setAttr ".cmpSts" 1;
createNode aiOptions -s -n "defaultArnoldRenderOptions";
	rename -uid "FB399D80-0000-D412-61E1-41FA000002C0";
	addAttr -ci true -sn "ARV_options" -ln "ARV_options" -dt "string";
	setAttr ".version" -type "string" "5.0.0.2";
	setAttr ".ARV_options" -type "string" "Test Resolution=100%;Camera=perspShape;Color Management.Gamma=1;Color Management.Exposure=0;Background.BG=BG Color;Background.Color=0 0 0;Background.Image=;Background.Scale=1 1;Background.Offset=0 0;Background.Apply Color Management=1;Foreground.Enable FG=0;Foreground.Image=;Foreground.Scale=1 1;Foreground.Offset=0 0;Foreground.Apply Color Management=1;";
createNode aiAOVFilter -s -n "defaultArnoldFilter";
	rename -uid "FB399D80-0000-D412-61E1-41FA000002C1";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVDriver -s -n "defaultArnoldDriver";
	rename -uid "FB399D80-0000-D412-61E1-41FA000002C2";
	setAttr ".color_management" 1;
	setAttr ".ai_translator" -type "string" "tif";
createNode aiAOVDriver -s -n "defaultArnoldDisplayDriver";
	rename -uid "FB399D80-0000-D412-61E1-41FA000002C3";
	setAttr ".output_mode" 0;
	setAttr ".ai_translator" -type "string" "maya";
createNode aiStandardSurface -n "aiStandardSurface1";
	rename -uid "FB399D80-0000-D412-61E1-4237000002C5";
createNode shadingEngine -n "aiStandardSurface1SG";
	rename -uid "FB399D80-0000-D412-61E1-4237000002C6";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
	rename -uid "FB399D80-0000-D412-61E1-4237000002C7";
createNode polyPlane -n "polyPlane1";
	rename -uid "FB399D80-0000-D412-61E1-4251000002C8";
	setAttr ".cuv" 2;
createNode script -n "sceneConfigurationScriptNode";
	rename -uid "FB399D80-0000-D412-61E1-42E1000002CE";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 120 -ast 1 -aet 200 ";
	setAttr ".st" 6;
createNode nodeGraphEditorInfo -n "hyperShadePrimaryNodeEditorSavedTabsInfo";
	rename -uid "FB399D80-0000-D412-61E1-42E1000002CF";
	setAttr ".tgi[0].tn" -type "string" "Untitled_1";
	setAttr ".tgi[0].vl" -type "double2" -334.92062161208514 -503.9682339423556 ;
	setAttr ".tgi[0].vh" -type "double2" 370.63490590721273 346.03173228168038 ;
	setAttr -s 4 ".tgi[0].ni";
	setAttr ".tgi[0].ni[0].x" 52.539676666259766;
	setAttr ".tgi[0].ni[0].y" 79.365074157714844;
	setAttr ".tgi[0].ni[0].nvs" 2579;
	setAttr ".tgi[0].ni[1].x" -253.96824645996094;
	setAttr ".tgi[0].ni[1].y" 119.04761505126953;
	setAttr ".tgi[0].ni[1].nvs" 18306;
	setAttr ".tgi[0].ni[2].x" 360.31744384765625;
	setAttr ".tgi[0].ni[2].y" -109.5238037109375;
	setAttr ".tgi[0].ni[2].nvs" 1923;
	setAttr ".tgi[0].ni[3].x" -74.285713195800781;
	setAttr ".tgi[0].ni[3].y" 307.14285278320312;
	setAttr ".tgi[0].ni[3].nvs" 1923;
createNode script -n "uiConfigurationScriptNode";
	rename -uid "0A405D80-0000-5859-61F7-C076000002B4";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $nodeEditorPanelVisible = stringArrayContains(\"nodeEditorPanel1\", `getPanel -vis`);\n\tint    $nodeEditorWorkspaceControlOpen = (`workspaceControl -exists nodeEditorPanel1Window` && `workspaceControl -q -visible nodeEditorPanel1Window`);\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\n\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"|top|topShape\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 32768\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n"
		+ "            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n"
		+ "            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n"
		+ "\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"|side|sideShape\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n"
		+ "            -textureDisplay \"modulate\" \n            -textureMaxSize 32768\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n"
		+ "            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n"
		+ "            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"|front|frontShape\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n"
		+ "            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 32768\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n"
		+ "            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n"
		+ "            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"|persp|perspShape\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n"
		+ "            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 32768\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n"
		+ "            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n"
		+ "            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 2018\n            -height 1384\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"ToggledOutliner\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\toutlinerPanel -edit -l (localizedPanelLabel(\"ToggledOutliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -docTag \"isolOutln_fromSeln\" \n            -showShapes 0\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 1\n            -showReferenceMembers 1\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -organizeByClip 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 0\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showParentContainers 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n"
		+ "            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -isSet 0\n            -isSetMember 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n"
		+ "            -renderFilterVisible 0\n            -renderFilterIndex 0\n            -selectionOrder \"chronological\" \n            -expandAttribute 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 0\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 0\n            -showReferenceMembers 0\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -organizeByClip 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n"
		+ "            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showParentContainers 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n"
		+ "            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n"
		+ "                -showMuteInfo 0\n                -organizeByLayer 1\n                -organizeByClip 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showParentContainers 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n"
		+ "                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayValues 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showPlayRangeShades \"on\" \n                -lockPlayRangeShades \"off\" \n"
		+ "                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -keyMinScale 1\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -valueLinesToggle 1\n                -highlightAffectedCurves 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n"
		+ "                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -organizeByClip 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showParentContainers 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n"
		+ "                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayValues 0\n"
		+ "                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"timeEditorPanel\" (localizedPanelLabel(\"Time Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Time Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n"
		+ "            clipEditor -e \n                -displayValues 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayValues 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n"
		+ "                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n"
		+ "\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"nodeEditorPanel\" (localizedPanelLabel(\"Node Editor\")) `;\n\tif ($nodeEditorPanelVisible || $nodeEditorWorkspaceControlOpen) {\n\t\tif (\"\" == $panelName) {\n\t\t\tif ($useSceneConfig) {\n\t\t\t\t$panelName = `scriptedPanel -unParent  -type \"nodeEditorPanel\" -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -connectNodeOnCreation 0\n                -connectOnDrop 0\n                -copyConnectionsOnPaste 0\n                -connectionStyle \"bezier\" \n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n"
		+ "                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -crosshairOnEdgeDragging 0\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -editorMode \"default\" \n                -hasWatchpoint 0\n                $editorName;\n\t\t\t}\n\t\t} else {\n\t\t\t$label = `panel -q -label $panelName`;\n\t\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -connectNodeOnCreation 0\n                -connectOnDrop 0\n"
		+ "                -copyConnectionsOnPaste 0\n                -connectionStyle \"bezier\" \n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -crosshairOnEdgeDragging 0\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -editorMode \"default\" \n                -hasWatchpoint 0\n                $editorName;\n\t\t\tif (!$useSceneConfig) {\n\t\t\t\tpanel -e -l $label $panelName;\n\t\t\t}\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"createNodePanel\" (localizedPanelLabel(\"Create Node\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"shapePanel\" (localizedPanelLabel(\"Shape Editor\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tshapePanel -edit -l (localizedPanelLabel(\"Shape Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"posePanel\" (localizedPanelLabel(\"Pose Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tposePanel -edit -l (localizedPanelLabel(\"Pose Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"profilerPanel\" (localizedPanelLabel(\"Profiler Tool\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Profiler Tool\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"contentBrowserPanel\" (localizedPanelLabel(\"Content Browser\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Content Browser\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-userCreated false\n\t\t\t\t-defaultImage \"vacantCell.xP:/\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 32768\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -controllers 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 2018\\n    -height 1384\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 32768\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -controllers 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 2018\\n    -height 1384\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
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
	setAttr -s 6 ".s";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
select -ne :defaultRenderingList1;
select -ne :lightList1;
select -ne :initialShadingGroup;
	setAttr ".ro" yes;
select -ne :initialParticleSE;
	setAttr ".ro" yes;
select -ne :defaultRenderGlobals;
	addAttr -ci true -h true -sn "dss" -ln "defaultSurfaceShader" -dt "string";
	setAttr ".ren" -type "string" "arnold";
	setAttr ".outf" 51;
	setAttr ".imfkey" -type "string" "tif";
	setAttr ".ifp" -type "string" "testrender";
	setAttr ".dss" -type "string" "lambert1";
select -ne :defaultResolution;
	setAttr ".w" 160;
	setAttr ".h" 120;
	setAttr ".pa" 1.3329999446868896;
select -ne :defaultLightSet;
select -ne :defaultColorMgtGlobals;
	setAttr ".cfe" no;
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
connectAttr "polyPlane1.out" "pPlaneShape1.i";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandardSurface1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandardSurface1SG.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr ":defaultArnoldDisplayDriver.msg" ":defaultArnoldRenderOptions.drivers"
		 -na;
connectAttr ":defaultArnoldFilter.msg" ":defaultArnoldRenderOptions.filt";
connectAttr ":defaultArnoldDriver.msg" ":defaultArnoldRenderOptions.drvr";
connectAttr "aiOslShader1.out" "aiStandardSurface1.base_color";
connectAttr "aiStandardSurface1.out" "aiStandardSurface1SG.ss";
connectAttr "pPlaneShape1.iog" "aiStandardSurface1SG.dsm" -na;
connectAttr "aiStandardSurface1SG.msg" "materialInfo1.sg";
connectAttr "aiStandardSurface1.msg" "materialInfo1.m";
connectAttr "aiStandardSurface1.msg" "materialInfo1.t" -na;
connectAttr "aiStandardSurface1.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[0].dn"
		;
connectAttr "aiOslShader1.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[1].dn"
		;
connectAttr "aiStandardSurface1SG.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[2].dn"
		;
connectAttr "aiSkyDomeLightShape1.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[3].dn"
		;
connectAttr "aiStandardSurface1SG.pa" ":renderPartition.st" -na;
connectAttr "aiStandardSurface1.msg" ":defaultShaderList1.s" -na;
connectAttr "aiOslShader1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "aiSkyDomeLightShape1.ltd" ":lightList1.l" -na;
connectAttr "aiSkyDomeLight1.iog" ":defaultLightSet.dsm" -na;
// End of test.ma
