from __future__ import with_statement

import sys
import os.path
import xml.etree.ElementTree as et
#import lxml.etree as et

def indent(elem, level=0):
    i = "\n" + level*"  "
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = i + "  "
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
        for elem in elem:
            indent(elem, level+1)
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
    else:
        if level and (not elem.tail or not elem.tail.strip()):
            elem.tail = i

def parse_file(file, root=None):
    if root is None:
        root = et.Element('shaders')
    section = None
    shader = et.SubElement(root, os.path.splitext(os.path.basename(file))[0])
    params = et.SubElement(shader, 'Shader_params')
    param = None
    with open(file) as f:
        for line in f:
            line = line.strip()
            if line:
                parts = line.split()
                if section == 'PropertySet':
                    if parts[0] == '{':
                        section = 'start_params'
                elif section == 'start_params':
                    if parts[0] == '}':
                        section = None
                    elif parts[0] == 'Parameter':
                        section = 'params'
                        if parts[2] == 'input':
                            val = parts[1]
                            if val.startswith('"'):
                                val = val[1:-1]
                            param = et.SubElement(params, val)
                elif section == 'params':
                    if parts[0] == '{':
                        section = 'params_data'
                elif section == 'params_data':
                    if parts[0] == '}':
                        section = 'start_params'
                    else:
                        data = [x.strip() for x in line.split('=',1)]
                        if len(data) == 2:
                            var, val = data
                            val = val.rstrip(';')
                            if val.startswith('"'):
                                val = val[1:-1]
                            if var == 'value':
                                data = et.SubElement(param, 'Default')
                                data.text = val
                            elif var == 'value minimum':
                                data = et.SubElement(param, 'softMin')
                                data.text = val
                            elif var == 'value maximum':
                                data = et.SubElement(param, 'softMax')
                                data.text = val
                        else:
                            sys.stderr.write( "unknown line type %r\n" % line)
                elif parts[0] == 'PropertySet':
                    section = 'PropertySet'
    return root

def parse_dir(path):
    root = et.Element('shaders')
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
    indent(root)
    print(et.tostring(root))
