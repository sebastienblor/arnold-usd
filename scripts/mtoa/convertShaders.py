from __future__ import print_function
import maya.cmds as cmds
import math


replaceShaders = True
targetShaders = ['aiStandard', 'aiHair', 'alSurface', 'alHair', 'alLayerColor', 'alRemapColor', 'alRemapFloat', 'alFractal', 'alFlake', 'alLayer', 'lambert', 'blinn', 'phong', 'VRayMtl', 'mia_material_x_passes', 'mia_material_x', 'dielectric_material']
    
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
    if inShd == 'lambert1':
        return None

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
    elif 'alFlake' in shaderType:
        ret = convertAlFlake(inShd)
    elif 'alLayer' in shaderType:
        ret = convertAlLayer(inShd)
    elif 'lambert' in shaderType:
        ret = convertLambert(inShd)
    elif 'blinn' in shaderType:
        ret = convertBlinn(inShd)
    elif 'phong' in shaderType:
        ret = convertPhong(inShd)
    elif 'VRayMtl' in shaderType:
        ret = convertVRayMtl(inShd)        
    elif 'mia_material_x_passes' in shaderType:
        ret = convertMiaMaterialX(inShd)
    elif 'mia_material_x' in shaderType:
        ret = convertMiaMaterialX(inShd)
    elif 'dielectric_material' in shaderType:
        ret = convertDielectricMaterial(inShd)
    
    if ret:
        # assign objects to the new shader
        assignToNewShader(inShd, ret)

def assignToNewShader(oldShd, newShd):

    output_conns = cmds.listConnections( oldShd, d=True, s=False, c=True, plugs=True )
    output_types = []
    for output_conn in output_conns:
        output_types.append(cmds.getAttr(output_conn, type=True))

    if ':' in oldShd:
        aiName = oldShd.rsplit(':')[-1] + '_old'
    else:
        aiName = oldShd + '_old'
    
    replaceCurrentShader = replaceShaders

    try:
        cmds.rename(oldShd, aiName)
        cmds.rename(newShd, oldShd)

        newShd = oldShd
        oldShd = aiName
    except RuntimeError as err:
        return

    if output_conns:
        lenConn = len(output_conns)
        for i in range(0, lenConn, 2):
            connSplit = output_conns[i].split('.')
            if len(connSplit) > 1:

                if not cmds.attributeQuery(connSplit[1], node=connSplit[0], exists=True):
                    
                    if connSplit[1] == 'outValue':
                        if output_types[i] == 'float':
                            output_conns[i] = connSplit[0] + '.outColorR' 
                        else:
                            output_conns[i] = connSplit[0] + '.outColor' 
                    elif connSplit[1] == 'outValueX':
                        output_conns[i] = connSplit[0] + '.outColorR'
                    elif connSplit[1] == 'outValueY':
                        output_conns[i] = connSplit[0] + '.outColorG'
                    elif connSplit[1] == 'outValueZ':
                        output_conns[i] = connSplit[0] + '.outColorB'

            cmds.connectAttr(output_conns[i], output_conns[i+1], force=True)

    if replaceCurrentShader:
        cmds.delete(oldShd)        
    

def setupConnections(inShd, fromAttr, outShd, toAttr):
    conns = cmds.listConnections( inShd + '.' + fromAttr, d=False, s=True, plugs=True )
    if conns:
        cmds.connectAttr(conns[0], outShd + '.' + toAttr, force=True)
        return True

    return False

def createArnoldShader(inShd, nodeType):     
    if ':' in inShd:
        aiName = inShd.rsplit(':')[-1] + '_new'
    else:
        aiName = inShd + '_new'    
    
    outNode = cmds.shadingNode(nodeType, name=aiName, asShader=True)
    return outNode

            

def convertAiStandard(inShd):

    outNode = createArnoldShader(inShd, 'aiStandardSurface')

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
    print("Converted %s to aiStandardSurface" % inShd)
    return outNode


def convertAiHair(inShd):
    outNode = createArnoldShader(inShd, 'aiStandardHair')

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

    print("Converted %s to aiStandardHair" % inShd)
    return outNode


def transparencyToOpacity(inShd, outShd):
    transpMap = cmds.listConnections( inShd + '.transparency', d=False, s=True, plugs=True )
    if transpMap:
        # map is connected, argh...
        # need to add a reverse node in the shading tree

        # create reverse
        invertNode = cmds.shadingNode('reverse', name=outShd + '_rev', asUtility=True)

        #connect transparency Map to reverse 'input'
        cmds.connectAttr(transpMap[0], invertNode + '.input', force=True)

        #connect reverse to opacity
        cmds.connectAttr(invertNode + '.output', outShd + '.opacity', force=True)
    else:
        #print inShd

        transparency = cmds.getAttr(inShd + '.transparency')
        opacity = [(1.0 - transparency[0][0], 1.0 - transparency[0][1], 1.0 - transparency[0][2])]

        #print opacity
        setValue(outShd + '.opacity', opacity)



