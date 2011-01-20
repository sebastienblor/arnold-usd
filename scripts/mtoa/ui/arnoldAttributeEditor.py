import maya.cmds as cmds
from mtoa.ui.callback import *
import os
import re

UI_NAME = 'ArnoldAttributeUI'

# Custom attribute creation command

def arnoldAttributeEditor_AddExportTangentsAttr(shape):
   cmds.addAttr(shape, ln="mtoa_export_tangents", nn="Export Tangents", at="bool")

def arnoldAttributeEditor_AddExportVertexColorAttr(shape):
   cmds.addAttr(shape, ln="mtoa_export_vertexColor", nn="Export Vertex Color", at="bool")

def arnoldAttributeEditor_AddInvisibleAttr(shape):
   cmds.addAttr(shape, ln="mtoa_invisible", nn="Invisible", at="bool")

def arnoldAttributeEditor_AddObjectIDAttr(shape):
   cmds.addAttr(shape, ln="mtoa_objectid", nn="Object ID", at="long")

def arnoldAttributeEditor_AddMatteOutAttr(shape):
   cmds.addAttr(shape, ln="mtoa_matteout", nn="Matte Out", at="bool")


# Utility functions

def arnoldAttributeEditor_LeafName(name):
   return name.split("|")[-1]

def arnoldAttributeEditor_ListPerspCameras():
   return filter(lambda x: cmds.camera(x, query=1, orthographic=1) == 0, cmds.ls(type="camera"))

def arnoldAttributeEditor_LabelPrefix(nodes, attr):

   def attributeStatus(nodes, attr):
      """
      check attribute status for a given set of node
      return value:
          1 : all nodes have the attribute and the same value
          0 : all nodes have the attribute but varying value
         -1 : one or more nodes are missing the attribute
      """
      rv = 1
      refval = None

      dattr = "." + attr

      for node in nodes:
         if not cmds.attributeQuery(attr, exists=1, node=node):
            return -1
         else:
            if refval is None:
               refval = cmds.getAttr(node+dattr)
            elif cmds.getAttr(node+dattr) != refval:
               rv = 0

      return rv

   code = attributeStatus(nodes, attr)

   if code == -1:
      return "- "
   elif code == 0:
      return "* "
   else:
      return ""

def arnoldAttributeEditor_FirstShapeWithAttr(nodes, attr):
   for node in nodes:
      if cmds.attributeQuery(attr, exists=1, node=node):
         return node
   return None


# UI Callbacks

