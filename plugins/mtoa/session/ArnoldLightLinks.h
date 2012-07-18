#ifndef ARNOLDLIGHTLINKS_H
#define	ARNOLDLIGHTLINKS_H

#include <ai.h>

#include <map>
#include <set>
#include <string>

class CArnoldLightLinks{
public:
   CArnoldLightLinks();
   ~CArnoldLightLinks();
   
   // Building the database from the lightLinker nodes
   void ClearLightLinks();
   void ParseLightLinks();
   void ExportLightLinking(AtNode* shape);
private:
   // I could try using an MObject for this, or some other trick
   std::map<std::string, std::set<AtNode*> > m_links;
   std::map<std::string, std::set<AtNode*> > m_ignores;
   
   AtNode** p_lights;
   std::vector<AtNode*> m_tempLights;
   unsigned int m_numLights;
};

#endif	/* ARNOLDLIGHTLINKS_H */

