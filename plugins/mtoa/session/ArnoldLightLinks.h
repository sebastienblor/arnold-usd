#ifndef ARNOLDLIGHTLINKS_H
#define	ARNOLDLIGHTLINKS_H

#include <ai.h>

#include <map>
#include <set>
#include <vector>
#include <string>

class CArnoldLightLinks{
public:
   CArnoldLightLinks();
   ~CArnoldLightLinks();
   
   // Building the database from the lightLinker nodes
   void ClearLightLinks();
   void ParseLightLinks();
   void ExportLightLinking(AtNode* shape);
   void SetLinkingMode(int light, int shadow);
private:
   // Try using an MObject for this, or some other trick, instead of strings
   // that might cause problems with specially named / grouped nodes or something
   std::map<std::string, std::vector<AtNode*> > m_lightLinks;
   std::map<std::string, std::vector<AtNode*> > m_shadowLinks;
   
   int m_lightMode;
   int m_shadowMode;
};

#endif	/* ARNOLDLIGHTLINKS_H */

