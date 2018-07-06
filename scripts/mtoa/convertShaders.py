import maya.cmds as cmds
import math


replaceShaders = True
targetShaders = ['aiStandard', 'aiHair', 'alSurface', 'alHair', 'alLayerColor', 'alRemapColor', 'alRemapFloat', 'alFractal', 'alFlakes']
    
def convertUi():
    ret = cmds.confirmDialog( title='Convert shaders', message='Convert all shaders in scene, or selected shaders?', button=['All', 'Selected', 'Cancel'], defaultButton='All', cancelButton='Cancel' )
    if ret == 'All':
        convertAllShaders()
    elif ret == 'Selected':
        convertSelection()
               
def convertSelection():
    """
    Loops through the selection and attempts to create arnold shaders on whatever it finds
    """
    
    sel = cmds.ls(sl=True)
    if sel:
        for s in sel:
            ret = doMapping(s)

def convertAllShaders():
    """
    Converts each (in-use) material in the scene
    """
    # better to loop over the types instead of calling
    # ls -type targetShader
    # if a shader in the list is not registered (i.e. VrayMtl)
    # everything would fail
    loadedNodeTypes = cmds.ls(nodeTypes=True)

    for shdType in targetShaders:
        if shdType in loadedNodeTypes:
            shaderColl = cmds.ls(exactType=shdType)
            if shaderColl:
                for x in shaderColl:
                    # query the objects assigned to the shader
                    # only convert things with members
                    #shdGroup = cmds.listConnections(x, type="shadingEngine")
                    #setMem = cmds.sets( shdGroup, query=True )
                    #if setMem:
                    doMapping(x)


def doMapping(inShd):
    """
    Figures out which attribute mapping to use, and does the thing.
    
    @param inShd: Shader name
    @type inShd: String
    """
    ret = None
    
    shaderType = cmds.objectType(inShd)
    if 'aiStandard' in shaderType :
        ret = convertAiStandard(inShd)
    elif 'aiHair' in shaderType :
        ret = convertAiHair(inShd)
    elif 'alHair' in shaderType:
        ret = convertAlHair(inShd)
    elif 'alSurface' in shaderType:
        ret = convertAlSurface(inShd)
    elif 'alLayerColor' in shaderType:
        ret = convertAlLayerColor(inShd)
    elif 'alRemapColor' in shaderType:
        ret = convertAlRemapColor(inShd)
    elif 'alRemapFloat' in shaderType:
        ret = convertAlRemapFloat(inShd)
    elif 'alFractal' in shaderType:
        ret = convertAlFractal(inShd)
    elif 'alFlakes' in shaderType:
        ret = convertAlFlakes(inShd)
        
        
    
    if ret:
        # assign objects to the new shader
        assignToNewShader(inShd, ret)

def assignToNewShader(oldShd, newShd):


    if ':' in oldShd:
        aiName = oldShd.rsplit(':')[-1] + '_old'
    else:
        aiName = oldShd + '_old'
    
    cmds.rename(oldShd, aiName)
    cmds.rename(newShd, oldShd)

    newShd = oldShd
    oldShd = aiName

    """
    Creates a shading group for the new shader, and assigns members of the old shader to it
    
    @param oldShd: Old shader to upgrade
    @type oldShd: String
    @param newShd: New shader
    @type newShd: String
    """
    
    retVal = False
    
    shdGroups = cmds.listConnections(oldShd + '.outColor', plugs=True)
    
    #print 'shdGroup:', shdGroup
    if shdGroups != None:    
        for shdGroup in  shdGroups:
            cmds.connectAttr(newShd + '.outColor', shdGroup, force=True)
            retVal =True

    if replaceShaders:
        cmds.delete(oldShd)        
    return retVal


def setupConnections(inShd, fromAttr, outShd, toAttr):
    conns = cmds.listConnections( inShd + '.' + fromAttr, d=False, s=True, plugs=True )
    if conns:
        cmds.connectAttr(conns[0], outShd + '.' + toAttr, force=True)
        return True

    return False
                
            

