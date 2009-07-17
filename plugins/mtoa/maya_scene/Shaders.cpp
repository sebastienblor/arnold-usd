
#include "Shaders.h"

#include <ai_msg.h>
#include <ai_nodes.h>

#include <maya/MFnBlinnShader.h>
#include <maya/MFnLambertShader.h>
#include <maya/MFnPhongShader.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>

#include <vector>

static std::vector<CShaderData> s_processedShaders;

MObject GetNodeShader(MObject dagNode)
{
   MPlugArray        connections;
   MFnDependencyNode fnDGNode(dagNode);

   // Find the "instObjGroups" attribute. Follow that plug to see where it is connected.
   MPlug plug(dagNode, fnDGNode.attribute("instObjGroups"));

   plug.elementByLogicalIndex(0).connectedTo(connections, false, true);

   if (connections.length() != 1)
   {
      return MObject::kNullObj;
   }

   MObject shadingGroup(connections[0].node());

   fnDGNode.setObject(shadingGroup);

   MPlug shaderPlug(shadingGroup, fnDGNode.attribute("surfaceShader"));

   connections.clear();

   shaderPlug.connectedTo(connections, true, false);

   return connections[0].node();
}  // GetNodeShader()


AtNode* ProcessShader(MObject mayaShader)
{
   // First check if this shader has already been processed
   for (std::vector<CShaderData>::const_iterator it = s_processedShaders.begin(); (it != s_processedShaders.end()); ++it)
   {
      if (it->mayaShader == mayaShader)
      {
         return it->arnoldShader;
      }
   }

   AtNode* shader = NULL;

   AiMsgDebug("[mtoa] Processing shader");

   switch (mayaShader.apiType())
   {
   case MFn::kLambert:
      {
         MFnLambertShader fnShader(mayaShader);

         shader = AiNode("lambert");
      }
      break;

   case MFn::kPhong:
      {
      }
      break;

   case MFn::kBlinn:
      {
      }
      break;

   case MFn::kPluginDependNode:
      {
         MFnDependencyNode node(mayaShader);

         AiMsgDebug(node.typeName().asChar());

         if (!strcmp(node.typeName().asChar(), "ArnoldStandardShader"))
         {
         }
         else
         {
            AiMsgWarning("[mtoa] Shader type not supported.");
         }
      }
      break;

   default:
      {
         AiMsgWarning("[mtoa] Shader type not supported.");
      }
   }

   if (shader)
   {
      CShaderData   data;

      data.mayaShader   = mayaShader;
      data.arnoldShader = shader;

      s_processedShaders.push_back(data);
   }

   return shader;

}  // ProcessShader()
