//
// Alternative to exportAssCmd as a proper Maya file translator
// Advantages :
// Allows the file type to be recognized by Maya
// file -type "ASS" becomes available
// Correct sub-directory will be created in projects and proposed as default path for write / read
// It's easier to track scenes references when proper use of the file


#include "ArnoldAssTranslator.h"

#include <maya/MGlobal.h>
#include <maya/MStringArray.h>
#include <maya/M3dView.h>

// Set some fixed values in the translator class
char CArnoldAssTranslator::fileTypeImport[]        = "ASS";
char CArnoldAssTranslator::fileTypeExport[]        = "ASS Export";
char CArnoldAssTranslator::fileExtension[]         = "ass";
char CArnoldAssTranslator::fileIcon[]              = "arnoldAssFile.xpm";
char CArnoldAssTranslator::optionScriptImport[]    = "";
char CArnoldAssTranslator::optionScriptExport[]    = "arnoldAssOpts";
char CArnoldAssTranslator::optionDefault[]         = "";

inline bool CArnoldAssTranslator::haveReadMethod () const
{
   return m_isImporter;
}

inline bool CArnoldAssTranslator::haveWriteMethod () const
{
   return !m_isImporter;
}

inline bool CArnoldAssTranslator::canBeOpened () const
{
   return false;
}

inline MString CArnoldAssTranslator::defaultExtension() const
{
   return MString(fileExtension);
}

inline MString CArnoldAssTranslator::filter() const
{
   return MString("*.ass *.ass.gz");
}

void* CArnoldAssTranslator::importCreator()
{
   CArnoldAssTranslator* at = new CArnoldAssTranslator();
   at->m_isImporter = true;
   return at;
}

void* CArnoldAssTranslator::exportCreator()
{
   CArnoldAssTranslator* at = new CArnoldAssTranslator();
   at->m_isImporter = false;
   return at;
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
      &&	(fileName.substring(startOfExtension, fileNameLen) == fileExtension ||
          fileName.substring(startOfExtension, fileNameLen) == "gz"))
      {
         return kIsMyFileType;
      }
   }

   return kNotMyFileType;
}
/*
FileAccessMode {
 kUnknownAccessMode, kOpenAccessMode, kReferenceAccessMode, kImportAccessMode,
 kSaveAccessMode, kExportAccessMode, kExportActiveAccessMode
}
*/
MStatus CArnoldAssTranslator::reader(const MFileObject& file,
      const MString& options,
      MPxFileTranslator::FileAccessMode mode)
{
   MStatus status = MStatus::kSuccess;

   if (mode == MPxFileTranslator::kOpenAccessMode)
   {
      status = MStatus::kFailure;
   }
   else if (mode == MPxFileTranslator::kImportAccessMode || mode == MPxFileTranslator::kReferenceAccessMode)
   {
      // If we've got StandIns selected, update the dso attribute
      MSelectionList selected;
      MGlobal::getActiveSelectionList(selected);
      bool standInCreated = false;
      if (selected.length()>0)
      {
         // Get a expanded, flattened, filtered list of every dag
         // paths we need to export
         status = IterSelection(selected);
         MItSelectionList it(selected, MFn::kInvalid, &status);
         MDagPath path;
         for (it.reset(); !it.isDone(); it.next())
         {
            if (it.getDagPath(path) == MStatus::kSuccess)
            {
               MFnDagNode fnDagNode(path);
               if(fnDagNode.typeName() == "aiStandIn")
               {
                  MPlug m_dso = fnDagNode.findPlug("dso", true);
                  if (!m_dso.isNull())
                  {
                     m_dso.setValue(file.resolvedFullName());
                     standInCreated = true;
                  }
               }
            }
            else
            {
               status = MStatus::kFailure;
            }
         }
      }

      // If we have not StandIns selected. Create a StandIns and set it
      if(!standInCreated)
      {
         MFnDagNode fnDagNode;
         MObject m_standin = fnDagNode.create("aiStandIn","ArnoldStandInShape");
         for (uint i=0;i<fnDagNode.childCount();i++)
         {
            m_standin = fnDagNode.child(i);
            MFnDagNode fnDagNodeChildren(m_standin);
            MPlug m_dso = fnDagNodeChildren.findPlug("dso", true);
            if (!m_dso.isNull())
            {
               m_dso.setValue(file.resolvedFullName());
            }

            MSelectionList activeList;
            activeList.add(MString("ArnoldStandInDefaultLightSet"));
            
            // Only create ArnoldStandInDefaultLightSet if it does not exist
            if(activeList.length() == 0)
            {
               // Shared option will make that importing files with "ArnoldStandInDefaultLightSet" sets will not
               //   create ArnoldStandInDefaultLightSet1, ArnoldStandInDefaultLightSet2, ...
               MGlobal::executeCommand("createNode \"objectSet\" -name \"ArnoldStandInDefaultLightSet\" -shared;");
               MGlobal::executeCommand("lightlink -object \"ArnoldStandInDefaultLightSet\" -light \"defaultLightSet\";");
               MGlobal::executeCommand("lightlink -make -shadow -object \"ArnoldStandInDefaultLightSet\" -light \"defaultLightSet\";");
               activeList.add(MString("ArnoldStandInDefaultLightSet"));
            }
            
            MObject m_setObject;
            activeList.getDependNode(0,m_setObject);
            MFnSet m_fnSet(m_setObject);
            m_fnSet.addMember(m_standin);
         }
      }
   }
   return status;
}

// Maya calls this method to have the translator write out a file.
// At the moment just a call to arnoldExportAss cmd but would be cleaner to move
// the code of arnoldExportAss cmd here and use the file command :
// 'file -type "ASS"'
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
MStatus CArnoldAssTranslator::IterSelection(MSelectionList& selected)
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
         for (uint m_child = 0; (m_child < path.childCount()); m_child++)
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