def convertAiStandard(inShd):
    if ':' in inShd:
        aiName = inShd.rsplit(':')[-1] + '_new'
    else:
        aiName = inShd + '_new'
    
    
    #print 'creating '+ aiName
    outNode = cmds.shadingNode('aiStandardSurface', name=aiName, asShader=True)
    

    convertAttr(inShd, 'Kd', outNode, 'base')
    convertAttr(inShd, 'color', outNode, 'baseColor')
    convertAttr(inShd, 'diffuseRoughness', outNode, 'diffuseRoughness')

    convertAttr(inShd, 'Ks', outNode, 'specular')
    convertAttr(inShd, 'KsColor', outNode, 'specularColor')
    convertAttr(inShd, 'specularRoughness', outNode, 'specularRoughness')

    reflectionFresnel = cmds.getAttr(inShd + '.Fresnel')
    fresnel_use_ior = cmds.getAttr(inShd + '.FresnelUseIOR')
    specularFresnel = cmds.getAttr(inShd + '.specular_Fresnel')

    if int(specularFresnel) > 0:
        if int(fresnel_use_ior) > 0:
            convertAttr(inShd, 'IOR', outNode, 'specular_IOR')
        else:
            convertAttr(inShd, 'Ksn', outNode, 'specular_IOR', krnToIorRemap)
    else:
        # what to do when there is no fresnel ? new shaders will always apply a 
        # fresnel using IOR no matter what. So we want to set specular_IOR to a very high 
        # value, to simulate a no-fresnel effect.
        Kt = cmds.getAttr(inShd + '.Kt')
        KtColorR = cmds.getAttr(inShd + '.KtColorR') * Kt
        KtColorG = cmds.getAttr(inShd + '.KtColorG') * Kt
        KtColorB = cmds.getAttr(inShd + '.KtColorB') * Kt
        if KtColorR < 0.001 and KtColorG < 0.001 and KtColorB < 0.001:
            cmds.setAttr(outNode +  ".specular_IOR", 10)



    if int(reflectionFresnel) > 0:
        if int(fresnel_use_ior) > 0:
            convertAttr(inShd, 'IOR', outNode, 'coat_IOR')
        else:
            convertAttr(inShd, 'Krn', outNode, 'coat_IOR', krnToIorRemap)
    else:
        # apparently coat_IOR isn't used during refractions, so I can safely set whichever value I want
        cmds.setAttr(outNode + ".coat_IOR", 10)


    convertAttr(inShd, 'specularAnisotropy', outNode, 'specularAnisotropy', anisotropyRemap)

    convertAttr(inShd, 'specularRotation', outNode, 'specularRotation', rotationRemap)
    convertAttr(inShd, 'Kt', outNode, 'transmission')

    convertAttr(inShd, 'KtColor', outNode, 'transmissionColor') # not multiplying by transmittance

    # transmission_depth => (transmittance == AI_RGB_WHITE) ? 0.0 : 1.0
    convertAttr(inShd, 'dispersionAbbe', outNode, 'transmissionDispersion') # not multiplying by transmittance    

    # transmission_extra_roughness => refraction_roughness - specular_roughness    

    convertAttr(inShd, 'Ksss', outNode, 'subsurface')
    convertAttr(inShd, 'KsssColor', outNode, 'subsurfaceColor')
    convertAttr(inShd, 'sssRadius', outNode, 'subsurfaceRadius')

    convertAttr(inShd, 'Kr', outNode, 'coat')
    convertAttr(inShd, 'KrColor', outNode, 'coatColor')
    cmds.setAttr(outNode + '.coat_roughness', 0)


    convertAttr(inShd, 'emission', outNode, 'emission')
    convertAttr(inShd, 'emissionColor', outNode, 'emissionColor')
    convertAttr(inShd, 'opacity', outNode, 'opacity')

    # caustics => enable_glossy_caustics || enable_reflective_caustics || enable_refractive_caustics
    convertAttr(inShd, 'enable_internal_reflections', outNode, 'internal_reflections')
    convertAttr(inShd, 'indirect_diffuse', outNode, 'indirect_diffuse')
    convertAttr(inShd, 'indirect_specular', outNode, 'indirect_specular')
    # exit_to_background => reflection_exit_use_environment || refraction_exit_use_environment

    convertAttr(inShd, 'normalCamera', outNode, 'normalCamera') # not multiplying by transmittance
    print "Converted %s to aiStandardSurface" % inShd
    return outNode


