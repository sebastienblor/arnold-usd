
#include "MayaScene.h"

#include <ai_cameras.h>
#include <ai_constants.h>
#include <ai_msg.h>
#include <ai_nodes.h>

#include <maya/M3DView.h>
#include <maya/MDagPath.h>
#include <maya/MFloatVector.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFnAmbientLight.h>
#include <maya/MFnBlinnShader.h>
#include <maya/MFnCamera.h>
#include <maya/MFnDirectionalLight.h>
#include <maya/MFnLambertShader.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MFnPhongShader.h>
#include <maya/MFnPointLight.h>
#include <maya/MFnSpotLight.h>
#include <maya/MFnStringData.h>
#include <maya/MImage.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>

#include <vector>

namespace  // <anonymous>
{

   struct CShaderData
   {

      MObject mayaShader;
      AtNode* arnoldShader;

   };  // struct CShaderData

   std::vector<CShaderData> processedShaders;

   MObject GetNodeShader(MObject dagNode)
   {

      MPlugArray        connections;
      MFnDependencyNode fnDGNode(dagNode);

      // Find the "instObjGroups" attribute. Follow that plug to see where it is connected.
      MPlug plug(dagNode, fnDGNode.attribute("instObjGroups"));

      plug.elementByLogicalIndex(0).connectedTo(connections, false, true);

      if (connections.length() != 1)
      {
         return MObject::kNullObj;
      }

      MObject shadingGroup(connections[0].node());

      fnDGNode.setObject(shadingGroup);

      MPlug shaderPlug(shadingGroup, fnDGNode.attribute("surfaceShader"));

      connections.clear();

      shaderPlug.connectedTo(connections, true, false);

      return connections[0].node();

   }  // GetNodeShader()

   /*
   CImage* ProcessTexture( const char* texture )
   {

   if ( !texture )
   {
   return NULL;
   }

   for (AtUInt32 J = 0; ( J < exportData.textureMaps.size() ); ++J)
   {
   std::map<std::string, CImage*>::iterator it = exportData.textureMaps.find( texture );

   if ( it != exportData.textureMaps.end() )
   {
   return it->second;
   }
   }

   MImage         image;
   unsigned int   width, height;

   image.readFromFile( texture );
   image.getSize( width, height );

   CImage*   textureMap = TheImageManager::GetInstance().createImage( width, height, NULL );

   CRGBColor   color;
   AtUInt32*     pixel = (AtUInt32*) image.pixels();

   for (unsigned int J = 0; ( J < height ); J++)
   {
   for (unsigned int I = 0; ( I < width ); I++)
   {
   color.getFromDWord( *pixel );

   textureMap->setPixel( I, J, color );

   pixel++;
   }
   }

   image.release();

   exportData.textureMaps[texture] = textureMap;

   exportData.scene->addMap( textureMap );

   return textureMap;

   }  // ProcessTexture()


   EResult ProcessBumpAttrib( MFnLambertShader shader, const char* name, CMaterialBumpProperty& property )
   {

   MObject             fileName;
   MPlug               filePlug, colorPlug;
   MPlugArray          connections;
   MFnDependencyNode   fnDGNode; 

   colorPlug = shader.findPlug( name );

   colorPlug.connectedTo( connections, true, false );

   if ( connections.length() == 0 )
   {
   return RESULT_ERROR;
   }

   fnDGNode.setObject( connections[0].node() );

   filePlug = fnDGNode.findPlug( "bumpValue" );

   connections.clear();

   filePlug.connectedTo( connections, true, false );

   if ( connections.length() == 0 )
   {
   return RESULT_ERROR;
   }

   fnDGNode.setObject( connections[0].node() );

   filePlug = fnDGNode.findPlug( "fileTextureName" );

   filePlug.getValue( fileName );

   MFnStringData   fileNameString( fileName );

   CImage*   textureMap = ProcessTexture( fileNameString.string().asChar(), exportData );

   property.setTexture( textureMap );

   return RESULT_OK;

   }  // ProcessBumpAttrib()


   EResult ProcessColorAttrib( MFnLambertShader shader, const char* name, MColor color, CMaterialColorProperty& property )
   {

   MObject             fileName;
   MPlug               filePlug, colorPlug;
   MPlugArray          connections;
   MFnDependencyNode   fnDGNode; 

   colorPlug = shader.findPlug( name );

   colorPlug.connectedTo( connections, true, false );

   if ( connections.length() == 0 )
   {
   property.setConstant( CRGBColor( color.r, color.g, color.b ) );

   return RESULT_OK;
   }

   fnDGNode.setObject( connections[0].node() );

   filePlug = fnDGNode.findPlug( "fileTextureName" );

   filePlug.getValue( fileName );

   MFnStringData   fileNameString( fileName );

   CImage*   textureMap = ProcessTexture( fileNameString.string().asChar(), exportData );

   property.setTexture( textureMap );

   return RESULT_OK;

   }  // ProcessColorAttrib()


   EResult ProcessFloatAttrib( MFnLambertShader shader, const char* name, float value, CMaterialFloatProperty& property )
   {

   MObject             fileName;
   MPlug               filePlug, colorPlug;
   MPlugArray          connections;
   MFnDependencyNode   fnDGNode; 

   colorPlug = shader.findPlug( name );

   colorPlug.connectedTo( connections, true, false );

   if ( connections.length() == 0 )
   {
   property.setConstant( value );

   return RESULT_OK;
   }

   fnDGNode.setObject( connections[0].node() );

   filePlug = fnDGNode.findPlug( "fileTextureName" );

   filePlug.getValue( fileName );

   MFnStringData   fileNameString( fileName );

   CImage*   textureMap = ProcessTexture( fileNameString.string().asChar(), exportData );

   property.setTexture( textureMap );

   return RESULT_OK;

   }  // ProcessFloatAttrib()
   */


