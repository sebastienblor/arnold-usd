Work done - Feb 23 - 28...  Branched of  FB-2704

Initial bifrost directory was replaced by new_bifrost.  
Just remove bifrost and use new_bifrost renamed bifrost.
Build and installs worked better

bifrost_shaders are built separately

bifrostrendercore was adjusted to be a DLL for windows.

From Cynthia
Ok now everything should be built separately... until we do something better......

"new_bifrost" and "bifrost_shaders" should be built the same way "shave" is.
"new_bifrost" is meant to replace "bifrost"

To use it,
- put 
bifrostTranslator.so, bifrostTranslator.py in MTOA_LOCATION/extensions
bifrost_shaders.so in MTOA_LOCATION/shaders
BifrostImplicits.so, BifrostPolyMesh.so, BifrostPrimitives.so, BifrostVolume.so in MTOA_LOCATION/procedurals

- copy 
libbifrostrendercore.so (in the build directory) in MAYA_LOCATION/lib
(it needs to be find at maya runtime) (to be tested if this is enough)

"bifrost_shaders" plugin depends on "bifrostshaders" maya plugin (should be in (PRs) when https://codereview.autodesk.com/ui#review:id=161826 is in (@Andre))
@Pedro, @Eric, all binaries/python "new_bifrost" and "bifrost_shaders" should be build on all platform and made available on LAB

@Andre, you have "bifrostshaders" from my shelf, it's up to you to push it now


From André

Copy the new_bifrost_shaders to the shaders of your mtoa install
Copy the new_bifrost_shaders_translators.* to the extensions of your mtoa install