def convertAiHair(inShd):
    if ':' in inShd:
        aiName = inShd.rsplit(':')[-1] + '_new'
    else:
        aiName = inShd + '_new'    
    
    outNode = cmds.shadingNode('aiStandardHair', name=aiName, asShader=True)
    convertAttr(inShd, 'tipcolor', outNode, 'base_color') #not converting root_color here
    convertAttr(inShd, 'KdInd', outNode, 'indirect_diffuse')
    #convertAttr(inShd, 'spec', outNode, 'specular')
    convertAttr(inShd, 'specColor', outNode, 'specular_tint')
    #convertAttr(inShd, 'spec2', outNode, 'specular2')
    convertAttr(inShd, 'spec2Color', outNode, 'specular2_tint')
    convertAttr(inShd, 'specGloss', outNode, 'roughness', glossRemap)
    convertAttr(inShd, 'specShift', outNode, 'shift', shiftRemap)
    convertAttr(inShd, 'transmission_color', outNode, 'transmission_tint')
    convertAttr(inShd, 'opacity', outNode, 'opacity')

    cmds.setAttr(outNode + '.melanin', 0)

    print "Converted %s to aiStandardHair" % inShd
    return outNode

def convertAlSurface(inShd):
    if ':' in inShd:
        aiName = inShd.rsplit(':')[-1] + '_new'
    else:
        aiName = inShd + '_new'
    
    
    #print 'creating '+ aiName
    outNode = cmds.shadingNode('aiStandardSurface', name=aiName, asShader=True)
    convertAttr(inShd, 'diffuseStrength', outNode, 'base')
    convertAttr(inShd, 'diffuseColor', outNode, 'baseColor')

    convertAttr(inShd, 'diffuseRoughness', outNode, 'diffuseRoughness')
    convertAttr(inShd, 'sssMix', outNode, 'subsurface')
    convertAttr(inShd, 'sssDensityScale', outNode, 'subsurfaceScale')
    convertAttr(inShd, 'sssRadiusColor2', outNode, 'subsurfaceRadius')
    convertAttr(inShd, 'specular1Strength', outNode, 'specular')
    convertAttr(inShd, 'specular1Color', outNode, 'specularColor')
    convertAttr(inShd, 'specular1Roughness', outNode, 'specularRoughness')
    convertAttr(inShd, 'specular1Anisotropy', outNode, 'specularAnisotropy')
    convertAttr(inShd, 'specular1Rotation', outNode, 'specularRotation')
    convertAttr(inShd, 'specular1Ior', outNode, 'specularIOR')
    convertAttr(inShd, 'specular2Strength', outNode, 'coat')
    convertAttr(inShd, 'specular2Color', outNode, 'coatColor')
    convertAttr(inShd, 'specular2Roughness', outNode, 'coatRoughness')
    convertAttr(inShd, 'specular2Ior', outNode, 'coatIOR')
    convertAttr(inShd, 'specular2Normal', outNode, 'coatNormal')
    convertAttr(inShd, 'transmissionStrength', outNode, 'transmission')
    convertAttr(inShd, 'transmissionColor', outNode, 'transmissionColor')
    convertAttr(inShd, 'transmissionRoughness', outNode, 'transmissionExtraRoughness')
    convertAttr(inShd, 'ssAttenuationColor', outNode, 'transmissionScatter')
    convertAttr(inShd, 'ssInScatteringStrength', outNode, 'transmissionDepth')
    convertAttr(inShd, 'ssDirection', outNode, 'transmissionScatterAnisotropy')
    convertAttr(inShd, 'emissionStrength', outNode, 'emission')
    convertAttr(inShd, 'emissionColor', outNode, 'emissionColor')
    convertAttr(inShd, 'opacity', outNode, 'opacity')
    convertAttr(inShd, 'normalCamera', outNode, 'normalCamera')

    print "Converted %s to aiStandardSurface" % inShd
    return outNode


