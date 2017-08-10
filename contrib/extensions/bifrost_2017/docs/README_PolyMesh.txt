MeshProcedural:
----------------
- Procedural Polygon Geo which matches the Maya mesher
- Loads in a bif liguid particle file AND a bif solids voxel file and spits out geo for prman.
- Attach a bxdf and render like regular geo.

- Typical call:
Procedural "DynamicLoad" [ "BifrostPolyMesh" "0.04166 1.0 3.0 1.4 1.2 2.0 1.0 2 5.0 0 vorticity,fsdfks,sdfshj bifs/bifrostLiquidContainer1_liquid_point.XXXX.bif bifs/bifrostLiquidContainer1_liquid_voxel.XXXX.bif"] [-1000 1000 -1000 1000 -1000 1000]

- Parameters:
		- velMul: a multiplier for the velocity vector
			- Group: GlobalParameters
			- Default should be 1/24 fps = 0.04166
			- Katana:
				- Attribute Name: Velocity Multiplier
				- Type: Float Numeric Input
				- Default: 0.04166
		- spaceScale - space scalar, assumes scaling around origin
			- Katana:
				- Group: GlobalParameters
				- Attribute Name: SpaceScale
				- Type: Float Numeric Input
				- Default: 1.0
    

		(User should enter the parameters he put in maya attribute editor for the mesh group)
		- dropletRevealFactor - A small droplet reveal factor will cause splashes to erode away more easily.
			- Katana:
				- Group: MeshParameters
				- Attribute Name: Droplet Reveal Factor
				- Type: Float Numeric Input
				- Default: 3.0
		- surfaceRadius - The radius of each particle "blob" used to create the surface part of the mesh (in voxels)
			- Katana:
				- Group: MeshParameters
				- Attribute Name: Surface Radius
				- Type: Float Numeric Input
				- Default: 1.2
		- dropletRadius - The radius of each individual droplet (in voxels)
			- Katana:
				- Group: MeshParameters
				- Attribute Name: Droplet Radius
				- Type: Float Numeric Input
				- Default: 1.2
		- kernelfactor - Controls the width of the surface smoothing kernel.  Smaller values give sharper features and higher curvature in the resulting mesh.
		    Larger values result in smoother features.
			- Katana:
				- Group: MeshParameters
				- Attribute Name: Kernel Factor
				- Type: Float Numeric Input
				- Default: 2.0
		- resolutionFactor - The amount to uprez the original particle body when creating the surface mesh.
			- Katana:
				- Group: MeshParameters
				- Attribute Name: Resolution Factor
				- Type: Float Numeric Input
				- Default: 1.0
		- smoothing - The number of smoothing steps performed on the mesh.
			- Katana:
				- Group: MeshParameters
				- Attribute Name: Smoothing
				- Type: Integer Slider
				- Default: 2
		- minimumFeatureAngle - Minimum angle between adjacent triangles (in degrees) that defines an uncollapsable feature edge.
			- Katana:
				- Group: MeshParameters
				- Attribute Name: Minimum Feature Angle
				- Type: Float Numeric Input
				- Default: 5.0

		- debug - turns verbose output on off
			- Default should be off:
			- Katana:
				- Group: GlobalParameters
				- Attribute Name: Verbose Output
				- Type: checkbox
				- Default: off

		- primVarNames - a string input for comma sperated list of primvar names, if there is no primvars specified by the user, "none" should be output to RIB.
			- Default should be vorticity?
			- Katana:
				- Group: PrimVars
				- Attribute Name: Output PrimVar Names
				- Type: string input
				- Default: vorticity

		- inPointFile - name of the point cache
		- inVoxelFile - name of the solids cache

- Please see bifrost_mesh.h or maya docs for explanation of parameters
