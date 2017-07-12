Volume Density Extractor:
--------------------------------

This plugin is used for volumerendering liquid/aero/foam particle/voxels caches. If the input is a volume cache it looks up the the channel specified for volume "density". If the input is a particle cache the plugin
splats particles into a voxel representation for volume rendering.
Please look down for example RIB CALL!

Main features
- Motion blur is NOT supported as renderman does not support deformation motion blur as of now.
- Clip sim domain to a specified box
- Primvar support
- Ability to load a particle cache instead of voxel cache and convert to voxel representation on the fly
	- Gives us the ability to uprez voxel data
- Filtering stack
	- Different types of smoothing filters can be applied to the input channel
		- Mean Value
		- Median
		- Mean Curvature
		- Laplacian Flow
	- Arbitrary channel for filter blending
		- You specify a channel to use as a blender.
		- Plugin evaluates that channel and remap and clamps it to [0,1] range by using params which are RemapMin, RemapMap, and Rempainvert
		- Formula for remap is clamp ( (val - remapMinVal) / (RemapMaxVal - remapMinVal), 0, 1)
		- The idea is to have an activeness metric for example velocity magnitude and where velocity is greater than the remapmax value we use the raw simulation data,
		if we have a velocity magnitude lower than the remapmin value than we use fully smoothed result. That results in spatially blended raw and smoothed sim which is cool.

- Rib Call:
Volume "blobbydso:BifrostVolume" [-100000 100000 -100000 100000 -100000 100000] # enter big numbers here
[0 0 0]
"varying float density" [] # output primvars as such
"constant float blobbydso:threshold" [0]
"constant float[30] blobbydso:floatargs" [0.041660 10.0   0 0 1 1 0.2 0.8 1      0 -30 20 -15 15 -40 60    1.0 1 1.0 20 0.2 1.0 0.0 1 0 1.0 0.0 1.0    2   0] 
"constant string[4] blobbydso:stringargs" ["bifs/licorn_foam.0287.bif" "density" "none" "density,vorticity,speed,dsds"] # primvars list at the end should include the lookup channel

FLOAT PARAMS:
--------------
1- velocityScale: for scaling motion vectors: the default should be 1sec/#framespersecond
	- Katana:
		- Group: GlobalParameters
		- Attribute Name: VelocityMultiplier
		- Type: Float Numeric Input
		- Default: 0.04166

2- spaceScale - space scalar, assumes scaling around origin
	- Katana:
		- Group: GlobalParameters
		- Attribute Name: SpaceScale
		- Type: Float Numeric Input
		- Default: 1.0

3- smoothOn: turns smoothing on/off
	- Katana:
		- Group: Smoothing
		- Attribute Name: Smooth
		- Type: setHintString("{'widget':'checkBox'}")
		- Default: off

4- smoothMode: Smoothing Filter Type
	- Katana:
		- Group: Smoothing
		- Attribute Name: Type
		- Type: setHintString("{'widget':'mapper', 'options':'Mean Value:0|Gaussian:1|Median Value:2|Laplacian Flow:3|Curvature Flow:4'}")
		- Default: Mean Value

5- smoothAmount: Size of the smoothing filter in voxels. If you enter 1 each voxel looks at its immediate neighbouring voxels to do the filtering.  
	- Katana:
		- Group: Smoothing
		- Attribute Name: Size
		- Type: Integer Numeric Input
		- Default: 0
			
6- smoothIterations: number of smoothing passes.
	- Katana:
		- Group: Smoothing
		- Attribute Name: Iterations
		- Type: Integer Numeric Input
		- Default: 1

7- smoothRemapMin: minimum value used for remapping smoothing channel's values to 0->1 range. Any value in the smoothing channel lower than this will be treated as 0.0
	- Katana:
		- Group: Smoothing/BlendRawAndSmoothValues
		- Attribute Name: RemapMin
		- Type: Float Numeric Input
		- Default: 0.0

8- smoothRemapMax: maximum value used for remapping smoothing channel's values to 0->1 range. Any value in the smoothing channel higher than this will be treated as 1.0
	- Katana:
		- Group: Smoothing/BlendRawAndSmoothValues
		- Attribute Name: RemapMax
		- Type: Float Numeric Input
		- Default: 1.0

9- smoothRemapInvert: invert remapping result?
	- Katana:
		- Group: Smoothing/BlendRawAndSmoothValues
		- Attribute Name: Invert
		- Type: setHintString("{'widget':'checkBox'}")
		- Default: off

10- clipBoxOn: clip sim to the coordinates of the box specified with below parameters.
	- Katana:
		- Group: Clipping
		- Attribute Name: On
		- Type: setHintString("{'widget':'checkBox'}")
		- Default: off

11- clipBoxMinX: Minimum X coordinate of clipbox.
	- Katana:
		- Group: Clipping
		- Attribute Name: MinimumX
		- Type: Float Numeric Input
		- Default: 0.0

12- clipBoxMaxX: Maximum X coordinate of clipbox.
	- Katana:
		- Group: Clipping
		- Attribute Name: MaximumX
		- Type: Float Numeric Input
		- Default: 1.0
		
13- clipBoxMinY: Minimum Y coordinate of clipbox.
	- Katana:
		- Group: Clipping
		- Attribute Name: MinimumY
		- Type: Float Numeric Input
		- Default: -1.0