def convertAlHair(inShd):
    if ':' in inShd:
        aiName = inShd.rsplit(':')[-1] + '_new'
    else:
        aiName = inShd + '_new'    
    
    outNode = cmds.shadingNode('aiStandardHair', name=aiName, asShader=True)

    convertAttr(inShd, 'dyeColor', outNode, 'baseColor')
    convertAttr(inShd, 'melanin', outNode, 'melanin')
    convertAttr(inShd, 'opacity', outNode, 'opacity')
    convertAttr(inShd, 'randomMelanin', outNode, 'melaninRandomize')
    convertAttr(inShd, 'diffuseStrength', outNode, 'diffuse')
    convertAttr(inShd, 'diffuseColor', outNode, 'diffuseColor')
    
    print "Converted %s to aiStandardHair" % inShd
    return outNode

def convertAlRemapColor(inShd):
    if ':' in inShd:
        aiName = inShd.rsplit(':')[-1] + '_new'
    else:
        aiName = inShd + '_new'    
    
    outNode = cmds.shadingNode('aiColorCorrect', name=aiName, asShader=True)

    convertAttr(inShd, 'input', outNode, 'input')
    convertAttr(inShd, 'gamma', outNode, 'gamma')
    convertAttr(inShd, 'saturation', outNode, 'saturation')
    convertAttr(inShd, 'hueShift', outNode, 'hueOffset')
    convertAttr(inShd, 'contrast', outNode, 'contrast')
    convertAttr(inShd, 'contrastPivot', outNode, 'contrastPivot')
    convertAttr(inShd, 'gain', outNode, 'multiplyR')
    #connect multiplyG and multiplyB to multiplyR since the input parameter is float but output is RGB
    cmds.connectAttr('{}.multiplyR {}.multiplyG'.format(outNode, outNode), force=True)
    cmds.connectAttr('{}.multiplyR {}.multiplyB'.format(outNode, outNode), force=True)

    convertAttr(inShd, 'exposure', outNode, 'exposure')
    convertAttr(inShd, 'mask', outNode, 'mask')    
    
    print "Converted %s to aiColorCorrect" % inShd
    return outNode

def convertAlRemapFloat(inShd):
    if ':' in inShd:
        aiName = inShd.rsplit(':')[-1] + '_new'
    else:
        aiName = inShd + '_new'    
    
    outNode = cmds.shadingNode('aiRange', name=aiName, asShader=True)

    convertAttr(inShd, 'input', outNode, 'inputR') 
    #connect inputG and inputB to inputR since the input parameter is float but output is RGB
    cmds.connectAttr('{}.inputR {}.inputG'.format(outNode, outNode), force=True)
    cmds.connectAttr('{}.inputR {}.inputB'.format(outNode, outNode), force=True)

    convertAttr(inShd, 'RMPinputMin', outNode, 'inputMin')
    convertAttr(inShd, 'RMPinputMax', outNode, 'inputMax')
    convertAttr(inShd, 'RMPoutputMin', outNode, 'outputMin')
    convertAttr(inShd, 'RMPoutputMax', outNode, 'outputMax')
    
    print "Converted %s to aiRange" % inShd
    return outNode

def convertAlFractal(inShd):
    if ':' in inShd:
        aiName = inShd.rsplit(':')[-1] + '_new'
    else:
        aiName = inShd + '_new'    
    
    outNode = cmds.shadingNode('aiNoise', name=aiName, asShader=True)

    convertAttr(inShd, 'mode', outNode, 'mode') 
    convertAttr(inShd, 'space', outNode, 'space') 
    freq = cmds.getAttr('{}.frequency'.format(inShd))
    cmds.setAttr('{}.scaleX'.format(outNode), cmds.getAttr('{}.scaleX'.format(outNode)) * freq)
    cmds.setAttr('{}.scaleY'.format(outNode), cmds.getAttr('{}.scaleY'.format(outNode)) * freq)
    cmds.setAttr('{}.scaleZ'.format(outNode), cmds.getAttr('{}.scaleZ'.format(outNode)) * freq)
    convertAttr(inShd, 'time', outNode, 'time') 
    convertAttr(inShd, 'octaves', outNode, 'octaves') 
    convertAttr(inShd, 'distortion', outNode, 'distortion')
    convertAttr(inShd, 'lacunarity', outNode, 'lacunarity')
    convertAttr(inShd, 'gain', outNode, 'amplitude')
    # need to divide amplitude by 2
    cmds.setAttr('{}.amplitude'.format(outNode), cmds.getAttr('{}.amplitude'.format(outNode)) * 0.5)
    convertAttr(inShd, 'color1', outNode, 'color1')
    convertAttr(inShd, 'color2', outNode, 'color2')
    convertAttr(inShd, 'P', outNode, 'P')

    print "Converted %s to aiNoise" % inShd
    return outNode