def convertLambert(inShd):
    outNode = createArnoldShader(inShd, 'aiStandardSurface')

    convertAttr(inShd, 'diffuse', outNode, 'base')
    convertAttr(inShd, 'color', outNode, 'baseColor')
    setValue(outNode + '.emission', 1)
    setValue(outNode + '.specular', 0)
    convertAttr(inShd, 'normalCamera', outNode, 'normalCamera')
    convertAttr(inShd, 'incandescence', outNode, 'emissionColor')
    transparencyToOpacity(inShd, outNode)
    
    # not converting translucence since we don't have a direct equivalent
    print("Converted %s to aiStandardSurface" % inShd)
    return outNode

def convertBlinn(inShd):
    outNode = createArnoldShader(inShd, 'aiStandardSurface')

    convertAttr(inShd, 'diffuse', outNode, 'base')
    convertAttr(inShd, 'color', outNode, 'baseColor')
    convertAttr(inShd, 'specularRollOff', outNode, 'specular')
    convertAttr(inShd, 'specularColor', outNode, 'specularColor')
    # not converting translucence as there no equivalent

    convertAttr(inShd, 'reflectivity', outNode, 'coat')
    convertAttr(inShd, 'reflectedColor', outNode, 'coatColor')
    cmds.setAttr(outNode + '.coat_roughness', 0)

    convertAttr(inShd, 'eccentricity', outNode, 'specularRoughness')
    convertAttr(inShd, 'normalCamera', outNode, 'normalCamera')
    setValue(outNode + '.emission', 1)
    
    convertAttr(inShd, 'incandescence', outNode, 'emissionColor')
    
    transparencyToOpacity(inShd, outNode)
    
    # not converting translucence since we don't have a direct equivalent
    print("Converted %s to aiStandardSurface" % inShd)
    return outNode

def convertPhong(inShd):
    outNode = createArnoldShader(inShd, 'aiStandardSurface')

    convertAttr(inShd, 'diffuse', outNode, 'base')
    convertAttr(inShd, 'color', outNode, 'baseColor')
    cosinePower = cmds.getAttr(inShd + '.cosinePower')
    roughness = math.sqrt(1.0 / (0.454 * cosinePower + 3.357))
    setValue(outNode + '.specularRoughness', roughness)
    setValue(outNode + '.specular', 1.0)

    convertAttr(inShd, 'specularColor', outNode, 'specularColor')
    # not converting translucence as there no equivalent

    convertAttr(inShd, 'reflectivity', outNode, 'coat')
    convertAttr(inShd, 'reflectedColor', outNode, 'coatColor')
    cmds.setAttr(outNode + '.coat_roughness', 0)

    convertAttr(inShd, 'eccentricity', outNode, 'specularRoughness')
    convertAttr(inShd, 'normalCamera', outNode, 'normalCamera')
    setValue(outNode + '.emission', 1)
    convertAttr(inShd, 'incandescence', outNode, 'emissionColor')
    
    transparencyToOpacity(inShd, outNode)
    
    # not converting translucence since we don't have a direct equivalent
    print("Converted %s to aiStandardSurface" % inShd)
    return outNode

def convertAlSurface(inShd):
    outNode = createArnoldShader(inShd, 'aiStandardSurface')

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

    print("Converted %s to aiStandardSurface" % inShd)
    return outNode


def convertAlHair(inShd):
    outNode = createArnoldShader(inShd, 'aiStandardHair')

    convertAttr(inShd, 'dyeColor', outNode, 'baseColor')
    convertAttr(inShd, 'melanin', outNode, 'melanin')
    convertAttr(inShd, 'opacity', outNode, 'opacity')
    convertAttr(inShd, 'randomMelanin', outNode, 'melaninRandomize')
    convertAttr(inShd, 'diffuseStrength', outNode, 'diffuse')
    convertAttr(inShd, 'diffuseColor', outNode, 'diffuseColor')
    
    print("Converted %s to aiStandardHair" % inShd)
    return outNode

