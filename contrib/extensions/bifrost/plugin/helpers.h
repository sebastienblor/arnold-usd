#ifndef HELPERS_H
#define HELPERS_H

#include <extension/Extension.h>

namespace ArnoldBifrost{

class MAttrData : public CAttrData{
public:
   MAttrData(MString mayaNodeClassName);

   template<typename T>
   MAttrData& setup(const char* longName, const char* shortName, T value);

   template<typename T>
   MAttrData& setDefault(T value);
   template<typename T>
   MAttrData& setSoft(T min, T max);
   template<typename T>
   MAttrData& setMin(T min);

   void makeInput();
   void makeInputMesh(const char* longName, const char* shortName);

private:
   CExtensionAttrHelper m_helper;
};

}

#endif // HELPERS_H
