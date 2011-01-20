from __future__ import with_statement

import sys
import os.path
import yaml

def stringToType(val, type):
    types = {
             'scalar':'float',
             'boolean':'int',
             'color':'float',
            }
    
    if type not in types.keys():
        return val
    
    if isinstance(val, list):
        val = [eval('%s(%s)'%(types[type], x)) for x in val]
    else:
        if type == 'boolean':
            if val == 'true' or val == 'True' or val == 'on':
                return 1
            elif val == 'false' or val == 'False' or val == 'off':
                return 0
        
        val = eval('%s(%s)'%(types[type], val))
        
    
    return val

def parse_file(file, root=None):
    if root is None:
        root = {'shaders':{}}
    section = None
    shader = {os.path.splitext(os.path.basename(file))[0]:{}}
    shader_data = {'attributes':{}, 'metadata':{'maya.id':2012}}
    param = None
    param_data = {}
    with open(file) as f:
        for line in f:
            line = line.strip()
            if line:
                parts = line.split()
                if parts[0] == 'PropertySet':
                    section = 'PropertySet'
                elif section == 'PropertySet':
                    if parts[0] == '{':
                        section = 'start_params'
                elif section == 'start_params':
                    if parts[0] == '}':
                        section = None
                    elif parts[0] == 'Parameter':
                        if parts[2] == 'input':
                            section = 'params'
                            val = parts[1]
                            if val.startswith('"'):
                                val = val[1:-1]
                            param = val
                            param_data = {}
                        else:
                            section = 'skip_param'
                elif section == 'skip_param':
                    if parts[0] == '}':
                        section = 'start_params'
                elif section == 'params':
                    if parts[0] == '{':
                        section = 'params_data'
                elif section == 'params_data':
                    if parts[0] == '}':
                        param_data['maya.shortname'] = ""
                        if 'type' in param_data.keys():
                            check = ['default', 'min', 'max']
                            for x in check:
                                if x in param_data.keys():
                                    param_data[x] = stringToType(param_data[x], param_data['type'])
                                        
                        shader_data['attributes'].update({param:param_data})
                        section = 'start_params'
                    else:
                        data = [x.strip() for x in line.split('=', 1)]
                        if len(data) == 2:
                            var, val = data
                            val = val.rstrip(';')
                            if val.startswith('"'):
                                val = val[1:-1]
                            if var == 'value':
                                if len(val.split()) > 1:
                                    if len(val.split()) > 3:
                                        val = val.split()[:-1]
                                    else:
                                        val = val.split()
                                    param_data['default'] = val
                                    param_data['min'] = [0.0,0.0,0.0]
                                    param_data['max'] = [1.0, 1.0, 1.0]
                                else:
                                    param_data['default'] = val
                            elif var == 'value minimum':
                                param_data['min'] = val
                            elif var == 'value maximum':
                                param_data['max'] = val
                            elif var == 'title':
                                if val:
                                    param_data['label'] = val
                            elif var == 'type' or var == 'Type':
                                param_data['type'] = val
                            else:
                                pass
                                #sys.stderr.write( "unknown line type %r\n" % line)
                
                #DEFAULTS SECTION    
                elif parts[0] == 'Defaults':
                    section = 'Defaults'
                elif section == 'Defaults':
                    if parts[0] == '{':
                        section = 'start_defaults'
                elif section == 'start_defaults':
                    if parts[0] == '}':
                        section = None
                    elif parts[0] in shader_data['attributes'].keys():
                        section = 'default'
                        param = parts[0]
                elif section == 'default':
                    if parts[0] == '{':
                        section = 'default_data'
                elif section == 'default_data':
                    if parts[0] == '}':
                        param = None
                        section = 'start_defaults'
                    else:
                        data = [x.strip() for x in line.split('=', 1)]
                        if len(data) == 2:
                            var, val = data
                            val = val.rstrip(';')
                            if val.startswith('"'):
                                val = val[1:-1]
                            if var == 'Name':
                                if 'label' not in shader_data['attributes'][param].keys():
                                    shader_data['attributes'][param]['label'] = val.replace('-', ' ')
                                
    shader[shader.keys()[0]] = shader_data
    stream = open(file.replace('spdl', 'yaml'), 'w')
    yaml.dump(shader, stream, default_flow_style=False)
    stream.close()
    root['shaders'].update(shader)
    return root

def parse_dir(path):
    #root = et.Element('shaders')
    root = {'shaders':{}}
    for file in os.listdir(path):
        if file.endswith('.spdl'):
            parse_file(os.path.join(path, file), root)
    return root
if __name__ == '__main__':
    arg = sys.argv[1]
    if os.path.isdir(arg):
        root = parse_dir(arg)
    else:
        root = parse_file(arg)
    #indent(root)
    #print root
    print(yaml.dump(root, default_flow_style=False))
