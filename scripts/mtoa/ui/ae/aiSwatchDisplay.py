import maya.cmds as cmds
import maya.mel as mel

import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.utils import aeCallback

def aiSwatchLabel(nodeName) :
    type = cmds.nodeType(nodeName)
    classificationsList = cmds.getClassification(type)
    for classification in classificationsList :
        allClassList = classification.split(':')
        for allClass in allClassList :
            classList = allClass.split('/')
            if 'swatch' == classList[0] :
                continue
            else :
                if classList :
                    if 'shader' != classList[-1] :
                        classList = filter(lambda x:x!='shader', classList)
                    return "\n".join(map(lambda x:x.capitalize(), classList))
                else :
                    return "Sample"
                
def aiSwatchDisplayNew(plugName) :
    nodeAndAttrs = plugName.split(".")
    node = nodeAndAttrs[0]
        
    cmds.formLayout('swatchDisplayForm')
    cmds.text('swatchLabel', label=aiSwatchLabel(node))
    cmds.swatchDisplayPort('swatchDisplay', wh=(128, 128), rs=128)
    # button -e -c ("liquidShaderNodePreview "+$theNode) ("liquidAEswatchRefresh_"+$nodeType);
    # floatField -e -cc ("liquidShaderSetPreviewGamma "+$theNode) ("liquidAEswatchGamma_"+$nodeType);
    # floatField -e -v $previewGamma ("liquidAEswatchGamma_"+$nodeType);
    cmds.setParent(upLevel=True)
    gTextColumnWidthIndex = mel.eval("$tempVar=$gTextColumnWidthIndex;")
    cmds.formLayout('swatchDisplayForm',
                    edit=True,
                    af=[('swatchLabel',"top", 0),
                        ('swatchLabel', "bottom", 0),
                        ('swatchDisplay',"top", 0),
                        ('swatchDisplay', "bottom", 0),],
                    aof=[('swatchLabel', "right", -gTextColumnWidthIndex)],
                    an=[('swatchLabel', "left"),
                        ('swatchDisplay', "right")],
                    ac=[('swatchDisplay', "left", 5, 'swatchLabel')]
                    )

    # formLayout -e
     #   -af  $swatchLabel           top     0
     #   -af  $swatchLabel           bottom  0
     #   -af  $swatchLabel           left    0
     #   //-aof swatchLabel           right   (-$gTextColumnWidthIndex)
    
     #   -ac  $swatchDisplay         left    50   $swatchLabel
     #  -af  $swatchDisplay         top     0
     #   -an  $swatchDisplay         right
    
      #  -af  $liquidAEswatchRefresh top     -2
      #  -ac  $liquidAEswatchRefresh left    5   $swatchDisplay

      #  -ac  $liquidAEswatchGammaText top     5 $liquidAEswatchRefresh
       # -ac  $liquidAEswatchGammaText left    5   $swatchDisplay

       # -ac  $liquidAEswatchGamma top     5 $liquidAEswatchRefresh
       # -ac  $liquidAEswatchGamma left    5   $liquidAEswatchGammaText
    
       # -af  $liquidAEShaderReload  top     102
       # -ac  $liquidAEShaderReload  left    5   $swatchDisplay
    
       # -af  $liquidAEEditLif       top     76
       # -ac  $liquidAEEditLif       left    5   $swatchDisplay

    aiSwatchDisplayReplace(plugName)

def aiSwatchDisplayReplace(plugName) :
    nodeAndAttrs = plugName.split(".")
    node = nodeAndAttrs[0]
    
    cmds.swatchDisplayPort('swatchDisplay',
                      edit=True,
                      shadingNode=node,
                      wh=(128, 128), rs=128,
                      annotation='Refresh Swatch',
                      pressCommand='''import maya.mel as mel;mel.eval('updateFileNodeSwatch(\"%s\")')''' % node) 
    cmds.text('swatchLabel', edit=True, label=aiSwatchLabel(node))


def aiSwatchDisplay(nodeName) :
    cmds.editorTemplate(aeCallback(aiSwatchDisplayNew), aeCallback(aiSwatchDisplayReplace), "message", callCustom=True)