14- clipBoxMaxY: Maximum Y coordinate of clipbox.
	- Katana:
		- Group: Clipping
		- Attribute Name: MaximumY
		- Type: Float Numeric Input
		- Default: 0.0

15- clipBoxMinZ: Minimum Z coordinate of clipbox.
	- Katana:
		- Group: Clipping
		- Attribute Name: MinimumZ
		- Type: Float Numeric Input
		- Default: 0.0

16- clipBoxMaxZ: Maximum Z coordinate of clipbox.
	- Katana:
		- Group: Clipping
		- Attribute Name: MaximumZ
		- Type: Float Numeric Input
		- Default: 1.0

17- splatResolutionFactor - The scalar for the voxel size of volume cache compared to the input particle cache.
	- Katana:
		- Group: SplatParticlesToVolume
		- Attribute Name: ResolutionFactor
		- Type: Float Numeric Input
		- Default: 1.0

18- skip - we take every skip value particle into account. So if skip is 10 we will export 1/10th the particles. Mathematically skip is a modulo operator.
	- Katana:
		- Group: SplatParticlesToVolume
		- Attribute Name: Skip
		- Type: setHintString("{'int':'True'}")
		- Default: 1

19- densityMult - Multiplier for values of the specified channel for splatting
		- Katana:
			- Group: SplatParticlesToVolume
			- Attribute Name: ChannelValueMultiplier
			- Type: Float Numeric Input
			- Default: 1.0

20- splatSamples - number of splat samples per particle
		- Katana:
			- Group: SplatParticlesToVolume
			- Attribute Name: Samples
			- Type: setHintString("{'int':'True'}")
			- Default: 1
			
21- splatMinRadius - Min Radius for sample distribution per particle - particles distributed within a sphere with a random radius per particle between min and max radius
		- Katana:
			- Group: SplatParticlesToVolume
			- Attribute Name: MinRadius
			- Type: Float Numeric Input
			- Default: 0.0

22- splatMaxRadius - Max Radius for sample distribution per particle - particles distributed within a sphere with a random radius per particle between min and max radius
		- Katana:
			- Group: SplatParticlesToVolume
			- Attribute Name: MinRadius
			- Type: Float Numeric Input
			- Default: 1.0
			
23- splatSurfaceAttract - surface attraction for distributed particles - how close the distributed particles will be to the sphere, 1 means all particles will be on the surface
		- Katana:
			- Group: SplatParticlesToVolume
			- Attribute Name: SurfaceAttract
			- Type: Float Numeric Input
			- Default: 0.0

24- splatFallofType - is there any falloff applied to the channel input depending on the distance between distributed particle and the original particle
		- Katana:
			- Group: SplatParticlesToVolume
			- Attribute Name: FalloffType
			- Type: setHintString("{'widget':'mapper', 'options':'Linear:0|Smooth:1|Smoother:2|None:3'}")
			- Default: Linear

25- splatFalloffStart - where do we start applying falloff depending on the ratio of distance / radius
		- Katana:
			- Group: SplatParticlesToVolume
			- Attribute Name: FalloffStartPosition
			- Type: setHintString("{'slider': 'True', 'slidermax': '1', 'slidermin': '0'}")
			- Default: 0.5

26- splatFalloffEnd - where do we stop applying falloff depending on the ratio of distance / radius
		- Katana:
			- Group: SplatParticlesToVolume
			- Attribute Name: FalloffEndPosition
			- Type: setHintString("{'slider': 'True', 'slidermax': '1', 'slidermin': '0'}")
			- Default: 1.0
			
27- splatDisplacementValue - displacement applied to the distributed particles
		- Katana:
			- Group: SplatParticlesToVolume
			- Attribute Name: DisplacementValue
			- Type: Float Numeric Input
			- Default: 0.0
			
28- splatDisplacementNoiseFrequency - frequency of the noise for displacement
		- Katana:
			- Group: SplatParticlesToVolume
			- Attribute Name: DisplacementNoiseFrequency
			- Type: Float Numeric Input
			- Default: 1.0

29- DEBUG - turns verbose output on off
		- Default should be off
		- Katana:
			- Group: Diagnostics
			- Attribute Name: Verbose Output
			- Type: setHintString("{'widget':'checkBox'}")
			- Default: off

30- silent - turns all log output on/off
		- Default should be off
		- Katana:
			- Group: Diagnostics
			- Attribute Name: Silent
			- Type: setHintString("{'widget':'checkBox'}")
			- Default: off

STRING PARAMS:
---------------

1 - Path to bif file
2 - Channel to lookup density data. Generally density for foam, smoke for aero.
3 - Channel to use for blending raw and smoothed data. You can specify any channel in the bif file. If you put speed here the plugin calculates the velocity magnitude and uses that for blending.
4 - primVarNames - a string input for comma sperated list of primvar names. You can also use "speed" as a special parameter which is calculated as magnitude of velocity.
	    If there is no primvars specified by the user, "none" should be output to RIB.
		- Default should be vorticity?
		- Katana:
			- Group: PrimVars
			- Attribute Name: Output PrimVar Names
			- Type: string input
			- Default: vorticity



    
