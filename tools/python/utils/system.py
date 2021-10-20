# vim: filetype=python

import platform
import collections
import psutil
import shlex
try:
   import subprocess32 as subprocess
except ImportError:
   import subprocess
import signal
import sys
import threading
import time
from os import path as ospath

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
is_unix    = not is_windows

PATH = 'PATH'
LIBRARY_PATH = {
   _linux  : 'LD_LIBRARY_PATH',
   _darwin : 'DYLD_LIBRARY_PATH',
   _windows: 'PATH'
}.get(os, None)

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
   # Transform objects into strings, get the input parameters, set the default
   # values if not provided and write the whole string. Flush if requested.
   objects = (str(o) for o in args)
   sep = str(kwargs.get('sep', ' '))
   end = str(kwargs.get('end', '\n'))
   out = kwargs.get('file', sys.stdout)
   out.write(sep.join(objects) + end)
   if kwargs.get('flush', False):
      out.flush()

'''
def execute(cmd, env=None, cwd=None, verbose=False, shell=False, callback=lambda line: None, timeout=0):
   Executes a command and returns a tuple with the exit code and the output
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
      print('-------------------------')
      with p.stdout:
         print("finished")
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
'''
### Version from Arnold core

def execute(cmd, env=None, cwd=None, verbose=False, shell=False, callback=None, timeout=0, logToFile=None):
   '''
   Executes a command and returns a tuple with the exit code and the output
   '''
   # Things to do before executing the command:
   # - Split cmd into a list if it is a string
   # - Normalize environment to strings
   split_command = isinstance(cmd, basestring) and not shell
   # Create a dictionary with the arguments for subprocess.Popen()
   redirectOutputToFile = logToFile and not verbose and not callback
   popen_args = {
      'args'    : shlex.split(cmd, posix=is_unix) if split_command else cmd,
      'stdout'  : subprocess.PIPE if not redirectOutputToFile else open(logToFile, 'w'),
      'stderr'  : subprocess.STDOUT,
      'cwd'     : cwd,
      'env'     : {k : str(v) for k, v in env.items()} if env else None,
      'shell'   : shell,
      'bufsize' : 1,
      'universal_newlines': True,
   }
   t = time.time()
   try:
      process = subprocess.Popen(**popen_args)
   except OSError as e:
      return e.errno, e.strerror.splitlines()
   else:
      if timeout:
         def kill(p):
            if p.returncode is None:
               if is_linux:
                  execute("gstack " + str(p.pid) + " > timeout-stack.txt", cwd=cwd, shell=True, timeout=0)
               if is_windows:
                  # Try to create a minidump of the process
                  try:
                     # Check child itself as well as all subprocesses of the child
                     # Compare with the executable to find which process is the relevant one
                     relevant_command = shlex.split(cmd, posix=is_unix)[0]
                     child = psutil.Process(p.pid)
                     relevant_process = None
                     if child.cmdline()[0] == relevant_command:
                        relevant_process = child
                     else:
                        children = child.children(recursive=True)
                        for candidate_process in children:
                           if (candidate_process.cmdline()[0] == relevant_command):
                              relevant_process = candidate_process
                              break
                     if relevant_process:
                        procdump_command = "procdump -accepteula -mm {}".format(relevant_process.pid)
                        print("Generating minidump for process '{}' using command '{}')".format(" ".join(relevant_process.cmdline()), procdump_command))
                        # Create a minidump of the process
                        retval, err = execute(procdump_command, cwd=cwd)
                        print("procdump returned {}:\n{}".format(retval, "\n".join(err)))
                     else:
                        print("Could not find child process running {} to generate a minidump for.".format(relevant_command))
                  except psutil.Error as e:
                     print("Failed to generate minidump: {}".format(e))
                     # Ignore.

                  # Kill subprocess (recursively)
                  subprocess.call(['taskkill', '/F', '/T', '/PID', str(p.pid)])
               else:
                  p.send_signal(signal.SIGABRT)
         killer = threading.Timer(timeout, kill, [process])
         killer.start()
      output = []
      if not redirectOutputToFile:
         with process.stdout:
            for line in iter(process.stdout.readline, b''):
               if not line:
                  break
               line = line.rstrip('\n')
               output.append(line)
               if verbose:
                  print_safe(line)
               if callback:
                  callback(line)
      process.wait()
      if timeout:
         killer.cancel()
      
      if redirectOutputToFile:
         # Need to keep the existing function contract
         try:
            with open(logToFile, 'r') as log:
               output = log.readlines()
         except IOError as e:
            return e.errno, e.strerror.splitlines()
      elif logToFile:
         # We were told to write to file, but we could not because we were also told to log to stdout or callback
         try:
            with open(logToFile, 'w') as log:
               for line in output:
                  log.write(line + '\n')
         except IOError as e:
            return e.errno, e.strerror.splitlines()

      return process.returncode, output
