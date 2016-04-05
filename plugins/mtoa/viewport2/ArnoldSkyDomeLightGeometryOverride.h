#include "utils/DrawUtils.h"
#include "nodes/light/ArnoldSkyDomeLightNode.h"

#include <maya/MPxGeometryOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MObject.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MUintArray.h>

#if MAYA_API_VERSION >= 201650

class CArnoldSkyDomeLightGeometryOverride : public MHWRender::MPxGeometryOverride{
public:
   static MHWRender::MPxGeometryOverride* Creator(const MObject& obj);

   ~CArnoldSkyDomeLightGeometryOverride();

   virtual MHWRender::DrawAPI supportedDrawAPIs() const;
   virtual void updateDG();
   virtual bool isIndexingDirty(const MHWRender::MRenderItem &item);
   virtual bool isStreamDirty(const MHWRender::MVertexBufferDescriptor &desc);
   virtual void updateRenderItems(const MDagPath &path, MHWRender::MRenderItemList& list);
   virtual void populateGeometry(const MHWRender::MGeometryRequirements &requirements, 
      const MHWRender::MRenderItemList &renderItems, 
      MHWRender::MGeometry &data);
   virtual void cleanUp();

private:
   CArnoldSkyDomeLightGeometryOverride(const MObject& obj);

   CArnoldSkyDomeLightNode* p_skydomeNode;

   // Shaders used for drawing 
   MHWRender::MShaderInstance* m_wireframeShader;
   MHWRender::MShaderInstance* m_activeWireframeShader;
   MHWRender::MShaderInstance* m_texturedShader;
   MHWRender::MShaderInstance* m_texturedColorManagedShader;

   // Depth stencil and rasterizer states for filled item
   void createDisplayStates();
   void destroyDisplayStates();
   const MHWRender::MDepthStencilState* m_depthStencilState;
   const MHWRender::MRasterizerState* m_cullNoneState;
   const MHWRender::MRasterizerState* m_cullBackState;
   const MHWRender::MRasterizerState* m_cullFrontState;

   // Callbacks to allow state change per render item
   static void preDrawCallback(
      MHWRender::MDrawContext&			context,
      const MHWRender::MRenderItemList&	renderItemList,
      MHWRender::MShaderInstance*			shaderInstance);
   static void postDrawCallback(
      MHWRender::MDrawContext&			context,
      const MHWRender::MRenderItemList&	renderItemList,
      MHWRender::MShaderInstance*			shaderInstance);

   // Render item names
   static MString s_wireframeItemName;
   static MString s_activeWireframeItemName;
   static MString s_texturedItemName;

   // Associated object
   MObject m_LocatorNode;

   // Node data
   bool m_flipVData;
   float m_modelMatrix[4][4]; // cut out translation, that doesn't matter
   float m_wireframeColor[4];
   float m_radius;
   int m_format;    
   // 0 - MirroredBall
   // 1 - Angular
   // 2 - LatLong
   bool m_geometryDirty;
   // Data for color management
   MString m_inputColorSpace;
   MString m_workingColorSpace;

   void createFilledSkyDomeGeometry(unsigned int divisions[2],
      float radius );
   void createWireSkyDomeGeometry(unsigned int divisions[2],
      float radius);

   MFloatVectorArray m_filledPositions;
   MFloatArray  m_filledUvs[3]; // One set for each uv format
   MUintArray m_filledIndexing;

   MFloatVectorArray m_wirePositions;
   MUintArray m_wireIndexing;
};

#endif