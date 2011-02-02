#ifndef ARNOLD_ASS_TRANSLATOR_H
#define ARNOLD_ASS_TRANSLATOR_H

#include <maya/MPxFileTranslator.h>
#include <maya/MString.h>

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

	MFileKind	identifyFile(
					const MFileObject& file,
					const char* buffer,
					short size
				) const;
	/* So far no reader, but one use could be to create standin object on 
	the import of an ass file
	MStatus		reader(
					const MFileObject& file,
					const MString& options,
					FileAccessMode mode
				);
	*/
	MStatus		writer(
					const MFileObject& file,
					const MString& options,
					FileAccessMode mode
				);

	static void*	creator();

protected:
	


}; // class CArnoldAssTranslator



#endif // ARNOLD_ASS_TRANSLATOR_H
