Implicit Field Extractor Notes:
--------------------------------

This plugin converts liquid particle/voxels, aero caches to an isorsurface for surface rendering.
It consists of a procedural which is responsible for what gets output to RIB and the main implicit field plugin.
Please look down for example RIB CALL!

Main features
- Motion blur support
- Clip sim domain to a specified box
- Primvar support
- Infinite Surface Blending Support
	- This gives us the ability to blend into an infinite ocean surface
	- You can also specify an arbitrary channel to blend into the infinite surface
- Ability to load a particle cache instead of voxel cache and convert to voxel representation on the fly
	- Gives us the ability to uprez voxel data
- Filtering stack
	- Different types of filters can be applied to the input channel
		- Mean Value
		- Median
		- Mean Curvature
		- Laplacian Flow
	- Arbitrary channel for filter blending
		- You specify a channel to use as a blender.
		- Plugin evaluates that channel and remap and clamps it to [0,1] range by using the float params 7-8-9 which are RemapMin, RemapMap, and Rempainvert
		- Formula for remap is clamp ( (val - remapMinVal) / (RemapMaxVal - remapMinVal), 0, 1)
		- The idea is to have an activeness metric for example velocity magnitude and where velocity is greater than the remapmax value we use the raw simulation data,
		if we have a velocity magnitude lower than the remapmin value than we use fully smoothed result. That results in spatially blended raw and smoothed sim which is cool.

- RIB Call:
	Blobby 1 [1004 0 45 0 5 1]
	[1.5 0 0.75 0.95 10.0 0.041660 2.0
	0.0 0.0 0 0 0.0 1 0.0 1.0 0 0 0.0 1.0 0.0 1.0 0.0 1.0
	0 1 0.0   0.0 0.0 0.0 200.0 400.0 
	1 0.5 0.9 0.0 1.0 0 1.0 3.0 1.285982 1.285982 0.0 1 1 1]
	["BifrostImplicits" "bifs/voxel_liquid_volume.1041.bif" "distance" "none" "none" "vorticity,speed"]
	"constant float levelset" [0.5] # THIS SHOULD ALWAYS BE THERE
	"vertex float presence" [0] # THIS SHOULD ALWAYS BE THERE
	"vertex float vorticity" [0] "vertex float speed" [0]  # put primvars here

	Here is a recap of what you need to do for BifrostImplicits Katana Template:
	- Add an RiScale spaceScale spaceScale spaceScale call to the rib stream
	- Pass 1.0 as spaceScale to the plugin.
	- If cullsides is off pass: constant float presence [1] as a primvar to the Blobby call
	- If cullsides is on pass: vertex float presence [0] as a primvar to the Blobby call
	- always pass constant float levelset [0.5] as a primvar
	- Pass along comma separated primvar string as individual primvars of vertex float primvarName [0].
	- If uv is specified in the primvars string, pass vertex float u1[0], vertex float v1[0] as primvars
	- I would suggest leaving narrowbandwidthinvoxels in there as 1.5 is at the limit. Although i have yet to see a problem with 1.5 it is better to leave there.
	I would suggest binding it to a an expression so that it is harder to modify but if you know what you are doing you can still be able to change it.

FLOAT PARAMS:
--------------
1- narrowBandThicknessInVoxels: For converting the distance field to an implicit field. This defines the transition region around the surface where we go from a field value fo 0 to 1. So for the default of 1.5 voxels, we will look in a region centered around the surface which is 1.5 + 1.5 = 3 voxels thick. Can not be smaller than 1 and 1 is not recommended as you may see hard edges of voxels. the default value is a good tradeoff.
	- Katana:
		- Group: GlobalParameters
		- Attribute Name: NarrowBandThicknessInVoxels
		- Type: Float Numeric Input
		- Default: 1.5

2- cullSidesOn: Enables/disables open surface creation. ter and 1 is at boundary. This is to get an open surface with no thickness.
	- Katana:
		- Group: GlobalParameters/CullSides
		- Attribute Name: On
		- Type: setHintString("{'widget':'checkBox'}")
		- Default: off

3- cullSidesStart: Where do we start looking for side walls of the sim, starting from sim center? This is a value between 0 and 1 with 0 meaning at sim center and 1 is at boundary. This is to get an open surface with no thickness.
	- Katana:
		- Group: GlobalParameters/CullSides
		- Attribute Name: CullSidesStartPoint
		- Type: Float Numeric Input
		- Default: 0.75

