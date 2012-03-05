import pymel
import pymel.core as pm
from maya.utils import executeDeferred
from mtoa.ui.ae.utils import aeCallback, AttrControlGrp
from mtoa.utils import prettify
import mtoa.callbacks as callbacks
import mtoa.core as core
import arnold

def shapeTemplate(nodeName):
    """
    override for the builtin maya shapeTemplate procedure
    """
    # Run the hooks.
    # see mtoa.registerArnoldRenderer._addAEHooks for where loadArnoldTemplate gets added to AEshapeHooks
    for hook in pm.melGlobals['AEshapeHooks']:
        pm.mel.eval(hook + ' "' + nodeName + '"')

    pm.editorTemplate(beginLayout=pm.mel.uiRes("m_AEshapeTemplate.kObjectDisplay"))

    # include/call base class/node attributes
    pm.mel.AEdagNodeCommon(nodeName)
    pm.editorTemplate(endLayout=True)

    # include/call base class/node attributes
    pm.mel.AEdagNodeInclude(nodeName)