def convertAlRemapColor(inShd):
    outNode = createArnoldShader(inShd, 'aiColorCorrect')

    convertAttr(inShd, 'input', outNode, 'input')
    convertAttr(inShd, 'gamma', outNode, 'gamma')
    convertAttr(inShd, 'saturation', outNode, 'saturation')
    convertAttr(inShd, 'hueOffset', outNode, 'hueShift')
    convertAttr(inShd, 'contrast', outNode, 'contrast')
    convertAttr(inShd, 'contrastPivot', outNode, 'contrastPivot')
    convertAttr(inShd, 'gain', outNode, 'multiplyR')
    #connect multiplyG and multiplyB to multiplyR since the input parameter is float but output is RGB
    cmds.connectAttr('{}.multiplyR'.format(outNode), '{}.multiplyG'.format(outNode), force=True)
    cmds.connectAttr('{}.multiplyR'.format(outNode), '{}.multiplyB'.format(outNode), force=True)

    convertAttr(inShd, 'exposure', outNode, 'exposure')
    convertAttr(inShd, 'mask', outNode, 'mask')    
    
    print("Converted %s to aiColorCorrect" % inShd)
    return outNode

def convertAlRemapFloat(inShd):
    outNode = createArnoldShader(inShd, 'aiRange')

    convertAttr(inShd, 'input', outNode, 'inputR') 
    #connect inputG and inputB to inputR since the input parameter is float but output is RGB
    cmds.connectAttr('{}.inputR'.format(outNode), '{}.inputG'.format(outNode), force=True)
    cmds.connectAttr('{}.inputR'.format(outNode), '{}.inputB'.format(outNode), force=True)

    convertAttr(inShd, 'RMPinputMin', outNode, 'inputMin')
    convertAttr(inShd, 'RMPinputMax', outNode, 'inputMax')
    convertAttr(inShd, 'RMPoutputMin', outNode, 'outputMin')
    convertAttr(inShd, 'RMPoutputMax', outNode, 'outputMax')

    convertAttr(inShd, 'RMPcontrast', outNode, 'contrast')
    convertAttr(inShd, 'RMPcontrastPivot', outNode, 'contrastPivot')
    convertAttr(inShd, 'RMPbias', outNode, 'bias')
    convertAttr(inShd, 'RMPgain', outNode, 'gain')

    if cmds.getAttr('{}.RMPclampEnable'.format(inShd)):
        outClamp = cmds.shadingNode('aiClamp', name='{}_clamp'.format(inShd), asShader=True)
        convertAttr(inShd, 'RMPclampMin', outClamp, 'min')
        convertAttr(inShd, 'RMPclampMax', outClamp, 'max')
        cmds.connectAttr('{}.outColor'.format(outNode), '{}.input'.format(outClamp), force=True)
        outNode = outClamp
        if cmds.getAttr('{}.RMPthreshold'.format(inShd)):
            outThreshold = cmds.shadingNode('aiRange', name='{}_threshold'.format(inShd), asShader=True)
            convertAttr(inShd, 'RMPclampMin', outThreshold, 'inputMin')
            convertAttr(inShd, 'RMPclampMax', outThreshold, 'inputMax')
            cmds.connectAttr('{}.outColor'.format(outNode), '{}.input'.format(outThreshold), force=True)
            outNode = outThreshold

    print("Converted %s to aiRange" % inShd)
    return outNode

def convertAlFractal(inShd):
    outNode = createArnoldShader(inShd, 'aiNoise')

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

    print("Converted %s to aiNoise" % inShd)
    return outNode

def convertAlFlake(inShd):
    outNode = createArnoldShader(inShd, 'aiFlakes')

    convertAttr(inShd, 'space', outNode, 'outputSpace') 
    convertAttr(inShd, 'size', outNode, 'scale') 
    convertAttr(inShd, 'amount', outNode, 'density') 

    print("Converted %s to aiFlakes" % inShd)
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
            'Overlay': 'hard_light',
            'Soft Light': 'soft_light',
            'Hard Light': 'overlay',
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

    closureInputs = False
    
    # check if one of the inputs is a closure
    for ind in range(1, 9):
        if closureInputs:
            break            
        conns = cmds.listConnections('{}.layer{}'.format(inShd, ind), d=False, s=True, plugs=False ) or []
        
        closureMaterials = ['aiStandard', 'aiHair', 'aiStandardSurface', 'aiStandardHair', 'aiLayerShader', 'alSurface', 'alHair', 'lambert', 'blinn', 'phong', 'VRayMtl', 'mia_material_x_passes', 'mia_material_x', 'dielectric_material' ]
        for conn in conns:
            # get type of this node
            shaderType = cmds.objectType(conn)
            if shaderType in closureMaterials:
                closureInputs = True
                break

    if closureInputs:
        outNode = cmds.shadingNode('aiLayerShader', name=aiName, asShader=True)
    else:
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

    if closureInputs:
        print("Converted %s to aiLayerShader" % inShd)
        return outNode

    convertAttr(inShd, 'clampResult', outNode, 'clamp')
    
    enum_list = cmds.attributeQuery('operation1', node=outNode,
                                    listEnum=True)[0].split(':')

    for ind in range(1, 9):
        inBlendAttrName = '{}.layer{}blend'.format(inShd, ind)
        inBlendMode = cmds.getAttr(inBlendAttrName, asString=True)
        outOperation = mappingBlendMode[inBlendMode]
        if outOperation and len(outOperation) > 0:
            cmds.setAttr('{}.operation{}'.format(outNode, ind), enum_list.index(outOperation))    
        enableAttrName = '{}.enable{}'.format(outNode, ind)
        cmds.setAttr(enableAttrName, 1)
    
    print("Converted %s to aiLayerRgba" % inShd)
    return outNode

