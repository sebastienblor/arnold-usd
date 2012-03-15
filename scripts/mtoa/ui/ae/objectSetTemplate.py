import pymel.core as pm
import mtoa.ui.ae.templates as templates

class ObjectSetTemplate(templates.AttributeEditorTemplate):
    def setup(self):
        self.addControl("aiOverride")

    def update(self):
        # if self.nodeName is None or not pm.objExists(self.nodeName) \
         #   or self.networkCol is None or not pm.layout(self.networkCol, exists=True):
         #   return

        nodeAttr = pm.Attribute(self.nodeAttr('aiCustomAOVs'))
        self.updateAOVFrame(nodeAttr)




templates.registerAETemplate(ObjectSetTemplate, "objectSet")

