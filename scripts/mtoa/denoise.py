import maya.cmds as cmds
import maya.utils as utils
import maya.mel as mel
import os.path
import glob
import re
import sys, os
import subprocess
import threading
import mtoa.callbacks as callbacks
import maya.OpenMaya as om
import mtoa.utils as mutils
import mtoa.core as core
import platform
import maya.mel as mel

inputFile = ""

_noice_binary = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(__file__))), 'bin', 'noice')

# startupinfo to prevent Windows processes to display a console window
if platform.system().lower() == 'windows':
    _no_window = subprocess.STARTUPINFO()
    _no_window.dwFlags |= subprocess.STARTF_USESHOWWINDOW
else:
    _no_window = None

def expandFilename(input_file, start_frame):
    inFile = input_file
    if not '#' in inFile:
        return inFile

    extension_len = 0
    padding = 0
    for c in reversed(inFile):
        if c == '#':
            padding += 1
        elif padding > 0:
            break # I already found numerical characters and they're finished now
        else:
            # still haven't found a numerical parameter
            extension_len += 1
    
    start_frame_str = str(start_frame)
    for i in range(1, padding):
        if start_frame < pow(10, i):
            start_frame_str = '0' + start_frame_str
    
    inFile = inFile[:-extension_len - padding] + start_frame_str + inFile[-extension_len:]
    return inFile

def denoiseImage(inFile, outFile, f, temporal_frames, pixel_search_radius, pixel_patch_radius, variance, light_group_aovs):
    inFile = expandFilename(inFile, f)
    outFile = expandFilename(outFile, f)
    cmd = [_noice_binary]
    cmd += ['-i', inFile, '-o', outFile]
    cmd += ['-ef', str(temporal_frames), '-sr', str(pixel_search_radius), '-pr', str(pixel_patch_radius), '-v', str(variance)]

    if len(light_group_aovs) > 0:
        cmd += ['-l', light_group_aovs]
    
    print 'Denoising image {} '.format(inFile)
    cmd_str = ''
    for cmd_elem in cmd:
        cmd_str += cmd_elem + ' '
    print cmd_str

    res = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, startupinfo=_no_window).communicate()[0]
    print res

def noiceExecuteMel(progress_cmd):
    mel.eval(progress_cmd)

class NoiceThread (threading.Thread):
    def __init__(self,win, start_frame, end_frame, inFile, outFile, temporal_frames, pixel_search_radius, pixel_patch_radius, variance, light_group_aovs):
        threading.Thread.__init__(self)
        self.win = win
        self.start_frame = start_frame
        self.end_frame = end_frame
        self.inFile = inFile
        self.outFile = outFile
        self.temporal_frames = temporal_frames
        self.pixel_patch_radius = pixel_patch_radius
        self.pixel_search_radius = pixel_search_radius
        self.variance = variance
        self.light_group_aovs = light_group_aovs
        
    def run (self):        

        
        progress_cmd = 'global string $gMainProgressBar;'
        progress_cmd = 'progressBar -edit -beginProgress -status \"Denoising ...\" -maxValue {} -progress 0 $gMainProgressBar;'.format(int(self.end_frame) - int(self.start_frame) + 1);
        utils.executeDeferred(noiceExecuteMel,progress_cmd)


        for f in range(self.start_frame, self.end_frame + 1):
            if not self.win.running:
                break
            print  '------------------------ ({}/{})'.format(f - self.start_frame + 1, self.end_frame - self.start_frame + 1)
            denoiseImage(self.inFile, self.outFile, f, self.temporal_frames, self.pixel_search_radius, self.pixel_patch_radius, self.variance, self.light_group_aovs)
            progress_cmd = 'global string $gMainProgressBar;'
            progress_cmd += 'progressBar -edit -progress '
            progress_cmd += str(f - self.start_frame + 1)
            progress_cmd += ' $gMainProgressBar;'
            utils.executeDeferred(noiceExecuteMel,progress_cmd)


        progress_cmd = 'global string $gMainProgressBar;'
        progress_cmd += 'progressBar -edit -endProgress $gMainProgressBar;'
        utils.executeDeferred(noiceExecuteMel,progress_cmd)

        if self.win.window:        
            progress_cmd = 'deleteUI {}'.format(self.win.window)
            utils.executeDeferred(noiceExecuteMel,progress_cmd)
            self.win.window = None

        self.win.running = False
        self.win.thread = None
        