   AtNode* ProcessShader(MObject mayaShader)
   {
      // First check if this shader has already been processed
      for (std::vector<CShaderData>::const_iterator it = processedShaders.begin(); (it != processedShaders.end()); ++it)
      {
         if (it->mayaShader == mayaShader)
         {
            return it->arnoldShader;
         }
      }

      AtNode* shader = NULL;

      switch (mayaShader.apiType())
      {
      case MFn::kLambert:
         {
            MFnLambertShader fnShader(mayaShader);

            shader = AiNode("lambert");

            //ProcessBumpAttrib( fnShader, "normalCamera", material->bump, exportData );
            //ProcessColorAttrib( fnShader, "incandescence", fnShader.incandescence(), material->emission, exportData );
            //ProcessColorAttrib( fnShader, "color", fnShader.color(), material->diffuseColor, exportData );
            //ProcessFloatAttrib( fnShader, "diffuse", fnShader.diffuseCoeff(), material->diffuseCoefficient, exportData );
         }
         break;

      case MFn::kPhong:
         {
            //MFnPhongShader   fnShader( mayaMaterial );

            //material->bsdf = new CPhongBSDF( material );

            //ProcessBumpAttrib( fnShader, "normalCamera", material->bump, exportData );
            //ProcessColorAttrib( fnShader, "incandescence", fnShader.incandescence(), material->emission, exportData );
            //ProcessColorAttrib( fnShader, "color", fnShader.color(), material->diffuseColor, exportData );
            //ProcessFloatAttrib( fnShader, "diffuse", fnShader.diffuseCoeff(), material->diffuseCoefficient, exportData );
            //ProcessColorAttrib( fnShader, "specular color", fnShader.specularColor(), material->specularColor, exportData );
            //ProcessFloatAttrib( fnShader, "reflectivity", fnShader.reflectivity(), material->specularCoefficient, exportData );
            //ProcessFloatAttrib( fnShader, "cosine power", fnShader.cosPower(), material->specularExponent, exportData );
         }
         break;

      case MFn::kBlinn:
         {
            //MFnBlinnShader   fnShader( mayaMaterial );

            //// [_AJJ_TODO_] Crear una BSDF con el método de Blinn
            //material->bsdf = new CLambertianBSDF( material );

            //ProcessBumpAttrib( fnShader, "normalCamera", material->bump, exportData );
            //ProcessColorAttrib( fnShader, "incandescence", fnShader.incandescence(), material->emission, exportData );
            //ProcessColorAttrib( fnShader, "color", fnShader.color(), material->diffuseColor, exportData );
            //ProcessFloatAttrib( fnShader, "diffuse", fnShader.diffuseCoeff(), material->diffuseCoefficient, exportData );
         }
         break;
      }

      if (shader)
      {
         CShaderData   data;

         data.mayaShader   = mayaShader;
         data.arnoldShader = shader;

         processedShaders.push_back(data);
      }

      return shader;

   }  // ProcessShader()