def arnoldAttributeEditor_SelectShape(*args):
   sel = cmds.textScrollList("%s_shapeLst" % UI_NAME, query=1, selectItem=1)
   if sel is None:
      sel = []

   enable = (len(sel) >= 1)

   cmds.checkBoxGrp("%s_castsShadowsChk" % UI_NAME, edit=1, enable=enable)
   cmds.checkBoxGrp("%s_receiveShadowsChk" % UI_NAME, edit=1, enable=enable)
   cmds.checkBoxGrp("%s_motionBlurChk" % UI_NAME, edit=1, enable=enable)
   cmds.checkBoxGrp("%s_primaryVisibilityChk" % UI_NAME, edit=1, enable=enable)
   cmds.checkBoxGrp("%s_smoothShadingChk" % UI_NAME, edit=1, enable=enable)
   cmds.checkBoxGrp("%s_visibleInReflectionsChk" % UI_NAME, edit=1, enable=enable)
   cmds.checkBoxGrp("%s_visibleInRefractionsChk" % UI_NAME, edit=1, enable=enable)
   cmds.checkBoxGrp("%s_doubleSidedChk" % UI_NAME, edit=1, enable=enable)
   cmds.checkBoxGrp("%s_oppositeChk" % UI_NAME, edit=1, enable=enable)

   cmds.checkBoxGrp("%s_selfShadowsChk" % UI_NAME, edit=1,enable=enable)
   cmds.checkBoxGrp("%s_opaqueChk" % UI_NAME, edit=1,enable=enable)
   cmds.checkBoxGrp("%s_diffuseVisChk" % UI_NAME, edit=1,enable=enable)
   cmds.checkBoxGrp("%s_glossyVisChk" % UI_NAME, edit=1,enable=enable)

   cmds.checkBoxGrp("%s_subdivChk" % UI_NAME, edit=1,enable=enable)
   cmds.intSliderGrp("%s_subdivIterInt" % UI_NAME, edit=1,enable=enable)
   cmds.optionMenuGrp("%s_subdivMetricOpt" % UI_NAME, edit=1,enable=enable)
   cmds.floatSliderGrp("%s_subdivErrorFlt" % UI_NAME, edit=1,enable=enable)
   cmds.optionMenuGrp("%s_subdivDicingOpt" % UI_NAME, edit=1,enable=enable)

   cmds.checkBoxGrp("%s_sssGIChk" % UI_NAME, edit=1,enable=enable)
   cmds.intSliderGrp("%s_sssMaxSampInt" % UI_NAME, edit=1,enable=enable)
   cmds.floatSliderGrp("%s_sssSampSpcFlt" % UI_NAME, edit=1,enable=enable)

   cmds.intSliderGrp("%s_customObjectIDInt" % UI_NAME, edit=1,enable=enable)
   cmds.checkBoxGrp("%s_customMatteOutChk" % UI_NAME, edit=1,enable=enable)
   cmds.checkBoxGrp("%s_customExportTangentsChk" % UI_NAME, edit=1,enable=enable)
   cmds.checkBoxGrp("%s_customExportVertexColorChk" % UI_NAME, edit=1,enable=enable)
   cmds.checkBoxGrp("%s_customInvisibleChk" % UI_NAME, edit=1,enable=enable)

   if len(sel) == 0:
      return

   # Render stats

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "castsShadows") + "Casts Shadows"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "castsShadows")
   ival = cmds.getAttr(shape+".castsShadows") if shape != None else 1
   cmds.checkBoxGrp("%s_castsShadowsChk" % UI_NAME, edit=1,label=lbl, value1=ival)

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "receiveShadows") + "Receive Shadows"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "receiveShadows")
   ival = cmds.getAttr(shape+".receiveShadows") if shape != None else 1
   cmds.checkBoxGrp("%s_receiveShadowsChk" % UI_NAME, edit=1,label=lbl, value1=ival)

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "motionBlur") + "Motion Blur"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "motionBlur")
   ival = cmds.getAttr(shape+".motionBlur") if shape != None else 1
   cmds.checkBoxGrp("%s_motionBlurChk" % UI_NAME, edit=1,label=lbl, value1=ival)

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "primaryVisibility") + "Primary Visibility"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "primaryVisibility")
   ival = cmds.getAttr(shape+".primaryVisibility") if shape != None else 1
   cmds.checkBoxGrp("%s_primaryVisibilityChk" % UI_NAME, edit=1,label=lbl, value1=ival)

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "smoothShading") + "Smooth Shading"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "smoothShading")
   ival = cmds.getAttr(shape+".smoothShading") if shape != None else 1
   cmds.checkBoxGrp("%s_smoothShadingChk" % UI_NAME, edit=1,label=lbl, value1=ival)

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "visibleInReflections") + "Visible In Reflections"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "visibleInReflections")
   ival = cmds.getAttr(shape+".visibleInReflections") if shape != None else 1
   cmds.checkBoxGrp("%s_visibleInReflectionsChk" % UI_NAME, edit=1,label=lbl, value1=ival)

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "visibleInRefractions") + "Visible In Refractions"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "visibleInRefractions")
   ival = cmds.getAttr(shape+".visibleInRefractions") if shape != None else 1
   cmds.checkBoxGrp("%s_visibleInRefractionsChk" % UI_NAME, edit=1,label=lbl, value1=ival)

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "doubleSided") + "Double Sided"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "doubleSided")
   ival = cmds.getAttr(shape+".doubleSided") if shape != None else 1
   cmds.checkBoxGrp("%s_doubleSidedChk" % UI_NAME, edit=1,label=lbl, value1=ival)

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "opposite") + "Opposite"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "opposite")
   ival = cmds.getAttr(shape+".opposite") if shape != None else 1
   cmds.checkBoxGrp("%s_oppositeChk" % UI_NAME, edit=1,label=lbl, value1=ival)

   # Visibility attributes

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "self_shadows") + "Self Shadows"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "self_shadows")
   ival = cmds.getAttr(shape+".self_shadows") if shape != None else 1
   cmds.checkBoxGrp("%s_selfShadowsChk" % UI_NAME, edit=1,label=lbl, value1=ival)

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "opaque") + "Opaque"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "opaque")
   ival = cmds.getAttr(shape+".opaque") if shape != None else 1
   cmds.checkBoxGrp("%s_opaqueChk" % UI_NAME, edit=1,label=lbl, value1=ival)

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "diffuse_visibility") + "Visible in Diffuse"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "diffuse_visibility")
   ival = cmds.getAttr(shape+".diffuse_visibility") if shape != None else 1
   cmds.checkBoxGrp("%s_diffuseVisChk" % UI_NAME, edit=1,label=lbl, value1=ival)

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "glossy_visibility") + "Visible in Glossy"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "glossy_visibility")
   ival = cmds.getAttr(shape+".glossy_visibility") if shape != None else 1
   cmds.checkBoxGrp("%s_glossyVisChk" % UI_NAME, edit=1,label=lbl, value1=ival)

   # Subdiv attributes

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "subdiv_type") + "Subdivision Surface"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "subdiv_type")
   ival = cmds.getAttr(shape+".subdiv_type") if shape != None else 0
   cmds.checkBoxGrp("%s_subdivChk" % UI_NAME, edit=1,label=lbl, value1=ival)

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "subdiv_iterations") + "Iterations"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "subdiv_iterations")
   ival = cmds.getAttr(shape+".subdiv_iterations") if shape != None else 1
   cmds.intSliderGrp("%s_subdivIterInt" % UI_NAME, edit=1,label=lbl, value=ival)

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "subdiv_adaptive_metric") + "Iterations"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "subdiv_adaptive_metric")
   ival = cmds.getAttr(shape+".subdiv_adaptive_metric") if shape != None else 0
   cmds.optionMenuGrp("%s_subdivMetricOpt" % UI_NAME, edit=1,label=lbl, select=(ival+1))

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "subdiv_pixel_error") + "Pixel Error"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "subdiv_pixel_error")
   fval = cmds.getAttr(shape+".subdiv_pixel_error") if shape != None else 0
   cmds.floatSliderGrp("%s_subdivErrorFlt" % UI_NAME, edit=1,label=lbl, value=fval)

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "subdiv_dicing_camera") + "Dicing Camera"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "subdiv_dicing_camera")
   sval = cmds.getAttr(shape+".subdiv_dicing_camera") if shape != None else "Default"
   cmds.optionMenuGrp("%s_subdivDicingOpt" % UI_NAME, edit=1,label=lbl, value=sval)

   # SSS attributes

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "sss_use_gi") + "Use GI for SSS"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "sss_use_gi")
   ival = cmds.getAttr(shape+".sss_use_gi") if shape != None else 1
   cmds.checkBoxGrp("%s_sssGIChk" % UI_NAME, edit=1,label=lbl, value1=ival)

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "sss_max_samples") + "Max. Samples for SSS"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "sss_max_samples")
   ival = cmds.getAttr(shape+".sss_max_samples") if shape != None else 100000
   cmds.intSliderGrp("%s_sssMaxSampInt" % UI_NAME, edit=1,label=lbl, value=ival)

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "sss_sample_spacing") + "SSS Sample Spacing"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "sss_sample_spacing")
   fval = cmds.getAttr(shape+".sss_sample_spacing") if shape != None else 0.1
   cmds.floatSliderGrp("%s_sssSampSpcFlt" % UI_NAME, edit=1,label=lbl, value=fval)

   # Custom attributes

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "mtoa_objectid") + "Object ID"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "mtoa_objectid")
   ival = cmds.getAttr(shape+".mtoa_objectid") if shape != None else 0
   cmds.intSliderGrp("%s_customObjectIDInt" % UI_NAME, edit=1,label=lbl, value=ival)

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "mtoa_matteout") + "Matte Out"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "mtoa_matteout")
   ival = cmds.getAttr(shape+".mtoa_matteout") if shape != None else 0
   cmds.checkBoxGrp("%s_customMatteOutChk" % UI_NAME, edit=1,label=lbl, value1=ival)

   lbl = arnoldAttributeEditor_LabelPrefix(sel, "mtoa_export_tangents") + "Export Tangents"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "mtoa_export_tangents")
   ival = cmds.getAttr(shape+".mtoa_export_tangents") if shape != None else 0
   cmds.checkBoxGrp("%s_customExportTangentsChk" % UI_NAME, edit=1,label=lbl, value1=ival)
   
   lbl = arnoldAttributeEditor_LabelPrefix(sel, "mtoa_export_vertexColor") + "Export Vertex Color"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "mtoa_export_vertexColor")
   ival = cmds.getAttr(shape+".mtoa_export_vertexColor") if shape != None else 0
   cmds.checkBoxGrp("%s_customExportVertexColorChk" % UI_NAME, edit=1,label=lbl, value1=ival)
   
   lbl = arnoldAttributeEditor_LabelPrefix(sel, "mtoa_invisible") + "Invisible"
   shape = arnoldAttributeEditor_FirstShapeWithAttr(sel, "mtoa_invisible")
   ival = cmds.getAttr(shape+".mtoa_invisible") if shape != None else 0
   cmds.checkBoxGrp("%s_customInvisibleChk" % UI_NAME, edit=1,label=lbl, value1=ival)

