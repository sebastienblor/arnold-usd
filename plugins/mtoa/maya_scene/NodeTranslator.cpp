#include "NodeTranslator.h"
#include "render/RenderOptions.h"
#include "render/RenderSession.h"

#include <ai_ray.h>
#include <ai_metadata.h>

#include <maya/MPlugArray.h>
#include <maya/MFnNumericData.h>
#include <maya/MFnMatrixData.h>
#include <maya/MDagPathArray.h>

#include <string>

//------------ CNodeTranslator ------------//

// internal use only
AtNode* CNodeTranslator::DoExport(AtUInt step)
{
   if (step == 0)
      m_atNode = Export();
   else if (RequiresMotionData())
      ExportMotion(m_atNode, step);
   return m_atNode;
}

// internal use only
AtNode* CNodeTranslator::DoUpdate(AtUInt step)
{
   if (step == 0)
      Update(m_atNode);
   else if (RequiresMotionData())
      UpdateMotion(m_atNode, step);
   return m_atNode;
}

// populate an arnold matrix with values from a maya matrix
void CNodeTranslator::ConvertMatrix(AtMatrix& matrix, const MMatrix& mayaMatrix)
{
   for (int J = 0; (J < 4); ++J)
   {
      for (int I = 0; (I < 4); ++I)
      {
         matrix[I][J] = (float) mayaMatrix[I][J];
      }
   }
}

void CNodeTranslator::ExportDynamicFloatParameter(AtNode* arnoldNode, const char* paramName)
{
   MPlug plug = m_fnNode.findPlug(paramName);
   if (!plug.isNull())
      AiNodeSetFlt(arnoldNode, paramName, plug.asFloat());
}

void CNodeTranslator::ExportDynamicBooleanParameter(AtNode* arnoldNode, const char* paramName)
{
   MPlug plug = m_fnNode.findPlug(paramName);
   if (!plug.isNull())
      AiNodeSetBool(arnoldNode, paramName, plug.asBool());
}

void CNodeTranslator::ExportDynamicIntParameter(AtNode* arnoldNode, const char* paramName)
{
   MPlug plug = m_fnNode.findPlug(paramName);
   if (!plug.isNull())
      AiNodeSetInt(arnoldNode, paramName, plug.asInt());
}

AtNode* CNodeTranslator::ProcessParameter(AtNode* arnoldNode, const char* mayaAttrib, const AtParamEntry* paramEntry, int element)
{

   MPlug plug = m_fnNode.findPlug(mayaAttrib);
   return ProcessParameter(arnoldNode, plug, AiParamGetName(paramEntry), AiParamGetType(paramEntry), element);
}

AtNode* CNodeTranslator::ProcessParameter(AtNode* arnoldNode, const char* attrib, int arnoldAttribType, int element)
{
   MPlug plug = m_fnNode.findPlug(attrib);
   return ProcessParameter(arnoldNode, plug, attrib, arnoldAttribType, element);
}

AtNode* CNodeTranslator::ProcessParameter(AtNode* arnoldNode, const char* mayaAttrib, const char* arnoldAttrib, int arnoldAttribType, int element)
{
   MPlug plug = m_fnNode.findPlug(mayaAttrib);
   return ProcessParameter(arnoldNode, plug, arnoldAttrib, arnoldAttribType, element);
}

AtNode* CNodeTranslator::ProcessParameter(AtNode* arnoldNode, MPlug& plug, const AtParamEntry* paramEntry, int element)
{
   return ProcessParameter(arnoldNode, plug, AiParamGetName(paramEntry), AiParamGetType(paramEntry), element);
}

