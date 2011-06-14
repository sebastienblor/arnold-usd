#ifndef ARNOLD_ASS_TRANSLATOR_H
#define ARNOLD_ASS_TRANSLATOR_H

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
   static char fileType[];
   static char fileExtension[];
   static char fileIcon[];
   static char optionScript[];
   static char optionDefault[];

   bool		haveReadMethod() const;
   bool		haveWriteMethod() const;
   bool		canBeOpened() const;
   MString		defaultExtension() const;

   MFileKind	identifyFile(const MFileObject& file,
               const char* buffer,
               short size) const;

   MStatus		reader(const MFileObject& file,
               const MString& options,
               FileAccessMode mode);

   MStatus		writer(const MFileObject& file,
               const MString& options,
               FileAccessMode mode);

   MStatus     IterSelection(MSelectionList& selected);

   static void*	creator();

protected:
   


}; // class CArnoldAssTranslator



#endif // ARNOLD_ASS_TRANSLATOR_H