   void ProcessCamera(const MDagPath& dagPath)
   {

      MPoint     point;
      MVector    vector;
      MFnCamera  fnCamera(dagPath);
      MFnDagNode fnDagNode(dagPath);

      AtNode* camera = AiNode("persp_camera");

      AiNodeSetStr(camera, "name", fnDagNode.name().asChar());

      point = fnCamera.eyePoint(MSpace::kWorld);
      AiNodeSetPnt(camera, "position", point.x, point.y, point.z);

      vector = fnCamera.viewDirection(MSpace::kWorld);
      point  = point + vector;
      AiNodeSetPnt(camera, "look_at", point.x, point.y, point.z);

      vector = fnCamera.upDirection(MSpace::kWorld);
      AiNodeSetVec(camera, "up", vector.x, vector.y, vector.z);

      AiNodeSetFlt(camera, "fov", static_cast<float>(AI_RTOD * fnCamera.horizontalFieldOfView()));
      AiNodeSetStr(camera, "handedness", "right");

   }  // ProcessCamera()


   void ProcessLight(const MDagPath& dagPath)
   {

      MTransformationMatrix   lightWorldMatrix;
      MFloatVector            vector;
      MColor                  color;
      AtNode*                 light = NULL;
      MFnDagNode              fnDagNode(dagPath);

      if (dagPath.hasFn(MFn::kAmbientLight))
      {
      }
      else if (dagPath.hasFn(MFn::kDirectionalLight))
      {
         MFnDirectionalLight fnLight(dagPath);

         light = AiNode("distant_light");

         AiNodeSetStr(light, "name", fnDagNode.name().asChar());

         color = fnLight.color();

         AiNodeSetRGB(light, "color", color.r, color.g, color.b);
         AiNodeSetFlt(light, "intensity", fnLight.intensity());

         vector = fnLight.lightDirection(0, MSpace::kWorld);

         AiNodeSetVec(light, "direction", vector.x, vector.y, vector.z);
         AiNodeSetBool(light, "cast_shadows", fnLight.useRayTraceShadows());
      }
      else if (dagPath.hasFn(MFn::kPointLight))
      {
         MFnPointLight fnLight(dagPath);

         light = AiNode("point_light");

         AiNodeSetStr(light, "name", fnDagNode.name().asChar());

         color = fnLight.color();

         AiNodeSetRGB(light, "color", color.r, color.g, color.b);
         AiNodeSetFlt(light, "intensity", fnLight.intensity());

         lightWorldMatrix = dagPath.inclusiveMatrix();
         vector           = lightWorldMatrix.translation(MSpace::kWorld);

         AiNodeSetVec(light, "position", vector.x, vector.y, vector.z);

         AiNodeSetBool(light, "cast_shadows", fnLight.useRayTraceShadows());
      }
      else if (dagPath.hasFn(MFn::kSpotLight))
      {
      }
      else if (dagPath.hasFn(MFn::kAreaLight))
      {
      }
      else
      {
         MFnLight fnLight(dagPath);

         AiMsgError("[mtoa] ERROR: Unknown light type (%s).\n", fnLight.typeName().asChar());
      }

   }  // ProcessLight()


