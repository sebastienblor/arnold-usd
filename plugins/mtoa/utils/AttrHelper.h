#ifndef ARNOLDNODEHELPER_H
#define ARNOLDNODEHELPER_H

#include "platform/Platform.h"

#include <ai_node_entry.h>
#include <ai_params.h>
#include <ai_metadata.h>

#include <maya/MPxNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMessageAttribute.h>

#include <string>
#include <map>

#define OUT_NAME MString("outValue")
#define OUT_COLOR_NAME MString("outColor")
#define OUT_SHORTNAME MString("out")

// structure for holding attribute properties
struct CAttrData
{
   MString name;
   MString shortName;
   AtParamValue defaultValue;
   AtBoolean isArray;
   AtInt type;
   AtBoolean hasMin;
   AtFloat min;
   AtBoolean hasMax;
   AtFloat max;
   AtBoolean hasSoftMin;
   AtFloat softMin;
   AtBoolean hasSoftMax;
   AtFloat softMax;
   AtBoolean keyable;
};


typedef MStatus  (*AddAttributeFunction)(const MObject &attr);

// CDynamicAttrHelper
//
// Abstract base class for attribute helpers.
//
// these classes simplify the act of creating maya attributes from arnold node
// parameters, by reading parameter metadata from the passed arnold node type.
//
// below is a list of recognized metadata
//
// description     in maya, used as the annotation for the attribute in the Attribute Editor
// min             minimum value allowed
// max             maximum value allowed
// softmin         overridable lower limit used in GUI
// softmax         overridable upper limit used in GUI
// maya.shortname  attribute short name
// maya.name       alternate name to use for the attribute
// maya.hide       don't generate the attribute (default: false)
// maya.keyable    whether the maya attribute should be keyable (default: true)
//
class DLLEXPORT CBaseAttrHelper
{

public:
   CBaseAttrHelper(const AtNodeEntry* nodeEntry=NULL) :
      m_nodeEntry(nodeEntry),
      m_attrNum(0)
   {}
   CBaseAttrHelper(const char* nodeEntryName) :
      m_nodeEntry(AiNodeEntryLookUp(nodeEntryName)),
      m_attrNum(0)
   {}
   virtual ~CBaseAttrHelper() {};
   void GetAttrData(const char* paramName, CAttrData& data);

   void MakeInputInt(MObject& attrib, const char* paramName);
   void MakeInputInt(MObject& attrib, CAttrData& data);
   void MakeInputBoolean(MObject& attrib, const char* paramName);
   void MakeInputBoolean(MObject& attrib, CAttrData& data);
   void MakeInputFloat(MObject& attrib, const char* paramName);
   void MakeInputFloat(MObject& attrib, CAttrData& data);
   void MakeInputRGB(MObject& attrib, const char* paramName);
   void MakeInputRGB(MObject& attrib, CAttrData& data);
   void MakeInputRGBA(MObject& attrib, MObject& attribA, const char* paramName);
   void MakeInputRGBA(MObject& attrib, MObject& attribA, CAttrData& data);
   void MakeInputVector(MObject& attrib, const char* paramName);
   void MakeInputVector(MObject& attrib,CAttrData& data);
   void MakeInputPoint(MObject& attrib, const char* paramName);
   void MakeInputPoint(MObject& attrib, CAttrData& data);
   void MakeInputPoint2(MObject& attrib, MObject& attribX, MObject& attribY, const char* paramName);
   void MakeInputPoint2(MObject& attrib, MObject& attribX, MObject& attribY, CAttrData& data);
   void MakeInputString(MObject& attrib, const char* paramName);
   void MakeInputString(MObject& attrib, CAttrData& data);
   void MakeInputMatrix(MObject& attrib, const char* paramName);
   void MakeInputMatrix(MObject& attrib, CAttrData& data);
   void MakeInputEnum(MObject& attrib, const char* paramName);
   void MakeInputEnum(MObject& attrib, const char* paramName, CAttrData& data);
   void MakeInputNode(MObject& attrib, const char* paramName);
   void MakeInputNode(MObject& attrib, CAttrData& data);

   bool MakeInput(const char* paramName);
   bool MakeInput(const char* paramName, CAttrData& attrData);

   void MakeOutputInt(MObject& attrib, bool isArray=false);
   void MakeOutputBoolean(MObject& attrib, bool isArray=false);
   void MakeOutputFloat(MObject& attrib, bool isArray=false);
   void MakeOutputRGB(MObject& attrib, bool isArray=false);
   void MakeOutputRGBA(MObject& attrib, MObject& attribA, bool isArray=false);
   void MakeOutputVector(MObject& attrib, bool isArray=false);
   void MakeOutputPoint(MObject& attrib, bool isArray=false);
   void MakeOutputPoint2(MObject& attrib, MObject& attribX, MObject& attribY, bool isArray=false);
   void MakeOutputString(MObject& attrib, bool isArray=false);
   void MakeOutputMatrix(MObject& attrib, bool isArray=false);
   void MakeOutputEnum(MObject& attrib, bool isArray=false);
   void MakeOutputNode(MObject& attrib, bool isArray=false);

   bool MakeOutput();

   void SetNode(const char* arnoldNodeName);

protected:
   const AtNodeEntry* m_nodeEntry;
   int m_attrNum;

protected:
   MString GetMayaAttrName(const char* paramName);
   MString GetMayaAttrShortName(const char* paramName);
   std::map<std::string, MObject> m_attributes;
   virtual MStatus addAttribute(MObject& attrib){return MStatus::kFailure;};

};

// CStaticAttrHelper
//
// attribute helper for creating static attributes during node initialization
//
class DLLEXPORT CStaticAttrHelper : public CBaseAttrHelper
{

public:
   CStaticAttrHelper(AddAttributeFunction addFunc, const AtNodeEntry* nodeEntry=NULL) :
      CBaseAttrHelper(nodeEntry),
      m_addFunc(addFunc)
   {}
   CStaticAttrHelper(AddAttributeFunction addFunc, const char* nodeEntryName) :
      CBaseAttrHelper(nodeEntryName),
      m_addFunc(addFunc)
   {}

protected:
   AddAttributeFunction m_addFunc;

protected:
   MStatus addAttribute(MObject& attrib);
};

// CDynamicAttrHelper
//
// attribute helper for creating dynamic attributes on existing maya nodes
//
class DLLEXPORT CDynamicAttrHelper : public CBaseAttrHelper
{

public:
   CDynamicAttrHelper(MObject& obj, const AtNodeEntry* nodeEntry=NULL) :
      CBaseAttrHelper(nodeEntry),
      m_instance(obj)
   {}
   CDynamicAttrHelper(MObject& obj, const char* nodeEntryName) :
      CBaseAttrHelper(nodeEntryName),
      m_instance(obj)
   {}

protected:
   MObject m_instance;

protected:
   MStatus addAttribute(MObject& attrib);
};

#endif // ARNOLDNODEHELPER_H
