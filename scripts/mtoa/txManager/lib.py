from __future__ import print_function
from __future__ import absolute_import
import maya.cmds as cmds
import maya.utils as utils
import os.path
import re
import os
import platform
import mtoa.makeTx as makeTx
import arnold as ai
from mtoa.ui.qt.Qt import QtWidgets, QtCore
from mtoa.ui.qt.utils import getMayaWindow
# legacy imports
import copy
import logging
import string


valid_chars = string.ascii_letters

_regex = r'^([\w\_]+)\.([\w\_\*]+)$'
regex = re.compile(_regex)
logger = logging.getLogger(__name__)


img_extensions = [
    '.jpeg',
    '.jpg',
    '.tiff',
    '.tif',
    '.png',
    '.exr',
    '.hdr',
    '.bmp',
    '.tga',
]

default_texture_data = {
    'usage': {},
    'root': '',
    'name': '',
    'status': 'notx',
    'path': None,
    'txpath': None,
    'colorspace': None,
    'index': 0,
    'item':None
}

scene_default_texture_scan = [
    'file.fileTextureName',
    'aiImage.filename',
    'imagePlane.imageName',
    'mesh.mtoa_constant_*'
]

scene_expand_attributes = {
    'fileTextureName': 'computedFileTextureNamePattern'
}


class MakeTxThread (QtCore.QThread):

    maxProgress = QtCore.Signal(int)
    progress = QtCore.Signal(int)

    def __init__(self, manager, parent):
        QtCore.QThread.__init__(self, parent)
        self.txManager = manager
        self.filesCreated = 0
        self.createdErrors = 0
        self.is_canceled = False
        self.force = True

        self.processor = TxProcessor(self.txManager)

    def run(self):
        self.filesCreated = 0
        self.createdErrors = 0
        self.is_canceled = False
        self.processor.maxProgress.connect(self.emit_max_progress)
        self.processor.progress.connect(self.emit_progress)
        self.processor.createTx()

    # create a .tx file with the provided options. It will wait until it is finished
    def runMakeTx(self, texture, space):

        arg_options = self.txManager.get_tx_args()
        status = utils.executeInMainThreadWithResult( makeTx.makeTx, texture, colorspace=space, arguments=arg_options)
        return status

    def emit_max_progress(self, value):
        self.maxProgress.emit(value)

    def emit_progress(self, value):
        self.progress.emit(value)

    def set_forced(self, forced):
        self.processor.force = forced

    def cancel_tx(self):
        self.is_canceled = True


