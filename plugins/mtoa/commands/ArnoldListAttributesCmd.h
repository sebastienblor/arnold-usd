#ifndef ARNOLDLISTATTRIBUTES_H
#define	ARNOLDLISTATTRIBUTES_H

#include <maya/MPxCommand.h>

class CArnoldListAttributesCmd : public MPxCommand{
public:
   CArnoldListAttributesCmd();
   ~CArnoldListAttributesCmd();
   
   static void* creator();
   virtual MStatus doIt(const MArgList& args);
};

#endif	/* ARNOLDLISTATTRIBUTES_H */

