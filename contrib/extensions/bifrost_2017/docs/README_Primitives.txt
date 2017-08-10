BifrostPrimitives:
-------------------

- Procedural Geo

- It can load liquid/aero particle and foam caches

- Supports Ripoints, RiCurves, RiBlobby, RiSpheres as output

- Takes in a renderType, skip, radius, velocityMultiplier, useDensityGradientAsNormal
    - renderTypes: 0:points, 1:sphere, 2:Blob, 3:Curve
    - skip is an integer if you enter 10, you will get approximately 1/10th the particles.
    - useDensityGradientAsNormal only works for Ripoints and is an integer: 0 to turn off, 1 to turn on

- Outputs motion blocks if shutter min max values are not same. RiCurves does not need motion blur turned on as they are already shaped according to the velocity vector from sim.

- For curves, please add below to the rib stream:
    - Attribute "dice" "int roundcurve" [1] "hair" [1]

- Here are some specs for 10mil particles

                Particles:          RenderTime:         Memory:
RiSpheres       10million           5mins               16Gigs
RiCurves        10million           2mins               5Gigs   
RiPoints        10million           1mins               3Gigs
RiBlobby        10million           10mins              8Gigs

- Typical call:
Procedural "DynamicLoad" [ "BifrostPrimitives" "3 density 1.0 0 0    0.25 1.0 1 1000000    0.2 1   0 0 100 1 1 1    5 1 1 0.0 0 0.5 1 0.0 1.0   0   vorticity,speed,mpeed    bifs/voxel_liquid-volume.1000.bif"] [-100000 100000 -100000 100000 -100000 100000]

- Typical primvar usage rib:

Pattern "PxrPrimvar" "PxrPrimvar1" "string varname" ["bifrostVorticity"] "string type" ["float"] "string coordsys" [""] "__instanceid" ["PxrPrimvar1_0"]
Pattern "PxrMultiplyDivide" "multiplyDivide1" "int operation" [2] "reference color input1" ["PxrPrimvar1:resultRGB"] "color input2" [100 100 100] "__instanceid" ["multiplyDivide1_0"]
Pattern "PxrSeExpr" "clamp2" "vector min" [0 0 0] "vector max" [1 1 1] "reference color input" ["multiplyDivide1:output"] "string expression" ["resultRGB=clamp(input,min,max);resultRGB"] "__instanceid" ["clamp2_0"]
Pattern "PxrToFloat" "clamp2_output_0" "reference color input" ["clamp2:resultRGB"] "int mode" [0] "__instanceid" ["clamp2_output_0_0"]
Pattern "PxrMayaFloatToManifold" "Ramp1_uvCoord" "reference float uCoord" ["clamp2_output_0:resultF"] "reference float vCoord" ["clamp2_output_0:resultF"] "__instanceid" ["Ramp1_uvCoord_0"]
Pattern "PxrMayaRamp" "Ramp1" "int type" [0] "int interpolation" [1] "float[2] position" [0.485149 1] "color[2] color" [0.2 0.5 1.0 1 1 1] "float uWave" [0] "float vWave" [0] "float noise" [0] 
	"float noiseFreq" [0.5] "float hueNoise" [0] "float hueNoiseFreq" [0.5] "float satNoise" [0] "float satNoiseFreq" [0.5] "float valNoise" [0] "float valNoiseFreq" [0.5] "color defaultColor" [0.214041 0.214041 0.214041] "color colorGain" [0.370148 0.370148 0.370148] 
	"float alphaGain" [1] "float alphaOffset" [0] "int alphaIsLuminance" [0] "float filter" [1] "float filterOffset" [0] "int invert" [0] "reference struct manifold" ["Ramp1_uvCoord:result"] "int rman__flipU" [0] "int rman__flipV" [0] "__instanceid" ["Ramp1_0"]
Bxdf "PxrDiffuse" "BifrostColorSets" "reference color diffuseColor" ["Ramp1:outColor"] "float presence" [1] "__instanceid" ["BifrostColorSets_0"]