def convertAlLayer(inShd):
    
    if ':' in inShd:
        aiName = inShd.rsplit(':')[-1] + '_new'
    else:
        aiName = inShd + '_new'    

    outNode = cmds.shadingNode('aiMixShader', name=aiName, asShader=True)

    convertAttr(inShd, 'layer1', outNode, 'shader1')
    convertAttr(inShd, 'layer2', outNode, 'shader2')
    convertAttr(inShd, 'mix', outNode, 'mix')

    print("Converted %s to aiMixShader" % inShd)
    return outNode

def convertVRayMtl(inShd):
    outNode = createArnoldShader(inShd, 'aiStandardSurface')

    convertAttr(inShd, 'diffuseColorAmount', outNode, 'base')
    convertAttr(inShd, 'color', outNode, 'baseColor')
    convertAttr(inShd, 'roughnessAmount', outNode, 'diffuseRoughness')
    convertAttr(inShd, 'reflectionColorAmount', outNode, 'specular')
    convertAttr(inShd, 'reflectionColor', outNode, 'specularColor')
    convertAttr(inShd, 'normalCamera', outNode, 'normalCamera')

    convertAttr(inShd, 'Kt', outNode, 'transmission')
    convertAttr(inShd, 'KtColor', outNode, 'transmissionColor') # not multiplying by transmittance
    convertAttr(inShd, 'normalCamera', outNode, 'normalCamera')
    convertAttr(inShd, 'opacityMap', outNode, 'opacity')
    convertAttr(inShd, 'refractionIOR', outNode, 'specular_IOR')
    
    convertAttr(inShd, 'refractionColorAmount', outNode, 'transmission')
    convertAttr(inShd, 'refractionColor', outNode, 'transmissionColor')
    
    # not converting translucence since we don't have a direct equivalent
    print("Converted %s to aiStandardSurface" % inShd)
    return outNode

def convertMiaMaterialX(inShd):
    outNode = createArnoldShader(inShd, 'aiStandardSurface')
    convertAttr(inShd, 'diffuse_weight', outNode, 'base')
    convertAttr(inShd, 'diffuse', outNode, 'baseColor')
    convertAttr(inShd, 'diffuse_roughness', outNode, 'diffuseRoughness')

    convertAttr(inShd, 'reflectivity', outNode, 'specular')
    convertAttr(inShd, 'refl_color', outNode, 'specularColor')
    
    convertAttr(inShd, 'refr_ior', outNode, 'specular_IOR')
    convertAttr(inShd, 'refr_color', outNode, 'transmissionColor')
    convertAttr(inShd, 'transparency', outNode, 'transmission')
    convertAttr(inShd, 'cutout_opacity', outNode, 'transmission')
    convertAttr(inShd, 'anisotropy_rotation', outNode, 'specularRotation')
