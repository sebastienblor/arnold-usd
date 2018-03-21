#pragma once

#include "translators/NodeTranslator.h"
#include "render/AOV.h"

class CDriverTranslator;

struct CAOVOutput
{
   CAOVOutput() : driver(NULL), filter(NULL), mergeAOVs(false), 
      singleLayer(false), raw(false),
      driverTranslator(NULL) {}
   AtNode* driver;
   AtNode* filter;
   bool mergeAOVs; // whether or not to split AOVs
   bool singleLayer;
   bool raw;
   MString prefix;
   CDriverTranslator *driverTranslator;
};

struct CAOVOutputArray
{
   CAOVOutputArray() : type(0), shaderTranslator(NULL), cameraTranslator(NULL), denoised(false) {}
   MString name;
   int type;
   MString tokens;
   MString aovSuffix;
   MString lpe;
   CNodeTranslator *shaderTranslator;
   CNodeTranslator *cameraTranslator;
   std::vector<CAOVOutput> outputs;
   bool denoised;

};



class DLLEXPORT COptionsTranslator : public CNodeTranslator
{
public:
   virtual ~COptionsTranslator() {ClearAovCallbacks();}
   static void* creator(){return new COptionsTranslator();}
   virtual void Export(AtNode* options);
   AtNode* CreateArnoldNodes();

   void SetCamera(AtNode *options);
   bool IsActiveAOV(CAOV &aov) const
   {
      if (m_aovs.count(aov))
         return true;
      else
         return false;
   }

   AOVSet GetActiveAOVs() const
   {
      return m_aovs;
   }
   const MStringArray& GetActiveImageFilenames() const
   {
      return m_imageFilenames;
   }

   static void AddProjectFoldersToSearchPaths(AtNode* options);
   static void AovChangedCallback(MNodeMessage::AttributeMessage msg,
                                                    MPlug& plug, MPlug& otherPlug,
                                                    void* clientData);


protected:
   COptionsTranslator()  :
      CNodeTranslator(),
      m_aovs(),
      m_aovsEnabled(true),
      m_aovsInUse(false)
   {}
   
   void ProcessAOVs();
   void ClearAovCallbacks();
   void SetImageFilenames(MStringArray &outputs);
   void ExportAOVs();
   void ExportAtmosphere(AtNode* options);
   void CreateFileDirectory(const MString &filename) const;
   unsigned int GetOutputArray(const CAOV& aov,
                                     std::vector<CAOVOutput>& outputs);
   bool GetOutput(const MPlug& driverPlug,
                  const MPlug& filterPlug,
                  CAOVOutput& output);
   AtNode* ExportDriver(const MPlug& driverPlug,
                        CAOVOutput &output);
   AtNode* ExportFilter(const MPlug& filterPlug);

   virtual void NodeChanged(MObject& node, MPlug& plug);
protected:
   AOVSet m_aovs;
   std::vector<CAOVOutputArray> m_aovData;
   bool m_aovsEnabled;
   bool m_aovsInUse;
   unordered_map<std::string, AtNode*> m_multiDriverMap;
   MStringArray m_imageFilenames;
   std::vector<MCallbackId> m_aovCallbacks;
};