4- cullSidesEnd: where do we stop looking for side walls in the sim, starting from sim center? This is a value between 0 and 1 with 0 meaning at sim center and 1 is at boundary. This value is used only for NONAPAPTIVE sims. This is to get an open surface with no thickness.
	- Katana:
		- Group: GlobalParameters/CullSides
		- Attribute Name: CullSidesEndPoint
		- Type: Float Numeric Input
		- Default: 0.95

5- cullDepthAtStartInVoxels: This is to specify the culling depth at cullSidesStartPoint for NONADAPTIVE Sims. Not used for ADAPTIVE sims. The value is in voxels. This is to get an open surface with no thickness.
	- Katana:
		- Group: GlobalParameters/CullSides
		- Attribute Name: CullSidesEndPoint
		- Type: Float Numeric Input
		- Default: 10.0

6- velocityScale: for scaling motion vectors: the default should be 1sec/#framespersecond
	- Katana:
		- Group: GlobalParameters
		- Attribute Name: VelocityMultiplier
		- Type: Float Numeric Input
		- Default: 0.04166

7- spaceScale - space scalar, assumes scaling around origin
	- Katana:
		- Group: GlobalParameters
		- Attribute Name: SpaceScale
		- Type: Float Numeric Input
		- Default: 1.0
			
8- dilateAmount: There is a filtering stack like below:
	Distance Channel
	     |
	     |
	Dilate Filter
	     |
	     |
	Smooth Filter
	     |
	     |
	Erode Filter
	
	Dilation amount controls the amount of dilation applied to the surface. 0.0 means no dilation.
	- Katana:
		- Group: SurfaceProcessing
		- Attribute Name: DilationAmount
		- Type: Float Numeric Input
		- Default: 0.0

9- erodeAmount: Erosion amount controls the amount of erosion applied to the surface. 0.0 means no erosion.
	- Katana:
		- Group: SurfaceProcessing
		- Attribute Name: ErosionAmount
		- Type: Float Numeric Input
		- Default: 0.0

10- SmoothOn: turns smoothing on / off
	- Katana:
		- Group: SurfaceProcessing/SmoothSurface
		- Attribute Name: On
		- Type: setHintString("{'widget':'checkBox'}")
		- Default: off

11- smoothMode: Smoothing Filter Type applied in between Dialion and Erosion
	- Katana:
		- Group: SurfaceProcessing/SmoothSurface
		- Attribute Name: Type
		- Type: setHintString("{'widget':'mapper', 'options':'Mean Value:0|Gaussian:1|Median Value:2|Laplacian Flow:3|Curvature Flow:4'}")
		- Default: Mean Value

12- smoothAmount: Size of the smoothing filter in voxels. If you enter 1 each voxel looks at its immediate neighbouring voxels to do the filtering.  
	- Katana:
		- Group: SurfaceProcessing/SmoothSurface
		- Attribute Name: Size
		- Type: Integer Numeric Input
		- Default: 0
			
13- smoothIterations: number of smoothing passes.
	- Katana:
		- Group: SurfaceProcessing/SmoothSurface
		- Attribute Name: Iterations
		- Type: Integer Numeric Input
		- Default: 1

14- smoothRemapMin: minimum value used for remapping smoothing channel's values to 0->1 range. Any value in the smoothing channel lower than this will be treated as 0.0
	- Katana:
		- Group: SurfaceProcessing/SmoothSurface/BlendRawAndSmoothSurface
		- Attribute Name: RemapMin
		- Type: Float Numeric Input
		- Default: 0.0

15- smoothRemapMax: maximum value used for remapping smoothing channel's values to 0->1 range. Any value in the smoothing channel higher than this will be treated as 1.0
	- Katana:
		- Group: SurfaceProcessing/SmoothSurface/BlendRawAndSmoothSurface
		- Attribute Name: RemapMax
		- Type: Float Numeric Input
		- Default: 1.0

16- smoothRemapInvert: invert remapping result?
	- Katana:
		- Group: SurfaceProcessing/SmoothSurface/BlendRawAndSmoothSurface
		- Attribute Name: Invert
		- Type: setHintString("{'widget':'checkBox'}")
		- Default: off

