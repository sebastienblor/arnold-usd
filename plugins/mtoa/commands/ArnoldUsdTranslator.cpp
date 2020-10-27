//
// Alternative to exportAssCmd as a proper Maya file translator
// Advantages :
// Allows the file type to be recognized by Maya
// file -type "Arnold-USD" becomes available
// Correct sub-directory will be created in projects and proposed as default path for write / read
// It's easier to track scenes references when proper use of the file


#include "ArnoldUsdTranslator.h"

#include <maya/MGlobal.h>
#include <maya/MStringArray.h>
#include <maya/M3dView.h>

// Set some fixed values in the translator class
char CArnoldUsdTranslator::fileTypeImport[]        = "Arnold-USD";
char CArnoldUsdTranslator::fileTypeExport[]        = "Arnold-USD";
char CArnoldUsdTranslator::fileExtension[]         = "usd";
char CArnoldUsdTranslator::fileIcon[]              = "arnoldAssFile.xpm";
char CArnoldUsdTranslator::optionScriptImport[]    = "";
char CArnoldUsdTranslator::optionScriptExport[]    = "arnoldAssOpts";
char CArnoldUsdTranslator::optionDefault[]         = "";

inline bool CArnoldUsdTranslator::haveReadMethod () const
{
   return m_isImporter;
}

inline bool CArnoldUsdTranslator::haveWriteMethod () const
{
   return !m_isImporter;
}

inline bool CArnoldUsdTranslator::canBeOpened () const
{
   return false;
}

inline MString CArnoldUsdTranslator::defaultExtension() const
{
   return MString(fileExtension);
}

inline MString CArnoldUsdTranslator::filter() const
{
   return MString("*.usd *.usda *.usdc");
}

void* CArnoldUsdTranslator::importCreator()
{
   CArnoldUsdTranslator* at = new CArnoldUsdTranslator();
   at->m_isImporter = true;
   return at;
}

void* CArnoldUsdTranslator::exportCreator()
{
   CArnoldUsdTranslator* at = new CArnoldUsdTranslator();
   at->m_isImporter = false;
   return at;
}

// To identify a usd file just use the extension
MPxFileTranslator::MFileKind CArnoldUsdTranslator::identifyFile(const MFileObject& file, const char* buffer, short bufferLen) const
{/*
   MString	fileName(file.name());
   int		fileNameLen = fileName.length();
   int		startOfExtension = fileName.rindex('.') + 1;

   if ((startOfExtension > 0)
      &&	(startOfExtension < fileNameLen))
   {
      MString fileExt = fileName.substring(startOfExtension, fileNameLen);
      if (fileExt == MString("usd") || fileExt == MString("usda") || fileExt == MString("usdc"))
      {
         return kIsMyFileType;
      }
   }*/
   return kNotMyFileType;
}
/*
FileAccessMode {
 kUnknownAccessMode, kOpenAccessMode, kReferenceAccessMode, kImportAccessMode,
 kSaveAccessMode, kExportAccessMode, kExportActiveAccessMode
}
*/
MStatus CArnoldUsdTranslator::reader(const MFileObject& file,
      const MString& options,
      MPxFileTranslator::FileAccessMode mode)
{
   // TODO : not implemented yet
   return MStatus::kFailure;
}

// Maya calls this method to have the translator write out a file.
// At the moment just a call to arnoldExportAss cmd but would be cleaner to move
// the code of arnoldExportAss cmd here and use the file command :
// 'file -type "ASS"'
MStatus CArnoldUsdTranslator::writer(const MFileObject& file,
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
      int exportPrefixIndex = optionList[i].indexW(MString("-exportPrefix "));
      if (exportPrefixIndex >= 0 && optionList[i].indexW("/") >= 0)
      {
         optionList[i].substitute(MString("-exportPrefix "), MString(""));
         optionList[i].substitute(MString(" "), MString(""));
         optionList[i] = MString("-exportPrefix \"") + optionList[i] + MString("\" ");
      }      
      cmdStr += " " + optionList[i];
   }
   // If we're interactive, use the active camera
   if (MGlobal::mayaState() == MGlobal::kInteractive)
   {
      MDagPath camera;
      M3dView::active3dView().getCamera(camera);
      cmdStr += "-cam " + camera.partialPathName();
   }
   return MGlobal::executeCommand(cmdStr, true);

}

// Loop and export the selection, and all its hirarchy down stream
//
// @return              MS::kSuccess / MS::kFailure is returned in case of failure.
//
MStatus CArnoldUsdTranslator::IterSelection(MSelectionList& selected)
{
   MStatus status;

   MObject node;
   MDagPath path;
   MFnDagNode dgNode;
   MFnSet set;
   MSelectionList children;
   // loop users selection
   MItSelectionList it(selected, MFn::kInvalid, &status);
   selected.clear();
   for (it.reset(); !it.isDone(); it.next())
   {
      if (it.getDagPath(path) == MStatus::kSuccess)
      {
         // FIXME: if we selected a shape, and it's an instance,
         // should we export all its dag paths?
         for (unsigned int m_child = 0; (m_child < path.childCount()); m_child++)
         {
            MObject ChildObject = path.child(m_child);
            path.push(ChildObject);
            children.clear();
            children.add(path.fullPathName());
            dgNode.setObject(path.node());
            if (!dgNode.isIntermediateObject())
               selected.add (path, MObject::kNullObj, true);
            path.pop(1);
            if (MStatus::kSuccess != IterSelection(children))
               status = MStatus::kFailure;
            selected.merge(children);
         }
      }
      else if (MStatus::kSuccess == it.getDependNode(node))
      {
         // Got a dependency (not dag) node
         if (node.hasFn(MFn::kSet))
         {
            // if it's a set we actually iterate on its content
            set.setObject(node);
            children.clear();
            // get set members, we don't set flatten to true in case we'd want a
            // test on each set recursively
            set.getMembers(children, false);
            if (MStatus::kSuccess != IterSelection(children))
               status = MStatus::kFailure;
            selected.merge(children);
         }
         else
         {
            // TODO: if it's a node we don't support / export should we set status to failure
            // or just raise a warning?
         }
      }
      else
      {
         status = MStatus::kFailure;
      }
   }

   return status;
}
