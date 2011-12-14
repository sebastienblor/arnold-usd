#include "extension/Extension.h"
#include "translators/shape/ShapeTranslator.h"

#include <ai_nodes.h>
#include <ai_msg.h>

class CApiShapeTranslator
   :  public CShapeTranslator
{
public:
   AtNode* CreateArnoldNodes();
   void Export(AtNode*);
   void Update(AtNode*);
   void ExportMotion(AtNode*, unsigned int);
   static void NodeInitializer(CAbTranslator);
   static void* creator();
};


class CExtensionAttrHelper;


AtNode*  CApiShapeTranslator::CreateArnoldNodes()
{
   // TODO : it can actually be a box or a sphere
   return AddArnoldNode("box");
}

void CApiShapeTranslator::Export(AtNode* shape)
{
   AiMsgInfo("[apiShape extension] Exporting %s", GetMayaNodeName().asChar());
   Update(shape);
}

void CApiShapeTranslator::Update(AtNode* shape)
{
   AiMsgInfo("[apiShape extension] Update()");

   // Export the transform matrix
   ExportMatrix(shape, 0);

   // Get the visibiliy and render flags set.
   ProcessRenderFlags(shape);

   return;
}

void CApiShapeTranslator::ExportMotion(AtNode* shape, unsigned int step)
{
   // Check if motionblur is enabled and early out if it's not.
   if (!IsMotionBlurEnabled()) return;

   // Set transform matrix
   ExportMatrix(shape, step);
}

void CApiShapeTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "box");
   CShapeTranslator::MakeCommonAttributes(helper);
}

void* CApiShapeTranslator::creator()
{
   return new CApiShapeTranslator();
}

extern "C"
{

DLLEXPORT void initializeExtension(CExtension& extension)
{
   MStatus status;
   extension.Requires("apiMeshShape");
   status = extension.RegisterTranslator( "apiMesh",
                                          "",
                                          CApiShapeTranslator::creator,
                                          CApiShapeTranslator::NodeInitializer);
}

DLLEXPORT void deinitializeExtension(CExtension& extension)
{
}

}