// export values from a maya plug to an arnold parameter, recursively following
// connections in the dependency graph.
AtNode* CNodeTranslator::ProcessParameter(AtNode* arnoldNode, MPlug& plug, const char* arnoldAttrib, int arnoldAttribType, int element)
{
   AtNode* linkedNode = NULL;
   MPlugArray connections;
   if (element >= 0)
      plug = plug.elementByPhysicalIndex(element);
   plug.connectedTo(connections, true, false);

   if (connections.length() == 0)
   {
      // Unlink first, since this may be called during an IPR update
      AiNodeUnlink(arnoldNode, arnoldAttrib);

      switch(arnoldAttribType)
      {
      case AI_TYPE_RGB:
         {
            AiNodeSetRGB(arnoldNode, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
         }
         break;
      case AI_TYPE_RGBA:
         {
            // Is the source parameter RGB or RGBA?
            if (plug.numChildren() == 4)
            {
               AiNodeSetRGBA(arnoldNode, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat(), plug.child(3).asFloat());
            }
            else
            {
               // For RGB source parameter, set alpha value to 1
               AiNodeSetRGBA(arnoldNode, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat(), 1);
            }
         }
         break;
      case AI_TYPE_FLOAT:
         {
            AiNodeSetFlt(arnoldNode, arnoldAttrib, plug.asFloat());
         }
         break;
      case AI_TYPE_POINT2:
         {
            float x, y;
            MObject numObj = plug.asMObject();
            MFnNumericData numData(numObj);
            numData.getData2Float(x, y);
            AiNodeSetPnt2(arnoldNode, arnoldAttrib, x, y);
         }
         break;
      case AI_TYPE_MATRIX:
         {
            AtMatrix am;
            MObject matObj = plug.asMObject();
            MFnMatrixData matData(matObj);
            MMatrix mm = matData.matrix();
            ConvertMatrix(am, mm);
            AiNodeSetMatrix(arnoldNode, arnoldAttrib, am);
         }
         break;
      case AI_TYPE_BOOLEAN:
         {
            AiNodeSetBool(arnoldNode, arnoldAttrib, plug.asBool());
         }
         break;
      case AI_TYPE_ENUM:
         {
            AiNodeSetInt(arnoldNode, arnoldAttrib, plug.asInt());
         }
         break;
      case AI_TYPE_INT:
         {
            AiNodeSetInt(arnoldNode, arnoldAttrib, plug.asInt());
         }
         break;
      case AI_TYPE_STRING:
         {
            AiNodeSetStr(arnoldNode, arnoldAttrib, plug.asString().asChar());
         }
         break;
      case AI_TYPE_VECTOR:
         {
            AiNodeSetVec(arnoldNode, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
         }
         break;
      case AI_TYPE_POINT:
         {
            AiNodeSetPnt(arnoldNode, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
         }
         break;
      }
   }
   else
   {
      MString attrName = connections[0].partialName(false, false, false, false, false, true);

      linkedNode = m_scene->ExportShader(connections[0].node(), attrName);

      if (linkedNode != NULL)
         AiNodeLink(linkedNode, arnoldAttrib, arnoldNode);
   }
   return linkedNode;
}

// CDagTranslator
//

// populate an arnold AtMatrix with values from this Dag node's transformation.
// the dag node must have an inclusiveMatrix attribute.
void CDagTranslator::GetMatrix(AtMatrix& matrix)
{
   MMatrix tm = m_dagPath.inclusiveMatrix();

   for (int J = 0; (J < 4); ++J)
   {
      for (int I = 0; (I < 4); ++I)
      {
         matrix[I][J] = (float) tm[I][J];
      }
   }
}

// this is a utility method which handles the common tasks associated with
// exporting matrix information. it properly handles exporting a matrix array
// if motion blur is enabled and required by the node. it should be called
// at each motion step
void CDagTranslator::ExportMatrix(AtNode* node, AtUInt step)
{
   AtMatrix matrix;
   GetMatrix(matrix);
   if (step == 0)
   {
      if (RequiresMotionData())
      {
         AtArray* matrices = AiArrayAllocate(1, m_scene->m_motionBlurData.motion_steps, AI_TYPE_MATRIX);
         AiArraySetMtx(matrices, 0, matrix);
         AiNodeSetArray(node, "matrix", matrices);
      }
      else
      {
         AiNodeSetMatrix(node, "matrix", matrix);
      }
   }
   else
   {
      AtArray* matrices = AiNodeGetArray(node, "matrix");
      AiArraySetMtx(matrices, step, matrix);
   }
}

// use standardized render flag names to compute an arnold visibility mask
AtInt CDagTranslator::ComputeVisibility(bool mayaStyleAttrs)
{
   AtInt visibility = AI_RAY_ALL;
   MPlug plug;
   if (mayaStyleAttrs)
   {
      plug = m_fnNode.findPlug("castsShadows");
      if (!plug.isNull() && !plug.asBool())
      {
         visibility &= ~AI_RAY_SHADOW;
      }

      plug = m_fnNode.findPlug("primaryVisibility");
      if (!plug.isNull() && !plug.asBool())
      {
         visibility &= ~AI_RAY_CAMERA;
      }

      plug = m_fnNode.findPlug("visibleInReflections");
      if (!plug.isNull() && !plug.asBool())
      {
         visibility &= ~AI_RAY_REFLECTED;
      }

      plug = m_fnNode.findPlug("visibleInRefractions");
      if (!plug.isNull() && !plug.asBool())
      {
         visibility &= ~AI_RAY_REFRACTED;
      }
   }
   else
   {
      plug = m_fnNode.findPlug("casts_shadows");
      if (!plug.isNull() && !plug.asBool())
      {
         visibility &= ~AI_RAY_SHADOW;
      }

      plug = m_fnNode.findPlug("primary_visibility");
      if (!plug.isNull() && !plug.asBool())
      {
         visibility &= ~AI_RAY_CAMERA;
      }

      plug = m_fnNode.findPlug("visible_in_reflections");
      if (!plug.isNull() && !plug.asBool())
      {
         visibility &= ~AI_RAY_REFLECTED;
      }

      plug = m_fnNode.findPlug("visible_in_refractions");
      if (!plug.isNull() && !plug.asBool())
      {
         visibility &= ~AI_RAY_REFRACTED;
      }
   }

   plug = m_fnNode.findPlug("diffuse_visibility");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_DIFFUSE;
   }

   plug = m_fnNode.findPlug("glossy_visibility");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_GLOSSY;
   }

   return visibility;
}

