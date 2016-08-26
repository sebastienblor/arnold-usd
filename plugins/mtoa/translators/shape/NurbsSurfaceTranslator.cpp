#include "translators/NodeTranslatorImpl.h"
#include "NurbsSurfaceTranslator.h"

void CNurbsSurfaceTranslator::GetTessellationOptions(MTesselationParams & params,
                                              MFnNurbsSurface & surface)
{
   // Reference for this code is from the devkit:
   // /devkit/obsolete/games/MDtApi/MDtShape.cpp
   // It is similar, this is tidier and more condenced.

   // Get the tesselation attributes off the node
   const int modeU                  = surface.findPlug("modeU").asInt();
   const int numberU                = surface.findPlug("numberU").asInt();
   const int modeV                  = surface.findPlug("modeV").asInt();
   const int numberV                = surface.findPlug("numberV").asInt();
   const bool smoothEdge            = surface.findPlug("smoothEdge").asBool();
   const bool useChordHeightRatio   = surface.findPlug("useChordHeightRatio").asBool();
   const bool edgeSwap              = surface.findPlug("edgeSwap").asBool();
   const bool useMinScreen          = surface.findPlug("useMinScreen").asBool();
   const double chordHeightRatio    = surface.findPlug("chordHeightRatio").asDouble();
   const double minScreen           = surface.findPlug("minScreen").asDouble();

   // I don't actually know why these aren't used. I don't see where they'd be set
   // on MTesselationParams either.
   //const bool useChordHeight      = surface.findPlug("useChordHeight").asBool();
   //const double chordHeight       = surface.findPlug("chordHeight").asDouble();

   switch (modeU)
   {
      case 1:             // Per Surf # of Isoparms in 3D
         params.setUIsoparmType(MTesselationParams::kSurface3DEquiSpaced);
         break;
      case 2:             // Per Surf # of Isoparms
         params.setUIsoparmType(MTesselationParams::kSurfaceEquiSpaced);
         break;
      case 3:             // Per Span # of Isoparms
         params.setUIsoparmType(MTesselationParams::kSpanEquiSpaced);
         break;
      case 4:             // Best Guess Based on Screen Size, there is a comment that 4 uses mode 2 internally
         params.setUIsoparmType(MTesselationParams::kSurfaceEquiSpaced);
         break;
   }

   switch (modeV)
   {
      case 1:             // Per Surf # of Isoparms in 3D
         params.setVIsoparmType(MTesselationParams::kSurface3DEquiSpaced);
         break;
      case 2:             // Per Surf # of Isoparms
         params.setVIsoparmType(MTesselationParams::kSurfaceEquiSpaced);
         break;
      case 3:             // Per Span # of Isoparms
         params.setVIsoparmType(MTesselationParams::kSpanEquiSpaced);
         break;
      case 4:             // Best Guess Based on Screen Size, there is a comment that 4 uses mode 2 internally
         params.setVIsoparmType(MTesselationParams::kSurfaceEquiSpaced);
         break;
   }

   params.setUNumber(numberU);
   params.setVNumber(numberV);
   params.setSubdivisionFlag(MTesselationParams::kUseChordHeightRatio, useChordHeightRatio);
   params.setChordHeightRatio(chordHeightRatio);
   params.setSubdivisionFlag(MTesselationParams::kUseMinScreenSize,useMinScreen);
   params.setMinScreenSize(minScreen, minScreen);

   params.setSubdivisionFlag(MTesselationParams::kUseEdgeSmooth, smoothEdge);
   params.setSubdivisionFlag(MTesselationParams::kUseTriangleEdgeSwapping, edgeSwap);
}

bool CNurbsSurfaceTranslator::Tessellate(const MDagPath &dagPath)
{
   MStatus status;
   MFnNurbsSurface surface(dagPath, &status);
   if (!status)
   {
      AiMsgError("[mtoa] [translator %s] Could not attach to NURBS surface for tessellation: %s",
            GetTranslatorName().asChar(), status.errorString().asChar());
      return status;
   }

   MFnMeshData meshData;
   // This is a member variable. We have to keep hold of it or Maya will release it.
   MObject mesh_mobj = meshData.create();

   MTesselationParams params(MTesselationParams::kGeneralFormat, MTesselationParams::kTriangles);
   GetTessellationOptions(params, surface);
   mesh_mobj = surface.tesselate(params, mesh_mobj, &status);
   if (!status)
   {
      AiMsgError("[mtoa] [translator %s] Could not tessallate NURBS surface: %s",
            GetTranslatorName().asChar(), status.errorString().asChar());
      return status;
   }

   // set the m_geometry to the newly created (tessellated) node
   // This is a member variable. We have to keep hold of it or Maya will release it.
   m_geometry = mesh_mobj;

   return (status == MS::kSuccess);
}

AtNode* CNurbsSurfaceTranslator::CreateArnoldNodes()
{
   if (IsMasterInstance())
      return AddArnoldNode("polymesh");
   else
      return AddArnoldNode("ginstance");
}

void CNurbsSurfaceTranslator::ExportMotion(AtNode* anode)
{
   ExportMatrix(anode);

   // Re-tessalate the nurbs surface, but only if it's needed.
   if (m_motion && m_motionDeform && IsMasterInstance())
   {
      // TODO: Figure out how to get the same topology for
      // each tessellation.
      if (!Tessellate(m_dagPath)) return;
   }

   if (IsMasterInstance() && m_motionDeform) ExportMeshGeoData(anode);
}

// TODO: implement this check for nurbs.
bool CNurbsSurfaceTranslator::IsGeoDeforming()
{
   return true;
}

