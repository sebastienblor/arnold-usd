#include "ArnoldStandInGeometryOverride.h"

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MTransformationMatrix.h>

#include <iostream>

#include <ai.h>

#include "utils/MayaUtils.h"
#include "ViewportUtils.h"


static float cube[][3] = { {0.0f, 0.0f, 0.0f},
                     {1.0f, 0.0f, 0.0f},
                     {0.0f, 0.0f, 0.0f},
                     {0.0f, 1.0f, 0.0f},
                     {1.0f, 0.0f, 0.0f},
                     {1.0f, 1.0f, 0.0f},
                     {0.0f, 1.0f, 0.0f},
                     {1.0f, 1.0f, 0.0f},
                     
                     {0.0f, 0.0f, 1.0f},
                     {1.0f, 0.0f, 1.0f},
                     {0.0f, 0.0f, 1.0f},
                     {0.0f, 1.0f, 1.0f},
                     {1.0f, 0.0f, 1.0f},
                     {1.0f, 1.0f, 1.0f},
                     {0.0f, 1.0f, 1.0f},
                     {1.0f, 1.0f, 1.0f},
                     
                     {0.0f, 0.0f, 0.0f},
                     {0.0f, 0.0f, 1.0f},
                     {1.0f, 0.0f, 0.0f},
                     {1.0f, 0.0f, 1.0f},
                     {0.0f, 1.0f, 0.0f},
                     {0.0f, 1.0f, 1.0f},
                     {1.0f, 1.0f, 0.0f},
                     {1.0f, 1.0f, 1.0f},};
                     

static int cubeCount = 24;

CArnoldStandInGeometryOverride::CArnoldStandInGeometryOverride(const MObject& obj)
: MHWRender::MPxGeometryOverride(obj)
, mSolidUIShader(NULL)
, mLocatorNode(obj)
, minx(-1.0f), miny(-1.0f), minz(-1.0f)
, maxx(1.0f), maxy(1.0f), maxz(1.0f)
, mBBChanged(true)
{
    MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
    if (!renderer)
        return;

    const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
    if (!shaderMgr)
        return;

	mSolidUIShader = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dSolidShader);
}

CArnoldStandInGeometryOverride::~CArnoldStandInGeometryOverride()
{
	if(mSolidUIShader)
	{
		MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
		if (renderer)
		{
    		const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
    		if (shaderMgr)
    		{
				shaderMgr->releaseShader(mSolidUIShader);
			}
		}
		mSolidUIShader = NULL;
    }
}

MHWRender::DrawAPI CArnoldStandInGeometryOverride::supportedDrawAPIs() const
{
	// this plugin supports both GL and DX
	return (MHWRender::kAllDevices);
}

const MString colorParameterName_   = "solidColor";
const MString wireframecubeItemName_    = "cubeLocatorWires";


void CArnoldStandInGeometryOverride::updateDG()
{
   MFnDependencyNode depNode(mLocatorNode);
   
   minx = depNode.findPlug("MinBoundingBox0").asFloat();
   miny = depNode.findPlug("MinBoundingBox1").asFloat();
   minz = depNode.findPlug("MinBoundingBox2").asFloat();
   
   maxx = depNode.findPlug("MaxBoundingBox0").asFloat();
   maxy = depNode.findPlug("MaxBoundingBox1").asFloat();
   maxz = depNode.findPlug("MaxBoundingBox2").asFloat();

   mBBChanged = true;
}

void CArnoldStandInGeometryOverride::updateRenderItems( const MDagPath& path, MHWRender::MRenderItemList& list )
{
    MHWRender::MRenderItem* wireframecube = NULL;

    int index = list.indexOf(wireframecubeItemName_);

    if (index < 0)
    {
        wireframecube = MHWRender::MRenderItem::Create(
            wireframecubeItemName_,
            MHWRender::MRenderItem::NonMaterialSceneItem,
            MHWRender::MGeometry::kLines);
        wireframecube->setDrawMode(MHWRender::MGeometry::kAll);
        wireframecube->depthPriority(4);

        list.append(wireframecube);
    }
    else
    {
        wireframecube = list.itemAt(index);
    }

    if(wireframecube)
    {
		if (mSolidUIShader)
		{
			MColor color = MHWRender::MGeometryUtilities::wireframeColor(path);
			float  wireframeColor[4] = { color.r, color.g, color.b, 1.0f };
			mSolidUIShader->setParameter(colorParameterName_, wireframeColor);
			wireframecube->setShader(mSolidUIShader);
		}
		wireframecube->enable(true);
    }


}

void CArnoldStandInGeometryOverride::populateGeometry(
    const MHWRender::MGeometryRequirements& requirements,
    const MHWRender::MRenderItemList& renderItems,
    MHWRender::MGeometry& data)
{
    MHWRender::MVertexBuffer* verticesBuffer       = NULL;

    float* vertices                     = NULL;

	const MHWRender::MVertexBufferDescriptorList& vertexBufferDescriptorList = requirements.vertexRequirements();
	const int numberOfVertexRequirments = vertexBufferDescriptorList.length();

	MHWRender::MVertexBufferDescriptor vertexBufferDescriptor;
	for (int requirmentNumber = 0; requirmentNumber < numberOfVertexRequirments; ++requirmentNumber)
	{
		if (!vertexBufferDescriptorList.getDescriptor(requirmentNumber, vertexBufferDescriptor))
		{
			continue;
		}

		switch (vertexBufferDescriptor.semantic())
		{
		case MHWRender::MGeometry::kPosition:
			{
				if (!verticesBuffer)
				{
					verticesBuffer = data.createVertexBuffer(vertexBufferDescriptor);
					if (verticesBuffer)
					{
						vertices = (float*)verticesBuffer->acquire(cubeCount);
					}
				}
			}
			break;
		default:
			// do nothing for stuff we don't understand
			break;
		}
	}

	int verticesPointerOffset = 0;

	// We add the cube into a vertex buffer.
	for (int currentVertex = 0 ; currentVertex < cubeCount; ++currentVertex)
	{
		if(vertices)
		{
         vertices[verticesPointerOffset++] = cube[currentVertex][0] * (maxx - minx) + minx;
         vertices[verticesPointerOffset++] = cube[currentVertex][1] * (maxy - miny) + miny;
         vertices[verticesPointerOffset++] = cube[currentVertex][2] * (maxz - minz) + minz;
		}
	}

	if(verticesBuffer && vertices)
	{
		verticesBuffer->commit(vertices);
	}

	for (int i=0; i < renderItems.length(); ++i)
	{
		const MHWRender::MRenderItem* item = renderItems.itemAt(i);

		if (!item)
		{
			continue;
		}

		// Start position in the index buffer:
		int startIndex = 0;
		// Number of index to generate (for lines)
		int numIndex = 0;

		if (item->name() == wireframecubeItemName_ )
		{
			numIndex = cubeCount;
		}


		if (numIndex)
		{
		    MHWRender::MIndexBuffer*  indexBuffer = data.createIndexBuffer(MHWRender::MGeometry::kUnsignedInt32);
		    unsigned int* indices = (unsigned int*)indexBuffer->acquire(numIndex);

			for(int i = 0; i < numIndex; )
			{
            indices[i] = startIndex + i;
            i++;
			}

			indexBuffer->commit(indices);
			item->associateWithIndexBuffer(indexBuffer);
		}
	}
   mBBChanged = false;
}