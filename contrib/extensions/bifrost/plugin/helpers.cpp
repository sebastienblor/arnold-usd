#include "helpers.h"
#include <maya/MDGModifier.h>

namespace ArnoldBifrost{

MAttrData::MAttrData(MString mayaNodeClassName, const MString &arnoldNodeEntryName)
   : CAttrData(), m_helper(mayaNodeClassName, arnoldNodeEntryName){}

template<typename T>
MAttrData& MAttrData::setup(const char* longName, const char* shortName, T value){
   this->name = longName;
   this->shortName = shortName;
   hasMin = hasMax = hasSoftMin = hasSoftMax = false;
   type = AI_TYPE_UNDEFINED;
   return setDefault(value);
}

template MAttrData& MAttrData::setup(const char* longName, const char* shortName, float value);
template MAttrData& MAttrData::setup(const char* longName, const char* shortName, int value);
template MAttrData& MAttrData::setup(const char* longName, const char* shortName, MString value);
template MAttrData& MAttrData::setup(const char* longName, const char* shortName, bool value);
template MAttrData& MAttrData::setup(const char* longName, const char* shortName, AtVector value);
template MAttrData& MAttrData::setup(const char* longName, const char* shortName, std::initializer_list<const char*> value);

void MAttrData::makeInput(){
   switch(type){
   case AI_TYPE_BOOLEAN: m_helper.MakeInputBoolean(*this); break;
   case AI_TYPE_INT:     m_helper.MakeInputInt(*this);     break;
   case AI_TYPE_STRING:  m_helper.MakeInputString(*this);  break;
   case AI_TYPE_FLOAT:   m_helper.MakeInputFloat(*this);   break;
   case AI_TYPE_VECTOR:  m_helper.MakeInputVector(*this);  break;
   case AI_TYPE_ENUM:    m_helper.MakeInputEnum(*this);    break;
   default: std::runtime_error("Invalid type.");
   }
}

void MAttrData::makeInputMesh(const char* longName, const char* shortName){
   MStatus status;
   MFnTypedAttribute typeAttr;
   MObject obj = typeAttr.create(longName, shortName, MFnData::kMesh, &status);
   typeAttr.setStorable(false);
   typeAttr.setKeyable(true);
   typeAttr.setDisconnectBehavior(MFnAttribute::kReset);
   CHECK_MSTATUS(status);

   MDGModifier dgMod;
   if (dgMod.addExtensionAttribute(m_helper.GetMayaNodeTypeName(), obj) != MStatus::kSuccess) {
      AiMsgError("[mtoa.attr] Unable to create extension attribute %s.%s", m_helper.GetMayaNodeTypeName().asChar(), typeAttr.name().asChar());
   } else {
      AiMsgDebug("[mtoa.attr] Added extension attribute %s.%s", m_helper.GetMayaNodeTypeName().asChar(), typeAttr.name().asChar());
      CHECK_MSTATUS(dgMod.doIt());
   }
}

template<> MAttrData& MAttrData::setDefault<float>  (float value)  {
   type = AI_TYPE_FLOAT;
   defaultValue.FLT() = value;
   return *this;
}
template<> MAttrData& MAttrData::setDefault<int>   (int value)   {
   if(type != AI_TYPE_ENUM) type = AI_TYPE_INT;
   defaultValue.INT() = value;
   return *this;
}
template<> MAttrData& MAttrData::setDefault<MString>(MString value){
   type = AI_TYPE_STRING;
   stringDefault = value;
   return *this;
}
template<> MAttrData& MAttrData::setDefault<bool>   (bool value)   {
   type = AI_TYPE_BOOLEAN;
   defaultValue.BOOL() = value;
   return *this;
}
template<> MAttrData& MAttrData::setDefault<AtVector>  (AtVector value)  {
   type = AI_TYPE_VECTOR;
   defaultValue.VEC() = value;
   return *this;
}
template<> MAttrData& MAttrData::setDefault< std::initializer_list<const char*> >(std::initializer_list<const char*> value){
   type = AI_TYPE_ENUM;
   MStringArray enumsStrings;
   for(auto& str : value)
      enumsStrings.append(MString(str));
   this->enums = enumsStrings;
   defaultValue.INT() = 0;
   return *this;
}
template<> MAttrData& MAttrData::setSoft<float> (float min, float max)  {
   this->hasSoftMin = this->hasSoftMax = true;
   this->softMin.FLT() = min;
   this->softMax.FLT() = max;
   return *this;
}
template<> MAttrData& MAttrData::setMin<float>  (float min)  {
   this->hasMin = true;
   this->min.FLT() = min;
   return *this;
}
template<> MAttrData& MAttrData::setSoft<int>  (int min, int max)  {
   this->hasSoftMin = this->hasSoftMax = true;
   this->softMin.INT() = min;
   this->softMax.INT() = max;
   return *this;
}
template<> MAttrData& MAttrData::setMin<int>  (int min)  {
   this->hasMin = true;
   this->min.INT() = min;
   return *this;
}

}
