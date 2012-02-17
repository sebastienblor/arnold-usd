#ifndef SHAPETRANSLATOR_H
#define SHAPETRANSLATOR_H

#include "translators/NodeTranslator.h"
#include <maya/MPlugArray.h>


class DLLEXPORT CShapeTranslator : public CDagTranslator
{
public:
   virtual AtNode* Init(CArnoldSession* session, MDagPath& dagPath, MString outputAttr="")
   {
      m_atNode       = CDagTranslator::Init(session, dagPath, outputAttr);
      m_motion       = IsMotionBlurEnabled(MTOA_MBLUR_OBJECT);
      m_motionDeform = IsMotionBlurEnabled(MTOA_MBLUR_DEFORM);
      return m_atNode;
   }
   virtual bool RequiresMotionData()
   {
      return (m_motion && IsLocalMotionBlurEnabled());
   }
   virtual ~CShapeTranslator()
   {
      delete m_shaders;
   }

protected:
   CShapeTranslator() :
      CDagTranslator()
   {
      m_shaders = new AtNodeSet;
   }

   virtual void ProcessRenderFlags(AtNode* node);
   void ExportLightLinking(AtNode* polymesh);
   // for initializer callbacks:
   static void MakeCommonAttributes(CBaseAttrHelper& helper);

   void AddAOVDefaults(std::vector<AtNode*> &aovShaders);
   virtual AtNode* ExportRootShader(const MPlug& plug);
   virtual AtNode* ExportRootShader(AtNode *rootShader);
   virtual AtNode* ExportRootShader(const MObject& shadingGroup);
   AtNode* CreateShadingGroupShader(AtNode *rootShader, std::vector<AtNode*> &aovShaders);
   MObject GetNodeShadingGroup(MObject dagNode, int instanceNum);

protected:
   bool m_motion;
   bool m_motionDeform;


};

#endif // SHAPETRANSLATOR_H

