#ifndef ARNOLDSKYNODE_H
#define ARNOLDSKYNODE_H

#include "nodes/SphereLocator.h"
#include "attributes/AttrHelper.h"

#include <ai_types.h>

#include <maya/MPlugArray.h>
#include <maya/MDGModifier.h>
#include <maya/MGlobal.h>

class CArnoldSkyNode
   : public CSphereLocator
{

public:

   static void removeSky(MObject& node, void* clientData)
   {
      MFnDependencyNode depNode(node);

      MPlug plug = depNode.findPlug("message");

      MPlugArray conns;

      if (plug.connectedTo(conns, false, true))
      {
         MObject dstObj;
         MFnDependencyNode dstNode;
         MDGModifier dgMod;

         for (unsigned int i=0; i<conns.length(); ++i)
         {
            dstObj = conns[i].node();
            dstNode.setObject(dstObj);

            if (dstNode.name() == "defaultArnoldRenderOptions")
            {
               // Disconnect to avoid defaultArnoldRenderGlobals be deleted
               dgMod.disconnect(plug, conns[i]);
               dgMod.doIt();
               // And refresh arnold render options tab
               MGlobal::executeCommand("UpdateArnoldRendererGlobalsTab()");
               break;
            }
         }
      }
   }

   virtual void postConstructor()
   {
      // Initialize colorData
      m_colorData   = NULL;
      m_UData       = NULL;
      m_VData       = NULL;
      m_goSample    = true;
      m_goUVSample  = true;

      setMPSafe(true);
      MDGMessage::addNodeRemovedCallback(removeSky, "ArnoldSkyShader");
   }

   static void* creator();
   static MStatus initialize();

   static MTypeId id;

   static CStaticAttrHelper s_attributes;

   // Input attributes
   static MObject s_XX;
   static MObject s_XY;
   static MObject s_XZ;
   static MObject s_X;
   static MObject s_X_angle;
   static MObject s_YX;
   static MObject s_YY;
   static MObject s_YZ;
   static MObject s_Y;
   static MObject s_Y_angle;
   static MObject s_ZX;
   static MObject s_ZY;
   static MObject s_ZZ;
   static MObject s_Z;
   static MObject s_Z_angle;

   // Output attributes
   static MObject s_OUT_colorR;
   static MObject s_OUT_colorG;
   static MObject s_OUT_colorB;
   static MObject s_OUT_color;
   static MObject s_OUT_transparencyR;
   static MObject s_OUT_transparencyG;
   static MObject s_OUT_transparencyB;
   static MObject s_OUT_transparency;
};  // class CArnoldSkyNode

#endif // ARNOLDSKYNODE_H

