#include "extension/Extensions.h"
#include "scene/NodeTranslator.h"

#include <maya/MDagPath.h>
#include <maya/MPlug.h>

#include <string>
#include <map>

struct CCustomData
{
   MString exportCmd;
   MString cleanupCmd;
};

class CCustomShapeTranslator
   :   public CDagTranslator
{
public:
   AtNode* Export();
   void Update(AtNode* shape);
   void ExportMotion(AtNode* shape, AtUInt step);
   static void* creator()
   {
      return new CCustomShapeTranslator();
   }
   static void RegisterCustomShapes(CExtension& plugin);

private:
   static bool RegisterCustomShape(CExtension& plugin, std::string &shapeType, int nodeId);
   AtNode* ExportCustomShape(AtUInt step);
   void GetCustomShapeInstanceShader(MDagPath &dagPath, MFnDependencyNode &shadingEngineNode);

private:
   static std::map<std::string, CCustomData> s_customShapes;
};