17- clipBoxOn: clip sim to the coordinates of the box specified with below parameters.
	- Katana:
		- Group: SurfaceClipping
		- Attribute Name: On
		- Type: setHintString("{'widget':'checkBox'}")
		- Default: off

18- clipBoxMinX: Minimum X coordinate of clipbox.
	- Katana:
		- Group: SurfaceClipping
		- Attribute Name: MinimumX
		- Type: Float Numeric Input
		- Default: 0.0

19- clipBoxMaxX: Maximum X coordinate of clipbox.
	- Katana:
		- Group: SurfaceClipping
		- Attribute Name: MaximumX
		- Type: Float Numeric Input
		- Default: 1.0
		
20- clipBoxMinY: Minimum Y coordinate of clipbox.
	- Katana:
		- Group: SurfaceClipping
		- Attribute Name: MinimumY
		- Type: Float Numeric Input
		- Default: -1.0

21- clipBoxMaxY: Maximum Y coordinate of clipbox.
	- Katana:
		- Group: SurfaceClipping
		- Attribute Name: MaximumY
		- Type: Float Numeric Input
		- Default: 0.0

22- clipBoxMinZ: Minimum Z coordinate of clipbox.
	- Katana:
		- Group: SurfaceClipping
		- Attribute Name: MinimumZ
		- Type: Float Numeric Input
		- Default: 0.0

23- clipBoxMaxZ: Maximum Z coordinate of clipbox.
	- Katana:
		- Group: SurfaceClipping
		- Attribute Name: MaximumZ
		- Type: Float Numeric Input
		- Default: 1.0

24- infCubeBlendingOn: if turned on an infinite cube will be blended with the sim.
	- Katana:
		- Group: InfiniteSurfaceBlending
		- Attribute Name: On
		- Type: setHintString("{'widget':'checkBox'}")
		- Default: off

25- infCubeOutputType: What gets output to the renderer. If set to sim only the renderer is fed with flatten sim. if set to all you will get sim blended to infinite cube.
	- Katana:
		- Group: InfiniteSurfaceBlending
		- Attribute Name: OutputType
		- Type: setHintString("{'widget':'mapper', 'options':'SimOnly:0|All:1'}")
		- Default: All

26- simWaterLevel: This is used to define the height of water for the sim.
	- Katana:
		- Group: InfiniteSurfaceBlending
		- Attribute Name: simWaterLevel
		- Type: Float Numeric Input
		- Default: 0.0
		
27- infCubeTopCenterX: is virtual box's top center X Coordinate
	- Katana:
		- Group: InfiniteSurfaceBlending
		- Attribute Name: TopCenterPositionX
		- Type: Float Numeric Input
		- Default: 0.0

28- infCubeTopCenterY: is virtual box's top center Y Coordinate
	- Katana:
		- Group: InfiniteSurfaceBlending
		- Attribute Name: TopCenterPositionY
		- Type: Float Numeric Input
		- Default: 0.0

29- infCubeTopCenterZ: is virtual box's top center Z Coordinate
	- Katana:
		- Group: InfiniteSurfaceBlending
		- Attribute Name: TopCenterPositionZ
		- Type: Float Numeric Input
		- Default: 0.0

30- infCubeDimX: size of the virtual box in X
	- Katana:
		- Group: InfiniteSurfaceBlending
		- Attribute Name: SizeX
		- Type: Float Numeric Input
		- Default: 100.0	

31- infCubeDimZ: size of the virtual box in Z
	- Katana:
		- Group: InfiniteSurfaceBlending
		- Attribute Name: SizeZ
		- Type: Float Numeric Input
		- Default: 100.0

32- blendType: How to blend from sim to the infinite cube
	- Katana:
		- Group: InfiniteSurfaceBlending
		- Attribute Name: BlendingType
		- Type: setHintString("{'widget':'mapper', 'options':'Linear:0|Smooth Step:1|Smoother Step:2'}")
		- Default: Smooth Step

33- infCubeBlendStart: a number between 0.0 and 1.0, 0.5 says start the transition from the middle of sim domain
	- Katana:
		- Group: InfiniteSurfaceBlending
		- Attribute Name: BlendStartPoint
		- Type: setHintString("{'slider': 'True', 'slidermax': '1', 'slidermin': '0'}")
		- Default: 0.5