def arnoldAttributeEditor_RefreshShapes(*args):
   shapeTypes = set(["mesh", "nurbsSurface"])

   # add custom shape types
   if "MTOA_CUSTOM_SHAPES" in os.environ:
      for customShape in os.environ["MTOA_CUSTOM_SHAPES"].split(";"):
         customShape = customShape.strip()
         try:
            lst = cmds.ls(type=customShape)
         except:
            lst = []
         if len(customShape) > 0 and len(lst) > 0:
            shapeTypes.add(customShape)

   # convert to array in order to pass a listRelatives argument
   shapeTypes = list(shapeTypes)

   # list shapes in selection
   shapes = []
   
   for item in cmds.ls(selection=1, long=1):
      if cmds.nodeType(item) in shapeTypes:
         # skip intermediate shapes
         if not cmds.getAttr(item+".intermediateObject"):
            shapes.append(item)
      else:
         # not a shape, find shapes in subtree
         lst = cmds.listRelatives(item, allDescendents=1, noIntermediate=1, fullPath=1, type=shapeTypes)
         if lst != None:
            shapes.extend(lst)

   shapes.sort()

   # fill list ui
   cmds.textScrollList("%s_shapeLst" % UI_NAME, edit=1, removeAll=1)
   for shape in shapes:
      cmds.textScrollList("%s_shapeLst" % UI_NAME, edit=1, append=shape)

   arnoldAttributeEditor_SelectShape()

