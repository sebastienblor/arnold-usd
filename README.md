# Building MtoA
## Dependencies

To use the build system, you need to install:

[​Git](https://git-scm.com/)<br>
[​Python 2.7](https://www.python.org/downloads/release/python-2711/)<br>
[​Glew 1.10.0](https://sourceforge.net/projects/glew/files/glew/1.10.0/)<br>

|Maya Version|Visual Studio Version|Linux devtoolset/gcc|XCode|
|------------|------------|------------|------------|
| Maya 2019 | 2015 | 6.1 (3.2.1) | 7.3.1 |
| Maya 2020 | 2017 | 6.1 (6.3.1) | 10.2.1 |
| Maya 2022 | 2019 | 9 (9.3.1) | 10.2.1 |
| Maya 2023 | 2019 | 9 (9.3.1) | 10.2.1 |

On Centos 5.x install the simplejson module for python, this is required for the build scripts.

```bash
yum install python-simplejson
```

On linux download the ​GLEW source code, and call make & make install, while on windows it's included in the external repository. If you already have a GLEW built into your system you can use GLEW_LIB and GLEW_INCLUDES to override the default path.

The source code lives in a Git repository located in the arnold organization. 
Fork the repo to your user sandbox

Then clone your forked repo to your devlopment area:

```bash
> git clone https://git.autodesk.com/<username>/arnold-mtoa.git arnold-mtoa
```

MtoA has now the external dependencies into its own repository, so doing a checkout of the external repository is not needed anymore.

Finally, you will need to download the ​Arnold SDK from [artfactory](https://art-bobcat.autodesk.com/ui/repos/tree/General/arnold/core/ci/master) .

## Build Settings
The different paths and options for the build process are handled by setting scons parameters. Using `abuild -h` in the MtoA checkout directory will give you a complete list.

For commonly used values you will normally create/edit your `custom.py` file (located in the trunk directory) and set the variables there. This is a minimal example of a custom.py file:

```python
import os

# path to your Maya root. In OS X it should point to the Maya.app/Contents folder
MAYA_ROOT           = r'C:\Program Files\Autodesk\Maya2022'                 

# path where MtoA module is installed
TARGET_MODULE_PATH  = r'C:\Program Files\Autodesk\MtoA\2022'                

# current Arnold SDK root folder
ARNOLD              = r'C:\Program Files\Autodesk\Arnold\Arnold-7.1.0.0'    

# your Visual Studio version (14.0 = 2017, 14.2 = 2019)
MSVC_VERSION        = '14.2'                                                

# compilation mode (opt, debug)
MODE                = 'opt'                                                 

# disable treating warnings as errors, only use on OSX
# WARN_LEVEL          = 'warn-only'                                         

# Path to MayaUSD toolset
USD_PATH            = r'C:\Program Files\Autodesk\MayaUSD\Maya2022\0.17.0\USD3'
MAYAUSD_PATH        = r'C:\Program Files\Autodesk\MayaUSD\Maya2022\0.17.0\MayaUSD3'

#Others variables are set automatically, generated from the ones above, but you can override them
#GLEW_LIB
#GLEW_INCLUDES
```

:information_source: : The `MSVC_VERSION` is not required. It's hard coded based on the maya version to avoid compatibility issues when using an older compiler. If an older or different compiler is required, you have to maintain a custom scons locally and be prepared for unforeseen consequences.

:information_source: : On newer OSX systems, than our base build system, due to the newer compiler you might encounter warnings that are blocking the build unless the WARN_LEVEL is set to warn-only as seen above. We strongly discourage using the warn-only flag on other systems than OSX 10.8+.

## Building
On the command line you can then go to the directory where the MtoA trunk was checked out, and type:
```bash
> abuild          (to build the project)
```

You can also pass arguments in the command line when you call the build script:
```bash
> abuild MODE=opt TARGET_ARCH=x86_64
```

Finally deploy the project to your previously created module path:
```bash
> abuild install
```

To make a pack of MtoA you can use the target pack
```bash
> abuild pack
```

Also you can create an installer by installing [NSIS 2.46](http://nsis.sourceforge.net/Download), setting up the `NSIS_PATH` on windows, and using the installer target
```bash
> abuild installer
```

# Common Build Problems
`cl` is not recognized as an internal or external command,
This error means that SCons didn't found the compiler on Windows. It can be because it's not present in the PATH environment variable (You can check that by issuing cl in a shell) or because you are using the wrong compiler for your target (Using the 32 bits compiler to build a x64 target, if that's the case double check your VS installation options).

```
// Error: line 1: Unable to dynamically load : ../mtoa/plug-ins/mtoa.mll
The specified module could not be found.
// 
// Error: line 1: The specified module could not be found.
// 
// Error: line 1: The specified module could not be found.
 (mtoa) //
```

This error usually means that MtoA didn't found the Arnold binaries, you need to ensure that they are in the PATH on Windows or LD_LIBRARY_PATH on Linux.

# Running MtoA
Before launching Maya with MtoA the following environment variables must be set, directly or via a Maya.env file

`MAYA_MODULE_PATH` : the directory containing mtoa.mod. e.g. `TARGET_MODULE_PATH` <br>
`MAYA_RENDER_DESC_PATH` : the directory containing the Arnold Renderer Description .xml. e.g. `TARGET_MODULE_PATH` (Maya.env seems to only support this one in interactive mode, not 'Render' maya batch mode) <br>
Add the Arnold binaries path (`TARGET_MODULE_PATH\bin`) to your `PATH` environment variable

# Profiling on Linux
The valgrind package has a nice profiler, callgrind. To use that, create a small script file, somewhere in the `PATH`, and edit the file.

```
valgrind --tool=callgrind --instr-atstart=no $1
```
Launch maya using this command.
```
maya -d your_batch_command
```

This runs maya through valgrind, using the callgrind toolset (there are other ones, for that you need to change the `--tool` parameter), but the profiling is paused. This is very important, because profiling slows down the execution 20-40 times. Look for the pid of the valgrind process, that can be found in the command line, or looking for the callgrind-amd64 process in the system monitor. Using that process id, the callgrind_control can start the profiling.
```
callgrind_control -i on pid
```
And check the results using `kcachegrind`.

# ABI compatibility
For ABI compatibility checks abi-compliance-checker can be used. The process is the following.

Install and compile these three tools.

[vtable-dumper](​​https://github.com/lvc/vtable-dumper)<br>
[abi-dumper](​​https://github.com/lvc/abi-dumper)<br>
[abi-compliance-checker](​​https://github.com/lvc/abi-compliance-checker)

MtoA have to be compiled in debug mode to get the most out of the compliance checker tool. After compilation the abi dump can be created by calling this command. It's useful to specify the api version using the lver flag, so the generated dumps are easier to compare. Call this command both on the old and new so.
```
abi-dumper <libmtoa_api.so_path> -lver <version> -o <version>.dump
```
After generating both dumps, the comparison can be done using this command.
```
abi-compliance-checker -l libmtoa_api -old <old_version>.dump -new <new_version>.dump
```
This command both gives a short summary, and the path to the detailed, html based, statistics.

We also provide a simpler way of doing these checks through scons. There are two input variables what has to be defined.
```
REFERENCE_API_LIB   = '/work/deploy/2014_master_debug/bin/libmtoa_api.so'
REFERENCE_API_LIB points to the base libmtoa_api.so (built using MODE="debug").

REFERENCE_API_VERSION = '1.0.0.1'
```
`REFERENCE_API_VERSION` tells scons the version of the reference library. Right now there are no ways to query the version from the so, this might be solved in the future thus making `REFERENCE_API_VERSION` flag obsolete.

After this, build mtoa in debug mode, and call check_compliance.
```
./abuild MODE="debug" check_compliance
```
After running this command, the report can be found in the `compat_reports/libmtoa_api/<old_version>_to_<new_version>/compat_report.html` file.
