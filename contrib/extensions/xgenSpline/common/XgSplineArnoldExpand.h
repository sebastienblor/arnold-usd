#include <ai.h>
#include <SgCurve.h>
#include <XgSplineAPI.h>
#include <cstring>
#include <string>
#include <sstream>

namespace XgArnoldInternal
{

using namespace XGenSplineAPI;
// ============================================================================

// Procedural to create "curves" node from input parameters
class XgSplineProcedural
{
public:
   XgSplineProcedural()
      : _curves(NULL)
      , _numPoints(NULL)
      , _points(NULL)
      , _radius(NULL)
      , _uvCoord(NULL)
      , _shader(NULL)
      , _aiMinPixelWidth(0.0f)
      , _aiMode(0)
   {}

   ~XgSplineProcedural()
   {}

   bool Init(AtNode* procedural, bool procParent);

   int NumNodes()
   {
      return _curves ? 1 : 0;
   }

   AtNode* GetNode(int i)
   {
      return _curves;
   }

private:
   bool LoadSplineData(AtNode* procedural);
   void LoadParameters(AtNode* procedural);
   void CreateCurves(AtNode* procedural, bool procParent);

   SgVec3f Orientation(const SgVec3f& direction, const SgVec3f* positions, unsigned int i, unsigned int length, const unsigned int offset);

   void FillCurves(AtNode* procedural);

   template<typename T>
   inline static T PhantomFirst(const T* cpt, unsigned int)
   {
      return cpt[0] + (cpt[0] - cpt[1]);
   }

   template<typename T>
   inline static T PhantomLast(const T* cpt, unsigned int nump)
   {
      const unsigned int nump1 = nump - 1;
      return cpt[nump1] + (cpt[nump1] - cpt[nump1-1]);
   }

private:
   // Input spline data
   XgFnSpline  _splines;

   // Output curves node
   AtNode*     _curves;

   // Curve arrays
   AtArray*    _numPoints;
   AtArray*    _points;
   AtArray*    _radius;
   AtArray*    _uvCoord;
   
   // Material
   AtNode*     _shader;

   // Parameters
   float       _aiMinPixelWidth;
   int         _aiMode;
};


};