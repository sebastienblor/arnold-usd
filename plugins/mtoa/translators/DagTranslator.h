#pragma once

#include "NodeTranslator.h"

// Abstract base class for Dag node translators
//

class DLLEXPORT CDagTranslator : public CNodeTranslator
{

public:

//------- Virtual methods inherited from CNodeTranslator

   // initialize the translator
   virtual void Init();

   // Customize the callbacks invoked during IPR updates for DAG nodes
   virtual void AddUpdateCallbacks();

protected:
   CDagTranslator() : CNodeTranslator(){}

   // Export (convert from Maya to Arnold) this nodes
   virtual void Export(AtNode* atNode);

   // Export for motion steps
   virtual void ExportMotion(AtNode* atNode);

//-----------------------------------------

public:

   const MDagPath &GetMayaDagPath() const { return m_dagPath; }

   // Return true if this DAG node is meant to be rendered (or false it's hidden, etc...).
   // This method can be overridden if necessary
   virtual bool IsRenderable() const;

   // Create Maya visibility attributes with standardized render flag names
   // These are the attributes that compute the "visibility" parameter. there are other
   // attributes like self_shadow and opaque that are computed separately

   // This is for custom DAG nodes where none of the standard maya visibility attributes
   // are available. typically CDagTranslator::AddArnoldVisibilityAttrs() is the appropriate function.
   static void MakeMayaVisibilityFlags(CBaseAttrHelper& helper);
   
   // Arnold's visibiltity mask adds several relationships not available by default in Maya.
   // use in conjunction with CDagTranslator::ComputeVisibility() or CShapeTranslator::ProcessRenderFlags().
   static void MakeArnoldVisibilityFlags(CBaseAttrHelper& helper);

   // Static function to force the export of a specific MDagPath and get the generated translator.
   // Only needed in very rare situations
   static CDagTranslator *ExportDagPath(const MDagPath &dagPath);

   // returns true if this is one of the translate/rotate/scale parameters
   static bool IsTransformPlug(const MPlug &plug);

   // returns the arnold name for a corresponding maya dag path
   static MString GetArnoldNaming(const MDagPath &dagPath);

   // If this function returns false, children of this dag node won't be exported
   virtual bool ExportDagChildren() const {return true;}

protected:

   // This is a utility method which handles the common tasks associated with
   // exporting matrix information. It properly handles exporting a matrix array
   // if motion blur is enabled and required by the node. it should be called
   // at each motion step
   void ExportMatrix(AtNode* node);

   /// Return whether the current dag object is the master instance.
   /// The master is the first instance that is completely visible (including parent transforms)
   /// for which full geometry should be exported.
   bool IsMasterInstance();
   MDagPath& GetMasterInstance();

   // Use standardized render flag names to return an arnold visibility mask
   AtByte ComputeVisibility();

   // Compute the Arnold Matrix for current node. 
   // This method can be overridden if necessary
   virtual void GetMatrix(AtMatrix& matrix);
   
   // Maya Dag path of this maya object
   MDagPath m_dagPath;

private:
   // internal use only, don't override it
   virtual void CreateImplementation();
};
