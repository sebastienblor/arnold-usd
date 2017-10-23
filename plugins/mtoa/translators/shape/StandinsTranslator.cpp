#include "StandinsTranslator.h"
#include "attributes/AttrHelper.h"
#include "utils/time.h"

#include <ai_msg.h>
#include <ai_nodes.h>
#include <ai_ray.h>

#include <maya/MBoundingBox.h>
#include <maya/MDagPath.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlugArray.h>
#include <maya/MRenderUtil.h>


#include <maya/MString.h>


AtNode* CArnoldStandInsTranslator::CreateArnoldNodes()
{
   // #2771 for interactive sessions, we export standin instances as 
   // several procedurals instead of ginstances. We could do this for all
   // kind of exports, but for now we only change it in IPR.
   // We can only do this for .ass files, which arnold core will
   // handle through the procedural cache. We don't want to duplicate other kind
   // of procedurals, since they'd really be duplicated.
   bool isInstanced = (!IsMasterInstance()) && (!GetSessionOptions().IsInteractiveRender());

   // Note that the code below is possible because RequestUpdate sets the 
   // update mode to AI_RECREATE_NODE, otherwise we could have invalid arnold nodes
   // when the filename is changed during IPR
   if (!isInstanced)
      return AddArnoldNode("procedural");
   else
      return AddArnoldNode("ginstance");
}


void CArnoldStandInsTranslator::Export(AtNode* anode)
{
   const char* nodeType = AiNodeEntryGetName(AiNodeGetNodeEntry(anode));
   if (strcmp(nodeType, "ginstance") == 0)
   {
      ExportInstance(anode, GetMasterInstance());
   }
   else
   {
      // First export the generic procedural parameters defined in ProceduralTranslator
      ExportProcedural(anode);

      // Then export the standin filename
      ExportStandInFilename(anode);  
   }
}

void CArnoldStandInsTranslator::ExportStandInFilename(AtNode *node)
{
   // Is this needed ? since we always use AI_RECREATE_NODE
   // this test doesn't seem necessary
   if (IsExported())
      return;
   
   MString dso = m_DagNode.findPlug("dso").asString().expandEnvironmentVariablesAndTilde();
   MString filename;

   int frame = FindProceduralPlug("frameNumber").asInt();
   float frameOffset = FindProceduralPlug("frameOffset").asFloat();
   bool useFrameExtension = FindProceduralPlug( "useFrameExtension").asBool();
   bool useSubFrame = FindProceduralPlug("useSubFrame").asBool();

   MString frameNumber = "0";

   float framestep = frame + frameOffset;

   bool subFrames = ((framestep - floor(framestep)) >= 0.001);
   char frameExtWithHash[64];
   char frameExtWithDot[64];
   char frameExt[64];

   int start = 0;
   int end = 0;
   MStringArray pattern;
   MString newDso = "";
   int framePadding = 0;
   int subFramePadding = 0;
   bool resolved = false;
   MString a, b;
   
   start = dso.index('#');
   end = dso.rindex('#');

   if(start >= 0)
   {
      if(dso.substring(start-1,start-1) == "_")
         newDso = dso.substring(0,start-2) + ".#" + dso.substring(end+1,dso.length());
      else
         newDso = dso.substring(0,start-1) + "#" + dso.substring(end+1,dso.length());
      
      dso.substring(start,end).split('.',pattern);
      if(pattern.length() > 0)
      {
         framePadding = pattern[0].length();
         a = pattern[0];
      }
      if(pattern.length() > 1)
      {
         subFramePadding = pattern[1].length();
         b = pattern[1];
      }
   }
   else
   {
      newDso = dso;
   }

   if (subFrames || useSubFrame || (subFramePadding != 0))
   {
      int fullFrame = (int) floor(framestep);
      int subFrame = (int) floor((framestep - fullFrame) * 1000);
      sprintf(frameExtWithHash, "_%0*d.%0*d", framePadding, fullFrame, subFramePadding, subFrame);
      sprintf(frameExtWithDot, ".%0*d.%0*d", framePadding, fullFrame, subFramePadding, subFrame);
      sprintf(frameExt, "%0*d.%0*d", framePadding, fullFrame, subFramePadding, subFrame);
   }
   else
   {
      sprintf(frameExtWithHash, "_%0*d", framePadding, (int) framestep);
      sprintf(frameExtWithDot, ".%0*d", framePadding, (int) framestep);
      sprintf(frameExt, "%0*d", framePadding, (int) framestep);
   }
   frameNumber = frameExtWithDot;

   resolved = MRenderUtil::exactFileTextureName(newDso, useFrameExtension, frameNumber, filename);

   if (!resolved)
   {
      frameNumber = frameExtWithHash;
      resolved = MRenderUtil::exactFileTextureName(newDso, useFrameExtension, frameNumber, filename);
   }

   if (!resolved)
   {
      // If file has something after frame number, MRenderUtil::exactFileTextureName has problems to
      //  find the file.
      if (start >= 0)
      {
         MString baseName = dso.substring(0,start-1) + frameExt + dso.substring(end+1,dso.length());
         resolved = MRenderUtil::exactFileTextureName(baseName, false, frameNumber, filename);
      }
   }

   MString resolvedName;
   if (resolved)
      resolvedName = filename.asChar();
   else
      resolvedName = dso.asChar();
   
   GetSessionOptions().FormatProceduralPath(resolvedName);
   AiNodeSetStr(node, "filename", resolvedName.asChar());
   
}
