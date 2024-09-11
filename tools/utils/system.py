# vim: filetype=python
# Copyright 2022 Autodesk, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
import platform
import collections
import shlex
import subprocess
import sys
import time
from .misc import is_string

try:
   # Python 2
   # Empirically, it is faster to check explicitly for str and
   # unicode than for basestring.
   string_types = (str, unicode)
except NameError:
   # Python 3
   string_types = (str)

try:
   # The only option after Python 3.11
   from inspect import getfullargspec as inspect_getargspec
   popen_argspec = inspect_getargspec(subprocess.Popen.__init__)
   popen_argspec = popen_argspec.args + popen_argspec.kwonlyargs
except ImportError:
   # Deprecated in 3.0 and removed in Python 3.11
   from inspect import getargspec as inspect_getargspec
   popen_argspec = inspect_getargspec(subprocess.Popen.__init__).args

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

IS_LINUX = is_linux
IS_DARWIN = is_darwin
IS_WINDOWS = is_windows

PATH = 'PATH'
LIBRARY_PATH = {
   _linux  : 'LD_LIBRARY_PATH',
   _darwin : 'DYLD_LIBRARY_PATH',
   _windows: 'PATH',
}.get(os, None)
PYTHON_PATH = 'PYTHONPATH'
ARNOLD_PLUGIN_PATH = 'ARNOLD_PLUGIN_PATH'
LIB_EXTENSION = {
   _linux: '.so',
   _darwin: '.dylib',
   _windows: '.dll',
}.get(os, '')

# This "safe" version of "print" works atomically, avoiding the mess caused by
# multiple threads writing at the same time. It has the same declaration and
# behavior as the Python 3.3 print() function:
#   https://docs.python.org/3/library/functions.html?highlight=print#print
def print_safe(*args, **kwargs):
   # Check input parameters
   valid_kwargs = ('sep', 'end', 'file', 'flush')
   for key, value in kwargs.items():
      if key not in valid_kwargs:
         raise TypeError('\'{}\' is an invalid keyword argument for this function'.format(key))
      elif key in ['sep', 'end']:
         not_string = not isinstance(value, string_types)
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


def kill(process, cwd):
   if process.returncode is None:
      if is_windows and psutil:
         # Try to create a minidump of the process
         try:
            # Check child itself as well as all subprocesses of the child
            # Compare with the executable to find which process is the relevant one
            relevant_command = process.args[0]
            child = psutil.Process(process.pid)
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
         subprocess.call(['taskkill', '/F', '/T', '/PID', f'{process.pid}'])
      elif not is_windows:
         # Send a SIGABRT signal hoping to get a stacktrace from the process
         # TODO: Isn't SIGTERM a more convenient signal for a graceful termination
         # if the process is not hung? 
         process.send_signal(signal.SIGABRT)
         try:
            # This might deadlock the process. Let's wait some time
            # for more desperate measures
            process.wait(20)
         except subprocess.TimeoutExpired:
            # The process didn't terminate. Let's send an aggressive
            # SIGKILL signal
            process.send_signal(signal.SIGKILL)
            try:
               # Wait again for SIGKILL to do its job
               process.wait(20)
            except subprocess.TimeoutExpired:
               # SIGKILL didn't work. Let's inform the user
               print(f'Failed to kill process {process.pid}')

def execute(cmd, env=None, cwd=None, verbose=False, shell=False, callback=None, timeout=0, logToFile=None):
   '''
   Executes a command and returns a tuple with the exit code and the output
   '''
   # Things to do before executing the command:
   # - Split cmd into a list if it is a string
   # - Normalize environment to strings
   split_command = is_string(cmd) and not shell
   # Create a dictionary with the arguments for subprocess.Popen()
   redirectOutputToFile = logToFile and not verbose and not callback
   if redirectOutputToFile:
      stdout_f = open(logToFile, 'w')
   popen_args = {
      'args'    : shlex.split(cmd, posix=is_unix) if split_command else cmd,
      'stdout'  : subprocess.PIPE if not redirectOutputToFile else stdout_f,
      'stderr'  : subprocess.STDOUT,
      'cwd'     : cwd,
      'env'     : {k : str(v) for k, v in env.items()} if env else None,
      'shell'   : shell,
      'bufsize' : 1,
   }
   if 'errors' in popen_argspec: popen_args['errors'] = 'replace'
   if 'text'   in popen_argspec: popen_args['text'] = True
   else                        : popen_args['universal_newlines'] = True
   t = time.time()
   try:
      process = subprocess.Popen(**popen_args)
   except OSError as e:
      return e.errno, e.strerror.splitlines()
   else:
      if timeout:
         killer = threading.Timer(timeout, kill, [process, cwd])
         killer.start()
      output = []
      if not redirectOutputToFile:
         for line in iter(process.stdout.readline, b''):
            if not line:
               break
            line = line.rstrip('\n')
            output.append(line)
            if verbose:
               print_safe(line)
            if callback:
               callback(line)
      if process.stdout: process.stdout.close()
      if process.stderr: process.stderr.close()
      process.wait()
      if timeout:
         killer.cancel()
      
      if redirectOutputToFile:
         stdout_f.flush()
         stdout_f.close()
         # Need to keep the existing function contract
         try:
            with open(logToFile, 'rb') as log:
               output = log.read()
               output = output.decode('utf-8', 'replace')
               output = output.splitlines()
         except IOError as e:
            return e.errno, e.strerror.splitlines()
      elif logToFile:
         # We were told to write to file, but we could not because we were also told to log to stdout or callback
         try:
            with open(logToFile, 'w') as log:
               log.write('\n'.join(output))
         except IOError as e:
            return e.errno, e.strerror.splitlines()

      return process.returncode, output

