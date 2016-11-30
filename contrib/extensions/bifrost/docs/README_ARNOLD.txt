Bifrost Rendering Plugins Suite:
-----------------------------------

Here is the general info about how the suite is organized and how to compile it.

- There are two shaders: BifrostHydro and BifrostFoam please compile them and put them in:
    - MTOA_FOLDER/shaders
- Put BifrostHydroTemplate.py and BifrostFoam.py in:
    - MTOA_FOLDER/scripts/mtoa/ui/ae/
- I think you already did but also put BifrostMayaAI.dll
    - MTOA_FOLDER/extensions
    - when you are there move:
        - BifrostTranslator.dll and BifrsotTranslator.py into another folder to disable the default
- I think you already did this but copy stuff under the scripts folder:
    - MAYA_USER_FOLDER/2018/scripts to overwrite some files that is coming with Bifrost.

Once you do it all whenever you add a Bifrost node a proper shader is assigned to them. Let's talk once you get to this stage...
