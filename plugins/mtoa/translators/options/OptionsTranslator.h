#pragma once

#include "translators/NodeTranslator.h"
#include "render/AOV.h"

struct CAOVOutput
{
   CAOVOutput() : driver(NULL), filter(NULL), mergeAOVs(false), 
      singleLayer(false), raw(false),  lightGroups(false) {}
   AtNode* driver;
   AtNode* filter;
   bool mergeAOVs; // whether or not to split AOVs
   bool singleLayer;
   bool raw;
   MString prefix;
   MString lpe;
   bool lightGroups;
};

struct CAOVOutputArray
{
   MString name;
   int type;
   MString tokens;
   std::vector<CAOVOutput> outputs;
};



class DLLEXPORT COptionsTranslator : public CNodeTranslator
{
public:
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
protected:
   COptionsTranslator()  :
      CNodeTranslator(),
      m_aovs(),
      m_aovsEnabled(true),
      m_aovsInUse(false)
   {}
   
   void ProcessAOVs();
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
                        MString& prefix,
                        bool& mergeAOVs,
                        bool& singleLayer,
                        bool& raw);
   AtNode* ExportFilter(const MPlug& filterPlug);

   virtual void NodeChanged(MObject& node, MPlug& plug);
protected:
   AOVSet m_aovs;
   std::vector<CAOVOutputArray> m_aovData;
   bool m_aovsEnabled;
   bool m_aovsInUse;
   unordered_map<std::string, AtNode*> m_multiDriverMap;
   MStringArray m_imageFilenames;
};
