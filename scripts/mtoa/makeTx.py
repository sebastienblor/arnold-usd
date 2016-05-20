import glob
import os
import platform
import re
import subprocess
import maya.cmds as cmds
from arnold import *

# FIXME As of Arnold 4.2.13.6 the texture API functions have no binding yet
# so we'll temporarily provide bindings until this is resolved, see core #5293

AiTextureGetFormatFunc = ai.AiTextureGetFormat
AiTextureGetFormatFunc.argtypes = [AtString, POINTER(c_uint)]
AiTextureGetFormatFunc.restype = c_bool

def AiTextureGetFormat(filename):
    fmt = c_uint(0)
    success = AiTextureGetFormatFunc(filename, fmt)
    return int(fmt.value) if success else None

AiTextureGetBitDepthFunc = ai.AiTextureGetBitDepth
AiTextureGetBitDepthFunc.argtypes = [AtString, POINTER(c_uint)]
AiTextureGetBitDepthFunc.restype = c_bool

def AiTextureGetBitDepth(filename):
    bit_depth = c_uint(0)
    success = AiTextureGetBitDepthFunc(filename, bit_depth)
    return int(bit_depth.value) if success else None

AiTextureInvalidate = ai.AiTextureInvalidate
AiTextureInvalidate.argtypes = [AtString]

# startupinfo to prevent Windows processes to display a console window
if platform.system().lower() == 'windows':
    _no_window = subprocess.STARTUPINFO()
    _no_window.dwFlags |= subprocess.STARTF_USESHOWWINDOW
else:
    _no_window = None

## Compiled regex for expandTokens()
_token_attr_rx = re.compile('<attr:[^>]*>')
_token_udim_rx = re.compile('<udim:?[^>]*>')
_token_tile_rx = re.compile('<tile:?[^>]*>')

def expandTokens(filename):
    '''Return a list of image filenames with all tokens expanded.
    The supported tokens are <udim>, <tile>, <attr:>.
    '''
    expand_glob = re.sub(_token_udim_rx, '[1-9][0-9][0-9][0-9]', filename)
    expand_glob = re.sub(_token_tile_rx, '_u[0-9]*_v[0-9]*', expand_glob)
    expand_glob = re.sub(_token_attr_rx, '*', expand_glob)
    
    # retain only image files that Arnold can read
    return filter(lambda p: AiTextureGetFormat(p), glob.glob(expand_glob))

def guessColorspace(filename):
    '''Guess the colorspace of the input image filename.
    @return: a string suitable for the --colorconvert option of maketx (linear, sRGB, Rec709)
    '''
    try:
        if AiTextureGetFormat(filename) == AI_TYPE_UINT and AiTextureGetBitDepth(filename) <= 16:
            return 'sRGB'
        else:
            return 'linear'
    except:
        print '[maketx] Error: Could not guess colorspace for "%s"' % filename
        return 'linear'

## Compiled regexes for makeTx()
_maketx_rx_stats = re.compile('maketx run time \(seconds\):\s*(.+)')
_maketx_rx_noupdate = re.compile('no update required')
_maketx_binary = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(__file__))), 'bin', 'maketx')

def makeTx(filename, colorspace='auto'):
    '''Generate a TX texture with maketx
    '''
    status = {'updated': 0, 'skipped': 0, 'error': 0}
    
    if cmds.colorManagementPrefs(q=True, cmConfigFileEnabled=True):
        color_config = cmds.colorManagementPrefs(q=True, configFilePath=True)
    else:
        color_config = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(cmds.__file__)))))), 'synColor')

    render_colorspace = cmds.colorManagementPrefs(q=True, renderingSpaceName=True)
    
    if colorspace not in cmds.colorManagementPrefs(q=True, inputSpaceNames=True):
        print '[maketx] Warning: Invalid input colorspace "%s" for "%s", falling back to rendering colorspace ("%s")' % (colorspace, filename, render_colorspace)
        colorspace = render_colorspace

    for tile in expandTokens(filename):
        if os.path.splitext(tile)[1] == '.tx':
            print '[maketx] Skipping native TX texture: %s' % tile
            status['skipped'] += 1
            continue

        if colorspace == 'auto':
            colorspace = guessColorspace(tile)
        
        cmd = [_maketx_binary, '-v', '-u', '--unpremult', '--oiio', '--colorengine', 'syncolor', '--colorconfig', color_config, '--colorconvert', colorspace, render_colorspace, tile]
        res = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, startupinfo=_no_window).communicate()[0]
        
        if re.search(_maketx_rx_noupdate, res):
            print '[maketx] TX texture is up to date for "%s" (%s)' % (tile, colorspace)
            status['skipped'] += 1
        else:
            mo = re.search(_maketx_rx_stats, res)
            if mo:
                print '[maketx] Generated TX for "%s" (%s) in %s seconds' % (tile, colorspace, mo.group(1))
                AiTextureInvalidate(os.path.splitext(tile)[0] + '.tx') 
                status['updated'] += 1
            else:
                print '[maketx] Error: Could not generate TX for "%s" (%s)' % (tile, colorspace)
                status['error'] += 1
                
    return status
