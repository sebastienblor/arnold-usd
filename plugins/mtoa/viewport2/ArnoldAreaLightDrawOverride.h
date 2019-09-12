#pragma once

#include "utils/DrawUtils.h"

#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>

class CArnoldAreaLightDrawOverride : public MHWRender::MPxDrawOverride{
public:
   static MHWRender::MPxDrawOverride* creator(const MObject& obj);

   ~CArnoldAreaLightDrawOverride();

   virtual bool isBounded(
      const MDagPath& objPath,
      const MDagPath& cameraPath) const;

   virtual MBoundingBox boundingBox(
      const MDagPath& objPath,
      const MDagPath& cameraPath) const;

   virtual bool disableInternalBoundingBoxDraw() const;

   virtual MUserData* prepareForDraw(
      const MDagPath& objPath,
      const MDagPath& cameraPath,
      const MHWRender::MFrameContext& frameContext,
      MUserData* oldData);

   virtual MHWRender::DrawAPI supportedDrawAPIs() const;

#if MAYA_API_VERSION < 201700
   static void clearGPUResources();
#else
   virtual bool hasUIDrawables() const { return true; }
   virtual void addUIDrawables(
      const MDagPath& objPath,
      MHWRender::MUIDrawManager& drawManager,
      const MHWRender::MFrameContext& frameContext,
      const MUserData* data);
#endif

   static void draw(const MHWRender::MDrawContext& context, const MUserData* data);
private:
   CArnoldAreaLightDrawOverride(const MObject& obj);

#if MAYA_API_VERSION < 201700
   static void initializeGPUResources();

    #ifdef _WIN32
    static CDXConstantBuffer* s_pDXConstantBuffer;
    static DXShader* s_pDXShader;
    #endif
    
    static GLuint s_vertexShader;
    static GLuint s_fragmentShader;
    static GLuint s_program;

    static GLint s_modelLoc;
    static GLint s_viewProjLoc;
    static GLint s_shadeColorLoc;
#else
   static void initializeUserData();
#endif

   static bool s_isValid;
   static bool s_isInitialized;
};