def convertAlFlakes(inShd):
    if ':' in inShd:
        aiName = inShd.rsplit(':')[-1] + '_new'
    else:
        aiName = inShd + '_new'    
    
    outNode = cmds.shadingNode('aiFlakes', name=aiName, asShader=True)

    convertAttr(inShd, 'space', outNode, 'outputSpace') 
    convertAttr(inShd, 'size', outNode, 'scale') 
    scale = cmds.getAttr('{}.scale'.format(outNode))
    if scale > 0:
        cmds.setAttr('{}.scale'.format(outNode), 1.0 / scale)

    convertAttr(inShd, 'amount', outNode, 'density') 

    print "Converted %s to aiFlakes" % inShd
    return outNode



def convertAlLayerColor(inShd):
    mappingBlendMode = {
            'Normal': 'overwrite',
            'Ligthen': 'max',  #or diffuseColor ?
            'Darken': 'min',            
            'Multiply': 'multiply',
            'Average': 'average',                   
            'Add': 'plus',
            'Subtract': 'subtract',
            'Difference': 'difference',
            'Negation': 'negation',
            'Exclusion': 'exclusion',
            'Screen': 'screen',
            'Overlay': 'overlay',
            'Soft Light': 'soft_light',
            'Hard Light': 'hard_light',
            'Color Dodge': 'color_dodge',
            'Color Burn': 'color_burn',
            'Linear Dodge': 'plus',
            'Linear Burn': 'subtract',
            'Linear Light': 'linear_light',
            'Vivid Light': 'vivid_light',
            'Pin Light': 'pin_light',
            'Hard Mix': 'hard_mix',
            'Reflect': 'reflect',
            'Glow': 'glow',
            'Phoenix': 'phoenix'
        }
    if ':' in inShd:
        aiName = inShd.rsplit(':')[-1] + '_new'
    else:
        aiName = inShd + '_new'    
    
    outNode = cmds.shadingNode('aiLayerRgba', name=aiName, asShader=True)

    convertAttr(inShd, 'layer1', outNode, 'input1')
    convertAttr(inShd, 'layer2', outNode, 'input2')
    convertAttr(inShd, 'layer3', outNode, 'input3')
    convertAttr(inShd, 'layer4', outNode, 'input4')
    convertAttr(inShd, 'layer5', outNode, 'input5')
    convertAttr(inShd, 'layer6', outNode, 'input6')
    convertAttr(inShd, 'layer7', outNode, 'input7')
    convertAttr(inShd, 'layer8', outNode, 'input8')
    
    convertAttr(inShd, 'layer1a', outNode, 'mix1')
    convertAttr(inShd, 'layer2a', outNode, 'mix2')
    convertAttr(inShd, 'layer3a', outNode, 'mix3')
    convertAttr(inShd, 'layer4a', outNode, 'mix4')
    convertAttr(inShd, 'layer5a', outNode, 'mix5')
    convertAttr(inShd, 'layer6a', outNode, 'mix6')
    convertAttr(inShd, 'layer7a', outNode, 'mix7')
    convertAttr(inShd, 'layer8a', outNode, 'mix8')

    for ind in range(1, 9):
        inBlendAttrName = '{}.layer{}blend'.format(inShd, ind)
        inBlendMode = cmds.getAttr(inBlendAttrName, asString=True)
        outOperation = mappingBlendMode[inBlendMode]
        if outOperation and len(outOperation) > 0:
            outOperationAttrName = '{}.operation{}'.format(outNode, ind)
            cmds.setAttr(outOperationAttrNAme, outOperation, type="string")    
        enableAttrName = '{}.enable{}'.format(outNode, ind)
        cmds.setAttr(enableAttrName, 1)
    
    print "Converted %s to aiLayerRgba" % inShd
    return outNode

