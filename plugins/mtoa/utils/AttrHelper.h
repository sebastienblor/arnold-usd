#ifndef ARNOLDNODEHELPER_H
#define ARNOLDNODEHELPER_H

#include "platform/Platform.h"
#include <ai_node_entry.h>
#include <ai_params.h>
#include <ai_metadata.h>
#include <ai_msg.h>

#include <maya/MPxNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MStringArray.h>

#include <maya/MTypes.h>
#if MAYA_API_VERSION < 201200
   #include "utils/MNodeClass.h"
#else
   #include <maya/MNodeClass.h>
#endif

#include <string>
#include <map>

#define OUT_NAME MString("outValue")
#define OUT_COLOR_NAME MString("outColor")
#define OUT_NODE_NAME MString("outNode")
#define OUT_MATRIX_NAME MString("outMatrix")
#define OUT_SHORTNAME MString("out")

/// Structure for holding attribute properties
struct CAttrData
{
   MString name;
   MString shortName;
   AtParamValue defaultValue;
   AtBoolean isArray;
   AtInt type;
   AtBoolean hasMin;
   AtParamValue min;
   AtBoolean hasMax;
   AtParamValue max;
   AtBoolean hasSoftMin;
   AtParamValue softMin;
   AtBoolean hasSoftMax;
   AtParamValue softMax;
   AtBoolean keyable;
   MStringArray enums;
   CAttrData() :  name(""),
                  shortName(""),
                  isArray(false),
                  hasMin(false),
                  hasMax(false),
                  hasSoftMin(false),
                  hasSoftMax(false),
                  keyable(true)   {}
};


typedef MStatus  (*AddAttributeFunction)(const MObject &attr);