def arnoldAttributeEditor_SelectAll(*args):
   n = cmds.textScrollList("%s_shapeLst" % UI_NAME, query=1, numberOfItems=1)
   for i in xrange(n):
      cmds.textScrollList("%s_shapeLst" % UI_NAME, edit=1, selectIndexedItem=(i+1))

   arnoldAttributeEditor_SelectShape()

def arnoldAttributeEditor_SelectInScene(*args):
   nodes = cmds.textScrollList("%s_shapeLst" % UI_NAME, query=1, selectItem=1)
   if nodes != None and len(nodes) > 0:
      cmds.select(nodes, r=1)

def arnoldAttributeEditor_UpdateIntOptionAttr(name, control, addCommand=None):
   val = cmds.optionMenuGrp(control, query=1, select=1) - 1

   lbl = cmds.optionMenuGrp(control, query=1, label=1)
   lbl = re.sub(r"^[-*]\s+", "", lbl)

   nodes = cmds.textScrollList("%s_shapeLst" % UI_NAME, query=1, selectItem=1)
   if nodes is None:
      nodes = []

   for node in nodes:
      if not cmds.attributeQuery(name, exists=1, node=node):
         if addCommand != None:
            addCommand(node)
         else:
            cmds.addAttr(node, ln=name, nn=lbl, at="long")
      cmds.setAttr(node+"."+name, val)

   if len(nodes) >= 1:
      cmds.optionMenuGrp(control, edit=1, label=lbl)

def arnoldAttributeEditor_UpdateStringOptionAttr(name, control, addCommand=None):
   val = cmds.optionMenuGrp(control, query=1, value=1)

   lbl = cmds.optionMenuGrp(control, query=1, label=1)
   lbl = re.sub(r"^[-*]\s+", "", lbl)

   nodes = cmds.textScrollList("%s_shapeLst" % UI_NAME, query=1, selectItem=1)
   if nodes is None:
      nodes = []

   for node in nodes:
      if not cmds.attributeQuery(name, exists=1, node=node):
         if addCommand != None:
            addCommand(node)
         else:
            cmds.addAttr(node, ln=name, nn=lbl, dt="string")
      cmds.setAttr(node+"."+name, val, type="string")

   if len(nodes) >= 1:
      cmds.optionMenuGrp(control, edit=1, label=lbl)

def arnoldAttributeEditor_UpdateFloatAttr(name, control, addCommand=None):
   val = cmds.floatSliderGrp(control, query=1, value=1)

   lbl = cmds.floatSliderGrp(control, query=1, label=1)
   lbl = re.sub(r"^[-*]\s+", "", lbl)

   nodes = cmds.textScrollList("%s_shapeLst" % UI_NAME, query=1, selectItem=1)
   if nodes is None:
      nodes = []
   
   for node in nodes:
      if not cmds.attributeQuery(name, exists=1, node=node):
         if addCommand != None:
            addCommand(node)
         else:
            cmds.addAttr(node, ln=name, nn=lbl, at="float")
      cmds.setAttr(node+"."+name, val)

   if len(nodes) >= 1:
      cmds.floatSliderGrp(control, edit=1, label=lbl)