34- infCubeBlendEnd: a number between 0 and 1, 0.9 says completely transition into the infinite cube at %90 of the sim domain
	- Katana:
		- Group: InfiniteSurfaceBlending
		- Attribute Name: BlendEndPoint
		- Type: setHintString("{'slider': 'True', 'slidermax': '1', 'slidermin': '0'}")
		- Default: 0.99

35- blendingChannelRemapMin: minimum value used for remapping blending channel's values to 0->1 range. Any value in the smoothing channel lower than this will be treated as 0.0
	- Katana:
		- Group: InfiniteSurfaceBlending/SurfaceBlendByChannel
		- Attribute Name: RemapMin
		- Type: Float Numeric Input
		- Default: 0.0

36- blendingChannelRemapMax: maximum value used for remapping blending channel's values to 0->1 range. Any value in the smoothing channel higher than this will be treated as 1.0
	- Katana:
		- Group: InfiniteSurfaceBlending/SurfaceBlendByChannel
		- Attribute Name: RemapMax
		- Type: Float Numeric Input
		- Default: 1.0

37- blendingChannelRemapInvert: invert remapping result?
	- Katana:
		- Group: InfiniteSurfaceBlending/SurfaceBlendByChannel
		- Attribute Name: RemapInvert
		- Type: setHintString("{'widget':'checkBox'}")
		- Default: off

38- resolutionFactor - The amount to uprez the original particle body when creating the surface mesh.
	- Katana:
		- Group: ConvertParticlesToImplicitSurface
		- Attribute Name: ResolutionFactor
		- Type: Float Numeric Input
		- Default: 1.0

39- dropletRevealFactor - A small droplet reveal factor will cause splashes to erode away more easily.
	- Katana:
		- Group: ConvertParticlesToImplicitSurface
		- Attribute Name: DropletRevealFactor
		- Type: Float Numeric Input
		- Default: 3.0

40- surfaceRadius - The radius of each particle "blob" used to create the surface part of the mesh (in voxels)
	- Katana:
		- Group: ConvertParticlesToImplicitSurface
		- Attribute Name: SurfaceRadius
		- Type: Float Numeric Input
		- Default: 1.2

41- dropletRadius - The radius of each individual droplet (in voxels)
	- Katana:
		- Group: ConvertParticlesToImplicitSurface
		- Attribute Name: DropletRadius
		- Type: Float Numeric Input
		- Default: 1.2

42- maxVolumeOfHolesToClose: threshold for which holes are considered for closing
	- Katana:
		- Group: ConvertParticlesToImplicitSurface
		- Attribute Name: MaxVolumeOfHolesToClose
		- Type: Float Numeric Input
		- Default: 8.0

43- doMorphologicalDilation - apply dilation filter to the resulting mesh
	- Katana:
		- Group: ConvertParticlesToImplicitSurface
		- Attribute Name: DoMorphologicalDilation
		- Type: setHintString("{'widget':'checkBox'}")
		- Default: on

44- doErodeSheetsAndDroplets - apply erosion filter to the resulting mesh
	- Katana:
		- Group: ConvertParticlesToImplicitSurface
		- Attribute Name: DoErodeSheetsAndDroplets
		- Type: setHintString("{'widget':'checkBox'}")
		- Default: on

45- DEBUG - turns verbose output on off
		- Default should be off
		- Katana:
			- Group: Statistics
			- Attribute Name: Verbose Output
			- Type: setHintString("{'widget':'checkBox'}")
			- Default: off

STRING PARAMS:
---------------

1 - Path to bif file
2 - Channel to lookup distance data. Generally distance.
3 - Channel to use for filter blending. You can specify any channel in the bif file. If you put speed here the plugin calculates the velocity magnitude and uses that for blending.
4 - Channel to control infinite surface blending. You can specify any channel in the bif file. If you put speed here the plugin calculates the velocity magnitude and uses that for blending.
5 - primVarNames - a string input for comma sperated list of primvar names. You can also use "speed" as a special parameter which is calculated as magnitude of velocity.
	    If there is no primvars specified by the user, "none" should be output to RIB.
		- Default should be vorticity?
		- Katana:
			- Group: PrimVars
			- Attribute Name: Output PrimVar Names
			- Type: string input
			- Default: vorticity



    
