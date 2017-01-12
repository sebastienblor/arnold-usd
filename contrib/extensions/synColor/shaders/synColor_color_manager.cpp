//
// SynColor implementation for the color_manager interface
//

#include <ai.h>

#include <atomic>

#include <synColor/synColorInit.h>


AI_COLOR_MANAGER_NODE_EXPORT_METHODS(synColor_manager_Methods);


// this will be retrieved through Get and SetLocalData
// useful for storing precomputed or cached values to be used
// during render time
class ColorManagerData
{
public:
   ColorManagerData()
   {
      m_printed_version.clear();
   }
   ~ColorManagerData() { }

   volatile std::atomic_flag m_printed_version;
};


node_parameters
{
   printf("PH ==> node_parameters()\n");

   AiMetaDataSetStr(nentry, NULL, "maya.name", "defaultColorMgtGlobals");
}

node_initialize
{
   ColorManagerData* my_data = new ColorManagerData();
   AiNodeSetLocalData(node, my_data);

   printf("PH ==> node_initialize()\n");
}

node_update
{
   printf("PH ==> node_update()\n");
}

color_manager_transform
{
   printf("PH ==> color_manager_transform()\n");

   ColorManagerData* my_data = (ColorManagerData*)AiNodeGetLocalData(node);

   // this version info probably should go at init time, but we print
   // it here only once, to stress that this call needs to be thread safe
   if (!my_data->m_printed_version.test_and_set())
      //AiMsgInfo("[color_manager] Using synColor_color_manager Version %s", SYNCOLOR::getVersionString());
      printf("[color_manager] Using synColor_color_manager Version %s\n", SYNCOLOR::getVersionString());


   return true;
}

color_manager_get_defaults
{
   printf("PH ==> color_manager_get_defaults()\n");
}

color_manager_get_chromaticities
{
   printf("PH ==> color_manager_get_chromaticities()\n");
   return false;
}

node_finish
{
   ColorManagerData* my_data =  (ColorManagerData*)AiNodeGetLocalData(node);
   delete my_data;

   printf("PH ==> node_finish()\n");
}

node_loader
{
   if (i > 0)
      return false;

   printf("PH ==> node_loader()\n");

   node->methods      = synColor_manager_Methods;
   node->name         = "synColor_color_manager";
   node->node_type    = AI_NODE_COLOR_MANAGER;
   strcpy(node->version, AI_VERSION);
   return true;
}