class MtoANoice(object):
    window = None
    def __new__(cls, *args, **kwargs):
        if not '_instance' in vars(cls):
            cls._instance = super(MtoANoice, cls).__new__(cls, *args, **kwargs)
        return cls._instance

    def __init__(self):
        if self.window is None:
            self.window = 'MtoANoice'
            self.listElements = []
            self.column = ''
            self.running = False
            self.thread = None

    def create(self):
        if cmds.window(self.window, exists=True):
            cmds.deleteUI(self.window)

        winTitle = "Arnold Denoiser (noice)"
        self.window = cmds.window(self.window, widthHeight=(490, 260), title=winTitle)
        self.createUI()

        cmds.setParent(menu=True)
        cmds.showWindow(self.window)

        try:
            initPos = cmds.windowPref( self.window, query=True, topLeftCorner=True )
            if initPos[0] < 0:
                initPos[0] = 0
            if initPos[1] < 0:
                initPos[1] = 0
            cmds.windowPref( self.window, edit=True, topLeftCorner=initPos )
        except :
            pass

    def frameRateChanged(self):
        frame_range = cmds.optionMenuGrp('noice_frame_range', q=True, v=True)
        cmds.intFieldGrp('noice_start_frame', e=True, enable=(False if frame_range == 'Complete Sequence' else True))
        cmds.intFieldGrp('noice_end_frame', e=True, enable= True if frame_range == 'Start / End' else False)

    def createUI(self):
        cmds.scrollLayout(childResizable=True,)
        cmds.columnLayout(adjustableColumn=True)
        
        label_margin = 150

        core.createOptions()

        global inputFile

        if mel.eval('attributeExists ("noice_input_file", "defaultArnoldRenderOptions")'):
            inputFile = str(cmds.getAttr('defaultArnoldRenderOptions.noice_input_file'))
        elif cmds.optionVar(exists='mtoa_noice_input_file'):
            inputFile = str(cmds.optionVar(query='mtoa_noice_input_file'))
        
        cmds.textFieldButtonGrp('noice_input', label='Input', cw3=(label_margin,290, 70), text=inputFile, buttonLabel='...', buttonCommand=lambda *args: self.browseInputFilename())
        
        noice_denoised_file = ''
        if mel.eval('attributeExists ("noice_denoised_file", "defaultArnoldRenderOptions")'):
            noice_denoised_file = str(cmds.getAttr('defaultArnoldRenderOptions.noice_denoised_file'))
        elif cmds.optionVar(exists='mtoa_noice_denoised_file'):
            noice_denoised_file = str(cmds.optionVar(query='mtoa_noice_denoised_file'))
        
        cmds.textFieldButtonGrp('noice_output', label='Output', cw3=(label_margin,290, 70), text=noice_denoised_file, buttonLabel='...', buttonCommand=lambda *args: self.browseOutputFilename())

        #cmds.setParent("..")
        
        cmds.optionMenuGrp('noice_frame_range', label='Frame Range')
        cmds.menuItem( label='Single Frame' )
        cmds.menuItem( label='Start / End' )
        cmds.menuItem( label='Complete Sequence' )

        noice_frame_range = 'Complete Sequence'
        if mel.eval('attributeExists ("noice_frame_range", "defaultArnoldRenderOptions")'):
            noice_frame_range = str(cmds.getAttr('defaultArnoldRenderOptions.noice_frame_range'))
        elif cmds.optionVar(exists='mtoa_noice_frame_range'):
            noice_frame_range = str(cmds.optionVar(query='mtoa_noice_frame_range'))
        
        cmds.optionMenuGrp('noice_frame_range', e=True, w=230, ct2=('left', 'left'), cw2=(label_margin,110), v=noice_frame_range, cc=lambda *args: self.frameRateChanged())

        noice_start_frame = 0
        if mel.eval('attributeExists ("noice_start_frame", "defaultArnoldRenderOptions")'):
            noice_start_frame = cmds.getAttr('defaultArnoldRenderOptions.noice_start_frame')
        elif cmds.optionVar(exists='mtoa_noice_start_frame'):
            noice_start_frame = cmds.optionVar(query='mtoa_noice_start_frame')
        
        cmds.intFieldGrp('noice_start_frame', label='Start Frame', value1=noice_start_frame, ct2=('left', 'left'),  cw2=(label_margin,110), w=230)

        noice_end_frame = 10
        if mel.eval('attributeExists ("noice_end_frame", "defaultArnoldRenderOptions")'):
            noice_end_frame = cmds.getAttr('defaultArnoldRenderOptions.noice_end_frame')
        elif cmds.optionVar(exists='mtoa_noice_end_frame'):
            noice_end_frame = cmds.optionVar(query='mtoa_noice_end_frame')
        
        cmds.intFieldGrp('noice_end_frame', label='End Frame', value1=noice_end_frame, ct2=('left', 'left'),  cw2=(label_margin,110), w=230)

        noice_temporal_frames = 0

        if mel.eval('attributeExists ("noice_temporal_frames", "defaultArnoldRenderOptions")'):
            noice_temporal_frames = cmds.getAttr('defaultArnoldRenderOptions.noice_temporal_frames')
        elif cmds.optionVar(exists='mtoa_noice_temporal_frames'):
            noice_temporal_frames = cmds.optionVar(query='mtoa_noice_temporal_frames')
        
        cmds.intFieldGrp('noice_temporal_frames', label='Temporal Stability Frames', value1=noice_temporal_frames, ct2=('left', 'left'),  cw2=(label_margin,110), w=230)
        
        noice_variance = 0.5
        if mel.eval('attributeExists ("noice_variance", "defaultArnoldRenderOptions")'):
            noice_variance = cmds.getAttr('defaultArnoldRenderOptions.noice_variance')
        elif cmds.optionVar(exists='mtoa_noice_variance'):
            noice_variance = cmds.optionVar(query='mtoa_noice_variance')
        
        cmds.floatFieldGrp('noice_variance', label='Variance', value1= noice_variance, ct2=('left', 'left'),  cw2=(label_margin,110), w=230)

        noice_pixel_search_radius = 9
        if mel.eval('attributeExists ("noice_pixel_search_radius", "defaultArnoldRenderOptions")'):
            noice_pixel_search_radius = cmds.getAttr('defaultArnoldRenderOptions.noice_pixel_search_radius')
        elif cmds.optionVar(exists='mtoa_noice_pixel_search_radius'):
            noice_pixel_search_radius = cmds.optionVar(query='mtoa_noice_pixel_search_radius')
        
        cmds.intFieldGrp('noice_pixel_search_radius', label='Pixel Search Radius', value1=noice_pixel_search_radius, ct2=('left', 'left'),  cw2=(label_margin,110), w=230)
        
        noice_pixel_patch_radius = 3
        if mel.eval('attributeExists ("noice_pixel_patch_radius", "defaultArnoldRenderOptions")'):
            noice_pixel_patch_radius = cmds.getAttr('defaultArnoldRenderOptions.noice_pixel_patch_radius')
        elif cmds.optionVar(exists='mtoa_noice_pixel_patch_radius'):
            noice_pixel_patch_radius = cmds.optionVar(query='mtoa_noice_pixel_patch_radius')
        
        cmds.intFieldGrp('noice_pixel_patch_radius', label='Pixel Patch Radius', value1=noice_pixel_patch_radius, ct2=('left', 'left'),  cw2=(label_margin,110), w=230)

        noice_light_group_aovs = ''
        if mel.eval('attributeExists ("noice_light_group_aovs", "defaultArnoldRenderOptions")'):
            noice_light_group_aovs = str(cmds.getAttr('defaultArnoldRenderOptions.noice_light_group_aovs'))
        elif cmds.optionVar(exists='mtoa_noice_light_group_aovs'):
            noice_light_group_aovs = str(cmds.optionVar(query='mtoa_noice_light_group_aovs'))
        
        cmds.textFieldGrp('noice_light_group_aovs', label='Light Group AOVs', ct2=('left', 'left'), cw2=(label_margin,110), text=noice_light_group_aovs, w=380)
        cmds.rowLayout(numberOfColumns=4, columnAlign4=('left', 'left', 'left', 'right'))
        cmds.text( '  ')

        self.denoise_button = cmds.button(label='Denoise', al='right', w=85, h=25, command=lambda *args: self.doDenoise())
        cmds.text( '                                                                                        ')
        cmds.button(label='Cancel', al='right', w=85, h=25, command=lambda *args: self.doCancel())
        cmds.setParent("..")

        self.frameRateChanged()

    def getStartEndFrames(self, inFile):
        begin_pos = inFile.find('#')
        end_pos = inFile.rfind('#')
        
        end_frame = start_frame = -1 
        globFile = inFile.replace('#', '[0-9]')
        filesList = glob.glob(globFile) or []
        
        for matchingFile in filesList:
            frame_token = int(matchingFile[begin_pos:end_pos+1])
            if start_frame < 0 or frame_token < start_frame:
                start_frame = frame_token
            if frame_token > end_frame:
                end_frame = frame_token

        return start_frame, end_frame

    def doDenoise(self):

        if self.running:
            return

        inFile = cmds.textFieldButtonGrp('noice_input', q=True, tx=True)
        outFile = cmds.textFieldButtonGrp('noice_output', q=True, tx=True)

        if inFile == '':
            cmds.confirmDialog( title='Arnold Denoiser', message='An input file must be selected', button=['Ok'], defaultButton='Ok', cancelButton='Ok', dismissString='Ok' )
            return False
        if outFile == '':
            cmds.confirmDialog( title='Arnold Denoiser', message='An output file must be selected', button=['Ok'], defaultButton='Ok', cancelButton='Ok', dismissString='Ok' )
            return False

        self.running = True
        start_frame = cmds.intFieldGrp('noice_start_frame', q=True, v1=True)
        frame_range = cmds.optionMenuGrp('noice_frame_range', q=True, v=True)
        if frame_range == 'Single Frame':
            end_frame = start_frame
        elif frame_range == 'Start / End':
            end_frame = cmds.intFieldGrp('noice_end_frame', q=True, v1=True)
        else: # complete sequence, need to check on disk all the existing input files
            start_frame, end_frame = self.getStartEndFrames(inFile)
            

        temporal_frames = cmds.intFieldGrp('noice_temporal_frames', q=True, v1=True)
        pixel_search_radius = cmds.intFieldGrp('noice_pixel_search_radius', q=True, v1=True)
        pixel_patch_radius = cmds.intFieldGrp('noice_pixel_patch_radius', q=True, v1=True)
        variance = cmds.floatFieldGrp('noice_variance', q=True, v1=True)
        light_group_aovs = cmds.textFieldGrp('noice_light_group_aovs', q=True, tx=True)
        
        # Now set these values both in the scenes and in the prefs
        core.createOptions()

        if not mel.eval('attributeExists("noice_input_file", "defaultArnoldRenderOptions")'):
            cmds.addAttr('defaultArnoldRenderOptions',ln='noice_input_file', dt='string')
        cmds.setAttr('defaultArnoldRenderOptions.noice_input_file', inFile, type='string')
        cmds.optionVar(sv=('mtoa_noice_input_file', inFile))

        if not mel.eval('attributeExists("noice_denoised_file", "defaultArnoldRenderOptions")'):
            cmds.addAttr('defaultArnoldRenderOptions',ln='noice_denoised_file', dt='string')
        cmds.setAttr('defaultArnoldRenderOptions.noice_denoised_file', outFile, type='string')
        cmds.optionVar(sv=('mtoa_noice_denoised_file', outFile))

        if not mel.eval('attributeExists("noice_frame_range", "defaultArnoldRenderOptions")'):
            cmds.addAttr('defaultArnoldRenderOptions',ln='noice_frame_range', dt='string')
        cmds.setAttr('defaultArnoldRenderOptions.noice_frame_range', frame_range, type='string')
        cmds.optionVar(sv=('mtoa_noice_frame_range', frame_range))

        if not mel.eval('attributeExists("noice_start_frame", "defaultArnoldRenderOptions")'):
            cmds.addAttr('defaultArnoldRenderOptions',ln='noice_start_frame', at='long')
        cmds.setAttr('defaultArnoldRenderOptions.noice_start_frame', start_frame)
        cmds.optionVar(iv=('mtoa_noice_start_frame', start_frame))

        if not mel.eval('attributeExists("noice_end_frame", "defaultArnoldRenderOptions")'):
            cmds.addAttr('defaultArnoldRenderOptions',ln='noice_end_frame', at='long')
        cmds.setAttr('defaultArnoldRenderOptions.noice_end_frame', end_frame)
        cmds.optionVar(iv=('mtoa_noice_end_frame', end_frame))

        if not mel.eval('attributeExists("noice_temporal_frames", "defaultArnoldRenderOptions")'):
            cmds.addAttr('defaultArnoldRenderOptions',ln='noice_temporal_frames', at='long')
        cmds.setAttr('defaultArnoldRenderOptions.noice_temporal_frames', temporal_frames)
        cmds.optionVar(iv=('mtoa_noice_temporal_frames', temporal_frames))

        if not mel.eval('attributeExists("noice_pixel_search_radius", "defaultArnoldRenderOptions")'):
            cmds.addAttr('defaultArnoldRenderOptions',ln='noice_pixel_search_radius', at='long')
        cmds.setAttr('defaultArnoldRenderOptions.noice_pixel_search_radius', pixel_search_radius)
        cmds.optionVar(iv=('mtoa_noice_pixel_search_radius', pixel_search_radius))

        if not mel.eval('attributeExists("noice_pixel_patch_radius", "defaultArnoldRenderOptions")'):
            cmds.addAttr('defaultArnoldRenderOptions',ln='noice_pixel_patch_radius', at='long')
        cmds.setAttr('defaultArnoldRenderOptions.noice_pixel_patch_radius', pixel_patch_radius)
        cmds.optionVar(iv=('mtoa_noice_pixel_patch_radius', pixel_patch_radius))

        if not mel.eval('attributeExists("noice_variance", "defaultArnoldRenderOptions")'):
            cmds.addAttr('defaultArnoldRenderOptions',ln='noice_variance', at='float')
        cmds.setAttr('defaultArnoldRenderOptions.noice_variance', variance)
        cmds.optionVar(fv=('mtoa_noice_variance', variance))

        if not mel.eval('attributeExists("noice_light_group_aovs", "defaultArnoldRenderOptions")'):
            cmds.addAttr('defaultArnoldRenderOptions',ln='noice_light_group_aovs', dt='string')
        cmds.setAttr('defaultArnoldRenderOptions.noice_light_group_aovs', light_group_aovs, type='string')
        cmds.optionVar(sv=('mtoa_noice_light_group_aovs', light_group_aovs))

        if not self.thread:
            self.thread = NoiceThread(self, start_frame, end_frame, inFile, outFile, temporal_frames, pixel_search_radius, pixel_patch_radius, variance, light_group_aovs)
            self.thread.start()

        return True

    def doCancel(self):
        self.running = False
        self.thread = None

        if self.window:
            cmds.deleteUI(self.window)
            self.window = None
        return True

    def browseInputFilename(self):
        global inputFile
        if inputFile == "":
            inputFile = cmds.workspace(q=True,rd=True, fn=True) # FIXME : get render image
        ret = cmds.fileDialog2(cap='Select Input File',okc='Select',ff="*.exr",fm=1,dir=inputFile)
        if ret is not None and len(ret):
            inputFile = ret[0]
            if len(inputFile) == 0:
                return

            extension_len = 0
            padding = 0
            for c in reversed(inputFile):
                if '0' <= c < '9':
                    padding += 1
                elif padding > 0:
                    break # I already found numerical characters and they're finished now
                else:
                    # still haven't found a numerical parameter
                    extension_len += 1

            if padding > 0:
                start_frame = int(inputFile[-extension_len - padding:-extension_len])
                cmds.intFieldGrp('noice_start_frame', e=True, v1=int(start_frame))
                end_frame = cmds.intFieldGrp('noice_end_frame', q=True, v1=True)
                if int(end_frame <= start_frame):
                    end_frame = start_frame + 1
                    cmds.intFieldGrp('noice_end_frame', e=True, v1=int(end_frame))
                inputFile = inputFile[:-extension_len - padding] + ('#' * padding) + inputFile[-extension_len:]
                
            cmds.textFieldButtonGrp('noice_input', e=True, text=inputFile)
            pos = inputFile.rfind('.')
            outputFile = inputFile[:pos]
            outputFile += '_denoised'
            outputFile += inputFile[pos:]
            cmds.textFieldButtonGrp('noice_output', e=True, text=outputFile)
            
    def browseOutputFilename(self):
        ret = cmds.fileDialog2(cap='Select Folder',okc='Select',ff="*.exr",fm=1)
        if ret is not None and len(ret):
            cmds.textFieldButtonGrp('noice_output', e=True, text=ret[0])
        return True
