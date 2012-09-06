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
   enum NodeLinkMode{
      MTOA_NODELINK_LINK,
      MTOA_NODELINK_IGNORE
   };
   
   const std::vector<AtNode*>& GetObjectsFromObjectSet(MFnDependencyNode& objectSet);

   void AppendNodesToList(MFnDependencyNode& linkedNodes, std::vector<AtNode*>& nodeList, 
        size_t& numLinkedNodes);
   void HandleLightLinker(MPlug& conn, 
      size_t& numLinkedLights, size_t& numLinkedShadows,
      NodeLinkMode& lightLinkMode, NodeLinkMode& shadowLinkMode);
   void CheckMessage(MFnDependencyNode& dNode, 
      size_t& numLinkedLights, size_t& numLinkedShadows,
      NodeLinkMode& lightLinkMode, NodeLinkMode& shadowLinkMode);
   // saving the lights here for faster access
   std::map<std::string, AtNode*> m_arnoldLights;
   std::map<std::string, std::vector<AtNode*> > m_cachedObjectSets;
   size_t m_numArnoldLights;
   
   std::vector<AtNode*> m_linkedLights;
   std::vector<AtNode*> m_linkedShadows;
   
   int m_lightMode;
   int m_shadowMode;
};

#endif	/* ARNOLDLIGHTLINKS_H */

