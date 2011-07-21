from pkgutil import extend_path
__path__ = extend_path(__path__, __name__)

__version__ = '1.0.0'
__authors__ = ['Chad Dombrova', 'Olivier Renouard', 'Ofer Koren', 'Paul Molodowitch']

import sys
assert sys.version_info > (2,5), "pymel version 1.0 is compatible with Maya2008/python2.5 or later"

_mtoaPatch = True
