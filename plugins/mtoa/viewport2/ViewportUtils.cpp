#include "ViewportUtils.h"

#include <GL/glew.h>

#include <maya/MShaderManager.h>
#include <maya/MFragmentManager.h>

namespace{
    MString searchPath = "";
    bool searchPathInitialized = false;

    void CheckPluginSearchPath(MHWRender::MFragmentManager* fragmentMgr, const MHWRender::MShaderManager* shaderMgr)
    {
        if (!searchPathInitialized)
        {
            searchPathInitialized = true;
            if (searchPath != "")
            {
                fragmentMgr->addFragmentPath(searchPath);
                shaderMgr->addShaderPath(searchPath);
            }
        }
    }
    int glewInitialized = -1;
}

void SetFragmentSearchPath(const MString& path)
{
    if (searchPath != path)
    {
        searchPath = path;
        searchPathInitialized = false;

        MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
        if (theRenderer)
        {
            MHWRender::MFragmentManager* fragmentMgr = theRenderer->getFragmentManager();
            const MHWRender::MShaderManager* shaderMgr = theRenderer->getShaderManager();
            if (fragmentMgr && shaderMgr)
            {
                CheckPluginSearchPath(fragmentMgr, shaderMgr);
            }
        }
    }
}

bool LoadFragmentGraph(const MString& fragmentGraph, const MStringArray& requirements)
{
    MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer(false);
    if (theRenderer)
    {
        MHWRender::MFragmentManager* fragmentMgr = theRenderer->getFragmentManager();
        const MHWRender::MShaderManager* shaderMgr = theRenderer->getShaderManager();
        if (fragmentMgr && shaderMgr)
        {
            CheckPluginSearchPath(fragmentMgr, shaderMgr);
            
            bool loadedSuccessfully = true;
            for (unsigned int i = 0; (i < requirements.length()) && loadedSuccessfully; ++i)
            {
                MString req = requirements[i];
                if (!fragmentMgr->hasFragment(req))
                    loadedSuccessfully &= (req == fragmentMgr->addShadeFragmentFromFile(req + MString(".xml"), false));
            }
            if (loadedSuccessfully && !fragmentMgr->hasFragment(fragmentGraph))
                loadedSuccessfully &= (fragmentGraph == fragmentMgr->addFragmentGraphFromFile(fragmentGraph + MString(".xml")));
            return loadedSuccessfully;
        }
    }
    return false;
}

bool LoadShadeFragment(const MString& shadeFragment)
{
    MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
    if (theRenderer)
    {
        MHWRender::MFragmentManager* fragmentMgr = theRenderer->getFragmentManager();
        const MHWRender::MShaderManager* shaderMgr = theRenderer->getShaderManager();
        if (fragmentMgr && shaderMgr)
        {
            CheckPluginSearchPath(fragmentMgr, shaderMgr);
            bool loadedSuccessfully = true;
            if (!fragmentMgr->hasFragment(shadeFragment))
                loadedSuccessfully &= (shadeFragment == fragmentMgr->addShadeFragmentFromFile(shadeFragment + MString(".xml"), false));
            return loadedSuccessfully;
        }
    }    
    return false;
}

bool InitializeGLEW()
{
    if (glewInitialized == -1)
    {
        GLenum err = glewInit();
        if (GLEW_OK != err)
            glewInitialized = 0;
        else
            glewInitialized = 1;
    }
    return glewInitialized == 1;
}