class TxProcessor(QtCore.QObject):
    """docstring for TxProcessor"""

    maxProgress = QtCore.Signal(int)
    progress = QtCore.Signal(int)

    def __init__(self, txmanager):
        super(TxProcessor, self).__init__()
        self.txManager = txmanager
        self.filesCreated = 0
        self.createdErrors = 0
        self.is_canceled = False
        self.force = True

    def test_progress(self):
        self.maxProgress.emit(100)
        for i in range(100):
            self.progress.emit(i)

        return 

    def createTx(self):
        print("createTx")
        selected_textures = utils.executeInMainThreadWithResult(self.txManager.get_selected_textures)
        if not selected_textures:
            return self.test_progress()
        
        # return self.test_progress()
       
        # first we need to make sure the options & color manager node were converted to arnold
        # cmds.arnoldScene(mode='create')
        # previous_flags = ai.AiMsgGetConsoleFlags(None)
        # ai.AiMsgSetConsoleFlags(None, ai.AI_LOG_INFO)
        render_colorspace = cmds.colorManagementPrefs(query=True, renderingSpaceName=True)

        cmEnable = cmds.colorManagementPrefs(query=True, cmEnabled=True)

        textureList = []

        arg_options = self.txManager.get_tx_args()

        for i, textureData in enumerate(selected_textures):
            texture = textureData['path']

            if textureData['colorspace'] != '':
                colorSpace = textureData['colorspace']
            
            if not texture or not colorSpace:
                continue
            
            # Process all the files that were found previously for this texture (eventually multiple tokens)
            inputFiles = utils.executeInMainThreadWithResult(makeTx.expandFilenameWithSearchPaths, texture)

            for inputFile in inputFiles:

                tile_info = makeTx.imageInfo(inputFile)

                txArguments = "-v --unpremult --oiio"
                if self.force:
                    txArguments += " -u"
                if not self.txManager.get_use_autotx():
                    txArguments = ' '.join(arg_options)

                if cmEnable and colorSpace != render_colorspace:

                    txArguments += ' --colorconvert "'
                    txArguments += colorSpace
                    txArguments += '" "'
                    txArguments += render_colorspace
                    txArguments += '"'

                    if tile_info['bit_depth'] <= 8:
                        txArguments += ' --format exr -d half --compression dwaa'

                textureList.append([inputFile, txArguments, textureData['item']])

        self.txManager.filesToCreate = len(textureList)
        texture_dict = {}
        for tex in textureList:
            if tex[0] not in texture_dict:
                texture_dict[tex[0]] = [tex[2]]
            else:
                texture_dict[tex[0]].append(tex[2])

        print(texture_dict)

        num_jobs_left = len(textureList)   # default to arbitrary value > 0

        # set the max progress on the progress bar/dialog
        self.maxProgress.emit(num_jobs_left)

        # Now I have a list of textures to be converted
        # let's give this list to arnold
        for i, textureToConvert in enumerate(textureList):
            self.txManager.set_status(textureToConvert[2], "processing ..")
            # self.progress.emit(i)
            print("Processing {}".format(textureToConvert[0]))
            ai.AiMakeTx(textureToConvert[0], textureToConvert[1])
        status = ai.POINTER(ai.AtMakeTxStatus)() # returns the current status of the input files
        source_files = ai.POINTER(ai.AtPythonString)() # returns the list of input files in the same order as the status
        num_submitted = ai.c_uint()

        self.createdErrors = 0
        self.filesCreated = 0

        processed = 0
        while (num_jobs_left > 0):
            if self.is_canceled:
                print("[mtoa.tx] tx generation has been cancelled")
                ai.AiMakeTxAbort(ai.byref(status), ai.byref(source_files), ai.byref(num_submitted))
                break

            num_jobs_left = ai.AiMakeTxWaitJob(ai.byref(status), ai.byref(source_files), ai.byref(num_submitted))
            for i in range(0, num_submitted.value):
                # get the status and update the ui
                src_str = str(source_files[i])
                print(src_str)
                items = texture_dict.get(src_str, [])
                print(items)
                print("{} Status : {}".format(src_str, status[i]))
                for item in items:
                    print(item)
                    data = item.data(0, QtCore.Qt.UserRole)
                    output_tx = get_output_tx_path(src_str, data['colorspace'], render_colorspace)
                    # utils.executeInMainThreadWithResult(self.txManager.update_data, item)
                    # if status[i] is not ai.AiTxPending and self.txManager.get_status(item) in ["processing ..", "notx"]:
                    if status[i] is not ai.AiTxPending and os.path.exists(output_tx):
                        processed += 1

            # emit progress to the progress bar/dialog
            # self.progress.emit(processed)
        # self.test_progress()

        if (num_submitted.value > len(textureList)):
            ai.AiMsgFatal("There are more submitted textures than there are textures! "
                          "Queue should have been cleared!")

        for i in range(0, num_submitted.value):

            src_str = str(source_files[i])

            if (status[i] == ai.AiTxUpdated):
                self.filesCreated += 1
                print("[mtoa.tx] {}: {} was updated".format(i, src_str))
            elif (status[i] == ai.AiTxError):
                self.createdErrors += 1
                print("[mtoa.tx] {}: {} could not be updated".format(i, src_str))
            elif (status[i] == ai.AiTxUpdate_unneeded):
                print("[mtoa.tx] {}: {} did not need to be updated".format(i, src_str))
            elif (status[i] == ai.AiTxAborted):
                print("[mtoa.tx] {}: {} was aborted".format(i, src_str))

        utils.executeDeferred(self.txManager.on_refresh)

        # an arnold scene was created above, let's delete it now
        # cmds.arnoldScene(mode="destroy")
        # ai.AiMsgSetConsoleFlags(None,previous_flags)

        return True


def sanitize_string(string):
    '''Converts a string with possible non-ascii characters into a clean safe
    string'''
    result = ''
    for letter in string:
        result += letter if letter in valid_chars else '_'
    return result


def is_image(file):
    '''Returns whether the input file is an image'''
    ext = os.path.splitext(file)[1]
    return ext in img_extensions


def get_colorspace(nodeattr):

    node = nodeattr.split('.')[0]
    colorSpace = 'auto'

    if cmds.attributeQuery("colorSpace", node=node, exists=True):
       
        nodeColorSpace = cmds.getAttr(node+'.colorSpace')

        colorSpace = nodeColorSpace

    return colorSpace


