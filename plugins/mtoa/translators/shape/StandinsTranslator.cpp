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
   // #3592 : Now we always export Standins as a procedural, even if they come from a Maya instance.
   // Arnold itself will handle the procedural cache, and the procedural is as lightweight as the ginstance anyway

   // We need to invoke IsMasterInstance first so that the m_isMasterDag value is initialized
   // before we test it in ExportUserAttribute #3673
   IsMasterInstance();

   MString dso = FindMayaPlug("dso").asString();
   if (dso.length() == 0)
      return NULL;

   MStringArray splitStr;
   dso.split('.', splitStr);

   if (splitStr.length() > 1)
   {
      MString ext = splitStr[splitStr.length() -1].toLowerCase();
      if (ext == "abc")
         return AddArnoldNode("alembic");

      if (ext == "usd")
      {
         if (AiNodeEntryLookUp("usd"))
         {
            // oh amazing, there's a usd node available ! let's use it
            return AddArnoldNode("usd");  
         }
         AiMsgError("[mtoa.standin] USD files not supported : %s", GetMayaNodeName().asChar());         
      }
   }
       
   return AddArnoldNode("procedural");
}


void CArnoldStandInsTranslator::Export(AtNode* anode)
{
   // First export the generic procedural parameters defined in ProceduralTranslator
   ExportProcedural(anode);

   // Then export the standin filename
   ExportStandInFilename(anode);  
}

void CArnoldStandInsTranslator::ExportStandInFilename(AtNode *node)
{
   // Is this needed ? since we always use AI_RECREATE_NODE
   // this test doesn't seem necessary
   // if (IsExported())
   //    return;
   
   MString dso = m_DagNode.findPlug("dso", true).asString().expandEnvironmentVariablesAndTilde();
   MString filename;
   MString nodeName = m_dagPath.fullPathName();

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

   resolved = MRenderUtil::exactFileTextureName(newDso, useFrameExtension, frameNumber, nodeName, filename);

   if (!resolved)
   {
      frameNumber = frameExtWithHash;
      resolved = MRenderUtil::exactFileTextureName(newDso, useFrameExtension, frameNumber, nodeName, filename);
   }

   if (!resolved)
   {
      // If file has something after frame number, MRenderUtil::exactFileTextureName has problems to
      //  find the file.
      if (start >= 0)
      {
         MString baseName = dso.substring(0,start-1) + frameExt + dso.substring(end+1,dso.length());
         resolved = MRenderUtil::exactFileTextureName(baseName, false, frameNumber, nodeName, filename);
      }
   }

   MString resolvedName;
   if (resolved)
      resolvedName = filename.asChar();
   else
   {
      resolvedName = dso.asChar();

      if (start >= 0)
      {
         MString expandedFile = dso.substring(0,start-1) + MString(frameExt) + dso.substring(end+1,dso.length());
         resolvedName = expandedFile.asChar();
      }
   }
   
   GetSessionOptions().FormatProceduralPath(resolvedName);
   AiNodeSetStr(node, "filename", resolvedName.asChar());

   if ( strcmp (AiNodeEntryGetName(AiNodeGetNodeEntry(node)), "alembic" ) == 0)
      AiNodeSetFlt(node, "frame", framestep);
}

void CArnoldStandInsTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   m_attrChanged = true; // this flag tells me that I've been through a NodeChanged call
   MString plugName = plug.partialName(false, false, false, false, false, true);

   // Discard all the attributes related to the Attribute Editor, or to the viewport
   if (plugName == "selectedItems" || plugName == "selected_items" || 
      plugName == "MinBoundingBox0" || plugName == "MinBoundingBox1" || plugName == "MinBoundingBox2" || 
      plugName == "MaxBoundingBox0" || plugName == "MaxBoundingBox1" || plugName == "MaxBoundingBox2" ||
      plugName == "standInDrawOverride" || plugName == "mode") return;

   // Since the created arnold type depends on the dso, we need to recreate the geometry if it changes
   if (plugName == "dso")
      SetUpdateMode(AI_RECREATE_NODE);

   if (plugName == "hideParented")
   {
      MGlobal::displayWarning("[mtoa] hideParented isn't updated properly during IPR. Please restart the render or do 'Update Full Scene' in the Arnold Render View");
   }
   // we're calling directly the shape translator function, as we don't want to make it a AI_RECREATE_NODE
   CShapeTranslator::NodeChanged(node, plug);  
}

bool CArnoldStandInsTranslator::ExportDagChildren() const
{
   return !FindMayaPlug("hideParented").asBool();
}