import glob
import os
import platform
import re
import subprocess
# from arnold import AiTextureInvalidate

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
    
    # retain only image files that OIIO can read
    return filter(lambda p: subprocess.call(['oiiotool', '-info', p], stdout=subprocess.PIPE, stderr=subprocess.PIPE, startupinfo=_no_window) == 0, glob.glob(expand_glob))

## Compiled regex for imageInfo()
_oiiotool_rx = re.compile(r'(.*\w) +: +(\d+) +x +(\d+)( x +\d+)?, (\d+) channel, (deep )?(volume )?(\w+)(.+)? (\w+)')
_oiiotool_depth = { 'uint8': 8, 'uint16': 16, 'uint': 32, 'uint64': 64,
                     'int8': 8,  'int16': 16,  'int': 32,  'int64': 64,
                    'half': 16, 'float': 32, 'double':64, 'pointer': 64 }

def imageInfo(filename):
    '''Get image information from oiiotool'''
    mo = re.search(_oiiotool_rx, subprocess.Popen(['oiiotool', '--info', filename], stdout=subprocess.PIPE, stderr=subprocess.PIPE, startupinfo=_no_window).communicate()[0])
    if mo:
        type_name = mo.group(8)
        bit_depth = _oiiotool_depth[type_name] if type_name in _oiiotool_depth else 0
        
        return { 'filename': mo.group(1),
                 'resolution': [int(mo.group(2)), int(mo.group(3))],
                 'channels': int(mo.group(5)),
                 'type': type_name,
                 'format': mo.group(10),
                 'depth': bit_depth }
    else:
        return None 

def guessColorspace(filename):
    '''Guess the colorspace of the input image filename.
    @return: a string suitable for the --colorconvert option of maketx (linear, sRGB, Rec709)
    '''
    iinfo = imageInfo(filename)
    
    if iinfo and iinfo['depth'] <= 16 and iinfo['type'].startswith('uint'):
        return 'sRGB'
    else:
        return 'linear'

## Compiled regexes for makeTx()
_maketx_rx_stats = re.compile('maketx run time \(seconds\):\s*(.+)')
_maketx_rx_noupdate = re.compile('no update required')

def makeTx(filename, colorspace='auto'):
    '''Generate a TX texture with maketx
    '''
    status = {'updated': 0, 'skipped': 0, 'error': 0}

    for tile in expandTokens(filename):
        if os.path.splitext(tile)[1] == '.tx':
            print '[maketx] Skipping native TX texture: %s' % tile
            status['skipped'] += 1
            continue

        if colorspace == 'auto':
            colorspace = guessColorspace(tile)
        
        res = subprocess.Popen(['maketx', '-v', '-u', '--unpremult', '--oiio', '--colorconvert', colorspace, 'linear', tile], stdout=subprocess.PIPE, stderr=subprocess.PIPE, startupinfo=_no_window).communicate()[0]
        
        if re.search(_maketx_rx_noupdate, res):
            print '[maketx] TX texture is up to date for "%s" (%s)' % (tile, colorspace)
            status['skipped'] += 1
        else:
            mo = re.search(_maketx_rx_stats, res)
            if mo:
                print '[maketx] Generated TX for "%s" (%s) in %s seconds' % (tile, colorspace, mo.group(1))
                # FIXME this API function has no Python binding yet, see core#5293
                # AiTextureInvalidate(os.path.splitext(tile)[0] + '.tx') 
                status['updated'] += 1
            else:
                print '[maketx] Error: Could not generate TX for "%s" (%s)' % (tile, colorspace)
                status['error'] += 1
                
    return status

def hasTx(filename):
    '''Does the filename have existing corresponding TX file(s)?
    '''
    base, ext = os.path.splitext(filename)
    
    if not ext:
        print '[maketx] Warning: No extension for filename "%s": Cannot check TX existence, returning False' % filename
        return False

    txfile = base + '.tx'
    return len(expandTokens(txfile)) > 0
        
def txTexture(filename):
    '''Return the corresponding .tx texture if it exists, or filename if not
    '''
    base, ext = os.path.splitext(filename)
    
    if not ext:
        print '[maketx] Warning: No extension for filename "%s": Cannot check TX existence, returning filename' % filename
        return filename
    
    txfile = base + '.tx'
    if expandTokens(txfile):
        print '[maketx] Using existing .tx texture for %s' % txfile
        return txfile
    else:
        return filename
