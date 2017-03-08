from arnold import *

def GetParamValueAsString(pentry, val, type):
    if type == AI_TYPE_BYTE:
        return str(val.contents.BYTE)   
    elif type == AI_TYPE_INT:
        return str(val.contents.INT)   
    elif type == AI_TYPE_UINT:
        return str(val.contents.UINT)   
    elif type == AI_TYPE_BOOLEAN:
        return "true" if (val.contents.BOOL != 0) else "false"   
    elif type == AI_TYPE_FLOAT:
        return "%g" % val.contents.FLT
    elif type == AI_TYPE_VECTOR:
        return "%g, %g, %g" % (val.contents.VEC.x, val.contents.VEC.y, val.contents.VEC.z)
    elif type == AI_TYPE_VECTOR2:
        return "%g, %g" % (val.contents.VEC2.x, val.contents.VEC2.y)
    elif type == AI_TYPE_RGB:
        return "%g, %g, %g" % (val.contents.RGB.r, val.contents.RGB.g, val.contents.RGB.b)
    elif type == AI_TYPE_RGBA:
        return "%g, %g, %g, %g" % (val.contents.RGBA.r, val.contents.RGBA.g, val.contents.RGBA.b, val.contents.RGBA.a)
    elif type == AI_TYPE_STRING:
        return val.contents.STR
    elif type == AI_TYPE_POINTER:
        return "" #"%p" % val.contents.PTR
    elif type == AI_TYPE_NODE:
        name = AiNodeGetName(val.contents.PTR)
        return str(name)
    elif type == AI_TYPE_ENUM:
        enum = AiParamGetEnum(pentry)
        return AiEnumGetString(enum, val.contents.INT)
    elif type == AI_TYPE_MATRIX:
        return ""
    elif type == AI_TYPE_ARRAY:
        array = val.contents.ARRAY.contents
        nelems = array.nelements
        if nelems == 0:
            return "(empty)"
        elif nelems == 1:
            if array.type == AI_TYPE_FLOAT:
                return "%g" % AiArrayGetFlt(array, 0)
            elif array.type == AI_TYPE_VECTOR:
                vec = AiArrayGetVec(array, 0)
                return "%g, %g, %g" % (vec.x, vec.y, vec.z)
            elif array.type == AI_TYPE_RGB:
                rgb = AiArrayGetRGB(array, 0)
                return "%g, %g, %g" % (rgb.r, rgb.g, rgb.b)
            elif array.type == AI_TYPE_RGBA:
                rgba = AiArrayGetRGBA(array, 0)
                return "%g, %g, %g" % (rgba.r, rgba.g, rgba.b, rgba.a)
            elif array.type == AI_TYPE_POINTER:
                ptr = cast(AiArrayGetPtr(array, 0), POINTER(AtNode))
                return "%p" % ptr
            elif array.type == AI_TYPE_NODE:
                ptr = cast(AiArrayGetPtr(array, 0), POINTER(AtNode))
                name = AiNodeGetName(ptr)
                return str(name)
            else:
                return ""
        else:
            return "(%u elements)" % nelems  
    else:
        return ""


AiBegin()
nnodes = AiGetNumInstalledNodes()
nodes = []
nodeDict = {}
i = 0
while i < nnodes:
    node_entry     = AiNodeEntryLookUpByIndex(i)
    node_name      = AiNodeEntryGetName(node_entry)
    node_type      = AiNodeEntryGetTypeName(node_entry)
    node_output    = AiParamGetTypeName(AiNodeEntryGetOutputType(node_entry))
    node_param_num = AiNodeEntryGetNumParams(node_entry)
    node_filename  = AiNodeEntryGetFilename(node_entry)
    node_version   = AiNodeEntryGetVersion(node_entry)
    node_filename  = (node_filename if node_filename != '' else "built-in")
    #res = ''
    #print AiMetaDataGetStr(node_entry, None, "description",res)

    #
    if nodeDict.has_key(node_type) == False:
        nodeDict[node_type] = {}
    #
    nodeDict[node_type][node_name] = {'type':node_type, 'output':node_output, 'filename':node_filename, 'version':node_version, 'parameters':{}}
    
    params = []
    for p in range(node_param_num):
        pentry = AiNodeEntryGetParameter(node_entry, p)
        params.append([AiParamGetName(pentry), p])
    
    for pp in params:
        pentry = AiNodeEntryGetParameter(node_entry, pp[1])
        param_type  = AiParamGetType(pentry)
        param_value = AiParamGetDefault(pentry)
        param_name  = AiParamGetName(pentry)
        
        if param_type == AI_TYPE_ARRAY:
            # We want to know the type of the elements in the array
            array = param_value.contents.ARRAY.contents
            type_string = "%s[]" % AiParamGetTypeName(array.type)
        else:
            type_string = AiParamGetTypeName(param_type)
        
        #
        default = GetParamValueAsString(pentry, param_value, param_type)
        nodeDict[node_type][node_name]['parameters'][param_name] = [type_string, default]
        #print "        %-32s  %-12s %-32s" % (param_name, type_string, default)

    i += 1

AiEnd()

totalNodeCount = 0
totalAttributeCount = 0

outHtml  = ""
outHtml += "<html>\n"
outHtml += "<body>\n"
outHtml += "<pre>\n"
# Pour chaque type de node
for mType in sorted(nodeDict.keys()):
    outHtml += "<b><a href=\"#"+mType+"\">%s</a></b>\n" % mType.capitalize()
    # Pour chaque node de ce type
    for mNode in sorted(nodeDict[mType].keys()):
        outHtml += "    <b><a href=\"#"+mNode+"\">%s</a></b>\n" % mNode

# Pour chaque type de node
for mType in sorted(nodeDict.keys()):
    outHtml += "<h1><a name=\""+mType+"\">%s</a></h1>\n" % mType.capitalize()
    #print mType.capitalize()
    # Pour chaque node de ce type
    for mNode in sorted(nodeDict[mType].keys()):
        totalNodeCount += 1
        outHtml += "    <b><a name=\""+mNode+"\">%s</a></b>\n" % mNode
        outHtml += "        Output     : %s\n" % nodeDict[mType][mNode]['output']
        #outHtml += "        Filename   : %s\n" % nodeDict[mType][mNode]['filename']
        #outHtml += "        Version    : %s\n" % nodeDict[mType][mNode]['version']
        outHtml += "        Parameters : %d\n" % len(nodeDict[mType][mNode]['parameters'])
        totalAttributeCount += len(nodeDict[mType][mNode]['parameters'])
        # Pour chaque attribut
        for mAttr in sorted(nodeDict[mType][mNode]['parameters']):
            tmpAttr = nodeDict[mType][mNode]['parameters'][mAttr]
            outHtml += "            %-32s %-12s %-32s\n" % (mAttr, tmpAttr[0], tmpAttr[1])
        outHtml += "\n"
    #outHtml += "\n"

outHtml += "</pre>\n"
outHtml += "</body>\n"
outHtml += "</html>\n"

print "Total type number %d " % len(nodeDict)
print "Total node number %d " % totalNodeCount
print "Total attribute count %d " % totalAttributeCount
htmlPath   = '/home/ndu/toto.html'
filehandle = open(htmlPath, 'w')
filehandle.writelines(outHtml)
filehandle.close()
