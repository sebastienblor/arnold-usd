#include "BifrostUtils.h"

// arnold_bifrost ABI revisions
//
// ABI revision 0: initial version (ArnoldBifrostABIVersion() function not introduced yet)
// ABI revision 1: add support for arbitrary ARNOLD array data types for serialized BOB data,
//                 and add support for "tfile" interpretation to delete temp files after use,
//                 and add ArnoldBifrostABIRevision() export so the revision can be queried.
// ABI revision 2: add support for handle conversion and serialization as extra APIs,
//                 ArnoldBifrostPrepareHandle() and ArnoldBifrostSerializeData().
//                 This also adds the "handle" interpretation for input parameters
//                 used to pass properly refcounted Bifrost data.
static int s_arnoldBifrostABIRev = 0;

static bool s_loadedProcedural = false;

// // Pass the results of this to arnold_bifrost as a "handle" parameter, it takes care of the rest
// typedef void*    (*ArnoldBifrostPrepareHandleFunc)(void *input_buffer, size_t input_length);
// // Write the contents to wherever you like, but use AiFree() to clean up the return buffer
// typedef uint8_t* (*ArnoldBifrostSerializeDataFunc)(void *input_buffer, size_t input_length, size_t& output_length);



ArnoldBifrostPrepareHandleFunc BifrostUtils::s_arnoldBifrostPrepareHandle = NULL;
ArnoldBifrostSerializeDataFunc BifrostUtils::s_arnoldBifrostSerializeData = NULL;

#ifdef _WIN32
static MString s_bifrostProcedural = "arnold_bifrost";
#else
static MString s_bifrostProcedural = "libarnold_bifrost";
#endif

static MString s_bifrostProceduralPath = "";

// 2 GB, e.g. 2^31 elements of single bytes, matching limits of Arnold 5.4.0.0 and earlier
// (newer versions have a fix allowing the full 2^32 elements, or 4 GB per array key)
static const size_t ARNOLD_MAX_BYTE_ARRAY_SIZE = size_t(2) * 1024UL * 1024UL * 1024UL;
// 32 GB, e.g. 2^31 elements of 16-byte RGBAs, matching the max size of any array key possible in Arnold
static const size_t ARNOLD_MAX_PARAMETER_BYTES = size_t(2) * 1024UL * 1024UL * 1024UL * sizeof(AtRGBA);

static const size_t MAX_PATH_LENGTH = 4096;

// Input is a file that ends with the maya scene (.ma or .mb), subtract off the
// extension and make it unique for this node and time sample.


// WARNING: The same function is duplicated in the "old" bifrost extension.
// If you change this one, don't forget to change the other
MString BifrostUtils::GetArnoldBifrostPath()
{
   char archStr[64];
   char majorStr[64];
   char minorStr[64];
   char fixStr[64];
   // Get the current Arnold version
   MString version = AiGetVersion(archStr, majorStr, minorStr, fixStr);
   // convert each token to integer for comparison
   int arch = MString(archStr).asInt();
   int major = MString(majorStr).asInt();
   int minor = MString(minorStr).asInt();
   int fix = MString(fixStr).asInt();
   // build a dummy unique version value
   int versionVal = fix + 100 * minor + 10000 * major + 1000000 * arch;

   // Get the path to the bifrost module
   MString cmd("import maya.cmds as cmds; cmds.getModulePath(moduleName='Bifrost')");
   MString bifrostPath = MGlobal::executePythonCommandStringResult(cmd);

   if (bifrostPath.length() == 0)
      return MString(); // didn't find it, Bifrost is probably not loaded

#ifdef _WIN32
   MString pathSep = "\\";
#else
   MString pathSep = "/";
#endif

   // Initial directory layout: bifrost/arnold-X.X.X.X
   // Future directory layout: bifrost/arnold/X.X.X.X
   MString directories[2] = { bifrostPath, bifrostPath + pathSep + "arnold" };
   MString prefixes[2] = { "arnold-", "" };

   DIR *dir;
   struct dirent *ent;
   int topVersionVal = -1;
   MString topVersionPath = "";

   for (int i = 0; i < 2; ++i)
   {
      // Loop over all directories in the candidate path, and look for the arnold versioned directories
      if ((dir = opendir(directories[i].asChar())) != NULL)
      {
         while ((ent = readdir(dir)) != NULL)
         {
            MString dirName(ent->d_name);
            // Skip the ones that don't start with the appropriate prefix (if any)
            if (prefixes[i].length() > 0 && (dirName.length() < prefixes[i].length() + 1 || dirName.substringW(0, prefixes[i].length() - 1) != prefixes[i]))
               continue;

            // Get the version number this bifrost procedural was built against
            MString folderVersion = prefixes[i].length() > 0 ? dirName.substringW(prefixes[i].length(), dirName.length() - 1) : dirName;

            // We want to get the most recent bifrost procedural that is compatible with
            // the current arnold version
            MStringArray splitVersion;
            folderVersion.split('.', splitVersion);
            if (splitVersion.length() < 2 || !splitVersion[0].isInt() || !splitVersion[1].isInt())
               continue; // something wrong with the folder format, we need at least the 2 first integers


            int currentArch = splitVersion[0].asInt();
            // we need a procedural that matches the current architectural version of arnold
            if (currentArch < arch)
               continue;

            int currentMajor = splitVersion[1].asInt();
            int currentVersionVal = 10000 * currentMajor + 1000000 * currentArch;

            // The following tests are in case we end up stripping the
            // minor and fix versions from the folders
            if (splitVersion.length() >= 3 && splitVersion[2].isInt())
               currentVersionVal += 100 * splitVersion[2].asInt();

            if (splitVersion.length() >= 4 && splitVersion[3].isInt())
               currentVersionVal += splitVersion[3].asInt();

            if (currentVersionVal > versionVal)
               continue; // the arnold version is older than the older package, ignore it

            if (currentVersionVal < topVersionVal)
               continue; // nah, we already have a better candidate;

            // this is the best candidate so far
            topVersionVal = currentVersionVal;
            topVersionPath = directories[i] + pathSep + dirName;
         }
         closedir(dir);
      }
   }
   return topVersionPath;
}