def arnoldAttributeEditor_UpdateIntAttr(name, control, addCommand=None):
   val = cmds.intSliderGrp(control, query=1, value=1)

   lbl = cmds.intSliderGrp(control, query=1, label=1)
   lbl = re.sub(r"^[-*]\s+", "", lbl)

   nodes = cmds.textScrollList("%s_shapeLst" % UI_NAME, query=1, selectItem=1)
   if nodes is None:
      nodes = []

   for node in nodes:
      if not cmds.attributeQuery(name, exists=1, node=node):
         if addCommand != None:
            addCommand(node)
         else:
            cmds.addAttr(node, ln=name, nn=lbl, at="long")
      cmds.setAttr(node+"."+name, val)

   if len(nodes) >= 1:
      cmds.intSliderGrp(control, edit=1, label=lbl)

def arnoldAttributeEditor_UpdateBoolAttr(name, control, addCommand=None):
   val = cmds.checkBoxGrp(control, query=1, value1=1)

   lbl = cmds.checkBoxGrp(control, query=1, label=1)
   lbl = re.sub(r"^[-*]\s+", "", lbl)

   nodes = cmds.textScrollList("%s_shapeLst" % UI_NAME, query=1, selectItem=1)
   if nodes is None:
      nodes = []

   for node in nodes:
      if not cmds.attributeQuery(name, exists=1, node=node):
         if addCommand != None:
            addCommand(node)
         else:
            cmds.addAttr(node, ln=name, nn=lbl, at="bool")
      cmds.setAttr(node+"."+name, val)

   if len(nodes) >= 1:
      cmds.checkBoxGrp(control, edit=1, label=lbl)


# Arnold Attribute Editor main

