# vim: filetype=python

import platform
import collections
import shlex
import subprocess
import sys
import time

# Obtain information about the system only once, when loaded
os = platform.system().lower()

_linux   = 'linux'
_darwin  = 'darwin'
_windows = 'windows'

_Allowed = collections.namedtuple('_Allowed', ['os'])

# allowed.os is a list of allowed operative system
allowed = _Allowed(
     os = [_linux, _darwin, _windows]
)

# These data avoid writing error prone checks like "os.system.os == 'linux'"
is_linux   = os == _linux
is_darwin  = os == _darwin
is_windows = os == _windows

PATH = 'PATH'
LIBRARY_PATH = {
   _linux  : 'LD_LIBRARY_PATH',
   _darwin : 'DYLD_LIBRARY_PATH',
   _windows: 'PATH'
}.get(os, None)
PYTHON_PATH = 'PYTHONPATH'
ARNOLD_PLUGIN_PATH = 'ARNOLD_PLUGIN_PATH'

# This "safe" version of "print" works atomically, avoiding the mess caused by
# multiple threads writing at the same time. It has the same declaration and
# behavior as the Python 3.3 print() function:
#   https://docs.python.org/3/library/functions.html?highlight=print#print
def print_safe(*args, **kwargs):
   # Check input parameters
   valid_kwargs = ('sep', 'end', 'file', 'flush')
   for key, value in kwargs.iteritems():
      if key not in valid_kwargs:
         raise TypeError('\'{}\' is an invalid keyword argument for this function'.format(key))
      elif key in ['sep', 'end']:
         not_string = not isinstance(value, basestring)
         not_None = value is not None
         if not_string and not_None:
            typename = type(value).__name__
            raise TypeError('\'{}\' must be None or a string, not {}'.format(key, typename))
   # Transform objects into string
   objects = (str(o) for o in args)
   # Get input parameters
   sep = kwargs.get('sep')
   end = kwargs.get('end')
   fhd = kwargs.get('file')
   ffl = kwargs.get('flush', False)
   # Set default values if not provided
   sep = ' ' if sep is None else sep
   end = '\n' if end is None else end
   fhd = sys.stdout if fhd is None else fhd
   # Write the whole string and flush if requested
   fhd.write(sep.join(objects) + end)
   if ffl:
      fhd.flush()

def execute(cmd, env=None, cwd=None, verbose=False, shell=False, callback=lambda line: None, timeout=0):
   '''
   Executes a command and returns a tuple with the exit code and the output
   '''
   # Things to do before executing the command:
   # - Split cmd into a list if it is a string
   # - Initialize the output and return codes
   # - Normalize environment to strings

   c = shlex.split(cmd, posix=(not is_windows)) if (type(cmd) == str) and not shell else cmd
   r, o = 0, []
   e = {k : str(v) for k, v in env.items()} if env else None
   # Create a dictionary with the arguments for subprocess.Popen()
   popen_args = {
      'args'    : c,
      'stdout'  : subprocess.PIPE,
      'stderr'  : subprocess.STDOUT,
      'cwd'     : cwd,
      'env'     : e,
      'shell'   : shell,
      'bufsize' : 1,
      'universal_newlines': True,
   }
   try:
      t = time.time()
      p = subprocess.Popen(**popen_args)
      with p.stdout:
         for line in iter(p.stdout.readline, b''):
            if not line:
               break
            elif timeout and (time.time() - t) > timeout:
               p.kill()
               break
            line = line.rstrip('\n')
            o.append(line)
            callback(line)
            if verbose:
               print(line)
      r = p.wait()
   except OSError as e:
      o = [e.strerror]
      r = e.errno
   return (r, o)