   void ProcessMesh(MObject mayaMesh, MObject dagNode, MMatrix tm)
   {

      MFnMesh    fnMesh(mayaMesh);
      MFnDagNode fnDagNode(dagNode);
      bool       hasUVs = (fnMesh.numUVs() > 0);
      AtNode*    polymesh = AiNode("polymesh");

      AiNodeSetStr(polymesh, "name", fnDagNode.name().asChar());

      AtMatrix matrix;

      for (int J = 0; (J < 4); ++J)
      {
         for (int I = 0; (I < 4); ++I)
         {
            matrix[I][J] = (float) tm[I][J];
         }
      }

      AiNodeSetMatrix(polymesh, "matrix", matrix);

      AiNodeSetBool(polymesh, "smoothing", 1);

      //
      // SHADERS
      //

      //MObject   mayaShader = GetNodeShader( dagNode );

      //if (!mayaShader.isNull())
      //{
      //	// This object has a single shader
      //}
      //else
      //{
      //	// This object has multiple shaders
      //}

      //
      // GEOMETRY
      //

      // Get all vertices
      if (fnMesh.numVertices() > 0)
      {
         MFloatPointArray pointsArray;

         fnMesh.getPoints(pointsArray, MSpace::kObject);

         float* vertices = new float[fnMesh.numVertices() * 3];

         for (int J = 0; ( J < fnMesh.numVertices() ); ++J)
         {
            vertices[J * 3 + 0] = pointsArray[J].x;
            vertices[J * 3 + 1] = pointsArray[J].y;
            vertices[J * 3 + 2] = pointsArray[J].z;
         }

         AiNodeSetArray(polymesh, "vlist", AiArrayConvert(fnMesh.numVertices() * 3, 1, AI_TYPE_FLOAT, vertices, TRUE));

         delete[] vertices;
      }

      // Get all normals
      if (fnMesh.numNormals() > 0)
      {
         MFloatVectorArray normalArray;

         fnMesh.getNormals(normalArray, MSpace::kObject);

         float* normals = new float[fnMesh.numNormals() * 3];

         for (int J = 0; (J < fnMesh.numNormals()); ++J)
         {
            normals[J * 3 + 0] = normalArray[J].x;
            normals[J * 3 + 1] = normalArray[J].y;
            normals[J * 3 + 2] = normalArray[J].z;
         }

         AiNodeSetArray(polymesh, "nlist", AiArrayConvert(fnMesh.numNormals() * 3, 1, AI_TYPE_FLOAT, normals, TRUE));

         delete[] normals;
      }

      // Get all UVs
      if (hasUVs)
      {
         MFloatArray uArray, vArray;

         fnMesh.getUVs(uArray, vArray);

         float* uvs = new float[fnMesh.numUVs() * 2];

         for (int J = 0; (J < fnMesh.numUVs()); ++J)
         {
            uvs[J * 2 + 0] = uArray[J];
            uvs[J * 2 + 1] = vArray[J];
         }

         AiNodeSetArray(polymesh, "uvlist", AiArrayConvert(fnMesh.numUVs() * 2, 1, AI_TYPE_FLOAT, uvs, TRUE));

         delete[] uvs;
      }

      // Traverse all polygons to export vidxs, nidxs, uvindxs y nsides
      AtByte* nsides = new AtByte[fnMesh.numPolygons()];

      std::vector<AtLong> vidxs, nidxs, uvidxs;

      MItMeshPolygon itMeshPolygon(mayaMesh);
      unsigned int   polygonIndex = 0;

      for (; (!itMeshPolygon.isDone()); itMeshPolygon.next())
      {
         unsigned int vertexCount = itMeshPolygon.polygonVertexCount();

         nsides[polygonIndex] = vertexCount;

         for (size_t V = 0; (V < vertexCount); ++V)
         {
            vidxs.push_back(itMeshPolygon.vertexIndex(V));
            nidxs.push_back(itMeshPolygon.normalIndex(V));

            if (hasUVs)
            {
               int uvIndex;

               itMeshPolygon.getUVIndex(V, uvIndex);

               uvidxs.push_back(uvIndex);
            }
         }

         ++polygonIndex;
      }

      AiNodeSetArray(polymesh, "nsides", AiArrayConvert(fnMesh.numPolygons(), 1, AI_TYPE_BYTE, nsides, TRUE));
      AiNodeSetArray(polymesh, "vidxs", AiArrayConvert(vidxs.size(), 1, AI_TYPE_UINT, &(vidxs[0]), TRUE));
      AiNodeSetArray(polymesh, "nidxs", AiArrayConvert(nidxs.size(), 1, AI_TYPE_UINT, &(nidxs[0]), TRUE));

      if (hasUVs)
      {
         AiNodeSetArray(polymesh, "uvidxs", AiArrayConvert(uvidxs.size(), 1, AI_TYPE_UINT, &(uvidxs[0]), TRUE));
      }

      delete[] nsides;

      /*
      MObjectArray        shaders;
      CMaterial*          material = 0;

      std::vector<CMaterial*>   meshMaterials;

      if ( !mayaMaterial.isNull() )
      {
      material = ProcessMaterial( mayaMaterial, exportData );
      }
      else
      {
      fnMesh.getConnectedShaders( 0, shaders, indices );

      for (int J = 0; ( J < (int) shaders.length() ); ++J)
      {
      MPlugArray          connections;
      MFnDependencyNode   fnDGNode( shaders[J] );
      MPlug               shaderPlug( shaders[J], fnDGNode.attribute( "surfaceShader" ) );

      shaderPlug.connectedTo( connections, true, false );

      meshMaterials.push_back( ProcessMaterial( connections[0].node(), exportData ) );
      }
      }

      MPoint   point;
      CMesh*   mesh = new CMesh;

      for (; ( !itMeshPolygon.isDone() ); itMeshPolygon.next())
      {
      CMaterial*   triangleMaterial;

      if ( material )
      {
      triangleMaterial = material;
      }
      else
      {
      AtUInt32   shaderIndex = indices[itMeshPolygon.index()];

      triangleMaterial = meshMaterials[shaderIndex];
      }

      int      numTriangles;
      double   area;

      itMeshPolygon.getArea( area );
      itMeshPolygon.numTriangles( numTriangles );

      if ( numTriangles == 1 )
      {
      assert ( itMeshPolygon.polygonVertexCount( &status ) == 3 );

      if ( area < 1e-5 )
      {
      continue;
      }

      CTriangle              triangle;
      CTriangleShadingData   triData;

      for (AtUInt32 I = 0; ( I < 3 ); I++)
      {
      int   index;

      fnMesh.getPoint( itMeshPolygon.vertexIndex( I ), point, MSpace::kWorld );

      triangle.vertices[I].set( (float) point.x, (float) point.y, (float) point.z );

      index = itMeshPolygon.normalIndex( I );

      triData.n[I].set( normalArray[index].x, normalArray[index].y, normalArray[index].z );

      if ( hasUV )
      {
      itMeshPolygon.getUVIndex( I, index );

      triData.tu[I] = uArray[index];
      triData.tv[I] = vArray[index];
      }
      }

      if ( triangle.initialize() < 0 )
      {
      continue;
      }

      triData.material = triangleMaterial;

      mesh->addTriangle( triangle, triData );
      }
      else
      {
      if ( !itMeshPolygon.hasValidTriangulation() || ( area < 1e-5 ) )
      {
      continue;
      }

      for (AtUInt32 J = 1; ( J < (itMeshPolygon.polygonVertexCount() - 1) ); ++J)
      {
      int       index;
      CVector   v1, v2, v3;

      CTriangle              triangle;
      CTriangleShadingData   triData;

      fnMesh.getPoint( itMeshPolygon.vertexIndex( 0 ), point, MSpace::kWorld );
      v1.set( (float) point.x, (float) point.y, (float) point.z );
      fnMesh.getPoint( itMeshPolygon.vertexIndex( J ), point, MSpace::kWorld );
      v2.set( (float) point.x, (float) point.y, (float) point.z );
      fnMesh.getPoint( itMeshPolygon.vertexIndex( J + 1 ), point, MSpace::kWorld );
      v3.set( (float) point.x, (float) point.y, (float) point.z );

      CVector cross;

      CrossProduct( cross, v2 - v1, v3 - v2 );

      if ( fabs( cross.norm() ) < 1e-5 )
      {
      continue;
      }

      // Write Vertex 0
      fnMesh.getPoint( itMeshPolygon.vertexIndex( 0 ), point, MSpace::kWorld );

      triangle.vertices[0].set( (float) point.x, (float) point.y, (float) point.z );

      index = itMeshPolygon.normalIndex( 0 );

      triData.n[0].set( normalArray[index].x, normalArray[index].y, normalArray[index].z );

      if ( hasUV )
      {
      itMeshPolygon.getUVIndex( 0, index );

      triData.tu[0] = uArray[index];
      triData.tv[0] = vArray[index];
      }

      // Write Vertex J
      fnMesh.getPoint( itMeshPolygon.vertexIndex( J ), point, MSpace::kWorld );

      triangle.vertices[J].set( (float) point.x, (float) point.y, (float) point.z );

      index = itMeshPolygon.normalIndex( J );

      triData.n[J].set( normalArray[index].x, normalArray[index].y, normalArray[index].z );

      if ( hasUV )
      {
      itMeshPolygon.getUVIndex( J, index );

      triData.tu[J] = uArray[index];
      triData.tv[J] = vArray[index];
      }

      // Write Vertex J + 1
      fnMesh.getPoint( itMeshPolygon.vertexIndex( J + 1 ), point, MSpace::kWorld );

      triangle.vertices[J + 1].set( (float) point.x, (float) point.y, (float) point.z );

      index = itMeshPolygon.normalIndex( J + 1 );

      triData.n[J + 1].set( normalArray[index].x, normalArray[index].y, normalArray[index].z );

      if ( hasUV )
      {
      itMeshPolygon.getUVIndex( J + 1, index );

      triData.tu[J + 1] = uArray[index];
      triData.tv[J + 1] = vArray[index];
      }

      if ( triangle.initialize() < 0 )
      {
      continue;
      }

      triData.material = triangleMaterial;

      mesh->addTriangle( triangle, triData );
      }
      }
      }

      // Create a new TriObject in Escher and add to scene
      CTriObject*   triObject = new CTriObject();
      CSceneNode*   node      = new CSceneNode();

      node->setObject( triObject );

      triObject->setMesh( mesh );

      exportData.scene->addNode( node );
      */

   }  // processMesh()

}  // namespace <anonymous>


