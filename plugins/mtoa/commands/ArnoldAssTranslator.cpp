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
   return true;
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
      std::cout << "kOpenAccessMode" << std::endl;
      status = MStatus::kFailure;
   }
   else if (mode == MPxFileTranslator::kImportAccessMode || mode == MPxFileTranslator::kReferenceAccessMode)
   {
      std::cout << "kImportAccessMode" << std::endl;
      // If we've got Standins selected, update the dso attribute
      MSelectionList selected;
      MGlobal::getActiveSelectionList(selected);
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
               std::cout << "path " << path.fullPathName() <<std::endl;
               MFnDagNode fnDagNode(path);
               MPlug m_dso = fnDagNode.findPlug("dso");
               if (!m_dso.isNull())
               {
                  std::cout << "file.resolvedFullName" << file.resolvedFullName() << std::endl;
                  m_dso.setValue(file.resolvedFullName());
               }
            }
            else
            {
               status = MStatus::kFailure;
            }
         }
      }
      // We have nothing selected. Create a Standins and set it
      else
      {
         MDagModifier m_dagModifier;
         MObject m_standin = m_dagModifier.createNode("ArnoldStandIn");
         MFnDagNode m_fnDagNode;
         m_fnDagNode.setObject(m_standin);
         std::cout << "fullPathName : " << m_fnDagNode.fullPathName() << std::endl;
         for (uint i=0;i<m_fnDagNode.childCount();i++)
         {
            m_standin = m_fnDagNode.child(i);
            m_fnDagNode.setObject(m_standin);
            MPlug m_dso = m_fnDagNode.findPlug("dso");
            if (!m_dso.isNull())
            {
               std::cout << "file.resolvedFullName" << file.resolvedFullName() << std::endl;
               m_dso.setValue(file.resolvedFullName());
            }
         }
      }
   }
   //else if (mode == MPxFileTranslator::kReferenceAccessMode)
   //{
   //   std::cout << "kReferenceAccessMode" << std::endl;
   //}
   else
   {

   }
   return status;
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
