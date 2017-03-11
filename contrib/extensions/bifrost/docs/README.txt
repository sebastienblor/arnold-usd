Bifrost Rendering Plugins Suite:
-----------------------------------

Here is the general info about how the suite is organized and how to compile it.

- For renderTime/general processing of Bif Data there is a common library which is called BifrostRenderCore.
    - This is referenced by all other plugins, so it needs to be compiled first and header files should be copied to a folder folder which is on search path for the later compiles

- For each renderer there is a library called BifrostRender(*). We currently support AI and RIS so the libraries are called BifrostRenderRIS and BifrostRenderAI
    - These refer to BifrostRenderCore so for my Visual Studio Setup, I have the BifrostRenderCore project as part of BifrostRenderAI/RIS solutions with correct dependencies so that if Core is modified, it is compiled first.

- For Maya Integration there are two plugins BifrostMayaRIS which does not exist yet and BifrostMayaAI. These are responsible for exporting the correct information to each renderer during an interactive Maya session so that users can render them properly. These in turn put stuff into an .ASS an .RIB stream proper information to render each bifrost object. These are still in development but Arnold is operational for all Bifrost Types.

- For Each Bifrost Type there are corresponding plugins.
    - For liquid Surfaces we have BifrostImplicitsAI/RIS and BifrostPolyMeshAI/RIS
    - For Foam we have BifrostPrimitivesRIS/AI
    - For Aero and Volumes we have BifrostVolumeRIS/AI
    - Although there is a corresponding plugin for each bifrostType, most of the plugins can render other types of data. For example BifrostPrimitives can render Liquid/Aero/Foam caches. This enables interesting pipeline possibilities which uses other software like Katana etc.
    - These are dependent on BifrostRenderCore and BifrostRenderAI/RIS depending on the renderer so those should be compiled first and header files should put in a library area so that they can be found. IF you look at the .vcxproj's in ecah folder you can see the lookup folders and dependencies.

- In the scripts folder there are some bifrost mel scripts i overwrote for proper Maya integration. Once things are finaled most probably the extra parameters we have for bifrostShape should be made part of that node but a dynamic solution is better at this point once we fully decide on parameters. I marked where i changed stuff in those files and if you just look for CAVE in the files you kind find them very easily.
    - There are also some old template files in there with annotations about whar each parameter does.
    - Docs folder also has all that information but for Katana. It also explains what each plugin does in general

- Let's concentrate on Arnold stuff first for now:
    - So compilation order:
        - BifrostRenderCore
        - BifrostRenderAI
        - BifrostPrimitivesAI
        - BifrostVolumeAI
        - BifrostPolyMeshAI

- There is a PropertySheet for the Release Configuration which defines a UserMacro called BUILDVERSION which is set to MAYA2018 in my visual studio projects. This in turn looks for an environment variable with the same name pointing to the MAYA2018 install root folder like C:\Autodesk\Maya2018. This UserMacro is referenced in include paths etc.

- There are some Post Build Event in both solutions to copy final files to some folders depending on our setup at Cave so you may need to modify those.

- I also include a linux compile script in the main folder as an example (build.sh). That is the one i use for MPC compilation tasks. Beware of the fact that those are fast hacks and has stuff relevant to our pipeline here. But all in all they should give you an idea on how to compile stuff for RIS on Linux. AI was not needed on Linux at MPC so I did not compile that on Linux.

- Once you are able to compile things let's talk about your integration steps with the aim that when i change things you should be use them as is. Let's also version the dumps starting with 1 for this one. Then for each dump we should refer dumps with their version.