// CBaseAttrHelper
//
/// Abstract base class for all attribute creation helper classes.
///
/// the attribute helper classes simplify the act of creating maya attributes from arnold node parameters,
/// by examining parameter properties and metadata from the passed arnold node type.
///
/// below is a list of recognized metadata
///
/// - <B>description</B>:     in maya, used as the annotation for the attribute in the Attribute Editor
/// - <B>min</B>:             minimum value allowed
/// - <B>max</B>:             maximum value allowed
/// - <B>softmin</B>:         overridable lower limit used in GUI
/// - <B>softmax</B>:         overridable upper limit used in GUI
/// - <B>maya.shortname</B>:  attribute short name
/// - <B>maya.name</B>:       alternate name to use for the attribute
/// - <B>maya.hide</B>:       don't generate the attribute (default: false)
/// - <B>maya.keyable</B>:    whether the maya attribute should be keyable (default: true)
///
/// CStaticAttrHelper is used within the initialize method of a custom MPxNode to generate static attributes.
///
/// CDynamicAttrHelper adds dynamic attributes to an existing Maya node.
///
/// CExtensionAttrHelper registers dynamic attributes for a node class, such that the attributes are
/// automatically added to all current and future node instances. Extension attribute helpers are used within a
/// translator's NodeInitializer function. Their primary use case is writing a translator for a built-in Maya
/// node type that requires additional Arnold-specific parameters. For example:
///
/// @code
///   CExtensionAttrHelper helper(nodeClassName, "polymesh");
///   helper.MakeInput("subdiv_type");
/// @endcode
///
/// metadata is used to determine all the qualities of the resulting maya attribute: name, keyability, default, min,
/// max, softmin, softmax, etc. (note that the node factory uses CStaticAttrHelper internally, so the same metadata
/// applies for automatically generated custom nodes, and is actually the most common use case). metadata can be
/// created in the shader's C++ source, or in a plain-text mtd file. values in the plain text file take precedence.
///
/// Attributes that do not have an Arnold equivalent can currently only be created from within a NodeInitializer.
/// For example:
/// @code
///   CAttrData data;
///   data.defaultValue.BOOL = false;
///   data.name = "exportTangents";
///   data.shortName = "exptan";
///   helper.MakeInputBoolean(data);
/// @endcode
///
/// Any call to any CBaseAttrHelper.MakeInput variants that does not explicitly provide a name will convert the Arnold
/// style "parameter_name" to maya style "parameterName".

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
   bool GetAttrData(const char* paramName, CAttrData& data);

   virtual void MakeInputInt(MObject& attrib, const char* paramName);
   virtual void MakeInputInt(CAttrData& data);
   virtual void MakeInputInt(MObject& attrib, CAttrData& data);
   virtual void MakeInputBoolean(MObject& attrib, const char* paramName);
   virtual void MakeInputBoolean(CAttrData& data);
   virtual void MakeInputBoolean(MObject& attrib, CAttrData& data);
   virtual void MakeInputFloat(MObject& attrib, const char* paramName);
   virtual void MakeInputFloat(CAttrData& data);
   virtual void MakeInputFloat(MObject& attrib, CAttrData& data);
   virtual void MakeInputRGB(MObject& attrib, const char* paramName);
   virtual void MakeInputRGB(CAttrData& data);
   virtual void MakeInputRGB(MObject& attrib, CAttrData& data);
   virtual void MakeInputRGBA(MObject& attrib, MObject& attribA, const char* paramName);
   virtual void MakeInputRGBA(CAttrData& data);
   virtual void MakeInputRGBA(MObject& attrib, MObject& attribA, CAttrData& data);
   virtual void MakeInputVector(MObject& attrib, const char* paramName);
   virtual void MakeInputVector(CAttrData& data);
   virtual void MakeInputVector(MObject& attrib,CAttrData& data);
   virtual void MakeInputPoint(MObject& attrib, const char* paramName);
   virtual void MakeInputPoint(CAttrData& data);
   virtual void MakeInputPoint(MObject& attrib, CAttrData& data);
   virtual void MakeInputPoint2(MObject& attrib, MObject& attribX, MObject& attribY, const char* paramName);
   virtual void MakeInputPoint2(CAttrData& data);
   virtual void MakeInputPoint2(MObject& attrib, MObject& attribX, MObject& attribY, CAttrData& data);
   virtual void MakeInputString(MObject& attrib, const char* paramName);
   virtual void MakeInputString(CAttrData& data);
   virtual void MakeInputString(MObject& attrib, CAttrData& data);
   virtual void MakeInputMatrix(MObject& attrib, const char* paramName);
   virtual void MakeInputMatrix(CAttrData& data);
   virtual void MakeInputMatrix(MObject& attrib, CAttrData& data);
   virtual void MakeInputEnum(MObject& attrib, const char* paramName);
   virtual void MakeInputEnum(CAttrData& data);
   virtual void MakeInputEnum(MObject& attrib, CAttrData& data);
   virtual void MakeInputNode(MObject& attrib, const char* paramName);
   virtual void MakeInputNode(CAttrData& data);
   virtual void MakeInputNode(MObject& attrib, CAttrData& data);

   virtual MObject MakeInput(const char* paramName);
   virtual MObject MakeInput(CAttrData& attrData);

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

   MObject MakeOutput();

   void SetNode(const char* arnoldNodeName);
   void GetMObject(const char* attrName);

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
/// Attribute helper for creating static attributes during Maya node initialization
///
class DLLEXPORT CStaticAttrHelper : public CBaseAttrHelper
{

public:
   /// @param addFunc  function to call to add a dynamic attribute: should be YourMPxSubClass::addAttribute
   /// @param nodeEntry  arnold node entry to use when checking parameter metadata
   CStaticAttrHelper(AddAttributeFunction addFunc, const AtNodeEntry* nodeEntry=NULL) :
      CBaseAttrHelper(nodeEntry),
      m_addFunc(addFunc)
   {}
   /// @param addFunc  function to call to add a dynamic attribute: should be YourMPxSubClass::addAttribute
   /// @param nodeEntryName  arnold node entry to use when checking parameter metadata
   CStaticAttrHelper(AddAttributeFunction addFunc, const char* nodeEntryName) :
      CBaseAttrHelper(nodeEntryName),
      m_addFunc(addFunc)
   {
      if (m_nodeEntry == NULL)
      {
         AiMsgWarning("[mtoa] CStaticAttrHelper passed unknown Arnold node type \"%s\"", nodeEntryName);
      }
   }

protected:
   AddAttributeFunction m_addFunc;

protected:
   virtual MStatus addAttribute(MObject& attrib);
};