- Parameters:
	- renderType - what type of primitive to output
		- Katana:
			- Group: GlobalParameters
			- Attribute Name: Render Type
			- Type: setHintString("{'widget':'mapper', 'options':'Points:0|Spheres:1|Blobs:2|Tubes:3'}")
			- Default: Points

			
	- channelName - which channel is used for density calculation, usually density.
		- Katana:
			- Group: GlobalParameters/RenderChannelParameters
			- Attribute Name: RenderChannelName
			- Type: String input
			- Default: density
	- channelMult - render channel value multiplier. This will multiply the values from the bif and use that instead of the original values.
		- Katana:
			- Group: GlobalParameters/RenderChannelParameters
			- Attribute Name: RenderChannelValueMultiplier
			- Type: Float Numeric Input
			- Default: 1.0
	- useChannelGradientAsNormal - should we use channel gradient as normal per primitive
		- Katana:
			- Group: GlobalParameters/RenderChannelParameters
			- Attribute Name: UseGradientAsNormal
			- Type: setHintString("{'widget':'checkBox'}")
			- Default: off
	- exportNormalAsPrimvar - this decides how do we name the normal primvar. Off: N On:gradientN.
		This way we can still have auto-oriented RiPoints but can use a normal for other shading purposes if we have useChannelGradientAsNormal turned on.
		- Katana:
			- Group: GlobalParameters/RenderChannelParameters
			- Attribute Name: exportNormalAsPrimvar
			- Type: setHintString("{'widget':'checkBox'}")
			- Default: off


	- velMul - velocity vector multiplier
		- Katana:
			- Group: GlobalParameters
			- Attribute Name: VelocityMultiplier
			- Type: Float Numeric Input
			- Default: 0.04166
	- spaceScale - space scalar, assumes scaling around origin
		- Katana:
			- Group: GlobalParameters
			- Attribute Name: SpaceScale
			- Type: Float Numeric Input
			- Default: 1.0
	- skip - we take "every skip value" particle into account. So if skip is 10 we will export one in every 10 particles hence 1/10th the total number of particles.
		- Katana:
			- Group: GlobalParameters
			- Attribute Name: Skip
			- Type: setHintString("{'int':'True'}")
			- Default: 1
	- chunkSize - the export process subdivides the number of particles into chunkSize chunks so that things are not exported all at once.
		- Katana:
			- Group: GlobalParameters
			- Attribute Name: ChunkSize
			- Type: setHintString("{'int':'True'}")
			- Default: 10000000



	- clipBoxOn: clip sim to the coordinates of the box specified with below parameters.
		- Katana:
			- Group: GlobalParameters/Clipping
			- Attribute Name: On
			- Type: setHintString("{'widget':'checkBox'}")
			- Default: off

	- clipBoxMinX: Minimum X coordinate of clipbox.
		- Katana:
			- Group: GlobalParameters/Clipping
			- Attribute Name: MinimumX
			- Type: Float Numeric Input
			- Default: 0.0

	- clipBoxMaxX: Maximum X coordinate of clipbox.
		- Katana:
			- Group: GlobalParameters/Clipping
			- Attribute Name: MaximumX
			- Type: Float Numeric Input
			- Default: 1.0
		
	- clipBoxMinY: Minimum Y coordinate of clipbox.
		- Katana:
			- Group: GlobalParameters/Clipping
			- Attribute Name: MinimumY
			- Type: Float Numeric Input
			- Default: -1.0

	- clipBoxMaxY: Maximum Y coordinate of clipbox.
		- Katana:
			- Group: GlobalParameters/Clipping
			- Attribute Name: MaximumY
			- Type: Float Numeric Input
			- Default: 0.0

	- clipBoxMinZ: Minimum Z coordinate of clipbox.
		- Katana:
			- Group: GlobalParameters/Clipping
			- Attribute Name: MinimumZ
			- Type: Float Numeric Input
			- Default: 0.0

	- clipBoxMaxZ: Maximum Z coordinate of clipbox.
		- Katana:
			- Group: GlobalParameters/Clipping
			- Attribute Name: MaximumZ
			- Type: Float Numeric Input
			- Default: 1.0			



	- radius - Radius of the particles
		- Katana:
			- Group: GlobalParameters/RadiusParameters
			- Attribute Name: Radius
			- Type: Float Numeric Input
			- Default: 0.05
	- useChannelToModulateRadius - are we using the renderChannel value to change the radius? If yes we multiply the radius with its corresponding renderChannel value for each particle.
		- Katana:
			- Group: GlobalParameters/RadiusParameters
			- Attribute Name: useRenderChannelToModulateRadius
			- Type: setHintString("{'widget':'checkBox'}")
			- Default: on
	- camRadiusOn - apply camera based radius?
		- Katana:
			- Group: GlobalParameters/RadiusParameters/CameraBasedRadius
			- Attribute Name: Activate
			- Type: setHintString("{'widget':'checkBox'}")
			- Default: off
	- camRadiusStartDistance - at which distance we start to apply camera based radius
		- Katana:
			- Group: GlobalParameters/RadiusParameters/CameraBasedRadius
			- Attribute Name: StartDistance
			- Type: Float Numeric Input
			- Default: 0.0
	- camRadiusEndDistance - at which distance we end to apply camera based radius
		- Katana:
			- Group: GlobalParameters/RadiusParameters/CameraBasedRadius
			- Attribute Name: EndDistance
			- Type: Float Numeric Input
			- Default: 100.0
	- camRadiusStartFactor - what is the multiplier for radius for distances at and smaller than startRadius
		- Katana:
			- Group: GlobalParameters/RadiusParameters/CameraBasedRadius
			- Attribute Name: StartFactor
			- Type: Float Numeric Input
			- Default: 1.0
	- camRadiusEndFactor - what is the multiplier for radius for distances at and greater than endRadius
		- Katana:
			- Group: GlobalParameters/RadiusParameters/CameraBasedRadius
			- Attribute Name: EndFactor
			- Type: Float Numeric Input
			- Default: 1.0
	- camRadiusFactorExponent - exponent for interpolation 1=linear, less than one slower change, greater than one faster change
		- Katana:
			- Group: GlobalParameters/RadiusParameters/CameraBasedRadius
			- Attribute Name: FactorExponent
			- Type: Float Numeric Input
			- Default: 1.0
			

	- mpSamples - number of multipoint particles per particle
		- Katana:
			- Group: MultiPointing
			- Attribute Name: Samples
			- Type: setHintString("{'int':'True'}")
			- Default: 1
	- mpMinRadius - Min Radius for sample distribution per particle - particles distributed within a sphere with a random radius per particle between min and max radius
		- Katana:
			- Group: MultiPointing
			- Attribute Name: MinRadius
			- Type: Float Numeric Input
			- Default: 1.0
	- mpMaxRadius - Max Radius for sample distribution per particle - particles distributed within a sphere with a random radius per particle between min and max radius
		- Katana:
			- Group: MultiPointing
			- Attribute Name: MinRadius
			- Type: Float Numeric Input
			- Default: 1.0
	- mpSurfaceAttract - surface attraction for distributed particles - how close the distributed particles will be to the sphere, 1 means all particles will be on the surface
		- Katana:
			- Group: MultiPointing
			- Attribute Name: SurfaceAttract
			- Type: Float Numeric Input
			- Default: 0.0
	- mpFallofType - is there any falloff applied to the channel input depending on the distance between distributed particle and the original particle
		- Katana:
			- Group: MultiPointing
			- Attribute Name: FalloffType
			- Type: setHintString("{'widget':'mapper', 'options':'Linear:0|Smooth:1|Smoother:2|None:3'}")
			- Default: Linear
	- mpFalloffStart - where do we start applying falloff depending on the ratio of distance / radius
		- Katana:
			- Group: MultiPointing
			- Attribute Name: FalloffStartPosition
			- Type: setHintString("{'slider': 'True', 'slidermax': '1', 'slidermin': '0'}")
			- Default: 0.5
	- mpFalloffend - where do we end applying falloff depending on the ratio of distance / radius
		- Katana:
			- Group: MultiPointing
			- Attribute Name: FalloffEndPosition
			- Type: setHintString("{'slider': 'True', 'slidermax': '1', 'slidermin': '0'}")
			- Default: 1.0
	- mpDisplacementValue - displacement applied to the distributed particles
		- Katana:
			- Group: MultiPointing
			- Attribute Name: DisplacementValue
			- Type: Float Numeric Input
			- Default: 0.0
	- mpDisplacementNoiseFrequency - frequency of the noise for displacement
		- Katana:
			- Group: MultiPointing
			- Attribute Name: DisplacementNoiseFrequency
			- Type: Float Numeric Input
			- Default: 1.0


	- DEBUG - turns verbose output on off
		- Default should be off
		- Katana:
			- Group: ExpertParameters
			- Attribute Name: Verbose Output
			- Type: checkbox
			- Default: off


	- primVarNames - a string input for comma sperated list of primvar names. You can also use "speed" as a special parameter which is calculated as magnitude of velocity.
	    If there is no primvars specified by the user, "none" should be output to RIB.
		- Default should be vorticity?
		- Katana:
			- Group: PrimVars
			- Attribute Name: Output PrimVar Names
			- Type: string input
			- Default: vorticity

	- inFile - input bifrost cache file