###########

    val1 = cmds.getAttr(inShd + '.refl_gloss')
    setValue(outNode + '.specularRoughness', 1.0 - val1)
    if cmds.getAttr(inShd + '.refl_hl_only'):
        setValue(outNode + '.indirectSpecular', 0)

    if cmds.getAttr(inShd + '.refl_is_metal'):
        # need to multiply reflection color by diffuse Color
        if not cmds.listConnections( inShd + '.refl_is_metal', d=False, s=True, plugs=True ):
            #in case reflection Color has been used to attenuate reflections 
            # multiply reflectivity by one of its channels
            reflColor = cmds.getAttr(inShd + '.refl_color')
            reflIntensity = cmds.getAttr(inShd + '.reflectivity')
            reflIntensity *= reflColor[0]
            cmds.setAttr(outNode+'.specular', reflIntensity)

        # assign specularColor to diffuse value
        if not setupConnections(inShd, 'diffuse', outNode, 'specularColor'):
            val = cmds.getAttr(inShd + '.diffuse')
            setValue(outNode + '.specularColor', val)
            
        
    val2 = cmds.getAttr(inShd + '.refr_gloss')

    setValue(outNode + '.transmissionExtraRoughness',val1 - val2)

    connOverallBump = cmds.listConnections( inShd + '.overall_bump', d=False, s=True, plugs=True )
    if connOverallBump:
        cmds.connectAttr(connOverallBump[0], outNode + '.normalCamera', force=True)
    else:
        connStandardBump = cmds.listConnections( inShd + '.standard_bump', d=False, s=True, plugs=True )
        if connStandardBump:
            cmds.connectAttr(connStandardBump[0], outNode + '.normalCamera', force=True)

    anisotropy = cmds.getAttr(inShd + '.anisotropy')
    if anisotropy > 1:
        #lerp from 1:10 to 0.5:1
        anisotropy = ((anisotropy - 1.0) * 0.5 / 9.0) + 0.5
        if anisotropy > 1:
            anisotropy = 1
    elif anisotropy < 1:
        #lerp from 0:1 to 0:0.5
        anisotropy = anisotropy * 0.5

    setValue(outNode+'.specularAnisotropy', anisotropy)

    '''    
    ior_fresnel =  cmds.getAttr(inShd + '.brdf_fresnel')

    reflectivity = 1.0
    connReflectivity = cmds.listConnections( outNode + '.Ks', d=False, s=True, plugs=True )
    if not connReflectivity:
        reflectivity = cmds.getAttr(outNode+'.Ks')

    if ior_fresnel:
        # compute from IOR
        # using Schlick's approximation
        ior = cmds.getAttr(inShd + '.refr_ior')
        frontRefl = (ior - 1.0) / (ior + 1.0)
        frontRefl *= frontRefl
        setValue(outNode +'.Ksn', frontRefl * reflectivity)
    else:
        # ignoring brdf_90_degree_refl as it's usually left to 1
        setValue(outNode +'.Ksn', cmds.getAttr(inShd + '.brdf_0_degree_refl') * reflectivity)

    # copy translucency value only if refr_translucency is enabled
    if cmds.getAttr(inShd + '.refr_translucency'):
        setValue(outShd +'.Kb', cmds.getAttr(inShd + '.refr_trans_weight'))     

    '''
    print("Converted %s to aiStandardSurface" % inShd)
    return outNode

def convertDielectricMaterial(inShd):
    outNode = createArnoldShader(inShd, 'aiStandardSurface')

    convertAttr(inShd, 'ior', outNode, 'specular_IOR')
    convertAttr(inShd, 'ior', outNode, 'coat_IOR')

    cosinePower = cmds.getAttr(inShd + '.phong_coef')
    
    if cosinePower > 0.0:
        roughness = math.sqrt(1.0 / (0.454 * cosinePower + 3.357))
        setValue(outNode + '.specularRoughness', roughness)
        setValue(outNode + '.specular', 1.0)
        #this "fake spec"  is only for direct illum
        setValue(outNode + '.indirectSpecular', 0)
    else:
        setValue(outNode + '.specular', 0.0)

    setValue(outNode + '.base', 0.0)
    setValue(outNode + '.coat', 1.0)

    setValue(outNode + '.coatRoughness', 0.0)
    setValue(outNode + '.transmission', 1.0)

    print("Converted %s to aiStandardSurface" % inShd)
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
                try:
                    cmds.setAttr(attr, value, type=aType)
                except RuntimeError as err:
                    pass
                    
            elif attrType in ['long', 'short', 'float', 'byte', 'double', 'doubleAngle', 'doubleLinear', 'bool']:
                aType = None
                try:
                    cmds.setAttr(attr, value)
                except RuntimeError as err:
                    pass    
                
            elif attrType in ['long2', 'short2', 'float2',  'double2', 'long3', 'short3', 'float3',  'double3']:
                if isinstance(value, float):
                    if attrType in ['long2', 'short2', 'float2',  'double2']:
                        value = [(value,value)]
                    elif attrType in ['long3', 'short3', 'float3',  'double3']:
                        value = [(value, value, value)]
                try:
                    cmds.setAttr(attr, *value[0], type=attrType)
                except RuntimeError as err:
                    pass    

        if isLocked:
            # restore the lock on the attr
            cmds.setAttr(attr, lock=True)


def convertArnoldShaders():
    if not cmds.pluginInfo( 'mtoa', query=True, loaded=True ):
        cmds.loadPlugin('mtoa')

    convertUi()