MStatus ProcessMayaScene(MItDag::TraversalType traversalType)
{

   MDagPath dagPath;
   MStatus  status;
   MItDag   dagIterator(traversalType, MFn::kInvalid);

   // Export current camera
   MDagPath cameraPath;

   M3dView::active3dView().getCamera(cameraPath);

   ProcessCamera(cameraPath);

   for (dagIterator.reset(); (!dagIterator.isDone()); dagIterator.next())
   {
      // MItDag::getPath() gets the reference to the object that the iterator is currently on.
      // This DAG path can then be used in a function set to operate on the object.
      // In general it is not a good idea to rearrange the DAG from with an iterator.
      if (!dagIterator.getPath(dagPath))
      {
         AiMsgError("[mtoa] ERROR: Could not get path for DAG iterator.");

         return status;
      }

      //MFnDagNode   dagNode( dagPath, &status );

      //if ( !status )
      //{
      //	AiMsgError( "ERROR: Could not get node.\n" );
      //
      //	return status;
      //}
      //
      //printf ("<NODE>\n");
      //printf ("  Name: %s\n", dagNode.name().asChar());
      //printf ("  Type: %s\n", dagNode.typeName().asChar());
      //printf ("  Path: %s\n", dagPath.fullPathName().asChar());
      //printf ("  Chld: %u\n", dagNode.childCount());

      // CON EL CHILDCOUNT DEL NODO PODEMOS SABER SI TIENE HIJOS O NO...
      // pero MItDag::item devuelve en MObject al que le podemos preguntar con hasFn que es con lo cual
      // parece que evitamos el problema de que una transformacion diga que es una camara (dagPath.hasFn
      // dice si el path (del cual al final puede haber un nodo que sea una camara), contiene una camara
      // con lo que el nodo de la transformacion dara como resultado true con lo que nos engaña.

      MMatrix tm = dagPath.inclusiveMatrix();

      if (dagIterator.item().hasFn(MFn::kLight))
      {
         ProcessLight(dagPath);
      }
      else if (dagIterator.item().hasFn(MFn::kNurbsSurface))
      {
         MFnNurbsSurface surface(dagPath, &status);

         if (!status)
         {
            AiMsgError("[mtoa] ERROR: Could not create NURBS surface.");

            return status;
         }

         MFnMeshData meshData;
         MObject     meshFromNURBS;
         MObject     meshDataObject = meshData.create();

         meshFromNURBS = surface.tesselate(MTesselationParams::fsDefaultTesselationParams, meshDataObject);

         AiMsgInfo("[mtoa] Processing NURBS surface...");

         ProcessMesh(meshFromNURBS, dagIterator.item(), tm);
      }
      else if (dagIterator.item().hasFn(MFn::kMesh))
      {
         unsigned int      numMeshGroups;
         MFnDependencyNode fnDGNode(dagIterator.item());

         // Buscamos el atributo "instObjGroups" del nodo para ver si esta conectado
         MPlug plug(dagIterator.item(), fnDGNode.attribute("instObjGroups"));

         if (plug.elementByLogicalIndex(0).isConnected())
         {
            numMeshGroups = 1;
         }
         else
         {
            MFnMesh      mesh(dagIterator.item());
            MObjectArray shaders;
            MIntArray    indices;

            mesh.getConnectedShaders(0, shaders, indices);

            unsigned int numMeshGroups = shaders.length();
         }

         if (numMeshGroups == 0)
         {
            AiMsgError("[mtoa] ERROR: Mesh not exported. It has 0 groups.");
         }
         else
         {
            AiMsgInfo( "[mtoa] Processing mesh..." );

            ProcessMesh(dagIterator.item(), dagIterator.item(), tm);
         }
      }
      else
      {
         if (!status)
         {
            AiMsgError("[mtoa] ERROR: Unrecognized node found while iterating DAG.");

            return status;
         }
      }
   }

   return MS::kSuccess;

}  // ProcessMayaScene()