def arnoldAttributeEditor(*args):
   if cmds.window(UI_NAME, exists=True):
      cmds.deleteUI(UI_NAME)

   cmds.window(UI_NAME, title="Arnold Attribute Editor", width=640, height=480)

   form = cmds.formLayout(numberOfDivisions=100)

   cmds.textScrollList("%s_shapeLst" % UI_NAME,
                       parent=form,
                       numberOfRows=10,
                       allowMultiSelection=True,
                       selectCommand=arnoldAttributeEditor_SelectShape)

   cmds.button("%s_refreshBtn" % UI_NAME,
               parent=form,
               label="Refesh",
               command=arnoldAttributeEditor_RefreshShapes)

   cmds.button("%s_selectAllBtn" % UI_NAME,
               parent=form,
               label="Select All",
               command=arnoldAttributeEditor_SelectAll)

   cmds.button("%s_selectObjBtn" % UI_NAME,
               parent=form,
               label="Select in Scene",
               command=arnoldAttributeEditor_SelectInScene)

   # Render attributes layout
   cmds.frameLayout("%s_rdrStat" % UI_NAME, parent=form, collapsable=True, label="Render Stats")
   layout = cmds.columnLayout(parent="%s_rdrStat" % UI_NAME, adjustableColumn=False, rowSpacing=2)
   cmds.checkBoxGrp("%s_castsShadowsChk" % UI_NAME,
                    parent=layout,
                    label="Casts Shadows",
                    columnAlign2=("left", "center"),
                    value1=True,
                    changeCommand=Callback(arnoldAttributeEditor_UpdateBoolAttr, "castsShadows", "%s_castsShadowsChk" % UI_NAME))
   cmds.checkBoxGrp("%s_receiveShadowsChk" % UI_NAME,
                    parent=layout,
                    label="Receive Shadows",
                    columnAlign2=("left", "center"),
                    value1=True,
                    changeCommand=Callback(arnoldAttributeEditor_UpdateBoolAttr, "receiveShadows", "%s_receiveShadowsChk" % UI_NAME))
   cmds.checkBoxGrp("%s_motionBlurChk" % UI_NAME,
                    parent=layout,
                    label="Motion Blur",
                    columnAlign2=("left", "center"),
                    value1=True,
                    changeCommand=Callback(arnoldAttributeEditor_UpdateBoolAttr, "motionBlur", "%s_motionBlurChk" % UI_NAME))
   cmds.checkBoxGrp("%s_primaryVisibilityChk" % UI_NAME,
                    parent=layout,
                    label="Primary Visibility",
                    columnAlign2=("left", "center"),
                    value1=True,
                    changeCommand=Callback(arnoldAttributeEditor_UpdateBoolAttr, "primaryVisibility", "%s_primaryVisibilityChk" % UI_NAME))
   cmds.checkBoxGrp("%s_smoothShadingChk" % UI_NAME,
                    parent=layout,
                    label="Smooth Shading",
                    columnAlign2=("left", "center"),
                    value1=True,
                    changeCommand=Callback(arnoldAttributeEditor_UpdateBoolAttr, "smoothShading", "%s_smoothShadingChk" % UI_NAME))
   cmds.checkBoxGrp("%s_visibleInReflectionsChk" % UI_NAME,
                    parent=layout,
                    label="Visible In Reflections",
                    columnAlign2=("left", "center"),
                    value1=True,
                    changeCommand=Callback(arnoldAttributeEditor_UpdateBoolAttr, "visibleInReflections", "%s_visibleInReflectionsChk" % UI_NAME))
   cmds.checkBoxGrp("%s_visibleInRefractionsChk" % UI_NAME,
                    parent=layout,
                    label="Visible In Refractions",
                    columnAlign2=("left", "center"),
                    value1=True,
                    changeCommand=Callback(arnoldAttributeEditor_UpdateBoolAttr, "visibleInRefractions", "%s_visibleInRefractionsChk" % UI_NAME))
   cmds.checkBoxGrp("%s_doubleSidedChk" % UI_NAME,
                    parent=layout,
                    label="Double Sided",
                    columnAlign2=("left", "center"),
                    value1=True,
                    changeCommand=Callback(arnoldAttributeEditor_UpdateBoolAttr, "doubleSided", "%s_doubleSidedChk" % UI_NAME))
   cmds.checkBoxGrp("%s_oppositeChk" % UI_NAME,
                    parent=layout,
                    label="Opposite",
                    columnAlign2=("left", "center"),
                    value1=True,
                    changeCommand=Callback(arnoldAttributeEditor_UpdateBoolAttr, "opposite", "%s_oppositeChk" % UI_NAME))

   # Visibility attributes layout
   cmds.frameLayout("%s_visFrm" % UI_NAME, parent=form, collapsable=True, label="Visibility")
   layout = cmds.columnLayout(parent="%s_visFrm" % UI_NAME, adjustableColumn=False, rowSpacing=2)
   cmds.checkBoxGrp("%s_selfShadowsChk" % UI_NAME,
                    parent=layout,
                    label="Self Shadows",
                    columnAlign2=("left", "center"),
                    value1=True,
                    changeCommand=Callback(arnoldAttributeEditor_UpdateBoolAttr, "self_shadows", "%s_selfShadowsChk" % UI_NAME))
   cmds.checkBoxGrp("%s_opaqueChk" % UI_NAME,
                    parent=layout,
                    label="Opaque",
                    columnAlign2=("left", "center"),
                    value1=True,
                    changeCommand=Callback(arnoldAttributeEditor_UpdateBoolAttr, "opaque", "%s_opaqueChk" % UI_NAME))
   cmds.checkBoxGrp("%s_diffuseVisChk" % UI_NAME,
                    parent=layout,
                    label="Visible in Diffuse",
                    columnAlign2=("left", "center"),
                    value1=True,
                    changeCommand=Callback(arnoldAttributeEditor_UpdateBoolAttr, "diffuse_visibility", "%s_diffuseVisChk" % UI_NAME))
   cmds.checkBoxGrp("%s_glossyVisChk" % UI_NAME,
                    parent=layout,
                    label="Visible in Glossy",
                    columnAlign2=("left", "center"),
                    value1=True,
                    changeCommand=Callback(arnoldAttributeEditor_UpdateBoolAttr, "glossy_visibility", "%s_glossyVisChk" % UI_NAME))

   # Subdivision attributes layout
   cmds.frameLayout("%s_subdFrm" % UI_NAME, parent=form, collapsable=True, label="Subdivision")
   layout = cmds.columnLayout(parent="%s_subdFrm" % UI_NAME, adjustableColumn=False, rowSpacing=2)
   cmds.checkBoxGrp("%s_subdivChk" % UI_NAME,
                    parent=layout,
                    label="Subdivision Surface",
                    columnAlign2=("left", "center"),
                    value1=False,
                    changeCommand=Callback(arnoldAttributeEditor_UpdateBoolAttr, "subdiv_type", "%s_subdivChk" % UI_NAME))
   cmds.intSliderGrp("%s_subdivIterInt" % UI_NAME,
                     parent=layout,
                     label="Iterations",
                     columnAlign=(1, "left"),
                     minValue=0,
                     maxValue=10,
                     field=True,
                     fieldMinValue=0,
                     fieldMaxValue=100,
                     value=1,
                     changeCommand=Callback(arnoldAttributeEditor_UpdateIntAttr, "subdiv_iterations", "%s_subdivIterInt" % UI_NAME))
   cmds.optionMenuGrp("%s_subdivMetricOpt" % UI_NAME,
                      parent=layout,
                      label="Adaptive Metric",
                      columnAlign2=("left", "center"),
                      changeCommand=Callback(arnoldAttributeEditor_UpdateIntOptionAttr, "subdiv_adaptive_metric", "%s_subdivMetricOpt" % UI_NAME))
   cmds.menuItem(label="Automatic")
   cmds.menuItem(label="Edge Length")
   cmds.menuItem(label="Flatness")
   cmds.optionMenuGrp("%s_subdivMetricOpt" % UI_NAME, edit=1, value="Automatic");
   cmds.floatSliderGrp("%s_subdivErrorFlt" % UI_NAME,
                       parent=layout,
                       columnAlign=(1, "left"),
                       field=True,
                       precision=3,
                       minValue=0,
                       maxValue=10,
                       fieldMinValue=0,
                       fieldMaxValue=50,
                       value=0,
                       label="Pixel Error",
                       changeCommand=Callback(arnoldAttributeEditor_UpdateFloatAttr, "subdiv_pixel_error", "%s_subdivErrorFlt" % UI_NAME))
   cmds.optionMenuGrp("%s_subdivDicingOpt" % UI_NAME,
                      parent=layout,
                      label="Dicing Camera",
                      columnAlign2=("left", "center"),
                      changeCommand=Callback(arnoldAttributeEditor_UpdateStringOptionAttr, "subdiv_dicing_camera", "%s_subdivDicingOpt" % UI_NAME))
   cmds.menuItem(label="Default")
   for cam in arnoldAttributeEditor_ListPerspCameras():
      cmds.menuItem(label=cam)
   cmds.optionMenuGrp("%s_subdivDicingOpt" % UI_NAME, edit=1, value="Default")

   # SSS attributes layout
   cmds.frameLayout("%s_sssFrm" % UI_NAME, parent=form, collapsable=True, label="SSS")
   layout = cmds.columnLayout(parent="%s_sssFrm" % UI_NAME, adjustableColumn=False, rowSpacing=2)
   cmds.checkBoxGrp("%s_sssGIChk" % UI_NAME,
                    parent=layout,
                    label="Use GI for SSS",
                    columnAlign2=("left", "center"),
                    value1=True,
                    changeCommand=Callback(arnoldAttributeEditor_UpdateBoolAttr, "sss_use_gi", "%s_sssGIChk" % UI_NAME))
   cmds.intSliderGrp("%s_sssMaxSampInt" % UI_NAME,
                     parent=layout,
                     field=True,
                     columnAlign=(1, "left"),
                     fieldMinValue=0,
                     fieldMaxValue=1000000000,
                     minValue=0,
                     maxValue=1000000,
                     value=100000,
                     label="Max. Samples for SSS",
                     changeCommand=Callback(arnoldAttributeEditor_UpdateIntAttr, "sss_max_samples", "%s_sssMaxSampInt" % UI_NAME))
   cmds.floatSliderGrp("%s_sssSampSpcFlt" % UI_NAME,
                       parent=layout,
                       field=True,
                       precision=3,
                       columnAlign=(1, "left"),
                       fieldMinValue=0,
                       fieldMaxValue=10,
                       minValue=0,
                       maxValue=1,
                       value=0.1,
                       label="SSS Sample Spacing",
                       changeCommand=Callback(arnoldAttributeEditor_UpdateFloatAttr, "sss_sample_spacing", "%s_sssSampSpcFlt" % UI_NAME))

   # Custom attributes layout
   cmds.frameLayout("%s_customFrm" % UI_NAME, parent=form, collapsable=True, label="Custom")
   layout = cmds.columnLayout(parent="%s_customFrm" % UI_NAME, adjustableColumn=True, rowSpacing=2)
   # from here should be dynamic
   cmds.intSliderGrp("%s_customObjectIDInt" % UI_NAME,
                     parent=layout,
                     columnAlign=(1, "left"),
                     field=True,
                     minValue=0,
                     maxValue=12,
                     value=0,
                     label="Object ID",
                     changeCommand=Callback(arnoldAttributeEditor_UpdateIntAttr, "mtoa_objectid", "%s_customObjectIDInt" % UI_NAME, addCommand=arnoldAttributeEditor_AddObjectIDAttr))
   cmds.checkBoxGrp("%s_customMatteOutChk" % UI_NAME,
                    parent=layout,
                    label="Matte Out",
                    columnAlign2=("left", "center"),
                    value1=False,
                    changeCommand=Callback(arnoldAttributeEditor_UpdateBoolAttr, "mtoa_matteout", "%s_customMatteOutChk" % UI_NAME, addCommand=arnoldAttributeEditor_AddMatteOutAttr))
   cmds.checkBoxGrp("%s_customExportTangentsChk" % UI_NAME, parent=layout,
                    label="Export Tangents",
                    columnAlign2=("left", "center"),
                    value1=False,
                    changeCommand=Callback(arnoldAttributeEditor_UpdateBoolAttr, "mtoa_export_tangents", "%s_customExportTangentsChk" % UI_NAME, addCommand=arnoldAttributeEditor_AddExportTangentsAttr))
   cmds.checkBoxGrp("%s_customExportVertexColorChk" % UI_NAME, parent=layout,
                    label="Export Vertex Color",
                    columnAlign2=("left", "center"),
                    value1=False,
                    changeCommand=Callback(arnoldAttributeEditor_UpdateBoolAttr, "mtoa_export_vertexColor", "%s_customExportVertexColorChk" % UI_NAME, addCommand=arnoldAttributeEditor_AddExportVertexColorAttr))
   cmds.checkBoxGrp("%s_customInvisibleChk" % UI_NAME,
                    parent=layout,
                    label="Invisible",
                    columnAlign2=("left", "center"),
                    value1=False,
                    changeCommand=Callback(arnoldAttributeEditor_UpdateBoolAttr, "mtoa_invisible", "%s_customInvisibleChk" % UI_NAME, addCommand=arnoldAttributeEditor_AddInvisibleAttr))
   
   # global layout
   
   cmds.formLayout(form,
                   edit=True,
                   attachForm=[("%s_shapeLst" % UI_NAME, "top", 5),
                               ("%s_shapeLst" % UI_NAME, "left", 5),
                               ("%s_refreshBtn" % UI_NAME, "left", 5),
                               ("%s_refreshBtn" % UI_NAME, "bottom", 5),
                               ("%s_selectAllBtn" % UI_NAME, "bottom", 5),
                               ("%s_selectObjBtn" % UI_NAME, "bottom", 5),
                               ("%s_rdrStat" % UI_NAME, "top", 5),
                               ("%s_rdrStat" % UI_NAME, "right", 5),
                               ("%s_visFrm" % UI_NAME, "right", 5),
                               ("%s_subdFrm" % UI_NAME, "right", 5),
                               ("%s_sssFrm" % UI_NAME, "right", 5),
                               ("%s_customFrm" % UI_NAME, "right", 5)],
                   attachControl=[("%s_shapeLst" % UI_NAME, "bottom", 5, "%s_refreshBtn" % UI_NAME),
                                  ("%s_visFrm" % UI_NAME, "top", 5, "%s_rdrStat" % UI_NAME),
                                  ("%s_subdFrm" % UI_NAME, "top", 5, "%s_visFrm" % UI_NAME),
                                  ("%s_sssFrm" % UI_NAME, "top", 5, "%s_subdFrm" % UI_NAME),
                                  ("%s_customFrm" % UI_NAME, "top", 5, "%s_sssFrm" % UI_NAME)],
                   attachPosition=[("%s_shapeLst" % UI_NAME, "right", 5, 50),
                                   ("%s_refreshBtn" % UI_NAME, "right", 1, 16),
                                   ("%s_selectAllBtn" % UI_NAME, "left", 1, 16),
                                   ("%s_selectAllBtn" % UI_NAME, "right", 1, 32),
                                   ("%s_selectObjBtn" % UI_NAME, "left", 1, 32),
                                   ("%s_selectObjBtn" % UI_NAME, "right", 5, 50),
                                   ("%s_rdrStat" % UI_NAME, "left", 5, 50),
                                   ("%s_visFrm" % UI_NAME, "left", 5, 50),
                                   ("%s_subdFrm" % UI_NAME, "left", 5, 50),
                                   ("%s_sssFrm" % UI_NAME, "left", 5, 50),
                                   ("%s_customFrm" % UI_NAME, "left", 5, 50)],
                   attachNone=[("%s_refreshBtn" % UI_NAME, "top"),
                               ("%s_selectAllBtn" % UI_NAME, "top"),
                               ("%s_selectObjBtn" % UI_NAME, "top"),
                               ("%s_rdrStat" % UI_NAME, "bottom"),
                               ("%s_visFrm" % UI_NAME, "bottom"),
                               ("%s_subdFrm" % UI_NAME, "bottom"),
                               ("%s_sssFrm" % UI_NAME, "bottom"),
                               ("%s_customFrm" % UI_NAME, "bottom")])

   cmds.showWindow(UI_NAME)

   arnoldAttributeEditor_RefreshShapes();