def get_folder_textures(folder, subfolders=False):
    '''Returns a dictionary with all textures found in a folder. If subfolders
    flag is True, subfolders will be also scanned.'''
    textures = {}
    files = []
    if not subfolders:
        files = [x for x in os.listdir(folder) if is_image(x)]
        files = [os.path.join(folder, x) for x in files]
    else:
        for root, fld, fileList in os.walk(folder):
            files += [os.path.join(root, x) for x in fileList if is_image(x)]

    # Normalize ALL texture paths to avoid slash conflicts
    files = [os.path.normpath(x) for x in files]

    for texture in files:
        textures[texture] = copy.deepcopy(default_texture_data)

    return build_texture_data(textures, expand=False)


def get_scanned_files(scan_attributes):
    '''Scans the current scene for textures based on an input list. The
    scan_attributes argument must be a list of strings, in which each of them
    should be the node type and the node attribute separated by a dot.
    Attributes accept the "*" wildcard.

    Example:
        >> get_scanned_files(['file.texturePath', 'mesh.mtoa_*'])
    '''
    textures = {}

    for scan in scan_attributes:
        if not regex.match(scan):
            raise ValueError(
                'Scan attribute "%s" does not match regex "%s"' %
                (scan, _regex))

        ntype, attr = scan.split('.')
        attributes = set()
        if '*' not in attr:
            nodes = cmds.ls('*.%s' % attr, r=True, type=ntype, o=True)
            [attributes.add('%s.%s' % (x, attr)) for x in nodes]
        else:
            for node in cmds.ls(type=ntype):
                for a in cmds.listAttr(node, r=True, st=attr) or []:
                    if not cmds.getAttr(".".join([node, a]), type=True) == 'string':
                        continue

                    attributes.add(".".join([node, a]))

        for attribute in attributes:

            node = attribute.split('.')[0]

            # for some attributes we need to use a differnt computed attribute
            # to get the actual result or expressions etc
            attr_exp = attribute
            for k, v in scene_expand_attributes.items():
                attr_exp = attribute.replace(k, v)

            texture_path = cmds.getAttr(attr_exp)
            if not texture_path:
                continue

            texture_path = os.path.normpath(texture_path)
            textures.setdefault(
                texture_path, copy.deepcopy(default_texture_data))
            textures[texture_path]['usage'][node] = {}
            textures[texture_path]['path'] = texture_path
            textures[texture_path]['name'] = os.path.basename(texture_path)

    return build_texture_data(textures)


def build_texture_data(textures, expand=True):
    '''Builds the texture's dictionary. If the expand flag is enabled, it will
    attempt to expand the variables in the path.'''
    for texture, texture_data in textures.items():
        if expand:
            texture_exp = makeTx.expandFilenameWithSearchPaths(texture)
            if len(texture_exp):
                texture_exp = texture_exp[0]
            else:
                texture_exp = texture
        else:
            texture_exp = texture

        root, name = os.path.split(texture_exp)
        name_noext, ext = os.path.splitext(name)
        render_colorspace = cmds.colorManagementPrefs(q=True, renderingSpaceName=True)
        combined_status = []
        iinfo = makeTx.imageInfo(texture_exp)
        auto_cs =  makeTx.guessColorspace(iinfo)
        if len(texture_data['usage']):
            for node in texture_data['usage'].keys():
                cs = get_colorspace(node)
                if cs == 'auto':
                    cs = auto_cs
                if cs == 'linear':
                    cs = 'Raw'

                if ext == '.tx':
                    txstatus = 'onlytx'
                    txpath = texture
                else:
                    # FIXME use core API to get the tx generated for this texture or None
                    # AiResolveTextureFilename()
                    txpath_exp = os.path.join(texture_exp + "_{}_{}".format(cs, render_colorspace) + '.tx')
                    if os.path.isfile(txpath_exp):
                        txstatus = 'hastx'
                        txpath = os.path.splitext(texture)[0] + '.tx'
                    else:
                        txstatus = 'notx'
                        txpath = None

                if not os.path.isfile(texture_exp):
                    txstatus = 'missing'

                if texture not in textures.keys():
                    textures.setdefault(
                        texture, copy.deepcopy(textures[texture]))
                
                if txstatus not in combined_status:
                    combined_status.append(txstatus)

                texture_data['usage'][node]['root'] = root
                texture_data['usage'][node]['name'] = node
                texture_data['usage'][node]['status'] = txstatus
                texture_data['usage'][node]['txpath'] = txpath
                texture_data['usage'][node]['path'] = texture
                texture_data['usage'][node]['colorspace'] = cs
                for k,v in iinfo.items():
                    texture_data['usage'][node][k] = v
            textures[texture]['status'] = ','.join(combined_status)
        else:
            cs = auto_cs

            if ext == '.tx':
                txstatus = 'onlytx'
                txpath = texture
            else:
                # FIXME use core API to get the tx generated for this texture or None
                # AiResolveTextureFilename()
                txpath_exp = get_output_tx_path(texture_exp, cs, render_colorspace)
                if os.path.isfile(txpath_exp):
                    txstatus = 'hastx'
                    txpath = os.path.splitext(texture)[0] + '.tx'
                else:
                    txstatus = 'notx'
                    txpath = None

            if not os.path.isfile(texture_exp):
                txstatus = 'missing'

            textures[texture]['txpath'] = txpath
        
        textures[texture]['root'] = root
        textures[texture]['name'] = name
        textures[texture]['status'] = txstatus
        textures[texture]['path'] = texture

    return textures


