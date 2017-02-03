#pragma once

#include <maya/MPxFileTranslator.h>
#include <maya/MItDag.h>
#include <maya/MString.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MPlug.h>
#include <maya/MDagModifier.h>
#include <maya/MFnSet.h>

class CArnoldAssTranslator : public MPxFileTranslator
{
public:
   static char fileTypeImport[];
   static char fileTypeExport[];
   static char fileExtension[];
   static char fileIcon[];
   static char optionScriptImport[];
   static char optionScriptExport[];
   static char optionDefault[];

   bool     haveReadMethod() const;
   bool     haveWriteMethod() const;
   bool     canBeOpened() const;
   MString     defaultExtension() const;
   MString     filter() const;

   MFileKind   identifyFile(const MFileObject& file,
               const char* buffer,
               short size) const;

   MStatus     reader(const MFileObject& file,
               const MString& options,
               FileAccessMode mode);

   MStatus     writer(const MFileObject& file,
               const MString& options,
               FileAccessMode mode);

   MStatus     IterSelection(MSelectionList& selected);

   static void*   importCreator();
   static void*   exportCreator();
   bool haveNamespaceSupport() const {return true;}

protected:
   bool m_isImporter;

}; // class CArnoldAssTranslator