void BifrostUtils::GetArnoldBifrostAPIExtensions(const char* dsoPath, const char* dsoName)
{
    typedef int (*ArnoldBifrostABIRevision)();
#ifdef _WIN32
    std::string fullPath = std::string(dsoPath) + "/" + std::string(dsoName) + ".dll";
    // Load and leak the handle, so Arnold doesn't unload it on us and invalidate our function references!
    HMODULE handle = LoadLibraryA(dsoName);
    if (handle != NULL)
    {
       ArnoldBifrostABIRevision revFunc = (ArnoldBifrostABIRevision)GetProcAddress(handle, "ArnoldBifrostABIRevision");
       if (revFunc != NULL)
          s_arnoldBifrostABIRev = revFunc();
       if (s_arnoldBifrostABIRev >= 2)
       {
          BifrostUtils::s_arnoldBifrostSerializeData = (ArnoldBifrostSerializeDataFunc)GetProcAddress(handle, "ArnoldBifrostSerializeData");
          BifrostUtils::s_arnoldBifrostPrepareHandle = (ArnoldBifrostPrepareHandleFunc)GetProcAddress(handle, "ArnoldBifrostPrepareHandle");
       }
    }
#else
    std::string fullPath = std::string(dsoPath) + "/" + std::string(dsoName) + ".so";
    // Load and leak the handle, so Arnold doesn't unload it on us and invalidate our function references!
    void *handle = dlopen(fullPath.c_str(), RTLD_LAZY | RTLD_LOCAL);
    if (handle != NULL)
    {
       ArnoldBifrostABIRevision revFunc = (ArnoldBifrostABIRevision)dlsym(handle, "ArnoldBifrostABIRevision");
       if (revFunc != NULL)
          s_arnoldBifrostABIRev = revFunc();
       if (s_arnoldBifrostABIRev >= 2)
       {
         BifrostUtils::s_arnoldBifrostSerializeData = (ArnoldBifrostSerializeDataFunc)dlsym(handle, "ArnoldBifrostSerializeData");
         BifrostUtils::s_arnoldBifrostPrepareHandle = (ArnoldBifrostPrepareHandleFunc)dlsym(handle, "ArnoldBifrostPrepareHandle");
       }
    }
#endif
}

bool BifrostUtils::LoadBifrostProcedural()
{
   //  if (s_loadedProcedural )
   //  {
   //      return true;
   //  }
    MString s_bifrostProceduralPath;
    s_bifrostProceduralPath = GetArnoldBifrostPath();

    if (AiNodeEntryLookUp("bifrost_graph") != NULL)
    {
        GetArnoldBifrostAPIExtensions(s_bifrostProceduralPath.asChar(), s_bifrostProcedural.asChar());
        s_loadedProcedural = true;
        return true;
    }
    else if ( s_arnoldBifrostPrepareHandle == NULL )
    {
      GetArnoldBifrostAPIExtensions(s_bifrostProceduralPath.asChar(), s_bifrostProcedural.asChar());
      s_loadedProcedural = true;
      return true;
    }

    AiMsgInfo("[bifrost] %s", s_bifrostProceduralPath.asChar());

    if (s_bifrostProceduralPath.length() > 0)
    {
        CExtension *extension = CExtensionsManager::GetExtensionByName("bifShapeTranslator");
        if (extension)
        {
            MFileObject fo;
            fo.setRawFullName(s_bifrostProceduralPath);
            if (fo.exists())
            {
                extension->LoadArnoldPlugin(s_bifrostProcedural, s_bifrostProceduralPath);
                GetArnoldBifrostAPIExtensions(s_bifrostProceduralPath.asChar(), s_bifrostProcedural.asChar());
                s_loadedProcedural = true;
                return true;
            }
        }
   }
   
   return false;

}

MString BifrostUtils::GetBifrostProceduralPath(){
   return s_bifrostProceduralPath;
}

int BifrostUtils::GetArnoldBifrostABIRev()
{
   return s_arnoldBifrostABIRev;
}