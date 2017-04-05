import maya.cmds as cmds
import math


replaceShaders = True
targetShaders = ['aiStandard', 'aiHair']

    
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

    for shdType in targetShaders:
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
    
    for shdGroup in  shdGroups:
        if replaceShaders:
            cmds.connectAttr(newShd + '.outColor', shdGroup, force=True)
            cmds.delete(oldShd)
        else:
            cmds.connectAttr(newShd + '.outColor', shdGroup, force=True)
        retVal =True
        
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
    convertAttr(inShd, 'Kd_color', outNode, 'base_color')
    convertAttr(inShd, 'diffuse_roughness', outNode, 'diffuse_roughness')

    convertAttr(inShd, 'Ks', outNode, 'specular')
    convertAttr(inShd, 'Ks_color', outNode, 'specular_color')
    convertAttr(inShd, 'specular_roughness', outNode, 'specular_roughness')

    fresnel = cmds.getAttr(inShd + '.Fresnel')
    fresnel_use_ior = cmds.getAttr(inShd + '.Fresnel_use_IOR')
    specular_Fresnel = cmds.getAttr(inShd + '.specular_Fresnel')

    if int(fresnel) > 0:
        if int(fresnel_use_ior) > 0:
            convertAttr(inShd, 'IOR', outNode, 'specular_IOR')
        else:
            convertAttr(inShd, 'Krn', outNode, 'specular_IOR', krnToIorRemap)

    if int(specular_Fresnel) > 0:
        if int(fresnel_use_ior) > 0:
            convertAttr(inShd, 'IOR', outNode, 'coat_IOR')
        else:
            convertAttr(inShd, 'Ksn', outNode, 'coat_IOR', krnToIorRemap)

    convertAttr(inShd, 'specular_anisotropy', outNode, 'specular_anisotropy', anisotropyRemap)

    convertAttr(inShd, 'specular_rotation', outNode, 'specular_rotation', rotationRemap)
    convertAttr(inShd, 'Kt', outNode, 'transmission')

    convertAttr(inShd, 'Kt_color', outNode, 'transmission_color') # not multiplying by transmittance

    # transmission_depth => (transmittance == AI_RGB_WHITE) ? 0.0 : 1.0
    convertAttr(inShd, 'dispersion_abbe', outNode, 'transmission_dispersion') # not multiplying by transmittance    

    # transmission_extra_roughness => refraction_roughness - specular_roughness    

    convertAttr(inShd, 'Ksss', outNode, 'subsurface')
    convertAttr(inShd, 'Ksss_color', outNode, 'subsurface_color')
    convertAttr(inShd, 'sss_radius', outNode, 'subsurface_radius')

    convertAttr(inShd, 'Kr', outNode, 'coat')
    convertAttr(inShd, 'Kr_color', outNode, 'coat_color')
    cmds.setAttr(outNode + '.coat_roughness', 0)


    convertAttr(inShd, 'emission', outNode, 'emission')
    convertAttr(inShd, 'emission_color', outNode, 'emission_color')
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
    convertAttr(inShd, 'Kd_ind', outNode, 'indirect_diffuse')
    #convertAttr(inShd, 'spec', outNode, 'specular')
    convertAttr(inShd, 'spec_color', outNode, 'specular_tint')
    #convertAttr(inShd, 'spec2', outNode, 'specular2')
    convertAttr(inShd, 'spec2_color', outNode, 'specular2_tint')
    convertAttr(inShd, 'spec_gloss', outNode, 'roughness', glossRemap)
    convertAttr(inShd, 'spec_shift', outNode, 'shift', shiftRemap)
    convertAttr(inShd, 'transmission_color', outNode, 'transmission_tint')
    convertAttr(inShd, 'opacity', outNode, 'opacity')

    cmds.setAttr(outNode + '.melanin', 0)

    print "Converted %s to aiStandardHair" % inShd
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

def convertAttr(inNode, inAttr, outNode, outAttr, functionPtr = None, secondaryAttrs = []):

    if cmds.objExists(inNode + '.' + inAttr):
        #print '\t', inAttr, ' -> ', outAttr
        
        if not setupConnections(inNode, inAttr, outNode, outAttr):
            # copy the values
            val = cmds.getAttr(inNode + '.' + inAttr)
            if functionPtr:
                if len(secondaryAttrs) == 0:
                    val = functionPtr(val)
                else:
                    secondaryVals = []
                    for secondaryAttr in secondaryAttrs:
                        secondaryVal = cmds.getAttr(inNode + '.' + secondaryAttr)
                        val.append
                
                
            setValue(outNode + '.' + outAttr, val)


def setValue(attr, value):
    """Simplified set attribute function.

    @param attr: Attribute to set. Type will be queried dynamically
    @param value: Value to set to. Should be compatible with the attr type.
    """

    aType = None

    if cmds.objExists(attr):
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






