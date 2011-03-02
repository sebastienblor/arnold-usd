//
// Alternative to exportAssCmd as a proper Maya file translator
// Advantages :
// Allows the file type to be recognized by Maya
// file -type "ArnoldSceneSource" becomes available
// Correct sub-directory will be created in projects and proposed as default path for write / read
// It's easier to track scenes references when proper use of the file

// Note: there is only a write method so far, but we could define a read (import) method
// that would create a standin object for that ass file in the scene.
// Advantages :
// Allows to reference ass files in the scene, dependency handling becomes
// very simple as they'll appear in the scene references header

#include "ArnoldAssTranslator.h"

#include <maya/MGlobal.h>
#include <maya/MStringArray.h>

// Set some fixed values in the translator class
char CArnoldAssTranslator::fileType[]        = "ArnoldSceneSource";
char CArnoldAssTranslator::fileExtension[]   = "ass";
char CArnoldAssTranslator::fileIcon[]        = "arnoldAssFile.xpm";
char CArnoldAssTranslator::optionScript[]    = "arnoldAssOpts";
char CArnoldAssTranslator::optionDefault[]   = "";

inline bool CArnoldAssTranslator::haveReadMethod () const
{
   return false;
}

inline bool CArnoldAssTranslator::haveWriteMethod () const
{
   return true;
}

inline bool CArnoldAssTranslator::canBeOpened () const
{
   return false;
}

inline MString CArnoldAssTranslator::defaultExtension() const
{
   return MString(fileExtension);
}

void* CArnoldAssTranslator::creator()
{
   return new CArnoldAssTranslator();
}

// To identify an ass file, look at the header if possible or just use the extension
MPxFileTranslator::MFileKind CArnoldAssTranslator::identifyFile(const MFileObject& file, const char* buffer, short bufferLen) const
{
   MString	tagStr = "### from:     Arnold";
   int		tagLen = tagStr.length();

   // If the buffer contains enough info to positively identify the file,
   // then use it.  Otherwise we'll base the identification on the file
   // extension.
   if (bufferLen >= tagLen)
   {
      MString	initialContents(buffer, bufferLen);
      MStringArray	initialLines;

      initialContents.split('\n', initialLines);

      if (initialLines.length() > 1)
      {
         if (((int)initialLines[1].length() >= tagLen)
         &&	(initialLines[1].substring(0, tagLen-1) == tagStr))
         {
            return kIsMyFileType;
         }
      }
   }
   else
   {
      MString	fileName(file.name());
      int		fileNameLen = fileName.length();
      int		startOfExtension = fileName.rindex('.') + 1;

      if ((startOfExtension > 0)
      &&	(startOfExtension < fileNameLen)
      &&	(fileName.substring(startOfExtension, fileNameLen) == fileExtension))
      {
         return kIsMyFileType;
      }
   }

   return kNotMyFileType;
}


// Maya calls this method to have the translator write out a file.
// At the moment just a call to arnoldExportAss cmd but would be cleaner to move
// the code of arnoldExportAss cmd here and use the file command :
// 'file -type "arnoldSceneSource"'
MStatus CArnoldAssTranslator::writer(const MFileObject& file,
		const MString& options,
		MPxFileTranslator::FileAccessMode mode)
{

	MString cmdStr = "arnoldExportAss";
	cmdStr += " -f \""+file.resolvedFullName()+"\"";
	if (mode == MPxFileTranslator::kExportActiveAccessMode)
		cmdStr += " -s";
	// What a great idea to assemble options in a ; separated string
   MStringArray optionList;
   options.split(';', optionList);
   unsigned int i, nopts = optionList.length();
   for (i=0; i<nopts; ++i)
   {
      cmdStr += " " + optionList[i];
   }

   // Rest of the options on arnoldExportAss are
   // syntax.addFlag("cam", "camera", MSyntax::kString);
   // syntax.addFlag("sf", "startFrame", MSyntax::kLong);
   // syntax.addFlag("ef", "endFrame", MSyntax::kLong);
   // syntax.addFlag("fs", "frameStep", MSyntax::kLong);
   // cmdStr += options;

   return MGlobal::executeCommand(cmdStr);

}
