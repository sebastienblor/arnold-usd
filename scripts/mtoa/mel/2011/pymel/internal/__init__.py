"""Low-level maya and pymel utilities.  Functions in this module are used by both `pymel.api` and `pymel.core`,
and are able to be defined before maya.standalone is initialized.
"""
from pkgutil import extend_path
__path__ = extend_path(__path__, __name__)

import plogging as _plogging
from plogging import getLogger
#from envparse import *
#from version import *
#assert mayaInit()
#from pwarnings import *
#import plogging
#from plogging import getLogger

def getConfigFile():
    return _plogging.getConfigFile()

def parsePymelConfig():
    import ConfigParser

    types = { 'skip_mel_init' : 'boolean' }
    defaults = {'skip_mel_init' : 'off' }
    
    config = ConfigParser.ConfigParser(defaults)
    config.read( getConfigFile() )
    
    d = {}
    for option in config.options('pymel'):
        getter = getattr( config, 'get' + types.get(option, '') ) 
        d[option] = getter( 'pymel', option )
    return d

pymel_options = parsePymelConfig() 
