#ifndef ARNOLDLIGHTLINKS_H
#define	ARNOLDLIGHTLINKS_H

#include <maya/MFnDependencyNode.h>

#include <ai.h>

#include <map>
#include <vector>
#include <string>

class CArnoldLightLinks{
public:
   CArnoldLightLinks() {}
   ~CArnoldLightLinks() {}
   
   // Building the database from the lightLinker nodes
   void ClearLightLinks();
   void ParseLightLinks();
   // for getting the node name
   // and maybe for a different light linking mode later
   void ExportLightLinking(AtNode* shape, MFnDependencyNode& dNode);
   void SetLinkingMode(int light, int shadow);
private:
   // saving the lights here for faster access
   std::map<std::string, AtNode*> m_arnoldLights;
   std::vector<AtNode*> m_linkedLights;
   
   int m_lightMode;
   int m_shadowMode;
};

#endif	/* ARNOLDLIGHTLINKS_H */

