#pragma once

#include "attributes/AttrHelper.h"
#include "ArnoldBaseProcedural.h"
#include "common/UnorderedContainer.h"
#include "ArnoldDrawGeometry.h"

#include <maya/MPxNode.h>
#include <maya/MString.h>
#include <maya/MPxSurfaceShape.h>
#include <maya/MPxSurfaceShapeUI.h>
#include <maya/MNodeMessage.h>
#include <ai_nodes.h>

#include <vector>
#include <map>

// Geometry class
class CArnoldStandInData : public CArnoldProceduralData
{
public:

   CArnoldStandInData();
   ~CArnoldStandInData();
   MString dso;
   MString filename;
   int frame;
   float frameOffset;
   bool useFrameExtension;
   bool useSubFrame;
   float scale;
   int dList;
   int drawOverride;
   bool useAutoInstancing;
   MString objectPath;
   MString abcLayers;
   float abcFps;
   bool m_updateFilename;
   bool m_loadFile;
   bool m_hasOverrides;

};

// Shape class - defines the non-UI part of a shape node
class CArnoldStandInShape: public CArnoldBaseProcedural
{

public:
   CArnoldStandInShape();
   virtual ~CArnoldStandInShape() {};

   
   MStatus GetPointPlugValue( MPlug plug, float3 & value );
   MStatus SetPointPlugValue( MPlug plug, float3   value );
   void CreateBoundingBox();
   bool LoadBoundingBox();
   MStatus LoadFile();

   virtual MStatus setDependentsDirty( const MPlug& plug, MPlugArray& plugArray);
 
   static void* creator();
   static MStatus initialize();
   virtual void updateGeometry();
   
   static MTypeId id;

private:
   CArnoldStandInData *GetStandinData();

   // Attributes
   static CStaticAttrHelper s_attributes;
   static MObject s_dso;

   static MObject s_useFrameExtension;
   static MObject s_frameNumber;
   static MObject s_useSubFrame;
   static MObject s_frameOffset;
   static MObject s_data;
   static MObject s_overrideNodes;
   static MObject s_namespaceName;
   //static MObject s_deferStandinLoad;
   static MObject s_scale;
   static MObject s_boundingBoxMin;
   static MObject s_boundingBoxMax;
   static MObject s_useAutoInstancing;
   static MObject s_drawOverride;
   static MObject s_ignoreGroupNodes;
   static MObject s_objectPath;
   static MObject s_abcLayers;
   static MObject s_abcFps;
   static MObject s_overrides;

}; // class CArnoldStandInShape