// CDynamicAttrHelper
//
/// Attribute helper for creating dynamic attributes on existing Maya nodes
///
class DLLEXPORT CDynamicAttrHelper : public CBaseAttrHelper
{

public:
   /// @param obj  maya node to add attributes to
   /// @param nodeEntry  arnold node entry to use when checking parameter metadata
   CDynamicAttrHelper(MObject& obj, const AtNodeEntry* nodeEntry=NULL) :
      CBaseAttrHelper(nodeEntry),
      m_instance(obj)
   {}
   /// @param obj  maya node to add attributes to
   /// @param nodeEntryName  arnold node entry to use when checking parameter metadata
   CDynamicAttrHelper(MObject& obj, const char* nodeEntryName) :
      CBaseAttrHelper(nodeEntryName),
      m_instance(obj)
   {
      if (m_nodeEntry == NULL)
      {
         AiMsgWarning("[mtoa] CDynamicAttrHelper passed unknown Arnold node type \"%s\" for Maya node type \"%s\"",
                      nodeEntryName, MFnDependencyNode(m_instance).typeName().asChar());
      }
   }

protected:
   MObject m_instance;

protected:
   virtual MStatus addAttribute(MObject& attrib);
};

// CExtensionAttrHelper
//
/// Attribute helper for adding extension attributes to Maya node classes
///
/// Extension attributes are like a cross between a static and a dynamic attribute. Like static
/// attributes they are added at the class level, but like a dynamic attribute they can be added after
/// the node class has been initialized.
///
/// Extension attributes are added in Maya 2012 via the new MNodeClass. MtoA provides a rough
/// equivalent of this class for versions prior to 2012

class DLLEXPORT CExtensionAttrHelper : public CBaseAttrHelper
{

public:
   /// @param nodeClassName  name of maya class to add attributes to
   /// @param nodeEntry  arnold node entry to use when checking parameter metadata
   CExtensionAttrHelper(MString nodeClassName, const AtNodeEntry* nodeEntry=NULL) :
      CBaseAttrHelper(nodeEntry),
      m_class(nodeClassName)
   {}
   /// @param nodeClassName  name of maya class to add attributes to
   /// @param nodeEntryName  arnold node entry to use when checking parameter metadata
   CExtensionAttrHelper(MString nodeClassName, const char* nodeEntryName) :
      CBaseAttrHelper(nodeEntryName),
      m_class(nodeClassName)
   {
      if (m_nodeEntry == NULL)
      {
         AiMsgWarning("[mtoa] CExtensionAttrHelper passed unknown Arnold node type \"%s\" for Maya node type \"%s\"",
                      nodeEntryName, nodeClassName.asChar());
      }
   }
#if MAYA_API_VERSION < 201200
   void MakeInputInt(CAttrData& data);
   void MakeInputBoolean(CAttrData& data);
   void MakeInputFloat(CAttrData& data);
   void MakeInputRGB(CAttrData& data);
   void MakeInputRGBA(CAttrData& data);
   void MakeInputVector(CAttrData& data);
   void MakeInputPoint(CAttrData& data);
   void MakeInputPoint2(CAttrData& data);
   void MakeInputString(CAttrData& data);
   void MakeInputMatrix(CAttrData& data);
   void MakeInputEnum(CAttrData& data);
   void MakeInputNode(CAttrData& data);
   MObject MakeInput(const char* paramName);
   MObject MakeInput(CAttrData& attrData);
#else
protected:
   MStatus virtual addAttribute(MObject& attrib);
#endif

protected:
   MNodeClass m_class;

};

#endif // ARNOLDNODEHELPER_H