def anisotropyRemap(val):
    return 2 * abs(val -0.5)

def rotationRemap(val):
    return 0.5 * val

def krnToIorRemap(val):
    
    if val > 0.99999:
        ior = 0.9999
    elif val < 0.0:
        ior = 0.0
    else:
        ior = val

    val_A = math.sqrt(ior)
    return (val_A + 1.0) / (1.0 - val_A)

def glossRemap(val):
    val =math.pow(float(val), float(-0.355))
    val *=  0.9928 
    
    if (val > 1):
        return 1

    return val

def shiftRemap(val):
    return 0.5 - (val/180.0)

def convertAttr(inNode, inAttr, outNode, outAttr, functionPtr = None):

    if cmds.objExists(inNode + '.' + inAttr):
        #print '\t', inAttr, ' -> ', outAttr
        
        if not setupConnections(inNode, inAttr, outNode, outAttr):
            # copy the values
            val = cmds.getAttr(inNode + '.' + inAttr)
            if functionPtr:
                val = functionPtr(val)
                
            setValue(outNode + '.' + outAttr, val)

            attrType = cmds.getAttr(inNode + '.' + inAttr, type=True)
            if attrType in ['float3']:
                subAttr = inAttr + '.' + inAttr + 'R'
                if cmds.objExists(inNode + '.' + subAttr):
                    setupConnections(inNode, subAttr, outNode, outAttr + '.' + outAttr + 'R')

                subAttr = inAttr + '.' + inAttr + 'G'
                if cmds.objExists(inNode + '.' + subAttr):
                    setupConnections(inNode, subAttr, outNode, outAttr + '.' + outAttr + 'G')

                subAttr = inAttr + '.' + inAttr + 'B'
                if cmds.objExists(inNode + '.' + subAttr):
                    setupConnections(inNode, subAttr, outNode, outAttr + '.' + outAttr + 'B')

                subAttr = inAttr + 'X'
                if cmds.objExists(inNode + '.' + subAttr):
                    setupConnections(inNode, subAttr, outNode, outAttr + 'X')

                subAttr = inAttr + 'Y'
                if cmds.objExists(inNode + '.' + subAttr):
                    setupConnections(inNode, subAttr, outNode, outAttr + 'Y')

                subAttr = inAttr + 'Z'
                if cmds.objExists(inNode + '.' + subAttr):
                    setupConnections(inNode, subAttr, outNode, outAttr + 'Z')



def setValue(attr, value):
    """Simplified set attribute function.

    @param attr: Attribute to set. Type will be queried dynamically
    @param value: Value to set to. Should be compatible with the attr type.
    """

    aType = None
    
    if cmds.objExists(attr):
        attrType = cmds.getAttr(attr, type=True)
        # temporarily unlock the attribute
        isLocked = cmds.getAttr(attr, lock=True)
        if isLocked:
            cmds.setAttr(attr, lock=False)

        # one last check to see if we can write to it
        if cmds.getAttr(attr, settable=True):
            attrType = cmds.getAttr(attr, type=True)
            
            #print value, type(value)
            
            if attrType in ['string']:
                aType = 'string'
                cmds.setAttr(attr, value, type=aType)
                
            elif attrType in ['long', 'short', 'float', 'byte', 'double', 'doubleAngle', 'doubleLinear', 'bool']:
                aType = None
                cmds.setAttr(attr, value)
                
            elif attrType in ['long2', 'short2', 'float2',  'double2', 'long3', 'short3', 'float3',  'double3']:
                if isinstance(value, float):
                    if attrType in ['long2', 'short2', 'float2',  'double2']:
                        value = [(value,value)]
                    elif attrType in ['long3', 'short3', 'float3',  'double3']:
                        value = [(value, value, value)]
                        
                cmds.setAttr(attr, *value[0], type=attrType)

        if isLocked:
            # restore the lock on the attr
            cmds.setAttr(attr, lock=True)


def convertArnoldShaders():
    if not cmds.pluginInfo( 'mtoa', query=True, loaded=True ):
        cmds.loadPlugin('mtoa')

    convertUi()






