<!-- SPDX-License-Identifier: Apache-2.0 -->
# Arnold USD

This repository contains a set of components and tools to use the [Arnold](https://www.arnoldrenderer.com) renderer with Pixar's [Universal Scene Description](https://github.com/PixarAnimationStudios/USD). Notably, the following components are included:

- Hydra render delegate
- Arnold procedural for USD
- Schemas to describe an Arnold scene in USD

Contributions are welcome! Please make sure to read the [contribution guidelines](CONTRIBUTING.md).

## Building and installation

Please follow the [building instructions](docs/building.md). To use the components, provided you installed in `<arnold-usd_dir>`, set the following environment variables:

- Add `<arnold-usd_dir>/procedural` to `ARNOLD_PLUGIN_PATH` for the Arnold `usd` procedural.
- Add `<arnold-usd_dir>/lib/python` to `PYTHONPATH` for the Python schema bindings.
- Add `<arnold-usd_dir>/plugin` to `PXR_PLUGINPATH_NAME` for the Hydra render delegate and the Node Registry plugin.
- Add `<arnold-usd_dir>/lib/usd` to `PXR_PLUGINPATH_NAME` for the USD schemas.
- Add `<arnold-usd_dir>/lib` to `LD_LIBRARY_PATH` on Linux, `PATH` on Windows and `DYLD_LIBRARY_PATH` on Mac.

## Hydra Render Delegate

The render delegate currently supports the following features:

- RPrim Support
  - Mesh
    - All primvars are supported, st/uv is accessible through the built-in uv attribute on the mesh
    - Normal primvars are translated to Arnold built-in attributes
    - Support for the displayColor primvar
    - Subdivision settings
  - Volume
  - Points
- SPrim Support
  - Materials
    - Arnold shaders are supported, the `info:id` attribute is used to determine the shader type
    - UsdPreviewSurface is translated to Arnold shaders
    - UsdTransform nodes
    - Per-face material assignments
  - Lights
    - Distant Light
    - Sphere Light
    - Disk Light
    - Rect Light
    - Cylinder Light
    - Dome Light
    - Shaping Parameters
  - Light and Shadow Linking
  - ExtComputation
  - HdCamera
    - Perspective projection
- BPrim Support
  - Render Buffer
  - OpenVDB Asset
- Point Instancer, including nesting of Point Instancers and instancing Volumes
- Selection in USD View and other applications using the `primId` AOV
- Displaying the Color, Depth and PrimID AOVs
- Motion Blur
  - Deformation
  - Transformation
  - Using motion vectors on Points
- Rendervar support
  - Raw Rendervars
  - Primvar Rendervars
  - LPE Rendervars
  - SourceName parameter
  - DataType parameter
  - Setting up Arnold filters via "arnold:filter_type"
    - Filtering parameters via "arnold:param_name"
    - Filtering parameters via "arnold:filter_node_entry:param_name"
- USD Skel support
- HdExtComputation support for polymeshes
- Render Settings via the Render Delegate
  - Sampling parameters
  - Threading parameters
  - Ignore parameters
  - Profiling and logging parameters
  - Switching between CPU and GPU mode seamlessly
  - Default values are configurable through environment variables for most of these parameters
- Deep render products
- Basis Curves support
  - Remapping uv/st to uvs
  - Remapping normals to orientations
- Pause and Resume API
- Half and Double types

**Limitations**

- Crash on linux at startup with usdview : Currently, the arnold library needs to be ld-preloaded to avoid the crash, e.g. `LD_PRELOAD=/path_to_arnold/bin/libai.so usdview scene.usda`
- Ignoring primvars:arnold:basis on curves
- No support for periodic or pinned curves
- No field3d volume grids
- No normal maps on the UsdPreviewSurface
- Only converging renders are supported (ie. it’s not possible to block the viewport until the render finishes)
- No coordsys support
- Face-varying primvars are not correctly interpolated
- Possible flicker when using cryptomatte: this is due to the prototypes not being named in a deterministic way. Setting the environment variable `USD_ASSIGN_PROTOTYPES_DETERMINISTICALLY` should fix this issue.

## Node Registry Plugin

The Node Registry plugin supports the current features:

- Registering Sdr nodes for every built-in shader and custom shader
  - Setting up the asset URI either to `<built-in>` or to the path of the shader library providing the shader.
  - Creating all parameters.

**Limitations**

- No nodes registered for
  - Shapes
  - Lights
  - Filters
  - Drivers
- No node is registered for the options node
- Metadata is not converted for any node

## Arnold USD Procedural

The procedural supports the following features:

- USD Geom primitives
  - UsdGeomMesh
  - UsdGeomCurves
  - UsdGeomBasisCurves
  - UsdGeomPoints
  - UsdGeomCube
  - UsdGeomSphere
  - UsdGeomCone
  - UsdGeomCylinder
  - UsdPointInstancer
  - UsdVolume
  - UsdGeomCamera
  - primvars are translated as user data
- USD Lux primitives
  - UsdLuxDistantLight
  - UsdLuxDomeLight
  - UsdLuxDiskLight
  - UsdLuxSphereLight
  - UsdLuxRectLight
  - UsdLuxGeometryLight
  - Support for textured lights (dome, rectangle and geometry)
- USD native shaders
  - UsdPreviewSurface
  - UsdPrimVar*
  - UsdUVTexture
  - Per channel connections via adapter nodes
  - UsdTransform nodes
- UsdSkel support
- Arnold shaders supported as UsdShade nodes (where info:id gives the shader type)
- Support for any additional Arnold parameter in USD nodes (e.g. attribute `primvars:arnold:subdiv_iterations` in a UsdGeomMesh)
- Support for any Arnold node type (e.g. USD type ArnoldSetParameter gets rendered as arnold `set_parameter` node)
- Support for multi-threaded parsing of a USD file
- UsdRender schema support
- Half and Double types
- Usd Purpose
- Light Linking
- Multiple frames in a single file

**Limitations**
Currently unsupported:

- Nurbs

## Scene delegate

The scene delegate supports the following features:

- Arnold built-in shapes
- Render time procedurals shipped with the core

**Limitations**
Currently unsupported:

- Custom procedurals
- Lights
- Displaying Arnold schemas with non-Arnold render delegates

## Testsuite

Running the arnold-usd testsuite requires the latest version of Arnold, that can be [downloaded](https://www.arnoldrenderer.com/arnold/download)
It is not supported for older versions of Arnold.

## Acknowledgments

- Luma Pictures' [usd-arnold](https://github.com/LumaPictures/usd-arnold)
- RodeoFX's [OpenWalter](https://github.com/rodeofx/OpenWalter)
- Ben Asher
- Sebastien Blaineau-Ortega
- Chad Dombrova
- Guillaume Laforge
- Julian Hodgson
- Thiago Ize
- Pal Mezei
- Paul Molodowitch
- Nathan Rusch
- Frederic Servant
- Charles Flèche
- Raymond Kreppene
- Julien Deboise
- Jerry Gamache
- Tom Minor
- Cyril Pichard
