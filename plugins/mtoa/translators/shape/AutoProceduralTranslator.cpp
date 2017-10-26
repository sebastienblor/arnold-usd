#include "AutoProceduralTranslator.h"
#include "translators/NodeTranslatorImpl.h"

// Auto Shape (procedural) translator
//

AtNode* CAutoProceduralTranslator::CreateArnoldNodes()
{
   if (IsMasterInstance())
      return AddArnoldNode(m_impl->m_abstract.arnold.asChar());
   else
      return AddArnoldNode("ginstance");
}


void CAutoProceduralTranslator::Export(AtNode* node)
{
   const char* nodeType = AiNodeEntryGetName(AiNodeGetNodeEntry(node));
   if (strcmp(nodeType, "ginstance") == 0)
   {
      ExportInstance(node, GetMasterInstance());
   }
   else
   {
      // First export generic procedural parameters from ProceduralTranslator
      ExportProcedural(node);

      // Now export the arnold-specific attributes
      AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(AiNodeGetNodeEntry(node));
      static unordered_set<std::string> s_ignoredArnoldParams;
      if (s_ignoredArnoldParams.empty())
      {
         // skip all parameters belonging to common procedural parameters
         // so that we only keep those defined by the user
         // FIXME is there a better way to do this ? we should rather 
         // create a dummy procedural class to find out if the attribute exists in there
         s_ignoredArnoldParams.insert("name");
         s_ignoredArnoldParams.insert("receive_shadows");
         s_ignoredArnoldParams.insert("visibility");
         s_ignoredArnoldParams.insert("matrix");
         s_ignoredArnoldParams.insert("matte");
         s_ignoredArnoldParams.insert("opaque");
         s_ignoredArnoldParams.insert("sidedness");
         s_ignoredArnoldParams.insert("self_shadows");
         s_ignoredArnoldParams.insert("shader");
         s_ignoredArnoldParams.insert("light_group");
         s_ignoredArnoldParams.insert("trace_sets");
         s_ignoredArnoldParams.insert("shadow_group");
         s_ignoredArnoldParams.insert("invert_normals");
         s_ignoredArnoldParams.insert("ray_bias");
         s_ignoredArnoldParams.insert("transform_type");
         s_ignoredArnoldParams.insert("use_light_group");
         s_ignoredArnoldParams.insert("use_shadow_group");
         s_ignoredArnoldParams.insert("motion_start");
         s_ignoredArnoldParams.insert("motion_end");
         s_ignoredArnoldParams.insert("id");
         s_ignoredArnoldParams.insert("override_nodes");
         s_ignoredArnoldParams.insert("namespace");
      }
      while (!AiParamIteratorFinished(nodeParam))
      {
         const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
         const char* paramName = AiParamGetName(paramEntry);
         std::string paramNameStr(paramName);

         // don't export these native parameters
         if (s_ignoredArnoldParams.find(paramNameStr) != s_ignoredArnoldParams.end())
            continue;
         
         ProcessParameter(node, paramName, AiParamGetType(paramEntry));
      }
      AiParamIteratorDestroy(nodeParam);
   }

}