def get_output_tx_path(input_file, colorspace, render_colorspace):

    # FIXME use core API to get the tx generated for this texture or None
    # AiResolveTextureFilename()
    txpath = os.path.join(input_file + "_{}_{}".format(colorspace, render_colorspace) + '.tx')
    return txpath

def update_texture_data(texture_data):
    path = texture_data['path']
    texture_exp = makeTx.expandFilenameWithSearchPaths(path)
    if len(texture_exp):
        texture_exp = texture_exp[0]
    else:
        texture_exp = path
    path_noext, ext = os.path.splitext(path)
    render_colorspace = cmds.colorManagementPrefs(q=True, renderingSpaceName=True)
    txpath = texture_data['txpath']
    txstatus = 'notx'
    if not txpath:
        txpath = get_output_tx_path(path, cs, render_colorspace)
        if os.path.isfile(txpath):
            txstatus = 'hastx'
        else:
            txstatus = 'notx'
            txpath = None
    if not os.path.isfile(texture_exp):
        txstatus = 'missing'
    texture_data['status'] = txstatus
    texture_data['txpath'] = txpath
    cs = get_colorspace(texture_data['name'])
    iinfo = makeTx.imageInfo(texture_exp)
    if cs == 'auto':
        cs = makeTx.guessColorspace(iinfo)
    if cs == 'linear':
        cs = 'Raw'
    texture_data['colorspace'] = cs
    for k,v in iinfo.items():
        texture_data[k] = v

    return texture_data


def build_tx_arguments(
        update=True,
        verbose=True,
        nans=True,
        stats=True,
        unpremutl=True,
        threads=None,
        preset=None,
        extra_args=None):
    '''Builds the maketx argument list'''
    args = []
    if update:
        args.append('-u')

    if verbose:
        args.append('-v')

    if nans:
        args.append('--checknan')

    if stats:
        args.append('--stats')

    if unpremutl:
        args.append('--unpremult')

    if preset is not None:
        args.append('--' + preset)

    if extra_args:
        args += [x for x in extra_args.split(' ') if x]

    if threads is not None and threads > 0:
        args += ['--threads', str(threads)]

    return args


class DummyManager(object):
    """docstring for DummyManager"""
    def __init__(self):
        super(DummyManager, self).__init__()
        self.filesToCreate = []
        self.textures = []

    def on_refresh(self):
        """ dummy method """
        pass

    def get_use_autotx(self):
        return True

    def set_status(self, index, status):
        # self.textures[index]['status'] = status
        pass

    def get_status(self, index):
        return self.textures[index]['status']

    def get_tx_args(self):
        return ""

    def get_selected_textures(self):
        _textures = utils.executeInMainThreadWithResult(get_scanned_files, scene_default_texture_scan)
        self.textures = []
        i = 0
        for k in sorted(_textures.keys()):
            _textures[k]['index'] = i
            self.textures.append(_textures[k])
            i += 1

        return self.textures

    def update_data(self, row):

        data = self.textures[row]
        self.textures[row] = update_texture_data(data)


def updateAllTx(force, threaded=True):

    manager = DummyManager()

    # check if we are in batch mode
    if not cmds.about(batch=True):
        mayawindow = getMayaWindow()
        thread = MakeTxThread(manager, mayawindow)
        thread.set_forced(force)
        progress = QtWidgets.QProgressDialog("processing textures...", "Abort", 0, 2, mayawindow)
        progress.forceShow()
        progress.setWindowModality(QtCore.Qt.WindowModal)

        progress.canceled.connect(thread.cancel_tx)
        thread.maxProgress.connect(progress.setMaximum)
        thread.progress.connect(progress.setValue)
        thread.finished.connect(progress.deleteLater)

        thread.start()

    else:

        processor = TxProcessor(manager)
        return processor.createTx()

    return False
